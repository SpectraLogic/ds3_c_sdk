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
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#include "ds3.h"

//---------- Define opaque struct ----------//
struct _ds3_request{
    http_verb   verb;
    ds3_str*    path;
    uint64_t    length;
    ds3_str*    md5;
    GHashTable* headers;
    GHashTable* query_params;

    //These next few elements are only for the bulk commands
    ds3_bulk_object_list* object_list;
    ds3_chunk_ordering chunk_ordering;
};

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

typedef struct {
    // These attributes are used when processing a response header
    uint64_t status_code;
    ds3_str* status_message;
    size_t header_count;
    GHashTable* headers;

    // These attributes are used when processing a response body
    GByteArray* body; // this will only be used when getting errors
    void* user_data;
    size_t (*user_func)(void*, size_t, size_t, void*);
}ds3_response_data;

typedef struct {
    ds3_str* key;
    GPtrArray* values; // A ds3_str list of the header values
}ds3_response_header;

static void LOG(const ds3_log* log, ds3_log_lvl lvl, const char* message, ...) {
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
 * This copies all the header values in the ds3_response_header struct so that they may be safely returned to the user
 * without having to worry about if the data is freed internally.
 */
static ds3_metadata_entry* ds3_metadata_entry_init(ds3_response_header* header) {
    uint i;
    ds3_str* header_value;
    GPtrArray* values = g_ptr_array_new();
    ds3_str* key_name;
    ds3_metadata_entry* response = g_new0(ds3_metadata_entry, 1);

    for (i = 0; i < header->values->len; i++) {
        header_value = g_ptr_array_index(header->values, i);
        g_ptr_array_add(values, ds3_str_dup(header_value));
    }

    key_name = ds3_str_init(header->key->value + 11);

    response->num_values = values->len;
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
static ds3_metadata* _init_metadata(GHashTable* response_headers) {
    struct _ds3_metadata* metadata = g_new0(struct _ds3_metadata, 1);
    GHashTableIter iter;
    gpointer _key, _value;
    char* key;
    ds3_response_header* header;
    ds3_metadata_entry* entry;
    metadata->metadata = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, _ds3_free_metadata_entry);

    if (response_headers == NULL) {
        fprintf(stderr, "response headers was null\n");
    }

    g_hash_table_iter_init(&iter, response_headers);

    while(g_hash_table_iter_next(&iter, &_key, &_value)) {
        key = (char*) _key;
        header = (ds3_response_header*) _value;
        if (g_str_has_prefix(key, "x-amz-meta-")) {
            entry = ds3_metadata_entry_init(header);
            g_hash_table_insert(metadata->metadata, g_strdup(entry->name->value), entry);
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

    for(i = 0; i < orig->num_values; i++) {
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
        g_ptr_array_add(return_keys, ds3_str_init(tmp_key->data));
        tmp_key = tmp_key->next;
    }

    g_list_free(keys);
    result->num_keys = return_keys->len;
    result->keys = (ds3_str**) g_ptr_array_free(return_keys, FALSE);
    return result;
}

ds3_str* ds3_str_init(const char* string) {
    size_t size = strlen(string);
    return ds3_str_init_with_size(string, size);
}

ds3_str* ds3_str_init_with_size(const char* string, size_t size) {
    ds3_str* str = g_new0(ds3_str, 1);
    str->value = g_strndup(string, size);
    str->size = size;
    return str;
}

ds3_str* ds3_str_dup(const ds3_str* string) {
    ds3_str* str = g_new0(ds3_str, 1);
    str->value = g_strdup(string->value);
    str->size = string->size;
    return str;
}

char* ds3_str_value(const ds3_str* string) {
    return string->value;
}

size_t ds3_str_size(const ds3_str* string){
    return string->size;
}

void ds3_str_free(ds3_str* string) {
    if (string == NULL) return;

    if (string->value != NULL) {
        g_free(string->value);
    }
    g_free(string);
}

/* This is used to free the entires in the values ptr array in the ds3_response_header
 */
static void _ds3_internal_str_free(gpointer data) {
    ds3_str_free((ds3_str*)data);
}

static void _ds3_free_response_header(gpointer data) {
    ds3_response_header* header;
    if (data == NULL) {
        return;
    }

    header = (ds3_response_header*) data;
    ds3_str_free(header->key);
    g_ptr_array_free(header->values, TRUE);
    g_free(data);
}

static ds3_str* _ds3_response_header_get_first(const ds3_response_header* header) {
    return g_ptr_array_index(header->values, 0);
}

static ds3_response_header* _ds3_init_response_header(const ds3_str* key) {
    ds3_response_header* header = g_new0(ds3_response_header, 1);
    header->key = ds3_str_dup(key);
    header->values = g_ptr_array_new_with_free_func(_ds3_internal_str_free);
    return header;
}

// caller frees all passed in values
static void _insert_header(GHashTable* headers, const ds3_str* key, const ds3_str* value) {
    ds3_response_header* header = g_hash_table_lookup(headers, key->value);

    if (header == NULL) {
        header = _ds3_init_response_header(key);
        g_hash_table_insert(headers, g_strdup(key->value), header);
    }

    g_ptr_array_add(header->values, ds3_str_dup(value));
}

static ds3_error* _ds3_create_error(ds3_error_code code, const char * message) {
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

static size_t load_buffer(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t realsize = size * nmemb;
    GByteArray* blob = (GByteArray*) user_data;

    g_byte_array_append(blob, (const guint8 *) buffer, realsize);
    return realsize;
}

static size_t _process_header_line(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t to_read;
    char* header_buff;
    char** split_result;
    ds3_str* header_key;
    ds3_str* header_value;
    ds3_response_data* response_data = (ds3_response_data*) user_data;
    GHashTable* headers = response_data->headers;

    to_read = size * nmemb;
    if (to_read < 2) {
        return 0;
    }

    header_buff = g_new0(char, to_read+1); //+1 for the null byte
    strncpy(header_buff, (char*)buffer, to_read);
    header_buff = g_strchomp(header_buff);

    // If we have read all the headers, then the last line will only be \n\r
    if (strlen(header_buff) == 0) {
        g_free(header_buff);
        return to_read;
    }

    if (response_data->header_count < 1) {
        if (g_str_has_prefix(header_buff, "HTTP/1.1") == TRUE) {
            // parse out status code and the status string
            char* endpointer;
            uint64_t status_code;
            split_result = g_strsplit(header_buff, " ", 1000);
            status_code = g_ascii_strtoll(split_result[1], &endpointer, 10);
            if (status_code == 0 && endpointer != NULL) {
                fprintf(stderr, "Encountered a problem parsing the status code\n");
                g_strfreev(split_result);
                g_free(header_buff);
                return 0;
            }
            if (status_code == 100) {
                g_free(header_buff);
                g_strfreev(split_result);
                return to_read;
            } else {
                char* status_message = g_strjoinv(" ", split_result + 2);
                response_data->status_code = status_code;
                response_data->status_message = ds3_str_init(status_message);
                g_free(status_message);
                g_strfreev(split_result);
            }
        } else {
            fprintf(stderr, "Unsupported Protocol\n");
            g_free(header_buff);
            return 0;
        }
    } else {
        split_result = g_strsplit(header_buff, ": ", 2);
        header_key = ds3_str_init(split_result[0]);
        header_value = ds3_str_init(split_result[1]);

        _insert_header(headers, header_key, header_value);

        ds3_str_free(header_key);
        ds3_str_free(header_value);
        g_strfreev(split_result);
    }
    response_data->header_count++;
    g_free(header_buff);
    return to_read;
}

static size_t _process_response_body(void* buffer, size_t size, size_t nmemb, void* user_data) {
    ds3_response_data* response_data = (ds3_response_data*) user_data;

    // If we got an error, collect the error body
    if (response_data->status_code >= 300) {
        return load_buffer(buffer, size, nmemb, response_data->body);
    } else { // If we did not get an error, call the user's defined callbacks.
        return response_data->user_func(buffer, size, nmemb, response_data->user_data);
    }
}

//---------- Networking code ----------//
static void _init_curl(void) {
    static ds3_bool initialized = False;

    if (!initialized) {
        if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
          fprintf(stderr, "Encountered an error initializing libcurl\n");
        }
        initialized = True;
    }
}

static char* _net_get_verb(http_verb verb) {
    switch(verb) {
        case HTTP_GET: return "GET";
        case HTTP_PUT: return "PUT";
        case HTTP_POST: return "POST";
        case HTTP_DELETE : return "DELETE";
        case HTTP_HEAD : return "HEAD";
    }

    return NULL;
}

static char* _get_object_type(const object_type type) {
    switch(type) {
        case DATA: return "DATA";
        case NO_TYPE: return NULL;
    }

    return NULL;
}

// curl_easy_escape'd strings must be freed using curl_free.  Copy
// the escaped string, using glib, since users of this function will
// eventually wind up freeing it with g_free.
static char* _escape_url(const char* url) {
    char* curl_escaped_url = curl_easy_escape(NULL, url, 0);
    char* escaped_url = g_strdup(curl_escaped_url);
    curl_free(curl_escaped_url);
    return escaped_url;
}

// Like _escape_url but don't encode "/".
static char* _escape_url_object_name(const char* url) {
    gchar** split = g_strsplit(url, "/", 0);
    gchar** ptr;
    gchar* escaped_ptr;
    for (ptr = split; *ptr; ptr++) {
        escaped_ptr = _escape_url(*ptr);
        g_free(*ptr);
        *ptr = escaped_ptr;
    }
    escaped_ptr = g_strjoinv("/", split);
    g_strfreev(split);
    return escaped_ptr;
}

static unsigned char* _generate_signature_str(http_verb verb, char* resource_name, char* date,
                               char* content_type, char* md5, char* amz_headers) {
    char* verb_str;
    if (resource_name == NULL) {
        fprintf(stderr, "resource_name is required\n");
        return NULL;
    }
    if (date == NULL) {
        fprintf(stderr, "date is required");
        return NULL;
    }
    verb_str = _net_get_verb(verb);

    return (unsigned char*) g_strconcat(verb_str, "\n", md5, "\n", content_type, "\n", date, "\n", amz_headers, resource_name, NULL);
}

static char* _generate_date_string(void) {
    GDateTime* time  = g_date_time_new_now_local();
    char* date_string = g_date_time_format(time, "%a, %d %b %Y %H:%M:%S %z");

    g_date_time_unref(time);

    return date_string;
}

static char* _net_compute_signature(const ds3_log* log, const ds3_creds* creds, http_verb verb, char* resource_name,
                             char* date, char* content_type, char* md5, char* amz_headers) {
    GHmac* hmac;
    gchar* signature;
    gsize bufSize = 256;
    guint8 buffer[256];

    unsigned char* signature_str = _generate_signature_str(verb, resource_name, date, content_type, md5, amz_headers);
    char* escaped_str = g_strescape((char*) signature_str, NULL);

    LOG(log, DS3_DEBUG, "signature string: %s", escaped_str);
    g_free(escaped_str);

    hmac = g_hmac_new(G_CHECKSUM_SHA1, (unsigned char*) creds->secret_key->value, creds->secret_key->size);
    g_hmac_update(hmac, signature_str, strlen((const char*)signature_str));
    g_hmac_get_digest(hmac, buffer, &bufSize);

    signature = g_base64_encode(buffer, bufSize);

    g_free(signature_str);
    g_hmac_unref(hmac);

    return signature;
}

typedef struct {
    char** entries;
    size_t size;
}query_entries;

static void _hash_for_each(gpointer _key, gpointer _value, gpointer _user_data) {
    char* key = (char*) _key;
    char* value = (char*) _value;
    query_entries* entries = (query_entries*) _user_data;
    if (value == NULL) {
        entries->entries[entries->size] = g_strconcat(key, NULL);
    } else {
        entries->entries[entries->size] = g_strconcat(key, "=", value, NULL);
    }
    entries->size++;
}

static char* _net_gen_query_params(GHashTable* query_params) {
    if (g_hash_table_size(query_params) > 0) {
        query_entries q_entries;
        char** entries;
        char* return_string;
        int i;
        //build the query string
        memset(&q_entries, 0, sizeof(query_entries));

        //We need the +1 so that it is NULL terminating for g_strjoinv
        entries = g_new0(char*, g_hash_table_size(query_params) + 1);
        q_entries.entries = entries;
        g_hash_table_foreach(query_params, _hash_for_each, &q_entries);

        return_string = g_strjoinv("&", entries);

        for (i = 0; ; i++) {
            char* current_string = entries[i];
            if (current_string == NULL) {
                break;
            }
            g_free(current_string);
        }

        g_free(entries);
        return return_string;
    } else {
        return NULL;
    }
}

static struct curl_slist* _append_headers(struct curl_slist* header_list, GHashTable* headers_map) {
    GHashTableIter iter;
    gpointer key, value;
    struct curl_slist* updated_list = header_list;
    g_hash_table_iter_init(&iter, headers_map);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char* header_value = g_strconcat((char*)key, ": ", (char*)value, NULL);
        updated_list = curl_slist_append(updated_list, header_value);
        g_free(header_value);
    }
    return updated_list;
}

static int ds3_curl_logger(CURL *handle, curl_infotype type, char* data, size_t size, void* userp) {
    char* text = "curl_log";
    ds3_log* log = (ds3_log*) userp;
    char* message;
    switch(type) {
        case CURLINFO_HEADER_OUT:
          text = "HEADER_SENT";
          break;
        case CURLINFO_HEADER_IN:
          text = "HEADER_RECV";
          break;

        case CURLINFO_DATA_IN:
        case CURLINFO_DATA_OUT:
        case CURLINFO_SSL_DATA_IN:
        case CURLINFO_SSL_DATA_OUT:
          // do not log any payload data
          return 0;
        default:
          break;
    }

    message = g_strndup(data, size);

    LOG(log, DS3_TRACE, "%s: %s", text, g_strchomp(message));

    g_free(message);
    return 0;
}

static gint _gstring_sort(gconstpointer a, gconstpointer b) {
    char** val1 = (char**)a;
    char** val2 = (char**)b;

    return g_strcmp0(*val1, *val2);
}

static char* _canonicalize_amz_headers(GHashTable* headers) {
    GList* keys = g_hash_table_get_keys(headers);
    GList* key = keys;
    GString* canonicalized_headers = g_string_new("");
    GPtrArray *signing_strings = g_ptr_array_new_with_free_func(g_free);  // stores char*
    GString* header_signing_value;
    char* signing_value;
    int i;

    while(key != NULL) {
        if(g_str_has_prefix((char*)key->data, "x-amz")){
            header_signing_value = g_string_new(key->data);
            header_signing_value = g_string_append(g_string_ascii_down(header_signing_value), ":");
            header_signing_value = g_string_append(header_signing_value, g_hash_table_lookup(headers, key->data));

            signing_value = g_string_free(header_signing_value, FALSE);
            g_ptr_array_add(signing_strings, signing_value);
        }
        key = key->next;
    }

    g_ptr_array_sort(signing_strings, _gstring_sort);

    for (i = 0; i < signing_strings->len; i++) {
        g_string_append(canonicalized_headers, g_ptr_array_index(signing_strings, i));
        g_string_append(canonicalized_headers, "\n");
    }

    g_list_free(keys);
    g_ptr_array_free(signing_strings, TRUE);

    return g_string_free(canonicalized_headers, FALSE);
}

static char* _canonicalized_resource(ds3_str* path, GHashTable* query_params) {
    if (g_hash_table_contains(query_params, "delete")) {
        return g_strconcat(path->value, "?delete", NULL);
    } else {
        return g_strdup(path->value);
    }
}

static ds3_error* _net_process_request(const ds3_client* client, const ds3_request* _request, void* read_user_struct, size_t (*read_handler_func)(void*, size_t, size_t, void*), void* write_user_struct, size_t (*write_handler_func)(void*, size_t, size_t, void*), GHashTable** return_headers) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    CURL* handle;
    CURLcode res;
    char* url;
    int retry_count = 0;
    char* query_params;

    _init_curl();

    query_params = _net_gen_query_params(request->query_params);

    if (query_params == NULL) {
        url = g_strconcat(client->endpoint->value, request->path->value, NULL);
    } else {
        url = g_strconcat(client->endpoint->value, request->path->value,"?",query_params, NULL);
        g_free(query_params);
    }

    while (retry_count < client->num_redirects) {
        handle = curl_easy_init();

        if (handle) {
            char* amz_headers;
            char* canonicalized_resource;
            char* date;
            char* date_header;
            char* signature;
            struct curl_slist* headers;
            char* auth_header;
            char* md5_value;
            ds3_response_data response_data;
            GHashTable* response_headers = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, _ds3_free_response_header);

            LOG(client->log, DS3_DEBUG, "Preparing to send request");

            memset(&response_data, 0, sizeof(ds3_response_data));
            response_data.headers = response_headers;
            response_data.body = g_byte_array_new();

            if (client->log != NULL) {
                curl_easy_setopt(handle, CURLOPT_DEBUGFUNCTION, ds3_curl_logger);
                curl_easy_setopt(handle, CURLOPT_DEBUGDATA, client->log);
                curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L); // turn on verbose logging
            }

            curl_easy_setopt(handle, CURLOPT_URL, url);

            curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 0); // explicitly disable

            // Setup header collection
            curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, _process_header_line);
            curl_easy_setopt(handle, CURLOPT_HEADERDATA, &response_data);

            if (client->proxy != NULL) {
              curl_easy_setopt(handle, CURLOPT_PROXY, client->proxy->value);
            }

            // Register the read and write handlers if they are set
            if (read_user_struct != NULL && read_handler_func != NULL) {
                response_data.user_data = read_user_struct;
                response_data.user_func = read_handler_func;
            }

            // We must always set this so we can collect the error message body
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, _process_response_body);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response_data);

            if (write_user_struct != NULL && write_handler_func != NULL) {
                curl_easy_setopt(handle, CURLOPT_READFUNCTION, write_handler_func);
                curl_easy_setopt(handle, CURLOPT_READDATA, write_user_struct);
            }

            switch(request->verb) {
                case HTTP_POST: {
                    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
                    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, request->length);
                    break;
                }
                case HTTP_PUT: {
                    curl_easy_setopt(handle, CURLOPT_PUT, 1L);
                    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, request->length);
                    break;
                }
                case HTTP_DELETE: {
                    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "DELETE");
                    break;
                }
                case HTTP_HEAD: {
                    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "HEAD");
                    curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);
                    break;
                }
                case HTTP_GET: {
                    //Placeholder if we need to put anything here.
                    break;
                }
            }

            date = _generate_date_string();
            date_header = g_strconcat("Date: ", date, NULL);
            headers = NULL;

            if (request->md5 == NULL) {
                md5_value = "";
            } else {
                char* md5_header;
                md5_value = request->md5->value;
                md5_header = g_strconcat("Content-MD5:", md5_value, NULL);
                headers = curl_slist_append(headers, md5_header);
                g_free(md5_header);
            }
            amz_headers = _canonicalize_amz_headers(request->headers);
            canonicalized_resource = _canonicalized_resource(request->path, request->query_params);
            signature = _net_compute_signature(client->log, client->creds, request->verb, canonicalized_resource, date, "", md5_value, amz_headers);

            g_free(amz_headers);
            g_free(canonicalized_resource);

            auth_header = g_strconcat("Authorization: AWS ", client->creds->access_id->value, ":", signature, NULL);

            headers = curl_slist_append(headers, auth_header);
            headers = curl_slist_append(headers, date_header);
            headers = _append_headers(headers, request->headers);

            curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

            res = curl_easy_perform(handle);

            g_free(date);
            g_free(date_header);
            g_free(signature);
            g_free(auth_header);
            curl_slist_free_all(headers);
            curl_easy_cleanup(handle);

            //process the response
            if (res != CURLE_OK) {
                char * message = g_strconcat("Request failed: ", curl_easy_strerror(res), NULL);
                ds3_error* error = _ds3_create_error(DS3_ERROR_REQUEST_FAILED, message);
                g_free(url);
                g_byte_array_free(response_data.body, TRUE);
                ds3_str_free(response_data.status_message);
                g_hash_table_destroy(response_headers);
                g_free(message);
                return error;
            }

            LOG(client->log, DS3_DEBUG, "Request completed with status code of: %d", response_data.status_code);

            if (response_data.status_code == 307) {
                LOG(client->log, DS3_INFO, "Request encountered a 307 redirect");
                ds3_str_free(response_data.status_message);

                if (response_data.body != NULL) {
                    g_byte_array_free(response_data.body, TRUE);
                }
                g_hash_table_destroy(response_headers);
                retry_count++;
                LOG(client->log, DS3_DEBUG, "Retry Attempt: %d | Max Retries: %d", retry_count, client->num_redirects);
                continue;
            }

            if (response_data.status_code < 200 || response_data.status_code >= 300) {
                ds3_error* error = _ds3_create_error(DS3_ERROR_BAD_STATUS_CODE, "Got an unexpected status code.");
                error->error = g_new0(ds3_error_response, 1);
                error->error->status_code = response_data.status_code;
                error->error->status_message = ds3_str_init(response_data.status_message->value);
                if (response_data.body != NULL) {
                    error->error->error_body = ds3_str_init_with_size((char*)response_data.body->data, response_data.body->len);
                    g_byte_array_free(response_data.body, TRUE);
                } else {
                    LOG(client->log, DS3_ERROR, "The response body for the error is empty");
                    error->error->error_body = NULL;
                }
                g_hash_table_destroy(response_headers);
                ds3_str_free(response_data.status_message);
                g_free(url);
                return error;
            }

            g_byte_array_free(response_data.body, TRUE);
            ds3_str_free(response_data.status_message);
            if (return_headers == NULL) {
                g_hash_table_destroy(response_headers);
            } else {
                *return_headers = response_headers;
            }

            break;
        } else {
            return _ds3_create_error(DS3_ERROR_CURL_HANDLE, "Failed to create curl handle");
        }
    }
    g_free(url);

    if (retry_count == client->num_redirects) {
      return _ds3_create_error(DS3_ERROR_TOO_MANY_REDIRECTS, "Encountered too many redirects while attempting to fullfil the request");
    }
    return NULL;
}

