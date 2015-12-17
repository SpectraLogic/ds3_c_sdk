/*
* ******************************************************************************
*   Copyright 2014-2015 Spectra Logic Corporation. All Rights Reserved.
*   Licensed under the Apache License, Version 2.0 (the "License"). You may not use
*   this file except in compliance with the License. A copy of the License is located at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
*   or in the "license" file accompanying this file.
*   This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
*   CONDITIONS OF ANY KIND, either express or implied. See the License for the
*   specific language governing permissions and limitations under the License.
* ****************************************************************************
*/


#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#include "ds3.h"
#include "ds3_request.h"
#include "ds3_string_multimap.h"
#include "ds3_string_multimap_impl.h"
#include "ds3_net.h"
#include "ds3_utils.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

struct _ds3_metadata {
    GHashTable* metadata;
};

typedef struct {
    char* buff;
    size_t size;
    size_t total_read;
}ds3_xml_send_buff;

typedef enum {
    BULK_PUT,
    BULK_GET,
    BULK_DELETE,
    GET_PHYSICAL_PLACEMENT
}object_list_type;

void ds3_log_message(const ds3_log* log, ds3_log_lvl lvl, const char* message, ...) {
    if (log == NULL) {
        return;
    }

    if (log->log_callback == NULL) {
        fprintf(stderr, "ERROR: ds3_c_sdk - User supplied log_callback is null, failed to log message.\n");
        return;
    }

    if (lvl <= log->log_lvl) {
        va_list args;
        char * log_message;

        va_start(args, message);
        log_message = g_strdup_vprintf(message, args);
        va_end(args);

        log->log_callback(log_message, log->user_data);

        g_free(log_message);
    }
}

void ds3_client_register_logging(ds3_client* client, ds3_log_lvl log_lvl, void (* log_callback)(const char* log_message, void* user_data), void* user_data) {
    if (client == NULL) {
        fprintf(stderr, "Cannot configure a null ds3_client for logging.\n");
        return;
    }
    if (client->log != NULL) {
        g_free(client->log);
    }
    ds3_log* log = g_new0(ds3_log, 1);
    log->log_callback = log_callback;
    log->user_data = user_data;
    log->log_lvl = log_lvl;

    client->log = log;
}

static void _ds3_free_metadata_entry(gpointer pointer) {
    ds3_metadata_entry* entry;
    if (pointer == NULL) {
        return; // do nothing
    }

    entry = (ds3_metadata_entry*) pointer;

    ds3_free_metadata_entry(entry);
}

/*
 * This copies all the header values in the ds3_string_multimap_entry struct so that they may be safely returned to the user
 * without having to worry about if the data is freed internally.
 */
static const char* METADATA_PREFIX = "x-amz-meta-";
static ds3_metadata_entry* ds3_metadata_entry_init(ds3_string_multimap_entry* header_entry) {
    guint i;
    ds3_str* header_value;
    GPtrArray* values = g_ptr_array_new();
    ds3_str* key_name;
    ds3_str* full_key;
    ds3_metadata_entry* response = g_new0(ds3_metadata_entry, 1);
    int metadata_prefix_length = strlen(METADATA_PREFIX);

    unsigned int num_values = ds3_string_multimap_entry_get_num_values(header_entry);
    for (i = 0; i < num_values; i++) {
        header_value = ds3_string_multimap_entry_get_value_by_index(header_entry, i);
        g_ptr_array_add(values, header_value);
    }

    full_key = ds3_string_multimap_entry_get_key(header_entry);
    key_name = ds3_str_init(full_key->value + metadata_prefix_length);
    ds3_str_free(full_key);

    response->num_values = num_values;
    response->name = key_name;
    response->values = (ds3_str**) g_ptr_array_free(values, FALSE);
    fprintf(stderr, "creating metadata entry of: %s\n", key_name->value);
    return response;
}

/* The headers hash table contains all the response headers which have the following types:
 * Key - char*
 * Value - ds3_response_header
 *
 * All values should be copied from the struct to avoid memory issues
 */
static ds3_metadata* _init_metadata(ds3_string_multimap* response_headers) {
    struct _ds3_metadata* metadata = g_new0(struct _ds3_metadata, 1);
    GHashTableIter iter;
    gpointer _key, _value;
    ds3_str* key;
    ds3_metadata_entry* entry;
    metadata->metadata = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, _ds3_free_metadata_entry);

    if (response_headers == NULL) {
        fprintf(stderr, "response headers was null\n");
    }

    //TODO refactor out glib: ds3_string_multimap needs a lookup_prefix()
    g_hash_table_iter_init(&iter, ds3_string_multimap_get_hashtable(response_headers));
    while(g_hash_table_iter_next(&iter, &_key, &_value)) {
        key = (ds3_str*) _key;
        if (g_str_has_prefix(key->value, "x-amz-meta-")) {
            ds3_string_multimap_entry* mm_entry = ds3_string_multimap_lookup(response_headers, key);
            entry = ds3_metadata_entry_init(mm_entry);
            g_hash_table_insert(metadata->metadata, g_strdup(entry->name->value), entry);
            ds3_string_multimap_entry_free(mm_entry);
        }
    }

    return (ds3_metadata*) metadata;
}

ds3_metadata_entry* ds3_metadata_get_entry(const ds3_metadata* _metadata, const char* name) {
    ds3_metadata_entry* copy;
    ds3_metadata_entry* orig;
    ds3_str** metadata_copy;
    uint64_t i;
    struct _ds3_metadata* metadata = (struct _ds3_metadata*) _metadata;

    if (_metadata == NULL) {
        return NULL;
    }

    orig = (ds3_metadata_entry*) g_hash_table_lookup(metadata->metadata, name);
    if (orig == NULL) {
        return NULL;
    }
    copy = g_new0(ds3_metadata_entry, 1);
    metadata_copy = g_new0(ds3_str*, orig->num_values);

    for (i = 0; i < orig->num_values; i++) {
        metadata_copy[i] = ds3_str_dup(orig->values[i]);
    }

    copy->num_values = orig->num_values;
    copy->name = ds3_str_dup(orig->name);
    copy->values = metadata_copy;

    return copy;
}

unsigned int ds3_metadata_size(const ds3_metadata* _metadata) {
    struct _ds3_metadata* metadata = (struct _ds3_metadata*) _metadata;
    if (metadata == NULL) {
        return 0;
    }
    return g_hash_table_size(metadata->metadata);
}

ds3_metadata_keys_result* ds3_metadata_keys(const ds3_metadata* _metadata) {
    GPtrArray* return_keys;
    ds3_metadata_keys_result* result;
    struct _ds3_metadata* metadata;
    GList* keys;
    GList* tmp_key;

    if (_metadata == NULL) {
        return NULL;
    }

    return_keys = g_ptr_array_new();
    result = g_new0(ds3_metadata_keys_result, 1);
    metadata = (struct _ds3_metadata*) _metadata;
    keys = g_hash_table_get_keys(metadata->metadata);
    tmp_key = keys;

    while(tmp_key != NULL) {
        g_ptr_array_add(return_keys, ds3_str_init((char*)tmp_key->data));
        tmp_key = tmp_key->next;
    }

    g_list_free(keys);
    result->num_keys = return_keys->len;
    result->keys = (ds3_str**) g_ptr_array_free(return_keys, FALSE);
    return result;
}

ds3_error* ds3_create_error(ds3_error_code code, const char * message) {
    ds3_error* error = g_new0(ds3_error, 1);
    error->code = code;
    error->message = ds3_str_init(message);
    error->error = NULL;
    return error;
}

static size_t _ds3_send_xml_buff(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t to_read;
    size_t remaining;
    ds3_xml_send_buff* xml_buff;

    xml_buff = (ds3_xml_send_buff*) user_data;
    to_read = size * nmemb;
    remaining = xml_buff->size - xml_buff->total_read;

    if (remaining < to_read) {
        to_read = remaining;
    }

    strncpy((char*)buffer, xml_buff->buff + xml_buff->total_read, to_read);
    xml_buff->total_read += to_read;
    return to_read;
}

size_t ds3_load_buffer(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t realsize = size * nmemb;
    GByteArray* blob = (GByteArray*) user_data;

    g_byte_array_append(blob, (const guint8 *) buffer, realsize);
    return realsize;
}

static void _cleanup_hash_value(gpointer value) {
    g_free(value);
}

static GHashTable* _create_hash_table(void) {
    GHashTable* hash =  g_hash_table_new_full(g_str_hash, g_str_equal, _cleanup_hash_value, _cleanup_hash_value);
    return hash;
}

ds3_creds* ds3_create_creds(const char* access_id, const char* secret_key) {
    ds3_creds* creds;
    if (access_id == NULL || secret_key == NULL) {
        fprintf(stderr, "Arguments cannot be NULL\n");
        return NULL;
    }

    creds = g_new0(ds3_creds, 1);

    creds->access_id = ds3_str_init(access_id);

    creds->secret_key = ds3_str_init(secret_key);

    return creds;
}

void ds3_client_register_net(ds3_client* client, ds3_error* (* net_callback)(const ds3_client* client,
                                                                             const ds3_request* _request,
                                                                             void* read_user_struct,
                                                                             size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                                                             void* write_user_struct,
                                                                             size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                                                             ds3_string_multimap** return_headers)) {
    if (client == NULL) {
        fprintf(stderr, "Cannot configure a null ds3_client for net_callback.\n");
        return;
    }

    client->net_callback = net_callback;
}

ds3_client* ds3_create_client(const char* endpoint, ds3_creds* creds) {
    ds3_client* client;
    if (endpoint == NULL) {
        fprintf(stderr, "Null endpoint\n");
        return NULL;
    }

    client = g_new0(ds3_client, 1);

    client->endpoint = ds3_str_init(endpoint);

    client->creds = creds;

    client->num_redirects = 5L; //default to 5 redirects before failing

    ds3_client_register_net( client, net_process_request );

    return client;
}

