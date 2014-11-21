/*
 * ******************************************************************************
 *   Copyright 2014 Spectra Logic Corporation. All Rights Reserved.
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

#include "ds3.h"

//---------- Define opaque struct ----------//  
struct _ds3_request{
    http_verb verb;
    ds3_str* path;
    uint64_t length;
    GHashTable* headers;
    GHashTable* query_params;

    //These next few elements are only for the bulk commands
    ds3_bulk_object_list* object_list;
};

typedef struct {
    char* buff;
    size_t size;
    size_t total_read;
}ds3_xml_send_buff;

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
    ds3_str* value;
}ds3_response_header;

ds3_str* ds3_str_init(const char* string){
    ds3_str* str = g_new0(ds3_str, 1);
    str->value = g_strdup(string);
    str->size = strlen(string);
    return str;
}

char* ds3_str_value(const ds3_str* string){
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

static void _ds3_free_response_header(gpointer data) {
    ds3_response_header* header;
    if (data == NULL) {
        return;
    }
        
    header = (ds3_response_header*) data;
    g_free(header->key);
    g_free(header->value);
    g_free(data);
}

static ds3_error* _ds3_create_error(ds3_error_code code, const char * message) {
    ds3_error* error = g_new0(ds3_error, 1);
    error->code = code;
    error->message = ds3_str_init(message);
    return error;
}

static size_t _ds3_send_xml_buff(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t to_read; 
    size_t remaining;
    ds3_xml_send_buff* xml_buff;
    
    xml_buff = (ds3_xml_send_buff*) user_data;
    to_read = size * nmemb;
    remaining = xml_buff->size - xml_buff->total_read;

    if(remaining < to_read) {
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
    ds3_response_header* header;
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
                fprintf(stderr, "Ignoring 100 status code header line\n");
                g_free(header_buff);
                g_strfreev(split_result);
                return to_read;
            }
            else {
                char* status_message = g_strjoinv(" ", split_result + 2);
                response_data->status_code = status_code;
                response_data->status_message = ds3_str_init(status_message);
                g_free(status_message);
                g_strfreev(split_result);
            }
        }
        else {
            fprintf(stderr, "Unsupported Protocol\n");
            g_free(header_buff);
            return 0;
        }
    }
    else {
        split_result = g_strsplit(header_buff, ": ", 2);
        header = g_new0(ds3_response_header, 1); 
        header->key = ds3_str_init(split_result[0]);
        header->value = ds3_str_init(split_result[1]);

        g_hash_table_insert(headers, header->key, header);
        
        g_strfreev(split_result);
    }
    response_data->header_count++;
    g_free(header_buff);
    return to_read;
}

static size_t _process_response_body(void* buffer, size_t size, size_t nmemb, void* user_data) {
    ds3_response_data* response_data = (ds3_response_data*) user_data;

    // If we got an error, collect the error body
    if (response_data->status_code >= 400) {
        return load_buffer(buffer, size, nmemb, response_data->body); 
    }
    else { // If we did not get an error, call he user's defined callbacks.
        return response_data->user_func(buffer, size, nmemb, response_data->user_data);
    }
}

//---------- Networking code ----------// 
static void _init_curl(void) {
    static ds3_bool initialized = False;

    if(!initialized) {
        if(curl_global_init(CURL_GLOBAL_ALL) != 0) {
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

static char* _escape_url(const char* url) {
    return curl_easy_escape(NULL, url, 0);
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
    if(resource_name == NULL) {
        fprintf(stderr, "resource_name is required\n");
        return NULL;
    }
    if(date == NULL) {
        fprintf(stderr, "date is required");
        return NULL;
    }
    verb_str = _net_get_verb(verb);

    return (unsigned char*) g_strconcat(verb_str, "\n", md5, "\n", content_type, "\n", date, "\n", amz_headers, resource_name, NULL);
}

static char* _generate_date_string(void) {
    GDateTime* time  = g_date_time_new_now_local();
    char* date_string = g_date_time_format(time, "%a, %d %b %Y %T %z");
    
    fprintf(stdout, "Date: %s\n", date_string);
    g_date_time_unref(time);

    return date_string;
}

static char* _net_compute_signature(const ds3_creds* creds, http_verb verb, char* resource_name,
                             char* date, char* content_type, char* md5, char* amz_headers) {
    GHmac* hmac;
    gchar* signature;
    gsize bufSize = 256;
    guint8 buffer[256];
    unsigned char* signature_str = _generate_signature_str(verb, resource_name, date, content_type, md5, amz_headers); 
    
    fprintf(stdout, "Signature:\n%s\n", signature_str);
  

    hmac = g_hmac_new(G_CHECKSUM_SHA1, (unsigned char*) creds->secret_key->value, creds->secret_key->size);
    g_hmac_update(hmac, signature_str, -1);
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

    entries->entries[entries->size] = g_strconcat(key, "=", value, NULL);
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

        for(i = 0; ; i++) {
            char* current_string = entries[i];
            if(current_string == NULL) {
                break;
            }
            g_free(current_string);
        }

        g_free(entries);
        return return_string;
    }
    else {
        return NULL;
    }
}

static ds3_error* _net_process_request(const ds3_client* client, const ds3_request* _request, void* read_user_struct, size_t (*read_handler_func)(void*, size_t, size_t, void*), void* write_user_struct, size_t (*write_handler_func)(void*, size_t, size_t, void*)) {
    _init_curl();
    
    struct _ds3_request* request = (struct _ds3_request*) _request;
    CURL* handle = curl_easy_init();
    CURLcode res;

    if(handle) {
        char* url;
        
        char* date;
        char* date_header;
        char* signature;
        struct curl_slist* headers;
        char* auth_header;
        char* query_params = _net_gen_query_params(request->query_params);
        ds3_response_data response_data;
        GHashTable* response_headers = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, _ds3_free_response_header);

        memset(&response_data, 0, sizeof(ds3_response_data));
        response_data.headers = response_headers;
        response_data.body = g_byte_array_new();

        if (query_params == NULL) {
            url = g_strconcat(client->endpoint->value, request->path->value, NULL);
        }
        else {
            url = g_strconcat(client->endpoint->value, request->path->value,"?",query_params, NULL);
            g_free(query_params);
        }
        curl_easy_setopt(handle, CURLOPT_URL, url);
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L); //tell curl to follow redirects
        curl_easy_setopt(handle, CURLOPT_MAXREDIRS, client->num_redirects);
        
        // Setup header collection
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, _process_header_line);
        curl_easy_setopt(handle, CURLOPT_HEADERDATA, &response_data);

        if(client->proxy != NULL) {
          curl_easy_setopt(handle, CURLOPT_PROXY, client->proxy->value);
        }

        // Register the read and write handlers if they are set
        if(read_user_struct != NULL && read_handler_func != NULL) {
            response_data.user_data = read_user_struct;
            response_data.user_func = read_handler_func;
        }

        // We must always set this so we can collect the error message body
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, _process_response_body);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response_data);

        if(write_user_struct != NULL && write_handler_func != NULL) {
            curl_easy_setopt(handle, CURLOPT_READFUNCTION, write_handler_func);
            curl_easy_setopt(handle, CURLOPT_READDATA, write_user_struct);
        }

        switch(request->verb) {
            case HTTP_POST: {
                if (write_user_struct == NULL || write_handler_func == NULL) {
                    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
                }
                else {
                    curl_easy_setopt(handle, CURLOPT_POST, 1L);
                    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(handle, CURLOPT_INFILESIZE, request->length);
                }
                break;
            }
            case HTTP_PUT: {
                if (write_user_struct == NULL || write_handler_func == NULL) {
                    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "PUT");
                }
                else {
                    curl_easy_setopt(handle, CURLOPT_PUT, 1L);
                    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(handle, CURLOPT_INFILESIZE, request->length);
                }
                break;
            }
            case HTTP_DELETE: {
                curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "DELETE");
                break;
            }
            case HTTP_HEAD: {
                curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "HEAD");
                break;
            }
            case HTTP_GET: {
                //Placeholder if we need to put anything here.
                break;
            }
        }

        date = _generate_date_string(); 
        date_header = g_strconcat("Date: ", date, NULL);
        signature = _net_compute_signature(client->creds, request->verb, request->path->value, date, "", "", "");
        headers = NULL;
        auth_header = g_strconcat("Authorization: AWS ", client->creds->access_id->value, ":", signature, NULL);

        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, date_header);

        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(handle);

        g_free(url);
        g_free(date);
        g_free(date_header);
        g_free(signature);
        g_free(auth_header);
        g_hash_table_destroy(response_headers);
        curl_slist_free_all(headers);
        curl_easy_cleanup(handle);
        
        //process the response
        if(res != CURLE_OK) {
            char * message = g_strconcat("Request failed: ", curl_easy_strerror(res), NULL);
            ds3_error* error = _ds3_create_error(DS3_ERROR_REQUEST_FAILED, message);
            g_byte_array_free(response_data.body, TRUE);
            g_free(response_data.status_message); 
            g_free(message);
            return error;
        }

        if(response_data.status_code < 200 || response_data.status_code >= 300) {
            ds3_error* error = _ds3_create_error(DS3_ERROR_BAD_STATUS_CODE, "Got an unexpected status code.");
            error->error = g_new0(ds3_error_response, 1);
            error->error->status_code = response_data.status_code;
            error->error->status_message = ds3_str_init(response_data.status_message->value);

            if (response_data.body != NULL && response_data.body->data != NULL) {
                error->error->error_body = ds3_str_init((char*)response_data.body->data);
            }
            else {
                error->error->error_body = ds3_str_init("(empty)");
            }

            g_byte_array_free(response_data.body, TRUE);
            g_free(response_data.status_message);
            return error;
        }
        g_byte_array_free(response_data.body, TRUE);
        g_free(response_data.status_message); 
    }
    else {
        return _ds3_create_error(DS3_ERROR_CURL_HANDLE, "Failed to create curl handle");
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
    GHashTable* hash =  g_hash_table_new_full(g_str_hash, g_str_equal, NULL, _cleanup_hash_value);
    return hash;
}

ds3_creds* ds3_create_creds(const char* access_id, const char* secret_key) {
    ds3_creds* creds;
    if(access_id == NULL || secret_key == NULL) {
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
    if(endpoint == NULL) {
        fprintf(stderr, "Null endpoint\n");
        return NULL;
    }

    client = g_new0(ds3_client, 1);
    
    client->endpoint = ds3_str_init(endpoint);
    
    client->creds = creds;

    client->num_redirects = 5L; //default to 5 redirects before failing
    return client;
}

static void _set_query_param(ds3_request* _request, const char* key, const char* value) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    gpointer escaped_key = (gpointer) _escape_url(key);
    gpointer escaped_value = (gpointer) _escape_url(value);

    g_hash_table_insert(request->query_params, escaped_key, escaped_value);
} 

void ds3_client_proxy(ds3_client* client, const char* proxy) {
    client->proxy = ds3_str_init(proxy);
}

void ds3_request_set_prefix(ds3_request* _request, const char* prefix) {
    _set_query_param(_request, "prefix", prefix);
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

ds3_request* ds3_init_get_service(void) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path( "/", NULL, NULL));
}

ds3_request* ds3_init_get_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_get_object(const char* bucket_name, const char* object_name) {
    return (ds3_request*) _common_request_init(HTTP_GET, _build_path("/", bucket_name, object_name));
}

ds3_request* ds3_init_delete_object(const char* bucket_name, const char* object_name) {
    return (ds3_request*) _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, object_name));
}

ds3_request* ds3_init_put_object(const char* bucket_name, const char* object_name, uint64_t length) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, object_name));
    request->length = length;
    return (ds3_request*) request;
}

ds3_request* ds3_init_put_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_PUT, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_delete_bucket(const char* bucket_name) {
    return (ds3_request*) _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, NULL));
}

ds3_request* ds3_init_get_bulk(const char* bucket_name, ds3_bulk_object_list* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "operation", "start_bulk_get");
    request->object_list = object_list;
    return (ds3_request*) request;
}

ds3_request* ds3_init_put_bulk(const char* bucket_name, ds3_bulk_object_list* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "operation", "start_bulk_put");
    request->object_list = object_list;
    return (ds3_request*) request;
}

static ds3_error* _internal_request_dispatcher(const ds3_client* client, const ds3_request* request, void* read_user_struct, size_t (*read_handler_func)(void*, size_t, size_t, void*), void* write_user_struct, size_t (*write_handler_func)(void*, size_t, size_t, void*)) {
    if(client == NULL || request == NULL) {
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing"); 
    }
    return _net_process_request(client, request, read_user_struct, read_handler_func, write_user_struct, write_handler_func);
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
    if(text == NULL) {
        return 0;
    }
    size = strtoul((const char*)text, NULL, 10);
    xmlFree(text);
    return size;
}

static ds3_str* xml_get_string(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    ds3_str* result;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    result = ds3_str_init((const char*) text);
    xmlFree(text);
    return result;
}

static uint64_t xml_get_uint64_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_uint64(doc, (xmlNodePtr) attribute);
}

static ds3_bool xml_get_bool_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    xmlChar* text;
    ds3_bool result;
    text = xmlNodeListGetString(doc, attribute->xmlChildrenNode, 1);
    if(xmlStrcmp(text, (xmlChar*)"true") == 0) {
        result = True;
    }
    else if (xmlStrcmp(text, (xmlChar*)"false") == 0) {
        result = False;
    }
    else {
        fprintf(stderr, "Unknown boolean value\n");
        result = False;
    }
    xmlFree(text);
    return result;
}

static void _parse_buckets(xmlDocPtr doc, xmlNodePtr buckets_node, ds3_get_service_response* response) {
    xmlNodePtr data_ptr; 
    xmlNodePtr curr;
    GArray* array = g_array_new(FALSE, TRUE, sizeof(ds3_bucket));

    for(curr = buckets_node->xmlChildrenNode; curr != NULL; curr = curr->next) {
        ds3_bucket bucket; 
        memset(&bucket, 0, sizeof(ds3_bucket));

        for(data_ptr = curr->xmlChildrenNode; data_ptr != NULL; data_ptr = data_ptr->next) {
            if(element_equal(data_ptr, "CreationDate")) {
                bucket.creation_date = xml_get_string(doc, data_ptr);
            }
            else if(element_equal(data_ptr, "Name")) {
                bucket.name = xml_get_string(doc, data_ptr);
            }
            else {
                fprintf(stderr, "Unknown element: (%s)\n", data_ptr->name);
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

    for(child_node = owner_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "DisplayName")) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            owner->name = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "ID")) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            owner->id = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else {
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
    
    if(error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);

    if(doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);
    
    if(element_equal(root, "ListAllMyBucketsResult") == false) {
        char* message = g_strconcat("Expected the root element to be 'ListAllMyBucketsResult'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    response = g_new0(ds3_get_service_response, 1);
    
    for(child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Buckets") == true) {
            //process buckets here
            _parse_buckets(doc, child_node, response);
        }
        else if(element_equal(child_node, "Owner") == true) {
            //process owner here
            ds3_owner * owner = _parse_owner(doc, child_node);
            response->owner = owner;
        }
        else {
            fprintf(stderr, "Unknown xml element: (%s)\b", child_node->name);
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

    for(child_node = contents_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Key") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            object.name = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "ETag") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.etag= ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "LastModified") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.last_modified = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "StorageClass") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.storage_class = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "Size") == true) {
            object.size = xml_get_uint64(doc, child_node);
        }
        else if(element_equal(child_node, "Owner") == true) { 
            ds3_owner* owner = _parse_owner(doc, child_node);
            object.owner = owner;
        }        
        else {
            fprintf(stderr, "Unknown xml element: (%s)\n", child_node->name);
        }
    }

    return object;
}

static ds3_str* _parse_common_prefixes(xmlDocPtr doc, xmlNodePtr contents_node) {
    xmlNodePtr child_node;
    ds3_str* prefix = NULL;
    
    for(child_node = contents_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Prefix") == true) {
            if(prefix) {
                fprintf(stderr, "More than one Prefix found in CommonPrefixes\n");
            }
            else {
                prefix = xml_get_string(doc, child_node);
            }
        }
        else {
            fprintf(stderr, "Unknown xml element: %s\n", child_node->name);
        }
    }

    return prefix;
}

ds3_error* ds3_get_bucket(const ds3_client* client, const ds3_request* request, ds3_get_bucket_response** _response) {
    ds3_get_bucket_response* response;
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    xmlChar* text;
    GArray* object_array = g_array_new(FALSE, TRUE, sizeof(ds3_object));
    GArray* common_prefix_array = g_array_new(FALSE, TRUE, sizeof(ds3_str*));
    GByteArray* xml_blob = g_byte_array_new();
    _internal_request_dispatcher(client, request, xml_blob, load_buffer, NULL, NULL);
    
    doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if(doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);

    if(element_equal(root, "ListBucketResult") == false) {
        char* message = g_strconcat("Expected the root element to be 'ListBucketsResult'.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        xmlFreeDoc(doc);
        ds3_error* error = _ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    response = g_new0(ds3_get_bucket_response, 1);

    for(child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Contents") == true) {
            ds3_object object = _parse_object(doc, child_node);
            g_array_append_val(object_array, object);
        }
        else if(element_equal(child_node, "CreationDate") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->creation_date = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "IsTruncated") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            if(strncmp((char*) text, "true", 4) == 0) {
                response->is_truncated = True; 
            }
            else {
                response->is_truncated = False; 
            }
            xmlFree(text);
        }
        else if(element_equal(child_node, "Marker") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->marker = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "MaxKeys") == true) {
            response->max_keys = xml_get_uint64(doc, child_node);
        }
        else if(element_equal(child_node, "Name") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->name = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "Delimiter") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->delimiter = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "NextMarker") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->next_marker = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "Prefix") == true) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->prefix = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(element_equal(child_node, "CommonPrefixes") == true) {
            ds3_str* prefix = _parse_common_prefixes(doc, child_node);
            g_array_append_val(common_prefix_array, prefix);
        }
        else {
            fprintf(stderr, "Unknown element: (%s)\n", child_node->name);
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

ds3_error* ds3_get_object(const ds3_client* client, const ds3_request* request, void* user_data, size_t(*callback)(void*,size_t, size_t, void*)) {
    return _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL);
}

ds3_error* ds3_put_object(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*)) {
    return _internal_request_dispatcher(client, request, NULL, NULL, user_data, callback);
}

ds3_error* ds3_delete_object(const ds3_client* client, const ds3_request* request) {
    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_put_bucket(const ds3_client* client, const ds3_request* request) {
    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_delete_bucket(const ds3_client* client, const ds3_request* request) {
    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

static ds3_bulk_object _parse_bulk_object(xmlDocPtr doc, xmlNodePtr object_node) {
    xmlNodePtr child_node;
    xmlChar* text;
    struct _xmlAttr* attribute;

    ds3_bulk_object response;
    memset(&response, 0, sizeof(ds3_bulk_object));

    for(attribute = object_node->properties; attribute != NULL; attribute = attribute->next) {
        if(attribute_equal(attribute, "Name") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response.name = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "InCache") == true) {
            response.in_cache = xml_get_bool_from_attribute(doc, attribute);
        } 
        else if(attribute_equal(attribute, "Length") == true) {
            response.length = xml_get_uint64_from_attribute(doc, attribute);
        }
        else if(attribute_equal(attribute, "Offset") == true) {
            response.offset = xml_get_uint64_from_attribute(doc, attribute);
        }
        else {
            fprintf(stderr, "Unknown attribute: (%s)\n", attribute->name);
        }
    }

    for(child_node = object_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        fprintf(stderr, "Unknown element: (%s)\n", child_node->name);
    }

    return response;
}

static ds3_bulk_object_list* _parse_bulk_objects(xmlDocPtr doc, xmlNodePtr objects_node) {
    xmlNodePtr child_node;
    xmlChar* text;
    struct _xmlAttr* attribute;

    ds3_bulk_object_list* response = g_new0(ds3_bulk_object_list, 1);
    GArray* object_array = g_array_new(FALSE, TRUE, sizeof(ds3_bulk_object));

    for(attribute = objects_node->properties; attribute != NULL; attribute = attribute->next) {
        if(attribute_equal(attribute, "ServerId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response->server_id = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "ChunkId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response->chunk_id = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "ChunkNumber") == true) {
            response->chunk_number = xml_get_uint64_from_attribute(doc, attribute);
        }
        else {
            fprintf(stderr, "Unknown attribute: (%s)\n", attribute->name);
        }

    }

    for(child_node = objects_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Object") == true) {
            ds3_bulk_object object = _parse_bulk_object(doc, child_node);
            g_array_append_val(object_array, object);
        }
        else {
            fprintf(stderr, "Unknown element: (%s)\n", child_node->name);
        }
    }

    response->list = (ds3_bulk_object*) object_array->data;
    response->size = object_array->len;
    g_array_free(object_array, FALSE);
    return response;
}

static ds3_error* _parse_master_object_list(xmlDocPtr doc, ds3_bulk_response** _response){
    struct _xmlAttr* attribute;
    GArray* objects_array; 
    xmlChar* text;
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

    objects_array = g_array_new(FALSE, TRUE, sizeof(ds3_bulk_object_list*));

    response = g_new0(ds3_bulk_response, 1);

    for(attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if(attribute_equal(attribute, "JobId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response->job_id = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "BucketName") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response->bucket_name = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "StartDate") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response->start_date = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "UserId") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response->user_id = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "UserName") == true) {
            text = xmlNodeListGetString(doc, attribute->children, 1);
            if(text == NULL) {
                continue;
            }
            response->user_name = ds3_str_init((const char*) text);
            xmlFree(text);
        }
        else if(attribute_equal(attribute, "CachedSizeInBytes") == true) {
            response->cached_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        }
        else if(attribute_equal(attribute, "CompletedSizeInBytes") == true) {
            response->completed_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        }
        else if(attribute_equal(attribute, "OriginalSizeInBytes") == true) {
            response->original_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        }
        else {
            fprintf(stderr, "Unknown attribute: (%s)\n", attribute->name);
        }
    }

    for(child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(element_equal(child_node, "Objects")  == true) {
            ds3_bulk_object_list* obj_list = _parse_bulk_objects(doc, child_node);
            g_array_append_val(objects_array, obj_list);
        }
        else {
            fprintf(stderr, "Unknown element: (%s)\n", child_node->name);
        }
    }
    
    response->list = (ds3_bulk_object_list**) objects_array->data;
    response->list_size = objects_array->len;
    g_array_free(objects_array, FALSE);
    
    *_response = response;
    return NULL;
}

#define LENGTH_BUFF_SIZE 21

static xmlDocPtr _generate_xml_objects_list(const ds3_bulk_object_list* obj_list) {
    char size_buff[LENGTH_BUFF_SIZE]; //The max size of an uint64_t should be 20 characters
    xmlDocPtr doc;
    ds3_bulk_object obj;
    xmlNodePtr objects_node, object_node;
    int i;
    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    
    objects_node = xmlNewNode(NULL, (xmlChar*) "Objects");
    
    for(i = 0; i < obj_list->size; i++) {
        memset(&obj, 0, sizeof(ds3_bulk_object));
        memset(size_buff, 0, sizeof(char) * LENGTH_BUFF_SIZE);
        
        obj = obj_list->list[i];
        g_snprintf(size_buff, sizeof(char) * LENGTH_BUFF_SIZE, "%llu", obj.length);

        object_node = xmlNewNode(NULL, (xmlChar*) "Object");
        xmlAddChild(objects_node, object_node);

        xmlSetProp(object_node, (xmlChar*) "Name", (xmlChar*) obj.name->value);
        xmlSetProp(object_node, (xmlChar*) "Size", (xmlChar*) size_buff );
    }

    xmlDocSetRootElement(doc, objects_node);

    return doc;
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
    
    if(client == NULL || _request == NULL) {
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing"); 
    }
    
    request = (struct _ds3_request*) _request;

    if(request->object_list == NULL || request->object_list->size == 0) {
        return _ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process"); 
    } 

    // Init the data structures declared above the null check
    memset(&send_buff, 0, sizeof(ds3_xml_send_buff));
    obj_list = request->object_list;

    doc = _generate_xml_objects_list(obj_list);

    xmlDocDumpFormatMemory(doc, &xml_buff, &buff_size, 1);

    send_buff.buff = (char*) xml_buff;
    send_buff.size = strlen(send_buff.buff);

    request->length = send_buff.size; // make sure to set the size of the request.

    xml_blob = g_byte_array_new();
    error_response = _net_process_request(client, request, xml_blob, load_buffer, (void*) &send_buff, _ds3_send_xml_buff);

    // Cleanup the data sent to the server.
    xmlFreeDoc(doc);
    xmlFree(xml_buff);
   
    if(error_response != NULL) {
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

    error_response = _parse_master_object_list(doc, response);

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);

    if (error_response != NULL) {
        return error_response;
    }

    return NULL;
}


void ds3_print_request(const ds3_request* _request) {
    const struct _ds3_request* request; 
    if(_request == NULL) {
      fprintf(stderr, "Request object was null\n");
      return;
    }
    request = (struct _ds3_request*)_request;
    printf("Verb: %s\n", _net_get_verb(request->verb));
    printf("Path: %s\n", request->path->value);
}

void ds3_free_bucket_response(ds3_get_bucket_response* response){
    size_t num_objects;
    int i;
    if(response == NULL) {
        return;
    }

    num_objects = response->num_objects;

    for(i = 0; i < num_objects; i++) {
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
        for(i = 0; i < response->num_common_prefixes; i++) {
            ds3_str_free(response->common_prefixes[i]);
        }
        g_free(response->common_prefixes);
    }

    g_free(response);
}

void ds3_free_service_response(ds3_get_service_response* response){
    size_t num_buckets;
    int i;

    if(response == NULL) {
        return;
    }

    num_buckets = response->num_buckets;

    for(i = 0; i<num_buckets; i++) {
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
    if(response == NULL) {
        fprintf(stderr, "Bulk response was NULL\n");
        return;
    }

    if(response->job_id != NULL) {
        ds3_str_free(response->job_id);
    }
    
    if(response->bucket_name != NULL) {
        ds3_str_free(response->bucket_name);
    }
    
    if(response->start_date != NULL) {
        ds3_str_free(response->start_date);
    }

    if(response->user_id != NULL) {
        ds3_str_free(response->user_id);
    }

    if(response->user_name != NULL) {
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

void ds3_free_owner(ds3_owner* owner) {
    if(owner == NULL) {
        fprintf(stderr, "Owner was NULL\n");
        return;
    }
    if(owner->name != NULL) {
        ds3_str_free(owner->name);
    }
    if(owner->id != NULL) {
        ds3_str_free(owner->id);
    }
    g_free(owner);
}

void ds3_free_creds(ds3_creds* creds) {
    if(creds == NULL) {
        return;
    }

    if(creds->access_id != NULL) {
        ds3_str_free(creds->access_id);
    }

    if(creds->secret_key != NULL) {
        ds3_str_free(creds->secret_key);
    }
    g_free(creds);
}

void ds3_free_client(ds3_client* client) {
    if(client == NULL) {
      return;
    }
    if(client->endpoint != NULL) {
        ds3_str_free(client->endpoint);
    }
    if(client->proxy != NULL) {
        ds3_str_free(client->proxy);
    }
    g_free(client);
}

void ds3_free_request(ds3_request* _request) {
    struct _ds3_request* request; 
    if(_request == NULL) {
        return;
    }
    request = (struct _ds3_request*) _request;
    if(request->path != NULL) {
        ds3_str_free(request->path);
    }
    if(request->headers != NULL) {
        g_hash_table_destroy(request->headers);
    }
    if(request->query_params != NULL) {
        g_hash_table_destroy(request->query_params);
    }
    g_free(request);
}

void ds3_free_error(ds3_error* error) {
    if(error == NULL) {
        return;
    }

    if(error->message != NULL) {
        ds3_str_free(error->message);
    }

    if(error->error != NULL) {
        ds3_error_response* error_response = error->error;

        if(error_response->status_message != NULL) {
            ds3_str_free(error_response->status_message);
        }

        if(error_response->error_body != NULL) {
            ds3_str_free(error_response->error_body);            
        }

        g_free(error_response);
    }

    g_free(error);
}

void ds3_cleanup(void) {
    net_cleanup();
}

size_t ds3_write_to_file(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return fwrite(buffer, size, nmemb, (FILE*) user_data);
}

size_t ds3_read_from_file(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return fread(buffer, size, nmemb, (FILE*) user_data);
}

static ds3_bulk_object _ds3_bulk_object_from_file(const char* file_name) {
    struct stat file_info;
    int result; 
    memset(&file_info, 0, sizeof(struct stat));

    result = stat(file_name, &file_info);
    if (result != 0) {
        fprintf(stderr, "Failed to get file info for %s\n", file_name);
    }

    ds3_bulk_object obj;
    memset(&obj, 0, sizeof(ds3_bulk_object));

    obj.name = ds3_str_init(file_name);
    obj.length = file_info.st_size;

    return obj;
}

ds3_bulk_object_list* ds3_convert_file_list(const char** file_list, uint64_t num_files) {
    uint64_t i;
    ds3_bulk_object_list* obj_list = ds3_init_bulk_object_list(num_files);
    
    for(i = 0; i < num_files; i++) {
        obj_list->list[i] = _ds3_bulk_object_from_file(file_list[i]);
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
    if(object_list == NULL) {
        return;
    }
    count = object_list->size;
    for(i = 0; i < count; i++) {
        ds3_str* file_name = object_list->list[i].name;
        if (file_name == NULL) {
            continue;
        }
        ds3_str_free(file_name);
    }

    if(object_list->server_id != NULL) {
        ds3_str_free(object_list->server_id);
    }

    g_free(object_list->list);
    g_free(object_list);
}