static void net_cleanup(void) {
    curl_global_cleanup();
}

static void _cleanup_hash_value(gpointer value) {
    g_free(value);
}

//---------- Ds3 code ----------//
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
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "Missing enviornment variable 'DS3_ENDPOINT'");
    }

    if (access_key == NULL) {
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "Missing enviornment variable 'DS3_ACCESS_KEY'");
    }

    if (secret_key == NULL) {
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "Missing enviornment variable 'DS3_SECRET_KEY'");
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
    gpointer escaped_key = (gpointer) _escape_url(key);

    //TODO update this to handle multiple values being set for a header field
    gpointer escaped_value;
    if (value != NULL) {
        escaped_value = (gpointer) _escape_url(value);
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

void ds3_request_set_custom_header(ds3_request* _request, const char* header_name, const char* header_value) {
   _set_header(_request, header_name, header_value);
}

void ds3_request_set_md5(ds3_request* _request, const char* md5) {
  struct _ds3_request* request = (struct _ds3_request*) _request;
  request->md5 = ds3_str_init(md5);
}

void ds3_request_set_delimiter(ds3_request* _request, const char* delimiter) {
    _set_query_param(_request, "delimiter", delimiter);
}

void ds3_request_set_marker(ds3_request* _request, const char* marker) {
    _set_query_param(_request, "marker", marker);
}

void ds3_request_set_max_keys(ds3_request* _request, uint32_t max_keys) {
    char max_keys_s[11];
    memset(max_keys_s, 0, sizeof(char) * 11);
    g_snprintf(max_keys_s, sizeof(char) * 11, "%u", max_keys);
    _set_query_param(_request, "max-keys", max_keys_s);
}

void ds3_request_set_name(ds3_request* _request, const char* name) {
    _set_query_param(_request, "name", name);
}

void ds3_request_set_id(ds3_request* _request, const char* id) {
    _set_query_param(_request, "id", id);
}

void ds3_request_set_type(ds3_request* _request, object_type type) {
    const char* type_as_string = _get_object_type(type);
    if(type_as_string != NULL) {
        _set_query_param(_request, "type", type_as_string);
    }
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
        escaped_bucket_name = _escape_url(bucket_name);
    }
    if (object_name != NULL) {
        escaped_object_name = _escape_url_object_name(object_name);
    }

    joined_path = g_strjoin("/", escaped_bucket_name, escaped_object_name, NULL);
    full_path = g_strconcat(path_prefix, joined_path, NULL);

    path = ds3_str_init(full_path);

    g_free(full_path);
    g_free(joined_path);

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

ds3_request* ds3_init_get_service(void) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path( "/", NULL, NULL));
}