ds3_error* ds3_create_client_from_env(ds3_client** client) {
    ds3_creds* creds;
    ds3_client* _client;
    char* endpoint = getenv("DS3_ENDPOINT");
    char* access_key = getenv("DS3_ACCESS_KEY");
    char* secret_key = getenv("DS3_SECRET_KEY");
    char* http_proxy = getenv("http_proxy");

    if (endpoint == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "Missing enviornment variable 'DS3_ENDPOINT'");
    }

    if (access_key == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "Missing enviornment variable 'DS3_ACCESS_KEY'");
    }

    if (secret_key == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "Missing enviornment variable 'DS3_SECRET_KEY'");
    }

    creds = ds3_create_creds(access_key, secret_key);
    _client = ds3_create_client(endpoint, creds);

    if (http_proxy != NULL) {
        ds3_client_proxy(_client, http_proxy);
    }

    *client = _client;

    return NULL;
}

static void _set_map_value(GHashTable* map, const char* key, const char* value) {
    gpointer escaped_key = (gpointer) escape_url(key);

    //TODO update this to handle multiple values being set for a header field
    gpointer escaped_value;
    if (value != NULL) {
        if (g_strcmp0(key, "Range") == 0) {
            escaped_value = (gpointer) escape_url_range_header(value);
        } else {
            escaped_value = (gpointer) escape_url(value);
        }
    } else {
        escaped_value = NULL;
    }
    g_hash_table_insert(map, escaped_key, escaped_value);
}

static void _set_query_param(ds3_request* _request, const char* key, const char* value) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    _set_map_value(request->query_params, key, value);
}

static void _set_header(ds3_request* _request, const char* key, const char* value) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    _set_map_value(request->headers, key, value);
}

static char* _get_ds3_object_type(const ds3_object_type type) {
    switch(type) {
        case DATA: return "DATA";
        case FOLDER: return "FOLDER";
    }

    return NULL;
}

void ds3_client_proxy(ds3_client* client, const char* proxy) {
    client->proxy = ds3_str_init(proxy);
}

void ds3_request_set_prefix(ds3_request* _request, const char* prefix) {
    _set_query_param(_request, "prefix", prefix);
}

void ds3_request_set_metadata(ds3_request* _request, const char* name, const char* value) {

    char* prefixed_name = g_strconcat("x-amz-meta-", name, NULL);

    _set_header(_request, prefixed_name, value);

    g_free(prefixed_name);
}

void ds3_request_reset_byte_range(ds3_request* _request) {
    g_hash_table_remove(_request->headers, "Range");
}

void ds3_request_set_byte_range(ds3_request* _request, int64_t rangeStart, int64_t rangeEnd) {
    char* range_value;
    
    gpointer header_value = g_hash_table_lookup(_request->headers, "Range");
    if(header_value != NULL) {
        range_value = g_strdup_printf("%s,%ld-%ld", (char*)header_value, rangeStart, rangeEnd);
    } else {
	range_value = g_strdup_printf("bytes=%ld-%ld", rangeStart, rangeEnd);
    }
    
    _set_header(_request, "Range", range_value);
    g_free(range_value);
}

void ds3_request_set_custom_header(ds3_request* _request, const char* header_name, const char* header_value) {
   _set_header(_request, header_name, header_value);
}

void ds3_request_set_bucket_name(ds3_request* _request, const char* bucket_name) {
    _set_query_param(_request, "bucket_id", bucket_name);
}

void ds3_request_set_creation_date(ds3_request* _request, const char* creation_date) {
    _set_query_param(_request, "creation_date", creation_date);
}

void ds3_request_set_md5(ds3_request* _request, const char* md5) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_MD5;
    request->checksum = ds3_str_init(md5);
}

void ds3_request_set_sha256(ds3_request* _request, const char* sha256) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_SHA256;
    request->checksum = ds3_str_init(sha256);
}

void ds3_request_set_sha512(ds3_request* _request, const char* sha512) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_SHA512;
    request->checksum = ds3_str_init(sha512);
}

void ds3_request_set_crc32(ds3_request* _request, const char* crc32) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_CRC32;
    request->checksum = ds3_str_init(crc32);
}

void ds3_request_set_crc32c(ds3_request* _request, const char* crc32c) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_CRC32C;
    request->checksum = ds3_str_init(crc32c);
}

void ds3_request_set_delimiter(ds3_request* _request, const char* delimiter) {
    _set_query_param(_request, "delimiter", delimiter);
}

void ds3_request_set_marker(ds3_request* _request, const char* marker) {
    _set_query_param(_request, "marker", marker);
}

void ds3_request_set_max_keys(ds3_request* _request, uint32_t max_keys) {
    int metadata_prefix_length = strlen(METADATA_PREFIX);
    char max_keys_s[metadata_prefix_length];
    memset(max_keys_s, 0, sizeof(char) * metadata_prefix_length);
    g_snprintf(max_keys_s, sizeof(char) * metadata_prefix_length, "%u", max_keys);
    _set_query_param(_request, "max-keys", max_keys_s);
}

void ds3_request_set_name(ds3_request* _request, const char* name) {
    _set_query_param(_request, "name", name);
}

void ds3_request_set_id(ds3_request* _request, const char* id) {
    _set_query_param(_request, "id", id);
}

void ds3_request_set_type(ds3_request* _request, ds3_object_type type) {
    const char* type_as_string = _get_ds3_object_type(type);
    if (type_as_string != NULL) {
        _set_query_param(_request, "type", type_as_string);
    }
}

void ds3_request_set_page_length(ds3_request* _request, const char* page_length) {
    _set_query_param(_request, "page_length", page_length);
}

void ds3_request_set_page_offset(ds3_request* _request, const char* page_offset) {
    _set_query_param(_request, "page_offset", page_offset);
}

void ds3_request_set_version(ds3_request* _request, const char* version) {
    _set_query_param(_request, "version", version);
}

static struct _ds3_request* _common_request_init(http_verb verb, ds3_str* path) {
    struct _ds3_request* request = g_new0(struct _ds3_request, 1);
    request->headers = _create_hash_table();
    request->query_params = _create_hash_table();
    request->verb = verb;
    request->path = path;
    return request;
}

static ds3_str* _build_path(const char* path_prefix, const char* bucket_name, const char* object_name) {
    ds3_str* path = NULL;
    char* escaped_bucket_name = NULL;
    char* escaped_object_name = NULL;
    char* joined_path = NULL;
    char* full_path = NULL;

    if (bucket_name != NULL) {
        if (g_str_has_suffix(bucket_name, "/") == TRUE) {
            char* chomp_bucket = g_strndup(bucket_name, strlen(bucket_name)-1);
            escaped_bucket_name = escape_url(chomp_bucket);
            g_free(chomp_bucket);
	} else {
	    escaped_bucket_name = escape_url(bucket_name);
	}
    }
    if (object_name != NULL) {
        escaped_object_name = escape_url_object_name(object_name);
    }

    joined_path = g_strjoin("/", escaped_bucket_name, escaped_object_name, NULL);
    full_path = g_strconcat(path_prefix, joined_path, NULL);
    g_free(joined_path);

    path = ds3_str_init(full_path);
    g_free(full_path);

    if (escaped_bucket_name != NULL) {
        g_free(escaped_bucket_name);
    }
    if (escaped_object_name != NULL) {
        g_free(escaped_object_name);
    }

    return path;
}

ds3_request* ds3_init_get_system_information(void) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path( "/_rest_/SYSTEM_INFORMATION", NULL, NULL));
}

ds3_request* ds3_init_verify_system_health(void) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path( "/_rest_/SYSTEM_HEALTH", NULL, NULL));
}

ds3_request* ds3_init_get_service(void) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path( "/", NULL, NULL));
}

ds3_request* ds3_init_get_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_head_object(const char* bucket_name, const char* object_name) {
    return (ds3_request*) _common_request_init(HTTP_HEAD, _build_path("/", bucket_name, object_name));
}

ds3_request* ds3_init_head_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_HEAD, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_get_object_for_job(const char* bucket_name, const char* object_name, uint64_t offset, const char* job_id) {
    char buff[21];
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, object_name));
    if (job_id != NULL) {
        _set_query_param((ds3_request*) request, "job", job_id);

        sprintf(buff, "%llu" , (unsigned long long int) offset);
        _set_query_param((ds3_request*) request, "offset", buff);
    }

    return (ds3_request*) request;
}

ds3_request* ds3_init_delete_object(const char* bucket_name, const char* object_name) {
    return (ds3_request*) _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, object_name));
}

ds3_request* ds3_init_delete_objects(const char* bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/", bucket_name, NULL));
    _set_query_param(request, "delete", NULL);
    return (ds3_request*) request;
}

ds3_request* ds3_init_delete_folder(const char* bucket_name, const char* folder_name) {
    char* folder = "folder";
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/", folder, folder_name));
    _set_query_param(request, "recursive", NULL);
    _set_query_param(request, "bucketId", bucket_name);
    return (ds3_request*) request;
}

ds3_request* ds3_init_put_object_for_job(const char* bucket_name, const char* object_name, uint64_t offset, uint64_t length, const char* job_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, object_name));
    char buff[21];

    request->length = length;
    if (job_id != NULL) {
        _set_query_param((ds3_request*) request, "job", job_id);

        sprintf(buff, "%llu" , (unsigned long long int) offset);
        _set_query_param((ds3_request*) request, "offset", buff);
    }

    return (ds3_request*) request;
}

ds3_request* ds3_init_put_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_PUT, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_delete_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_get_bulk(const char* bucket_name, ds3_bulk_object_list* object_list, ds3_chunk_ordering order) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "operation", "start_bulk_get");
    request->object_list = object_list;
    request->chunk_ordering = order;
    return (ds3_request*) request;
}

ds3_request* ds3_init_put_bulk(const char* bucket_name, ds3_bulk_object_list* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "operation", "start_bulk_put");
    request->object_list = object_list;
    return (ds3_request*) request;
}

ds3_request* ds3_init_get_physical_placement(const char* bucket_name, ds3_bulk_object_list* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "operation", "get_physical_placement");
    request->object_list = object_list;
    return (ds3_request*) request;
}

ds3_request* ds3_init_get_physical_placement_full_details(const char* bucket_name, ds3_bulk_object_list* object_list) {
  struct _ds3_request* request = ds3_init_get_physical_placement(bucket_name, object_list);
  _set_query_param((ds3_request*) request, "full_details", NULL);
  return (ds3_request*) request;
}

ds3_request* ds3_init_allocate_chunk(const char* chunk_id) {
    char* path = g_strconcat("/_rest_/job_chunk/", chunk_id, NULL);
    ds3_str* path_str = ds3_str_init(path);
    struct _ds3_request* request = _common_request_init(HTTP_PUT, path_str);

    _set_query_param((ds3_request*) request, "operation", "allocate");
    g_free(path);
    return (ds3_request*) request;
}

ds3_request* ds3_init_get_available_chunks(const char* job_id) {
    ds3_str* path_str = ds3_str_init("/_rest_/job_chunk/");
    struct _ds3_request* request = _common_request_init(HTTP_GET, path_str);

    _set_query_param((ds3_request*) request, "job", job_id);

    return (ds3_request*) request;
}

ds3_request* ds3_init_get_jobs(void) {
    ds3_str* path_str = ds3_str_init("/_rest_/job");
    struct _ds3_request* request = _common_request_init(HTTP_GET, path_str);

    return (ds3_request*) request;
}

ds3_request* ds3_init_get_job(const char* job_id) {
    char* path = g_strconcat("/_rest_/job/", job_id, NULL);
    ds3_str* path_str = ds3_str_init(path);
    struct _ds3_request* request = _common_request_init(HTTP_GET, path_str);

    g_free(path);
    return (ds3_request*) request;
}

ds3_request* ds3_init_put_job(const char* job_id) {
    char* path = g_strconcat("/_rest_/job/", job_id, NULL);
    ds3_str* path_str = ds3_str_init(path);
    struct _ds3_request* request = _common_request_init(HTTP_PUT, path_str);

    g_free(path);
    return (ds3_request*) request;
}

ds3_request* ds3_init_delete_job(const char* job_id) {
    char* path = g_strconcat("/_rest_/job/", job_id, NULL);
    ds3_str* path_str = ds3_str_init(path);
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, path_str);

    g_free(path);
    return (ds3_request*) request;
}

ds3_request* ds3_init_get_objects() {
    struct _ds3_request* request = _common_request_init(HTTP_GET, ds3_str_init("/_rest_/object/"));

    return (ds3_request*) request;
}


static ds3_error* _internal_request_dispatcher(const ds3_client* client, const ds3_request* request, void* read_user_struct, size_t (*read_handler_func)(void*, size_t, size_t, void*), void* write_user_struct, size_t (*write_handler_func)(void*, size_t, size_t, void*)) {
    if (client == NULL || request == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }
    return net_process_request(client, request, read_user_struct, read_handler_func, write_user_struct, write_handler_func, NULL);
}

static bool attribute_equal(const struct _xmlAttr* attribute, const char* attribute_name) {
    return xmlStrcmp(attribute->name, (const xmlChar*) attribute_name) == 0;
}

static bool element_equal(const xmlNodePtr xml_node, const char* element_name) {
    return xmlStrcmp(xml_node->name, (const xmlChar*) element_name) == 0;
}

static uint16_t xml_get_uint16(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    uint16_t size;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    if (text == NULL) {
        return 0;
    }
    size = atoi((char*)text);
    xmlFree(text);
    return size;
}

static uint64_t xml_get_uint16_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_uint16(doc, (xmlNodePtr) attribute);
}

static uint64_t xml_get_uint64(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    uint64_t size;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    if (text == NULL) {
        return 0;
    }
    size = g_ascii_strtoull((const char*)text, NULL, 10);
    xmlFree(text);
    return size;
}

static uint64_t xml_get_uint64_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_uint64(doc, (xmlNodePtr) attribute);
}

static ds3_str* xml_get_string(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    ds3_str* result;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    if (text == NULL) {
        // Element is found, but is empty: <name />
        return NULL;
    }
    result = ds3_str_init((const char*) text);
    xmlFree(text);
    return result;
}

static ds3_str* xml_get_string_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_string(doc, (xmlNodePtr) attribute);
}

static ds3_bool xml_get_bool(const ds3_log* log, xmlDocPtr doc, const xmlNodePtr xml_node) {
    xmlChar* text;
    ds3_bool result;
    text = xmlNodeListGetString(doc, xml_node->xmlChildrenNode, 1);
    if (xmlStrcmp(text, (xmlChar*)"true") == 0) {
        result = True;
    } else if (xmlStrcmp(text, (xmlChar*)"false") == 0) {
        result = False;
    } else {
        ds3_log_message(log, DS3_ERROR, "Unknown boolean value");
        result = False;
    }
    xmlFree(text);
    return result;
}

static uint64_t xml_get_bool_from_attribute(const ds3_log* log, xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_bool(log, doc, (xmlNodePtr) attribute);
}

static void _parse_build_information(const ds3_log* log, xmlDocPtr doc, xmlNodePtr build_info_node, ds3_build_information** _build_info_response) {
    ds3_build_information* build_info_response;
    xmlNodePtr build_element;
    build_info_response = g_new0(ds3_build_information, 1);

    for (build_element = build_info_node->xmlChildrenNode; build_element != NULL; build_element = build_element->next) {
        if (element_equal(build_element, "Branch") == true) {
            build_info_response->branch = xml_get_string(doc, build_element);
        } else if (element_equal(build_element, "Revision")) {
            build_info_response->revision = xml_get_string(doc, build_element);
        } else if (element_equal(build_element, "Version")) {
            build_info_response->version = xml_get_string(doc, build_element);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown element: (%s)\n", build_element->name);
        }
    }
    *_build_info_response = build_info_response;
}

ds3_error* ds3_get_system_information(const ds3_client* client, const ds3_request* request, ds3_get_system_information_response** _response) {
    ds3_get_system_information_response* response;
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr sys_info_node;
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();

    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, "Data") == false) {
        char* message = g_strconcat("Expected the root element to be 'Data'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    response = g_new0(ds3_get_system_information_response, 1);
    for (sys_info_node = root->xmlChildrenNode; sys_info_node != NULL; sys_info_node = sys_info_node->next) {
        if (element_equal(sys_info_node, "BuildInformation") == true) {
            _parse_build_information(client->log, doc, sys_info_node, &response->build_information);
        } else if (element_equal(sys_info_node, "ApiVersion")) {
            response->api_version = xml_get_string(doc, sys_info_node);
        } else if (element_equal(sys_info_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, sys_info_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown xml element: (%s)\b", sys_info_node->name);
        }
    }

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    *_response = response;
    return NULL;
}

ds3_error* ds3_verify_system_health(const ds3_client* client, const ds3_request* request, ds3_verify_system_health_response** _response) {
    ds3_verify_system_health_response* response;
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();

    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, "Data") == false) {
        char* message = g_strconcat("Expected the root element to be 'Data'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    response = g_new0(ds3_verify_system_health_response, 1);
    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "MsRequiredToVerifyDataPlannerHealth") == true) {
            response->ms_required_to_verify_data_planner_health = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown xml element: (%s)\b", child_node->name);
        }
    }

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    *_response = response;
    return NULL;
}

static void _parse_buckets(const ds3_log* log, xmlDocPtr doc, xmlNodePtr buckets_node, ds3_get_service_response* response) {
    xmlNodePtr data_ptr;
    xmlNodePtr curr;
    GArray* array = g_array_new(FALSE, TRUE, sizeof(ds3_bucket));

    for (curr = buckets_node->xmlChildrenNode; curr != NULL; curr = curr->next) {
        ds3_bucket bucket;
        memset(&bucket, 0, sizeof(ds3_bucket));

        for (data_ptr = curr->xmlChildrenNode; data_ptr != NULL; data_ptr = data_ptr->next) {
            if (element_equal(data_ptr, "CreationDate")) {
                bucket.creation_date = xml_get_string(doc, data_ptr);
            } else if (element_equal(data_ptr, "Name")) {
                bucket.name = xml_get_string(doc, data_ptr);
            } else {
                ds3_log_message(log, DS3_ERROR, "Unknown element: (%s)\n", data_ptr->name);
            }
        }
        g_array_append_val(array, bucket);
    }

    response->num_buckets = array->len;
    response->buckets = (ds3_bucket*)array->data;
    g_array_free(array, FALSE);
}

static ds3_owner* _parse_owner(xmlDocPtr doc, xmlNodePtr owner_node) {
    xmlNodePtr child_node;
    xmlChar* text;
    ds3_owner* owner = g_new0(ds3_owner, 1);

    for (child_node = owner_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DisplayName")) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            owner->name = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "ID")) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            owner->id = ds3_str_init((const char*) text);
            xmlFree(text);
        } else {
            fprintf(stderr, "Unknown xml element: (%s)\n", child_node->name);
        }
    }

    return owner;
}