ds3_request* ds3_init_get_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_head_object(const char* bucket_name, const char* object_name) {
    return (ds3_request*) _common_request_init(HTTP_HEAD, _build_path("/", bucket_name, object_name));
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

ds3_request* ds3_init_get_jobs(void){
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

ds3_request* ds3_init_get_objects(const char* bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, ds3_str_init("/_rest_/object/"));

    _set_query_param((ds3_request*) request, "bucket_id", bucket_name);

    return (ds3_request*) request;
}


static ds3_error* _internal_request_dispatcher(const ds3_client* client, const ds3_request* request, void* read_user_struct, size_t (*read_handler_func)(void*, size_t, size_t, void*), void* write_user_struct, size_t (*write_handler_func)(void*, size_t, size_t, void*)) {
    if (client == NULL || request == NULL) {
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }
    return _net_process_request(client, request, read_user_struct, read_handler_func, write_user_struct, write_handler_func, NULL);
}

static bool attribute_equal(const struct _xmlAttr* attribute, const char* attribute_name){
    return xmlStrcmp(attribute->name, (const xmlChar*) attribute_name) == 0;
}

static bool element_equal(const xmlNodePtr xml_node, const char* element_name){
    return xmlStrcmp(xml_node->name, (const xmlChar*) element_name) == 0;
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

static ds3_str* xml_get_string(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    ds3_str* result;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    if (NULL == text){
        // Element is found, but is empty: <name />
        return NULL;
    }
    result = ds3_str_init((const char*) text);
    xmlFree(text);
    return result;
}

static uint64_t xml_get_uint64_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_uint64(doc, (xmlNodePtr) attribute);
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
        LOG(log, DS3_ERROR, "Unknown boolean value");
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
            LOG(log, DS3_ERROR, "Unknown element: (%s)\n", build_element->name);
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

    error = _internal_request_dispatcher(client, request, xml_blob, load_buffer, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, "Data") == false) {
        char* message = g_strconcat("Expected the root element to be 'Data'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
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
            LOG(client->log, DS3_ERROR, "Unknown xml element: (%s)\b", sys_info_node->name);
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
                LOG(log, DS3_ERROR, "Unknown element: (%s)\n", data_ptr->name);
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

    error = _internal_request_dispatcher(client, request, xml_blob, load_buffer, NULL, NULL);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);

    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);

    if (element_equal(root, "ListAllMyBucketsResult") == false) {
        char* message = g_strconcat("Expected the root element to be 'ListAllMyBucketsResult'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
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
            LOG(client->log, DS3_ERROR, "Unknown xml element: (%s)\b", child_node->name);
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
                LOG(log, DS3_WARN, "More than one Prefix found in CommonPrefixes\n");
            } else {
                prefix = xml_get_string(doc, child_node);
            }
        } else {
            LOG(log, DS3_ERROR, "Unknown xml element: %s\n", child_node->name);
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
    xmlChar* text;
    GArray* object_array;
    GArray* common_prefix_array;
    GByteArray* xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, load_buffer, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);

    if (element_equal(root, "ListBucketResult") == false) {
        char* message = g_strconcat("Expected the root element to be 'ListBucketsResult'.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        xmlFreeDoc(doc);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
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
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            response->creation_date = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "IsTruncated") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            if (strncmp((char*) text, "true", 4) == 0) {
                response->is_truncated = True;
            }
            else {
                response->is_truncated = False;
            }
            xmlFree(text);
        } else if (element_equal(child_node, "Marker") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            response->marker = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "MaxKeys") == true) {
            response->max_keys = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            response->name = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "Delimiter") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            response->delimiter = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "NextMarker") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            response->next_marker = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "Prefix") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if (text == NULL) {
                continue;
            }
            response->prefix = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (element_equal(child_node, "CommonPrefixes") == true) {
            ds3_str* prefix = _parse_common_prefixes(client->log, doc, child_node);
            g_array_append_val(common_prefix_array, prefix);
        } else {
            LOG(client->log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
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

ds3_error* ds3_head_object(const ds3_client* client, const ds3_request* request, ds3_metadata** _metadata) {
    ds3_error* error;
    GHashTable* return_headers;
    ds3_metadata* metadata;

    error = _net_process_request(client, request, NULL, NULL, NULL, NULL, &return_headers);

    if (error == NULL) {
        fprintf(stderr, "Head object completed successfully\n");
        metadata = _init_metadata(return_headers);
        *_metadata = metadata;
        g_hash_table_destroy(return_headers);
    }

    return error;
}

ds3_error* ds3_get_object(const ds3_client* client, const ds3_request* request, void* user_data, size_t(*callback)(void*,size_t, size_t, void*)) {
    return _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL);
}

ds3_error* ds3_get_object_with_metadata(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*), ds3_metadata** _metadata) {
  ds3_error* error;
    GHashTable* return_headers;
    ds3_metadata* metadata;

    error = _net_process_request(client, request, user_data, callback, NULL, NULL, &return_headers);

    if (error == NULL) {
        fprintf(stderr, "Head object completed successfully\n");
        metadata = _init_metadata(return_headers);
        *_metadata = metadata;
        g_hash_table_destroy(return_headers);
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
    error = _internal_request_dispatcher(client, request, xml_blob, load_buffer, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);

    if (element_equal(root, "Data") == false) {
        char* message = g_strconcat("Expected the root element to be 'Data'.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        xmlFreeDoc(doc);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
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
            LOG(client->log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
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
    xmlChar* text;
    struct _xmlAttr* attribute;

    ds3_bulk_object response;
    memset(&response, 0, sizeof(ds3_bulk_object));

    for (attribute = object_node->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Name") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response.name = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "InCache") == true) {
            response.in_cache = xml_get_bool_from_attribute(log, doc, attribute);
        } else if (attribute_equal(attribute, "Length") == true) {
            response.length = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Offset") == true) {
            response.offset = xml_get_uint64_from_attribute(doc, attribute);
        } else {
            LOG(log, DS3_ERROR, "Unknown attribute: (%s)\n", attribute->name);
        }
    }

    for (child_node = object_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        LOG(log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
    }

    return response;
}

static ds3_bulk_object_list* _parse_bulk_objects(const ds3_log* log, xmlDocPtr doc, xmlNodePtr objects_node) {
    xmlNodePtr child_node;
    xmlChar* text;
    struct _xmlAttr* attribute;

    ds3_bulk_object_list* response = g_new0(ds3_bulk_object_list, 1);
    GArray* object_array = g_array_new(FALSE, TRUE, sizeof(ds3_bulk_object));

    for (attribute = objects_node->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "ChunkId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_id = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "NodeId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->node_id= ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "ChunkNumber") == true) {
            response->chunk_number = xml_get_uint64_from_attribute(doc, attribute);
        } else {
            LOG(log, DS3_ERROR, "Unknown attribute: (%s)\n", attribute->name);
        }

    }

    for (child_node = objects_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object") == true) {
            ds3_bulk_object object = _parse_bulk_object(log, doc, child_node);
            g_array_append_val(object_array, object);
        } else {
            LOG(log, DS3_ERROR, "Unknown element: (%s)\n", child_node->name);
        }
    }

    response->list = (ds3_bulk_object*) object_array->data;
    response->size = object_array->len;
    g_array_free(object_array, FALSE);
    return response;
}

static ds3_job_priority _match_priority(const ds3_log* log, const xmlChar* priority_str) {
    if (xmlStrcmp(priority_str, (const xmlChar*) "CRITICAL") == 0) {
        return CRITICAL;
    }else if (xmlStrcmp(priority_str, (const xmlChar*) "VERY_HIGH") == 0) {
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
        LOG(log, DS3_ERROR, "ERROR: Unknown priority type of '%s'.  Returning LOW to be safe.\n", priority_str);
        return LOW;
    }
}

static ds3_job_request_type _match_request_type(const ds3_log* log, const xmlChar* request_type) {
    if (xmlStrcmp(request_type, (const xmlChar*) "PUT") == 0) {
        return PUT;
    } else if (xmlStrcmp(request_type, (const xmlChar*) "GET") == 0) {
        return GET;
    } else {
        LOG(log, DS3_ERROR, "ERROR: Unknown request type of '%s'.  Returning GET for safety.\n", request_type);
        return GET;
    }
}

static ds3_write_optimization _match_write_optimization(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CAPACITY") == 0) {
        return CAPACITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "PERFORMANCE") == 0) {
        return PERFORMANCE;
    } else {
        LOG(log, DS3_ERROR, "ERROR: Unknown write optimization of '%s'.  Returning CAPACITY for safety.\n", text);
        return CAPACITY;
    }
}