ds3_error* ds3_get_service(const ds3_client* client, const ds3_request* request, ds3_get_service_response** _response) {
    ds3_get_service_response* response;
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();

    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);

    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);

    if (element_equal(root, "ListAllMyBucketsResult") == false) {
        char* message = g_strconcat("Expected the root element to be 'ListAllMyBucketsResult'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    response = g_new0(ds3_get_service_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Buckets") == true) {
            //process buckets here
            _parse_buckets(client->log, doc, child_node, response);
        } else if (element_equal(child_node, "Owner") == true) {
            //process owner here
            ds3_owner * owner = _parse_owner(doc, child_node);
            response->owner = owner;
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown xml element: (%s)\b", child_node->name);
        }
    }

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    *_response = response;
    return NULL;
}

static ds3_object _parse_object(xmlDocPtr doc, xmlNodePtr contents_node) {
    xmlNodePtr child_node;
    xmlChar* text;
    ds3_object object;
    memset(&object, 0, sizeof(ds3_object));

    for (child_node = contents_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Key") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            object.name = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "ETag") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            object.etag= ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "LastModified") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            object.last_modified = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageClass") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            object.storage_class = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "Size") == true) {
            object.size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Owner") == true) {
            ds3_owner* owner = _parse_owner(doc, child_node);
            object.owner = owner;
        } else {
            fprintf(stderr, "Unknown xml element: (%s)\n", child_node->name);
        }
    }

    return object;
}

static ds3_search_object* _parse_search_object(xmlDocPtr doc, xmlNodePtr contents_node) {
    xmlNodePtr child_node;
    xmlChar* text;
    ds3_search_object* object = g_new0(ds3_search_object, 1);

    for (child_node = contents_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text != NULL) {
                object->bucket_id = ds3_str_init((const char*) text);
            }
            xmlFree(text);
        } else if (element_equal(child_node, "Id") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text != NULL) {
                object->id = ds3_str_init((const char*) text);
            }
            xmlFree(text);
        } else if (element_equal(child_node, "Name") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text != NULL) {
                object->name = ds3_str_init((const char*) text);
            }
            xmlFree(text);
        } else if (element_equal(child_node, "CreationDate") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text != NULL) {
                object->last_modified = ds3_str_init((const char*) text);
            }
            xmlFree(text);
        } else if (element_equal(child_node, "StorageClass") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text != NULL) {
                object->storage_class = ds3_str_init((const char*) text);
            }
            xmlFree(text);
        } else if (element_equal(child_node, "Size") == true) {
            object->size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Owner") == true) {
            ds3_owner* owner = _parse_owner(doc, child_node);
            object->owner = owner;
        } else if (element_equal(child_node, "Type") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text != NULL) {
                object->type = ds3_str_init((const char*) text);
            }
            xmlFree(text);
        } else if (element_equal(child_node, "Version") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text != NULL) {
                object->version = ds3_str_init((const char*) text);
            }
            xmlFree(text);
        } else {
            fprintf(stderr, "Unknown xml element: (%s)\n", child_node->name);
        }
    }

    return object;
}

static ds3_str* _parse_common_prefixes(const ds3_log* log, xmlDocPtr doc, xmlNodePtr contents_node) {
    xmlNodePtr child_node;
    ds3_str* prefix = NULL;

    for (child_node = contents_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Prefix") == true) {
            if (prefix) {
                ds3_log_message(log, DS3_WARN, "More than one Prefix found in CommonPrefixes\n");
            } else {
                prefix = xml_get_string(doc, child_node);
            }
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown xml element: %s\n", child_node->name);
        }
    }

    return prefix;
}

ds3_error* ds3_get_bucket(const ds3_client* client, const ds3_request* request, ds3_get_bucket_response** _response) {
    ds3_get_bucket_response* response;
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_error* error;
    GArray* object_array;
    GArray* common_prefix_array;

    if (g_strcmp0(request->path->value, "/") == 0){
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    GByteArray* xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);

    if (element_equal(root, "ListBucketResult") == false) {
        char* message = g_strconcat("Expected the root element to be 'ListBucketsResult'.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        xmlFreeDoc(doc);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    object_array = g_array_new(FALSE, TRUE, sizeof(ds3_object));
    common_prefix_array = g_array_new(FALSE, TRUE, sizeof(ds3_str*));
    response = g_new0(ds3_get_bucket_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Contents") == true) {
            ds3_object object = _parse_object(doc, child_node);
            g_array_append_val(object_array, object);
        } else if (element_equal(child_node, "CreationDate") == true) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsTruncated") == true) {
            response->is_truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Marker") == true) {
            response->marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxKeys") == true) {
            response->max_keys = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name") == true) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Delimiter") == true) {
            response->delimiter = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NextMarker") == true) {
            response->next_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Prefix") == true) {
            response->prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CommonPrefixes") == true) {
            ds3_str* prefix = _parse_common_prefixes(client->log, doc, child_node);
            g_array_append_val(common_prefix_array, prefix);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
        }
    }

    response->objects = (ds3_object*) object_array->data;
    response->num_objects = object_array->len;
    response->common_prefixes = (ds3_str**)common_prefix_array->data;
    response->num_common_prefixes = common_prefix_array->len;
    xmlFreeDoc(doc);
    g_array_free(object_array, FALSE);
    g_array_free(common_prefix_array, FALSE);
    g_byte_array_free(xml_blob, TRUE);
    *_response = response;
    return NULL;
}

static int num_chars_in_ds3_str(const ds3_str* str, char ch) {
    int num_matches = 0;
    int index;

    for (index = 0; index < str->size; index++) {
        if (str->value[index] == '/') {
            num_matches++;
        }
    }

    return num_matches;
}

ds3_error* ds3_head_object(const ds3_client* client, const ds3_request* request, ds3_metadata** _metadata) {
    ds3_error* error;
    ds3_string_multimap* return_headers;
    ds3_metadata* metadata = NULL;

    if (num_chars_in_ds3_str(request->path, '/') < 2){
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }else if(g_ascii_strncasecmp(request->path->value, "//", 2) == 0){
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    error = net_process_request(client, request, NULL, NULL, NULL, NULL, &return_headers);

    if (error == NULL) {
        metadata = _init_metadata(return_headers);
        *_metadata = metadata;
        ds3_string_multimap_free(return_headers);
    }

    return error;
}

ds3_error* ds3_head_bucket(const ds3_client* client, const ds3_request* request) {
    ds3_error* error;

    error = net_process_request(client, request, NULL, NULL, NULL, NULL, NULL);

    return error;
}

ds3_error* ds3_get_object(const ds3_client* client, const ds3_request* request, void* user_data, size_t(*callback)(void*,size_t, size_t, void*)) {
    return _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL);
}

ds3_error* ds3_get_object_with_metadata(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*), ds3_metadata** _metadata) {
    ds3_error* error;
    ds3_string_multimap* return_headers;
    ds3_metadata* metadata;

    error = net_process_request(client, request, user_data, callback, NULL, NULL, &return_headers);

    if (error == NULL) {
        metadata = _init_metadata(return_headers);
        *_metadata = metadata;
        ds3_string_multimap_free(return_headers);
    }

    return error;
}

ds3_error* ds3_put_object(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*)) {
    return _internal_request_dispatcher(client, request, NULL, NULL, user_data, callback);
}

ds3_error* ds3_put_bucket(const ds3_client* client, const ds3_request* request) {
    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_delete_bucket(const ds3_client* client, const ds3_request* request) {
    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_get_objects(const ds3_client* client, const ds3_request* request, ds3_get_objects_response** _response) {
    ds3_get_objects_response* response;
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_error* error;
    GArray* object_array;
    GByteArray* xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);

    if (element_equal(root, "Data") == false) {
        char* message = g_strconcat("Expected the root element to be 'Data'.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        xmlFreeDoc(doc);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    object_array = g_array_new(FALSE, TRUE, sizeof(ds3_search_object*));
    response = g_new0(ds3_get_objects_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3Object") == true) {

            ds3_search_object* object = _parse_search_object(doc, child_node);
            g_array_append_val(object_array, object);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
        }
    }

    response->objects = (ds3_search_object**) object_array->data;
    response->num_objects = object_array->len;
    xmlFreeDoc(doc);

    g_array_free(object_array, FALSE);
    g_byte_array_free(xml_blob, TRUE);
    *_response = response;
    return NULL;
}

static ds3_bulk_object _parse_bulk_object(const ds3_log* log, xmlDocPtr doc, xmlNodePtr object_node) {
    xmlNodePtr child_node;
    struct _xmlAttr* attribute;

    ds3_bulk_object response;
    memset(&response, 0, sizeof(ds3_bulk_object));

    for (attribute = object_node->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Name") == true) {
            response.name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "InCache") == true) {
            response.in_cache = xml_get_bool_from_attribute(log, doc, attribute);
        } else if (attribute_equal(attribute, "Length") == true) {
            response.length = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Offset") == true) {
            response.offset = xml_get_uint64_from_attribute(doc, attribute);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown attribute: (%s)\n", attribute->name);
        }
    }

    for (child_node = object_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_log_message(log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
    }

    return response;
}

static ds3_bulk_object_list* _parse_bulk_objects(const ds3_log* log, xmlDocPtr doc, xmlNodePtr objects_node) {
    xmlNodePtr child_node;
    struct _xmlAttr* attribute;

    ds3_bulk_object_list* response = g_new0(ds3_bulk_object_list, 1);
    GArray* object_array = g_array_new(FALSE, TRUE, sizeof(ds3_bulk_object));

    for (attribute = objects_node->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "ChunkId") == true) {
            response->chunk_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "NodeId") == true) {
            response->node_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkNumber") == true) {
            response->chunk_number = xml_get_uint64_from_attribute(doc, attribute);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown attribute: (%s)\n", attribute->name);
        }

    }

    for (child_node = objects_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object") == true) {
            ds3_bulk_object object = _parse_bulk_object(log, doc, child_node);
            g_array_append_val(object_array, object);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
        }
    }

    response->list = (ds3_bulk_object*) object_array->data;
    response->size = object_array->len;
    g_array_free(object_array, FALSE);
    return response;
}

static ds3_node* _parse_node(const ds3_log* log, xmlDocPtr doc, xmlNodePtr node) {
    xmlNodePtr child_node;
    struct _xmlAttr* attribute;

    ds3_node* response = g_new0(ds3_node, 1);

    for (attribute = node->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "EndPoint") == true) {
            response->endpoint = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Id") == true) {
            response->id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "HttpPort") == true) {
            response->http_port = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "HttpsPort") == true) {
            response->https_port = xml_get_uint16_from_attribute(doc, attribute);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown attribute: (%s)\n", attribute->name);
        }
    }

    for (child_node = node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_log_message(log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
    }

    return response;
}

static ds3_nodes_list* _parse_nodes(const ds3_log* log, xmlDocPtr doc, xmlNodePtr nodes_node) {
    xmlNodePtr child_node;

    ds3_nodes_list* response = g_new0(ds3_nodes_list, 1);
    GPtrArray* nodes_array = g_ptr_array_new();

    for (child_node = nodes_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Node") == true) {
            ds3_node* node = _parse_node(log, doc, child_node);
            g_ptr_array_add(nodes_array, node);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
        }
    }

    response->list = (ds3_node**) nodes_array->pdata;
    response->size = nodes_array->len;

    g_ptr_array_free(nodes_array, FALSE);
    return response;
}

static ds3_job_priority _match_priority(const ds3_log* log, const xmlChar* priority_str) {
    if (xmlStrcmp(priority_str, (const xmlChar*) "CRITICAL") == 0) {
        return CRITICAL;
    } else if (xmlStrcmp(priority_str, (const xmlChar*) "VERY_HIGH") == 0) {
        return VERY_HIGH;
    } else if (xmlStrcmp(priority_str, (const xmlChar*) "HIGH") == 0) {
        return HIGH;
    } else if (xmlStrcmp(priority_str, (const xmlChar*) "NORMAL") == 0) {
        return NORMAL;
    } else if (xmlStrcmp(priority_str, (const xmlChar*) "LOW") == 0) {
        return LOW;
    } else if (xmlStrcmp(priority_str, (const xmlChar*) "BACKGROUND") == 0) {
        return BACKGROUND;
    } else if (xmlStrcmp(priority_str, (const xmlChar*) "MINIMIZED_DUE_TO_TOO_MANY_RETRIES") == 0) {
        return MINIMIZED_DUE_TO_TOO_MANY_RETRIES;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown priority type of '%s'.  Returning LOW to be safe.\n", priority_str);
        return LOW;
    }
}

static ds3_job_request_type _match_request_type(const ds3_log* log, const xmlChar* request_type) {
    if (xmlStrcmp(request_type, (const xmlChar*) "PUT") == 0) {
        return PUT;
    } else if (xmlStrcmp(request_type, (const xmlChar*) "GET") == 0) {
        return GET;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown request type of '%s'.  Returning GET for safety.\n", request_type);
        return GET;
    }
}

static ds3_write_optimization _match_write_optimization(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CAPACITY") == 0) {
        return CAPACITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "PERFORMANCE") == 0) {
        return PERFORMANCE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown write optimization of '%s'.  Returning CAPACITY for safety.\n", text);
        return CAPACITY;
    }
}

static ds3_chunk_ordering _match_chunk_order(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "IN_ORDER") == 0) {
        return IN_ORDER;
    } else if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return NONE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown chunk processing order guaruntee value of '%s'.  Returning IN_ORDER for safety.\n", text);
        return NONE;
    }
}

static ds3_job_status _match_job_status(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "IN_PROGRESS") == 0) {
        return IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "COMPLETED") == 0) {
        return COMPLETED;
    } else if (xmlStrcmp(text, (const xmlChar*) "CANCELED") == 0) {
        return CANCELED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown job status value of '%s'.  Returning IN_PROGRESS for safety.\n", text);
        return IN_PROGRESS;
    }
}

static ds3_tape_state _match_tape_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return TAPE_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return TAPE_STATE_OFFLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_PENDING") == 0) {
        return TAPE_STATE_ONLINE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_IN_PROGRESS") == 0) {
        return TAPE_STATE_ONLINE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "PENDING_INSPECTION") == 0) {
        return TAPE_STATE_PENDING_INSPECTION;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return TAPE_STATE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE") == 0) {
        return TAPE_STATE_DATA_CHECKPOINT_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_MISSING") == 0) {
        return TAPE_STATE_DATA_CHECKPOINT_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTFS_WITH_FOREIGN_DATA") == 0) {
        return TAPE_STATE_LTFS_WITH_FOREIGN_DATA;
    } else if (xmlStrcmp(text, (const xmlChar*) "FOREIGN") == 0) {
        return TAPE_STATE_FOREIGN;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_PENDING") == 0) {
        return TAPE_STATE_IMPORT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_IN_PROGRESS") == 0) {
        return TAPE_STATE_IMPORT_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOST") == 0) {
        return TAPE_STATE_LOST;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAD") == 0) {
        return TAPE_STATE_BAD;
    } else if (xmlStrcmp(text, (const xmlChar*) "SERIAL_NUMBER_MISMATCH") == 0) {
        return TAPE_STATE_SERIAL_NUMBER_MISMATCH;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAD_CODE_MISSING") == 0) {
        return TAPE_STATE_BAD_CODE_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_PENDING") == 0) {
        return TAPE_STATE_FORMAT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_IN_PROGRESS") == 0) {
        return TAPE_STATE_FORMAT_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_TO_EE_IN_PROGRESS") == 0) {
        return TAPE_STATE_EJECT_TO_EE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_FROM_EE_PENDING") == 0) {
        return TAPE_STATE_EJECT_FROM_EE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECTED") == 0) {
        return TAPE_STATE_EJECTED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown tape status value of '%s'.  Returning TAPE_STATE_UNKNOWN for safety.\n", text);
        return TAPE_STATE_UNKNOWN;
    }
}

static ds3_tape_type _match_tape_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "LTO5") == 0) {
        return TAPE_TYPE_LTO5;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO6") == 0) {
        return TAPE_TYPE_LTO6;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO7") == 0) {
        return TAPE_TYPE_LTO7;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO_CLEANING_TAPE") == 0) {
        return TAPE_TYPE_LTO_CLEANING_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JC") == 0) {
        return TAPE_TYPE_TS_JC;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JY") == 0) {
        return TAPE_TYPE_TS_JY;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JK") == 0) {
        return TAPE_TYPE_TS_JK;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JD") == 0) {
        return TAPE_TYPE_TS_JD;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JZ") == 0) {
        return TAPE_TYPE_TS_JZ;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JL") == 0) {
        return TAPE_TYPE_TS_JL;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_CLEANING_TAPE") == 0) {
        return TAPE_TYPE_TS_CLEANING_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return TAPE_TYPE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORBIDDEN") == 0) {
        return TAPE_TYPE_FORBIDDEN;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown tape status value of '%s'.  Returning TAPE_TYPE_UNKNOWN for safety.\n", text);
        return TAPE_TYPE_UNKNOWN;
    }
}

static ds3_error* _parse_bulk_response_attributes(const ds3_log* log, xmlDocPtr doc, xmlNodePtr node, ds3_bulk_response* response) {
    struct _xmlAttr* attribute;
    xmlChar* text;

    for (attribute = node->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "JobId") == true) {
            response->job_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "BucketName") == true) {
            response->bucket_name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "StartDate") == true) {
            response->start_date = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "UserId") == true) {
            response->user_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "UserName") == true) {
            response->user_name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "CachedSizeInBytes") == true) {
            response->cached_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "CompletedSizeInBytes") == true) {
            response->completed_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "OriginalSizeInBytes") == true) {
            response->original_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Priority") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_priority(log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "RequestType") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_request_type(log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "WriteOptimization") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->write_optimization = _match_write_optimization(log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "ChunkClientProcessingOrderGuarantee") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_order = _match_chunk_order(log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "Status") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->status = _match_job_status(log, text);
            xmlFree(text);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown attribute: (%s)", attribute->name);
        }
    }

    return NULL;
}

static ds3_error* _parse_master_object_list(const ds3_log* log, xmlDocPtr doc, ds3_bulk_response** _response) {
    GArray* objects_array;
    xmlNodePtr root, child_node;
    ds3_bulk_response* response;

    root = xmlDocGetRootElement(doc);

    if (element_equal(root, "MasterObjectList") == false) {
        char* message = g_strconcat("Expected the root element to be 'MasterObjectList'.  The actual response is: ", root->name, NULL);
        xmlFreeDoc(doc);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    response = g_new0(ds3_bulk_response, 1);

    _parse_bulk_response_attributes(log, doc, root, response);

    objects_array = g_array_new(FALSE, TRUE, sizeof(ds3_bulk_object_list*));

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Objects")  == true) {
            ds3_bulk_object_list* obj_list = _parse_bulk_objects(log, doc, child_node);
            g_array_append_val(objects_array, obj_list);
        } else if (element_equal(child_node, "Nodes")  == true) {
            response->nodes = _parse_nodes(log, doc, child_node);
        } else {
            ds3_log_message(log, DS3_ERROR, "Unknown element: (%s)", child_node->name);
        }
    }

    response->list = (ds3_bulk_object_list**) objects_array->data;
    response->list_size = objects_array->len;

    g_array_free(objects_array, FALSE);

    *_response = response;
    return NULL;
}

static char* _get_chunk_order_str(ds3_chunk_ordering order) {
    if (order == NONE) {
        return "NONE";
    } else {
        return "IN_ORDER";
    }
}