static ds3_chunk_ordering _match_chunk_order(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "IN_ORDER") == 0) {
        return IN_ORDER;
    } else if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return NONE;
    } else {
        LOG(log, DS3_ERROR, "ERROR: Unknown chunk processing order guaruntee value of '%s'.  Returning IN_ORDER for safety.\n", text);
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
        LOG(log, DS3_ERROR, "ERROR: Unknown job status value of '%s'.  Returning IN_PROGRESS for safety.\n", text);
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
        LOG(log, DS3_ERROR, "ERROR: Unknown tape status value of '%s'.  Returning TAPE_STATE_UNKNOWN for safety.\n", text);
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
        LOG(log, DS3_ERROR, "ERROR: Unknown tape status value of '%s'.  Returning TAPE_TYPE_UNKNOWN for safety.\n", text);
        return TAPE_TYPE_UNKNOWN;
    }
}
static ds3_error* _parse_bulk_response_attributes(const ds3_log* log, xmlDocPtr doc, xmlNodePtr node, ds3_bulk_response* response){
    struct _xmlAttr* attribute;
    xmlChar* text;

    for(attribute = node->properties; attribute != NULL; attribute = attribute->next) {
        if(attribute_equal(attribute, "JobId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->job_id = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "BucketName") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->bucket_name = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "StartDate") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->start_date = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "UserId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->user_id = ds3_str_init((const char*) text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "UserName") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->user_name = ds3_str_init((const char*) text);
            xmlFree(text);
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
            LOG(log, DS3_ERROR, "Unknown attribute: (%s)", attribute->name);
        }
    }

    return NULL;
}

static ds3_error* _parse_master_object_list(const ds3_log* log, xmlDocPtr doc, ds3_bulk_response** _response){
    GArray* objects_array;
    xmlNodePtr root, child_node;
    ds3_bulk_response* response;

    root = xmlDocGetRootElement(doc);

    if(element_equal(root, "MasterObjectList") == false) {
        char* message = g_strconcat("Expected the root element to be 'MasterObjectList'.  The actual response is: ", root->name, NULL);
        xmlFreeDoc(doc);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    response = g_new0(ds3_bulk_response, 1);

    _parse_bulk_response_attributes(log, doc, root, response);

    objects_array = g_array_new(FALSE, TRUE, sizeof(ds3_bulk_object_list*));

    for(child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Objects")  == true) {
            ds3_bulk_object_list* obj_list = _parse_bulk_objects(log, doc, child_node);
            g_array_append_val(objects_array, obj_list);
        } else {
            //TODO add Node xml handling
            LOG(log, DS3_ERROR, "Unknown element: (%s)", child_node->name);
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
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
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

    error_response = _net_process_request(client, request, xml_blob, load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Cleanup the data sent to the server.
    xmlFreeDoc(doc);
    xmlFree(xml_buff);

    g_byte_array_free(xml_blob, TRUE);
    return error_response;
}

ds3_error* ds3_delete_folder(const ds3_client* client, const ds3_request* _request) {
    struct _ds3_request* request;
    request = (struct _ds3_request*) _request;
    return _net_process_request(client, request, NULL, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_get_physical_placement(const ds3_client* client, const ds3_request* _request, ds3_get_physical_placement_response** _response){
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
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }

    request = (struct _ds3_request*) _request;

    if (request->object_list == NULL || request->object_list->size == 0) {
        g_array_free(tape_array, TRUE);
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
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
    error_response = _net_process_request(client, request, xml_blob, load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

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
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
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
            ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
            g_free(message);
            return error;
        }

        response = g_new0(ds3_get_physical_placement_response, 1);

        for (child_node = cur->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
            if (element_equal(child_node, "Tape") == true) {
                memset(&tape, 0, sizeof(ds3_tape));
                for (tape_node = child_node->xmlChildrenNode; tape_node != NULL; tape_node = tape_node->next){
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
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }

    request = (struct _ds3_request*) _request;

    if (request->object_list == NULL || request->object_list->size == 0) {
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
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
    error_response = _net_process_request(client, request, xml_blob, load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Cleanup the data sent to the server.
    xmlFreeDoc(doc);
    xmlFree(xml_buff);

    if (error_response != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error_response;
    }

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if(doc == NULL) {
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
    GHashTable* response_headers = NULL;
    ds3_response_header* retry_after_header;
    xmlDocPtr doc;
    xmlNodePtr root;

    error = _net_process_request(client, request, xml_blob, load_buffer, NULL, NULL, &response_headers);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    ds3_response = g_new0(ds3_allocate_chunk_response, 1);

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        g_byte_array_free(xml_blob, TRUE);
        retry_after_header = (ds3_response_header*)g_hash_table_lookup(response_headers, "Retry-After");
        if (retry_after_header != NULL) {
            ds3_str* retry_value = _ds3_response_header_get_first(retry_after_header);
            ds3_response->retry_after = g_ascii_strtoull(retry_value->value, NULL, 10);
        } else {
            g_hash_table_destroy(response_headers);
            return _ds3_create_error(DS3_ERROR_REQUEST_FAILED, "We did not get a response and did not find the 'Retry-After Header'");
        }
        g_hash_table_destroy(response_headers);
        return NULL;
    }

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, "Objects")  == true) {
        object_list = _parse_bulk_objects(client->log, doc, root);
        ds3_response->objects = object_list;
    } else {
        char* message = g_strconcat("Expected the root element to be 'Objects'.  The actual response is: ", root->name, NULL);
        xmlFreeDoc(doc);
        error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        g_byte_array_free(xml_blob, TRUE);
        g_hash_table_destroy(response_headers);
        return error;
    }

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    if (response_headers != NULL) {
        g_hash_table_destroy(response_headers);
    }
    *response = ds3_response;
    return NULL;
}

ds3_error* ds3_get_available_chunks(const ds3_client* client, const ds3_request* request, ds3_get_available_chunks_response** response) {
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();
    ds3_get_available_chunks_response* ds3_response;
    ds3_bulk_response* bulk_response;
    GHashTable* response_headers = NULL;
    ds3_response_header* retry_after_header;
    xmlDocPtr doc;

    error = _net_process_request(client, request, xml_blob, load_buffer, NULL, NULL, &response_headers);

    if (error != NULL) {
        if (response_headers != NULL) {
            g_hash_table_destroy(response_headers);
        }
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    ds3_response = g_new0(ds3_get_available_chunks_response, 1);

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (response_headers != NULL) {
        retry_after_header = (ds3_response_header*)g_hash_table_lookup(response_headers, "Retry-After");
        if (retry_after_header != NULL) {
            ds3_str* retry_value = _ds3_response_header_get_first(retry_after_header);
            ds3_response->retry_after = g_ascii_strtoull(retry_value->value, NULL, 10);
        }
    }

    _parse_master_object_list(client->log, doc, &bulk_response);
    ds3_response->object_list = bulk_response;

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    if (response_headers != NULL) {
        g_hash_table_destroy(response_headers);
    }
    *response = ds3_response;
    return NULL;
}

static ds3_error* _parse_jobs_list(const ds3_log* log, xmlDocPtr doc, ds3_get_jobs_response** _response){
    ds3_get_jobs_response* response = NULL;
    xmlNodePtr root, child_node;
    GPtrArray* jobs_array = NULL;
    ds3_bulk_response* job = NULL;
    ds3_error* error = NULL;

    root = xmlDocGetRootElement(doc);
    if(element_equal(root, "Jobs") == false) {
        char* message = g_strconcat("Expected the root element to be 'Jobs'.  The actual response is: ", root->name, NULL);
        xmlFreeDoc(doc);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    jobs_array = g_ptr_array_new();
    response = g_new0(ds3_get_jobs_response, 1);

    for(child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Job") == true) {
            job = g_new0(ds3_bulk_response, 1);
            error = _parse_bulk_response_attributes(log, doc, child_node, job);
            if( error )
            {
              g_ptr_array_free(jobs_array, TRUE);
              LOG(log, DS3_ERROR, "Error parsing bulk_response element");
              ds3_free_get_jobs_response(response);
              return error;
            } else {
              g_ptr_array_add(jobs_array, job);
            }
        } else{
            // Invalid XML block
            LOG(log, DS3_ERROR, "Unknown child node: (%s)", child_node->name);
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
    GHashTable* response_headers = NULL;
    xmlDocPtr doc;

    error = _net_process_request(client, request, xml_blob, load_buffer, NULL, NULL, &response_headers);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return _ds3_create_error(DS3_ERROR_REQUEST_FAILED, "Unexpected empty response body.");
    }

    _parse_jobs_list(client->log, doc, &get_jobs_response);

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    if (response_headers != NULL) {
        g_hash_table_destroy(response_headers);
    }
    *response = get_jobs_response;
    return NULL;
}

static ds3_error* _common_job(const ds3_client* client, const ds3_request* request, ds3_bulk_response** response) {
    ds3_error* error;
    GByteArray* xml_blob = g_byte_array_new();
    ds3_bulk_response* bulk_response;
    xmlDocPtr doc;

    error = _net_process_request(client, request, xml_blob, load_buffer, NULL, NULL, NULL);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    // Start processing the data that was received back.
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return _ds3_create_error(DS3_ERROR_REQUEST_FAILED, "Unexpected empty response body.");
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

void ds3_free_bucket_response(ds3_get_bucket_response* response){
    size_t num_objects;
    int i;
    if (response == NULL) {
        return;
    }

    num_objects = response->num_objects;

    for (i = 0; i < num_objects; i++) {
        ds3_object object = response->objects[i];
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
        for (i = 0; i < response->num_common_prefixes; i++) {
            ds3_str_free(response->common_prefixes[i]);
        }
        g_free(response->common_prefixes);
    }

    g_free(response);
}

void ds3_free_objects_response(ds3_get_objects_response* response){
    size_t num_objects;
    int i;
    if (response == NULL) {
        return;
    }

    num_objects = response->num_objects;
    ds3_search_object* object;
    for (i = 0; i < num_objects; i++) {
        object = response->objects[i];
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

void ds3_free_get_physical_placement_response(ds3_get_physical_placement_response* response){
    size_t num_tapes;
    int i;
    if (response == NULL) {
        return;
    }

    num_tapes = response->num_tapes;
    for (i = 0; i < num_tapes; i++) {
        ds3_tape tape = response->tapes[i];
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

void ds3_free_service_response(ds3_get_service_response* response){
    size_t num_buckets;
    int i;

    if (response == NULL) {
        return;
    }

    num_buckets = response->num_buckets;

    for (i = 0; i<num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        ds3_str_free(bucket.name);
        ds3_str_free(bucket.creation_date);
    }

    ds3_free_owner(response->owner);
    g_free(response->buckets);
    g_free(response);
}

void ds3_free_bulk_response(ds3_bulk_response* response) {
    int i;
    if (response == NULL) {
        return;
    }

    if (response->job_id != NULL) {
        ds3_str_free(response->job_id);
    }

    if (response->bucket_name != NULL) {
        ds3_str_free(response->bucket_name);
    }

    if (response->start_date != NULL) {
        ds3_str_free(response->start_date);
    }

    if (response->user_id != NULL) {
        ds3_str_free(response->user_id);
    }

    if (response->user_name != NULL) {
        ds3_str_free(response->user_name);
    }

    if (response->list != NULL ) {
        for (i = 0; i < response->list_size; i++) {
            ds3_free_bulk_object_list(response->list[i]);
        }
        g_free(response->list);
    }

    g_free(response);
}

void ds3_free_get_jobs_response(ds3_get_jobs_response* response) {
    int job_index;

    if (response == NULL) {
        return;
    }

    if (response->jobs != NULL ) {
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
    if (owner->name != NULL) {
        ds3_str_free(owner->name);
    }
    if (owner->id != NULL) {
        ds3_str_free(owner->id);
    }
    g_free(owner);
}

void ds3_free_creds(ds3_creds* creds) {
    if (creds == NULL) {
        return;
    }

    if (creds->access_id != NULL) {
        ds3_str_free(creds->access_id);
    }

    if (creds->secret_key != NULL) {
        ds3_str_free(creds->secret_key);
    }
    g_free(creds);
}

void ds3_free_client(ds3_client* client) {
    if (client == NULL) {
      return;
    }
    if (client->endpoint != NULL) {
        ds3_str_free(client->endpoint);
    }
    if (client->proxy != NULL) {
        ds3_str_free(client->proxy);
    }
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
    if (request->path != NULL) {
        ds3_str_free(request->path);
    }
    if (request->headers != NULL) {
        g_hash_table_destroy(request->headers);
    }
    if (request->query_params != NULL) {
        g_hash_table_destroy(request->query_params);
    }
    if (request->md5 != NULL) {
        ds3_str_free(request->md5);
    }
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

    if (error->message != NULL) {
        ds3_str_free(error->message);
    }

    if (error->error != NULL) {
        ds3_error_response* error_response = error->error;

        if (error_response->status_message != NULL) {
            ds3_str_free(error_response->status_message);
        }

        if (error_response->error_body != NULL) {
            ds3_str_free(error_response->error_body);
        }

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
    uint64_t i;
    ds3_bulk_object_list* obj_list = ds3_init_bulk_object_list(num_files);

    for (i = 0; i < num_files; i++) {
        obj_list->list[i] = _ds3_bulk_object_from_file(file_list[i], base_path);
    }

    return obj_list;
}

ds3_bulk_object_list* ds3_convert_object_list(const ds3_object* objects, uint64_t num_objects) {
    uint64_t i;
    ds3_bulk_object_list* obj_list = ds3_init_bulk_object_list(num_objects);

    for (i = 0; i < num_objects; i++) {
        ds3_bulk_object obj;
        memset(&obj, 0, sizeof(ds3_bulk_object));
        obj.name = ds3_str_dup(objects[i].name);
        obj_list->list[i] = obj;
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
    uint64_t i, count;
    if (object_list == NULL) {
        return;
    }
    count = object_list->size;
    for (i = 0; i < count; i++) {
        ds3_str* file_name = object_list->list[i].name;
        if (file_name == NULL) {
            continue;
        }
        ds3_str_free(file_name);
    }

    if (object_list->server_id != NULL) {
        ds3_str_free(object_list->server_id);
    }

    if (object_list->node_id != NULL) {
        ds3_str_free(object_list->node_id);
    }

    if (object_list->chunk_id != NULL) {
        ds3_str_free(object_list->chunk_id);
    }

    g_free(object_list->list);
    g_free(object_list);
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
    int i;
    ds3_str* value;
    if (entry->name != NULL) {
        ds3_str_free(entry->name);
    }
    if (entry->values != NULL) {
        for (i = 0; i < entry->num_values; i++) {
            value = entry->values[i];
            if (value != NULL) {
                ds3_str_free(value);
            }
        }
        g_free(entry->values);
    }
    g_free(entry);
}

void ds3_free_metadata_keys(ds3_metadata_keys_result* metadata_keys) {
    uint64_t i;
    ds3_str* value;
    if (metadata_keys == NULL) {
        return;
    }

    if (metadata_keys->keys != NULL) {
        for (i = 0; i < metadata_keys->num_keys; i++) {
            value = metadata_keys->keys[i];
            if (value != NULL) {
                ds3_str_free(value);
            }
        }
        g_free(metadata_keys->keys);
    }
    g_free(metadata_keys);
}

void ds3_free_build_information(ds3_build_information* build_info) {
    if (build_info == NULL) {
        return;
    }

    if (build_info->branch != NULL) {
        ds3_str_free(build_info->branch);
    }
    if (build_info->revision != NULL) {
        ds3_str_free(build_info->revision);
    }
    if (build_info->version != NULL) {
        ds3_str_free(build_info->version);
    }
    g_free(build_info);
}

void ds3_free_get_system_information(ds3_get_system_information_response* system_info) {
    if (system_info == NULL) {
        return;
    }

    if (system_info->api_version != NULL) {
        ds3_str_free(system_info->api_version);
    }
    if (system_info->serial_number != NULL) {
        ds3_str_free(system_info->serial_number);
    }
    if (system_info->build_information != NULL) {
        ds3_free_build_information(system_info->build_information);
    }
    g_free(system_info);
}