#define LENGTH_BUFF_SIZE 21

static xmlDocPtr _generate_xml_objects_list(const ds3_bulk_object_list* obj_list, object_list_type list_type, ds3_chunk_ordering order) {
    char size_buff[LENGTH_BUFF_SIZE]; //The max size of an uint64_t should be 20 characters
    xmlDocPtr doc;
    ds3_bulk_object obj;
    xmlNodePtr objects_node, object_node;
    int i;
    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    if (list_type == BULK_DELETE) {
        objects_node = xmlNewNode(NULL, (xmlChar*) "Delete");
    } else {
        objects_node = xmlNewNode(NULL, (xmlChar*) "Objects");
    }

    if (list_type == BULK_GET) {
        xmlSetProp(objects_node, (xmlChar*) "ChunkClientProcessingOrderGuarantee", (xmlChar*) _get_chunk_order_str(order));
    }

    for (i = 0; i < obj_list->size; i++) {
        memset(&obj, 0, sizeof(ds3_bulk_object));
        memset(size_buff, 0, sizeof(char) * LENGTH_BUFF_SIZE);

        obj = obj_list->list[i];
        g_snprintf(size_buff, sizeof(char) * LENGTH_BUFF_SIZE, "%llu", (unsigned long long int) obj.length);

        object_node = xmlNewNode(NULL, (xmlChar*) "Object");
        xmlAddChild(objects_node, object_node);

        if (list_type == BULK_DELETE) {
            xmlNewTextChild(object_node, NULL, (xmlChar*) "Key", (xmlChar*) obj.name->value);
        } else {
            xmlSetProp(object_node, (xmlChar*) "Name", (xmlChar*) obj.name->value);
            if (list_type == BULK_PUT) {
                xmlSetProp(object_node, (xmlChar*) "Size", (xmlChar*) size_buff);
            }
        }
    }

    xmlDocSetRootElement(doc, objects_node);

    return doc;
}

static object_list_type _bulk_request_type(const struct _ds3_request* request) {

    char* value = (char *) g_hash_table_lookup(request->query_params, "operation");

    if (strcmp(value, "start_bulk_get") == 0) {
        return BULK_GET;
    }
    return BULK_PUT;
}

ds3_error* ds3_delete_object(const ds3_client* client, const ds3_request* request) {
    if(g_ascii_strncasecmp(request->path->value, "//", 2) == 0){
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }
    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_delete_objects(const ds3_client* client, const ds3_request* _request, ds3_bulk_object_list *bulkObjList) {
    ds3_error* error_response;
    int buff_size;

    struct _ds3_request* request;
    ds3_bulk_object_list* obj_list;
    ds3_xml_send_buff send_buff;

    GByteArray* xml_blob;

    xmlDocPtr doc;
    xmlChar* xml_buff;

    request = (struct _ds3_request*) _request;
    request->object_list = bulkObjList;

    if (request->object_list == NULL || request->object_list->size == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
    }

    // Init the data structures declared above the null check
    memset(&send_buff, 0, sizeof(ds3_xml_send_buff));
    obj_list = request->object_list;

    // The chunk ordering is not used.  Just pass in NONE.
    doc = _generate_xml_objects_list(obj_list, BULK_DELETE, NONE);

    xmlDocDumpFormatMemory(doc, &xml_buff, &buff_size, 1);

    send_buff.buff = (char*) xml_buff;
    send_buff.size = strlen(send_buff.buff);

    request->length = send_buff.size; // make sure to set the size of the request.

    xml_blob = g_byte_array_new();

    error_response = net_process_request(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Cleanup the data sent to the server.
    xmlFreeDoc(doc);
    xmlFree(xml_buff);

    g_byte_array_free(xml_blob, TRUE);
    return error_response;
}

ds3_error* ds3_delete_folder(const ds3_client* client, const ds3_request* _request) {
    struct _ds3_request* request;
    request = (struct _ds3_request*) _request;
    return net_process_request(client, request, NULL, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_get_physical_placement(const ds3_client* client, const ds3_request* _request, ds3_get_physical_placement_response** _response) {
    ds3_error* error_response;
    ds3_get_physical_placement_response* response = NULL;

    int buff_size;

    struct _ds3_request* request;
    ds3_bulk_object_list* obj_list;
    ds3_xml_send_buff send_buff;

    xmlNodePtr cur, child_node, tape_node;

    GByteArray* xml_blob;

    xmlDocPtr doc;
    xmlChar* xml_buff;

    GArray* tape_array = g_array_new(FALSE, TRUE, sizeof(ds3_tape));
    ds3_tape tape;

    if (client == NULL || _request == NULL) {
        g_array_free(tape_array, TRUE);
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }

    request = (struct _ds3_request*) _request;

    if (request->object_list == NULL || request->object_list->size == 0) {
        g_array_free(tape_array, TRUE);
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
    }

    // Init the data structures declared above the null check
    memset(&send_buff, 0, sizeof(ds3_xml_send_buff));
    obj_list = request->object_list;

    // The chunk ordering is not used.  Just pass in NONE.
    doc = _generate_xml_objects_list(obj_list, GET_PHYSICAL_PLACEMENT, NONE);

    xmlDocDumpFormatMemory(doc, &xml_buff, &buff_size, 1);

    send_buff.buff = (char*) xml_buff;
    send_buff.size = strlen(send_buff.buff);

    request->length = send_buff.size; // make sure to set the size of the request.

    xml_blob = g_byte_array_new();
    error_response = net_process_request(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Cleanup the data sent to the server.
    xmlFreeDoc(doc);
    xmlFree(xml_buff);

    if (error_response != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        g_array_free(tape_array, TRUE);
        return error_response;
    }

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        //Bad result
        g_byte_array_free(xml_blob, TRUE);
        g_array_free(tape_array, TRUE);
        return NULL;
    }

    cur = xmlDocGetRootElement(doc);

    if (element_equal(cur, "Data") == false) {
        char* message = g_strconcat("Expected the root element to be 'Data'.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        g_array_free(tape_array, TRUE);
        xmlFreeDoc(doc);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    cur = cur->xmlChildrenNode;
    if (cur != NULL) {
        if (element_equal(cur, "Tapes") == false) {
            char* message = g_strconcat("Expected the interior element to be 'Tapes'.  The actual response is: ", xml_blob->data, NULL);
            g_byte_array_free(xml_blob, TRUE);
            g_array_free(tape_array, TRUE);
            xmlFreeDoc(doc);
            ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
            g_free(message);
            return error;
        }

        response = g_new0(ds3_get_physical_placement_response, 1);

        for (child_node = cur->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
            if (element_equal(child_node, "Tape") == true) {
                memset(&tape, 0, sizeof(ds3_tape));
                for (tape_node = child_node->xmlChildrenNode; tape_node != NULL; tape_node = tape_node->next) {
                    if (element_equal(tape_node, "AssignedToBucket") == true) {
                        tape.assigned_to_bucket = xml_get_bool(client->log, doc, tape_node);
                    } else if (element_equal(tape_node, "AvailableRawCapacity") == true) {
                        tape.available_raw_capacity = xml_get_uint64(doc, tape_node);
                    } else if (element_equal(tape_node, "BarCode") == true) {
                        tape.barcode = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "BucketId") == true) {
                        tape.bucket_id = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "DescriptionForIdentification") == true) {
                       tape.description = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "EjectDate") == true) {
                        tape.eject_date = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "EjectLabel") == true) {
                        tape.eject_label = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "EjectLocation") == true) {
                        tape.eject_location = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "EjectPending") == true) {
                        tape.eject_pending = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "FullOfData") == true) {
                        tape.full_of_data = xml_get_bool(client->log, doc, tape_node);
                    } else if (element_equal(tape_node, "Id") == true) {
                        tape.id = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "LastAccessed") == true) {
                        tape.last_accessed = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "LastCheckpoint") == true) {
                        tape.last_checkpoint = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "LastModified") == true) {
                        tape.last_modified = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "LastVerified") == true) {
                        tape.last_verified = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "PartitionId") == true) {
                        tape.partition_id = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "PreviousState") == true) {
                        xmlChar* text = xmlNodeListGetString(doc, tape_node, 1);
                        if (text == NULL) {
                            continue;
                        }
                        tape.previous_state = _match_tape_state(client->log, text);
                    } else if (element_equal(tape_node, "SerialNumber") == true) {
                        tape.serial_number = xml_get_string(doc, tape_node);
                    } else if (element_equal(tape_node, "State") == true) {
                        xmlChar* text = xmlNodeListGetString(doc, tape_node, 1);
                        if (text == NULL) {
                            continue;
                        }
                        tape.state = _match_tape_state(client->log, text);
                    } else if (element_equal(tape_node, "TotalRawCapacity") == true) {
                        tape.total_raw_capacity = xml_get_uint64(doc, tape_node);
                    } else if (element_equal(tape_node, "Type") == true) {
                        xmlChar* text = xmlNodeListGetString(doc, tape_node, 1);
                        if (text == NULL) {
                            continue;
                        }
                        tape.type = _match_tape_type(client->log, text);
                    } else if (element_equal(tape_node, "WriteProtected") == true) {
                        tape.write_protected = xml_get_bool(client->log, doc, tape_node);
                    }
                }
                g_array_append_val(tape_array, tape);
            }
        }
        response->num_tapes = tape_array->len;
        response->tapes = (ds3_tape*)tape_array->data;

    }
    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    g_array_free(tape_array, FALSE);

    *_response = response;
    return NULL;
}

ds3_error* ds3_bulk(const ds3_client* client, const ds3_request* _request, ds3_bulk_response** response) {
    ds3_error* error_response;

    int buff_size;

    struct _ds3_request* request;
    ds3_bulk_object_list* obj_list;
    ds3_xml_send_buff send_buff;

    GByteArray* xml_blob;

    xmlDocPtr doc;
    xmlChar* xml_buff;

    if (client == NULL || _request == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }

    request = (struct _ds3_request*) _request;

    if (request->object_list == NULL || request->object_list->size == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
    }


    // Init the data structures declared above the null check
    memset(&send_buff, 0, sizeof(ds3_xml_send_buff));
    obj_list = request->object_list;

    doc = _generate_xml_objects_list(obj_list, _bulk_request_type(_request), _request->chunk_ordering);

    xmlDocDumpFormatMemory(doc, &xml_buff, &buff_size, 1);

    send_buff.buff = (char*) xml_buff;
    send_buff.size = strlen(send_buff.buff);

    request->length = send_buff.size; // make sure to set the size of the request.

    xml_blob = g_byte_array_new();
    error_response = net_process_request(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Cleanup the data sent to the server.
    xmlFreeDoc(doc);
    xmlFree(xml_buff);

    if (error_response != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error_response;
    }

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        // Bulk put with just empty folder objects will return a 204 and thus
        // not have a body.
        g_byte_array_free(xml_blob, TRUE);
        return NULL;
    }

    error_response = _parse_master_object_list(client->log, doc, response);

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);

    if (error_response != NULL) {
        return error_response;
    }

    return NULL;
}

ds3_error* ds3_allocate_chunk(const ds3_client* client, const ds3_request* request, ds3_allocate_chunk_response** response) {
    ds3_error* error = NULL;
    GByteArray* xml_blob = g_byte_array_new();
    ds3_allocate_chunk_response* ds3_response = NULL;
    ds3_bulk_object_list* object_list = NULL;
    ds3_string_multimap* response_headers = NULL;
    ds3_string_multimap_entry* retry_after_header;
    xmlDocPtr doc;
    xmlNodePtr root;

    error = net_process_request(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &response_headers);

    if (error != NULL) {
        ds3_string_multimap_free(response_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    ds3_response = g_new0(ds3_allocate_chunk_response, 1);

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        g_byte_array_free(xml_blob, TRUE);
        ds3_str* retry_str = ds3_str_init("Retry-After");
        retry_after_header = ds3_string_multimap_lookup(response_headers, retry_str);
        if (retry_after_header != NULL) {
            ds3_str* retry_value = ds3_string_multimap_entry_get_value_by_index(retry_after_header, 0);
            ds3_response->retry_after = g_ascii_strtoull(retry_value->value, NULL, 10);
        } else {
            ds3_string_multimap_free(response_headers);
            ds3_str_free(retry_str);
            return ds3_create_error(DS3_ERROR_REQUEST_FAILED, "We did not get a response and did not find the 'Retry-After Header'");
        }
        ds3_string_multimap_free(response_headers);
        ds3_str_free(retry_str);
        return NULL;
    }

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, "Objects")  == true) {
        object_list = _parse_bulk_objects(client->log, doc, root);
        ds3_response->objects = object_list;
    } else {
        char* message = g_strconcat("Expected the root element to be 'Objects'.  The actual response is: ", root->name, NULL);
        xmlFreeDoc(doc);
        error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        g_byte_array_free(xml_blob, TRUE);
        ds3_string_multimap_free(response_headers);
        return error;
    }

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    ds3_string_multimap_free(response_headers);
    *response = ds3_response;
    return NULL;
}

ds3_error* ds3_get_available_chunks(const ds3_client* client, const ds3_request* request, ds3_get_available_chunks_response** response) {
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();
    ds3_get_available_chunks_response* ds3_response;
    ds3_bulk_response* bulk_response;
    ds3_string_multimap* response_headers = NULL;
    ds3_string_multimap_entry* retry_after_header;
    xmlDocPtr doc;

    error = net_process_request(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &response_headers);

    if (error != NULL) {
        ds3_string_multimap_free(response_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    ds3_response = g_new0(ds3_get_available_chunks_response, 1);

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (response_headers != NULL) {
        ds3_str* retry_str = ds3_str_init("Retry-After");
        retry_after_header = ds3_string_multimap_lookup(response_headers, retry_str);
        if (retry_after_header != NULL) {
            ds3_str* retry_value = ds3_string_multimap_entry_get_value_by_index(retry_after_header, 0);
            ds3_response->retry_after = g_ascii_strtoull(retry_value->value, NULL, 10);
            ds3_string_multimap_entry_free(retry_after_header);
            ds3_str_free(retry_value);
        }
        ds3_str_free(retry_str);
    }

    _parse_master_object_list(client->log, doc, &bulk_response);
    ds3_response->object_list = bulk_response;

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    ds3_string_multimap_free(response_headers);
    *response = ds3_response;
    return NULL;
}

static ds3_error* _parse_jobs_list(const ds3_log* log, xmlDocPtr doc, ds3_get_jobs_response** _response) {
    ds3_get_jobs_response* response = NULL;
    xmlNodePtr root, child_node;
    GPtrArray* jobs_array = NULL;
    ds3_bulk_response* job = NULL;
    ds3_error* error = NULL;

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, "Jobs") == false) {
        char* message = g_strconcat("Expected the root element to be 'Jobs'.  The actual response is: ", root->name, NULL);
        xmlFreeDoc(doc);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    jobs_array = g_ptr_array_new();
    response = g_new0(ds3_get_jobs_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Job") == true) {
            job = g_new0(ds3_bulk_response, 1);
            error = _parse_bulk_response_attributes(log, doc, child_node, job);
            if ( error )
            {
              g_ptr_array_free(jobs_array, TRUE);
              ds3_log_message(log, DS3_ERROR, "Error parsing bulk_response element");
              ds3_free_get_jobs_response(response);
              return error;
            } else {
              g_ptr_array_add(jobs_array, job);
            }
        } else {
            // Invalid XML block
            ds3_log_message(log, DS3_ERROR, "Unknown child node: (%s)", child_node->name);
        }
    }

    response->jobs_size = jobs_array->len;
    response->jobs = (ds3_bulk_response**) jobs_array->pdata;
    g_ptr_array_free(jobs_array, FALSE);

    *_response = response;
    return NULL;
}

ds3_error* ds3_get_jobs(const ds3_client* client, const ds3_request* request, ds3_get_jobs_response** response) {
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();
    ds3_get_jobs_response* get_jobs_response = NULL;
    xmlDocPtr doc;

    error = net_process_request(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return ds3_create_error(DS3_ERROR_REQUEST_FAILED, "Unexpected empty response body.");
    }

    _parse_jobs_list(client->log, doc, &get_jobs_response);

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    *response = get_jobs_response;
    return NULL;
}

static ds3_error* _common_job(const ds3_client* client, const ds3_request* request, ds3_bulk_response** response) {
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();
    ds3_bulk_response* bulk_response;
    xmlDocPtr doc;

    error = net_process_request(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return ds3_create_error(DS3_ERROR_REQUEST_FAILED, "Unexpected empty response body.");
    }

    _parse_master_object_list(client->log, doc, &bulk_response);

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);

    *response = bulk_response;
    return NULL;
}

ds3_error* ds3_get_job(const ds3_client* client, const ds3_request* request, ds3_bulk_response** response) {
    return _common_job(client, request, response);
}

ds3_error* ds3_put_job(const ds3_client* client, const ds3_request* request, ds3_bulk_response** response) {
    return _common_job(client, request, response);
}

ds3_error* ds3_delete_job(const ds3_client* client, const ds3_request* request) {
    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

void ds3_free_bucket_response(ds3_get_bucket_response* response) {
    size_t num_objects;
    int index;
    if (response == NULL) {
        return;
    }

    num_objects = response->num_objects;
    for (index = 0; index < num_objects; index++) {
        ds3_object object = response->objects[index];
        ds3_str_free(object.name);
        ds3_str_free(object.etag);
        ds3_str_free(object.storage_class);
        ds3_str_free(object.last_modified);
        ds3_free_owner(object.owner);
    }

    g_free(response->objects);
    ds3_str_free(response->creation_date);
    ds3_str_free(response->marker);
    ds3_str_free(response->delimiter);
    ds3_str_free(response->name);
    ds3_str_free(response->next_marker);
    ds3_str_free(response->prefix);

    if (response->common_prefixes != NULL) {
        for (index = 0; index < response->num_common_prefixes; index++) {
            ds3_str_free(response->common_prefixes[index]);
        }
        g_free(response->common_prefixes);
    }

    g_free(response);
}

void ds3_free_objects_response(ds3_get_objects_response* response) {
    size_t num_objects;
    int object_index;
    if (response == NULL) {
        return;
    }

    num_objects = response->num_objects;
    ds3_search_object* object;
    for (object_index = 0; object_index < num_objects; object_index++) {
        object = response->objects[object_index];
        ds3_str_free(object->bucket_id);
        ds3_str_free(object->id);
        ds3_str_free(object->name);
        ds3_str_free(object->storage_class);
        ds3_str_free(object->last_modified);
        ds3_str_free(object->type);
        ds3_str_free(object->version);
        ds3_free_owner(object->owner);
        g_free(object);
    }

    g_free(response->objects);
    g_free(response);
}

void ds3_free_get_physical_placement_response(ds3_get_physical_placement_response* response) {
    size_t num_tapes;
    int tape_index;
    if (response == NULL) {
        return;
    }

    num_tapes = response->num_tapes;
    for (tape_index = 0; tape_index < num_tapes; tape_index++) {
        ds3_tape tape = response->tapes[tape_index];
        ds3_str_free(tape.barcode);
        ds3_str_free(tape.bucket_id);
        ds3_str_free(tape.description);
        ds3_str_free(tape.eject_date);
        ds3_str_free(tape.eject_label);
        ds3_str_free(tape.eject_location);
        ds3_str_free(tape.eject_pending);
        ds3_str_free(tape.id);
        ds3_str_free(tape.last_accessed);
        ds3_str_free(tape.last_checkpoint);
        ds3_str_free(tape.last_modified);
        ds3_str_free(tape.last_verified);
        ds3_str_free(tape.partition_id);
        ds3_str_free(tape.serial_number);
    }

    g_free(response->tapes);
    g_free(response);
}

void ds3_free_service_response(ds3_get_service_response* response) {
    size_t num_buckets;
    int bucket_index;

    if (response == NULL) {
        return;
    }

    num_buckets = response->num_buckets;

    for (bucket_index = 0; bucket_index < num_buckets; bucket_index++) {
        ds3_bucket bucket = response->buckets[bucket_index];
        ds3_str_free(bucket.name);
        ds3_str_free(bucket.creation_date);
    }

    ds3_free_owner(response->owner);
    g_free(response->buckets);
    g_free(response);
}

void ds3_free_bulk_response(ds3_bulk_response* response) {
    int list_index;
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->job_id);
    ds3_str_free(response->bucket_name);
    ds3_str_free(response->start_date);
    ds3_str_free(response->user_id);
    ds3_str_free(response->user_name);

    if (response->list != NULL) {
        for (list_index = 0; list_index < response->list_size; list_index++) {
            ds3_free_bulk_object_list(response->list[list_index]);
        }
        g_free(response->list);
    }
    if (response->nodes != NULL) {
        ds3_free_nodes_list(response->nodes);
    }

    g_free(response);
}

void ds3_free_get_jobs_response(ds3_get_jobs_response* response) {
    int job_index;

    if (response == NULL) {
        return;
    }

    if (response->jobs != NULL) {
        for (job_index = 0; job_index < response->jobs_size; job_index++) {
            ds3_free_bulk_response(response->jobs[job_index]);
        }
        g_free(response->jobs);
    }

    g_free(response);
}

void ds3_free_owner(ds3_owner* owner) {
    if (owner == NULL) {
        return;
    }

    ds3_str_free(owner->name);
    ds3_str_free(owner->id);
    g_free(owner);
}

void ds3_free_creds(ds3_creds* creds) {
    if (creds == NULL) {
        return;
    }

    ds3_str_free(creds->access_id);
    ds3_str_free(creds->secret_key);
    g_free(creds);
}

void ds3_free_client(ds3_client* client) {
    if (client == NULL) {
      return;
    }

    ds3_str_free(client->endpoint);
    ds3_str_free(client->proxy);
    if (client->log != NULL) {
        g_free(client->log);
    }
    g_free(client);
}

void ds3_free_request(ds3_request* _request) {
    struct _ds3_request* request;
    if (_request == NULL) {
        return;
    }

    request = (struct _ds3_request*) _request;
    if (request->headers != NULL) {
        g_hash_table_destroy(request->headers);
    }
    if (request->query_params != NULL) {
        g_hash_table_destroy(request->query_params);
    }
    ds3_str_free(request->path);
    ds3_str_free(request->checksum);
    g_free(request);
}

void ds3_free_metadata(ds3_metadata* _metadata) {
    struct _ds3_metadata* metadata;
    if (_metadata == NULL) return;

    metadata = (struct _ds3_metadata*) _metadata;

    if (metadata->metadata == NULL) return;

    g_hash_table_destroy(metadata->metadata);

    g_free(metadata);
}

void ds3_free_error(ds3_error* error) {
    if (error == NULL) {
        return;
    }

    ds3_str_free(error->message);

    if (error->error != NULL) {
        ds3_error_response* error_response = error->error;

        ds3_str_free(error_response->status_message);
        ds3_str_free(error_response->error_body);

        g_free(error_response);
    }

    g_free(error);
}

void ds3_cleanup(void) {
    net_cleanup();
    xmlCleanupParser();
}

size_t ds3_write_to_file(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return fwrite(buffer, size, nmemb, (FILE*) user_data);
}

size_t ds3_read_from_file(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return fread(buffer, size, nmemb, (FILE*) user_data);
}

size_t ds3_write_to_fd(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return write(*(int*)user_data, buffer, size * nmemb);
}

size_t ds3_read_from_fd(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return read(*(int*)user_data, buffer, size * nmemb);
}

static ds3_bulk_object _ds3_bulk_object_from_file(const char* file_name, const char* base_path) {
    struct stat file_info;
    int result;
    ds3_bulk_object obj;
    char* file_to_stat;
    memset(&file_info, 0, sizeof(struct stat));
    memset(&obj, 0, sizeof(ds3_bulk_object));

    if (base_path != NULL) {
        file_to_stat = g_strconcat(base_path, file_name, NULL);
    } else {
        file_to_stat = g_strdup(file_name);
    }

    result = stat(file_to_stat, &file_info);
    if (result != 0) {
        fprintf(stderr, "Failed to get file info for %s\n", file_name);
    }

    memset(&obj, 0, sizeof(ds3_bulk_object));

    obj.name = ds3_str_init(file_name);

    if (S_ISDIR(file_info.st_mode)) {
        obj.length = 0;
    } else {
      obj.length = file_info.st_size;
    }

    g_free(file_to_stat);

    return obj;
}

ds3_bulk_object_list* ds3_convert_file_list(const char** file_list, uint64_t num_files) {
    return ds3_convert_file_list_with_basepath(file_list, num_files, NULL);
}

ds3_bulk_object_list* ds3_convert_file_list_with_basepath(const char** file_list, uint64_t num_files, const char* base_path) {
    uint64_t file_index;
    ds3_bulk_object_list* obj_list = ds3_init_bulk_object_list(num_files);

    for (file_index = 0; file_index < num_files; file_index++) {
        obj_list->list[file_index] = _ds3_bulk_object_from_file(file_list[file_index], base_path);
    }

    return obj_list;
}

ds3_bulk_object_list* ds3_convert_object_list(const ds3_object* objects, uint64_t num_objects) {
    uint64_t object_index;
    ds3_bulk_object_list* obj_list = ds3_init_bulk_object_list(num_objects);

    for (object_index = 0; object_index < num_objects; object_index++) {
        ds3_bulk_object obj;
        memset(&obj, 0, sizeof(ds3_bulk_object));
        obj.name = ds3_str_dup(objects[object_index].name);
        obj_list->list[object_index] = obj;
    }

    return obj_list;
}

ds3_bulk_object_list* ds3_init_bulk_object_list(uint64_t num_files) {
    ds3_bulk_object_list* obj_list = g_new0(ds3_bulk_object_list, 1);
    obj_list->size = num_files;
    obj_list->list = g_new0(ds3_bulk_object, num_files);

    return obj_list;
}

void ds3_free_bulk_object_list(ds3_bulk_object_list* object_list) {
    uint64_t list_index, count;
    if (object_list == NULL) {
        return;
    }

    count = object_list->size;
    for (list_index = 0; list_index < count; list_index++) {
        ds3_str_free(object_list->list[list_index].name);
    }

    ds3_str_free(object_list->server_id);
    ds3_str_free(object_list->node_id);
    ds3_str_free(object_list->chunk_id);

    g_free(object_list->list);
    g_free(object_list);
}

void ds3_free_nodes_list(ds3_nodes_list* nodes_list) {
    uint64_t list_index;
    if (nodes_list == NULL || nodes_list->list == NULL) {
        return;
    }

    for (list_index = 0; list_index < nodes_list->size; list_index++) {
        ds3_node* current_node = nodes_list->list[list_index];

        ds3_str_free(current_node->endpoint);
        ds3_str_free(current_node->id);
        g_free(current_node);
    }

    g_free(nodes_list->list);
    g_free(nodes_list);
}

void ds3_free_allocate_chunk_response(ds3_allocate_chunk_response* response) {
    if (response == NULL) {
        return;
    }

    if (response->objects != NULL) {
        ds3_free_bulk_object_list(response->objects);
    }

    g_free(response);
}

void ds3_free_available_chunks_response(ds3_get_available_chunks_response* response) {
    if (response == NULL) {
        return;
    }

    if (response->object_list != NULL) {
        ds3_free_bulk_response(response->object_list);
    }

    g_free(response);
}

void ds3_free_metadata_entry(ds3_metadata_entry* entry) {
    int value_index;
    ds3_str* value;
    if (entry->name != NULL) {
        ds3_str_free(entry->name);
    }
    if (entry->values != NULL) {
        for (value_index = 0; value_index < entry->num_values; value_index++) {
            value = entry->values[value_index];
            ds3_str_free(value);
        }
        g_free(entry->values);
    }
    g_free(entry);
}

void ds3_free_metadata_keys(ds3_metadata_keys_result* metadata_keys) {
    uint64_t key_index;
    if (metadata_keys == NULL) {
        return;
    }

    if (metadata_keys->keys != NULL) {
        for (key_index = 0; key_index < metadata_keys->num_keys; key_index++) {
            ds3_str_free(metadata_keys->keys[key_index]);
        }
        g_free(metadata_keys->keys);
    }
    g_free(metadata_keys);
}

void ds3_free_build_information(ds3_build_information* build_info) {
    if (build_info == NULL) {
        return;
    }

    ds3_str_free(build_info->branch);
    ds3_str_free(build_info->revision);
    ds3_str_free(build_info->version);

    g_free(build_info);
}

void ds3_free_get_system_information(ds3_get_system_information_response* system_info) {
    if (system_info == NULL) {
        return;
    }

    ds3_str_free(system_info->api_version);
    ds3_str_free(system_info->serial_number);
    ds3_free_build_information(system_info->build_information);

    g_free(system_info);
}

void ds3_free_verify_system_health(ds3_verify_system_health_response* response) {
    if (response == NULL) {
        return;
    }

    g_free(response);
}
