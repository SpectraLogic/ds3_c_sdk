/*
 * ******************************************************************************
 *   Copyright 2014-2016 Spectra Logic Corporation. All Rights Reserved.
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

/* This Code is Auto-Generated; DO NOT MODIFY! */


#include <curl/curl.h>

#include "ds3_request.h"
#include "ds3.h"
#include "ds3_net.h"
#include "ds3_utils.h"
#include "ds3_string_multimap.h"
#include "ds3_string_multimap_impl.h"
#include "ds3_connection.h"

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

static char* _get_checksum_type_header(const ds3_checksum_type type) {
    switch(type) {
        case DS3_CHECKSUM_TYPE_MD5: return "Content-MD5:";
        case DS3_CHECKSUM_TYPE_SHA_256: return "Content-SHA256:";
        case DS3_CHECKSUM_TYPE_SHA_512: return "Content-SHA512:";
        case DS3_CHECKSUM_TYPE_CRC_32: return "Content-CRC32:";
        case DS3_CHECKSUM_TYPE_CRC_32C: return "Content-CRC32C:";
    }

    return NULL;
}


// curl_easy_escape'd strings must be freed using curl_free.  Copy
// the escaped string, using glib, since users of this function will
// eventually wind up freeing it with g_free.
char* escape_url(const char* url) {
    char* curl_escaped_url = curl_easy_escape(NULL, url, 0);
    char* escaped_url = g_strdup(curl_escaped_url);
    curl_free(curl_escaped_url);
    return escaped_url;
}

// Like escape_url but don't encode "/".
char* escape_url_extended(const char* url, const char** delimiters, uint32_t num_delimiters) {
    gchar** split = g_strsplit(url, delimiters[num_delimiters-1], 0);
    gchar** ptr;
    gchar* escaped_ptr;
    for (ptr = split; *ptr; ptr++) {
        if( num_delimiters > 1){
            escaped_ptr = escape_url_extended(*ptr, delimiters, num_delimiters-1);
        }else{
            escaped_ptr = escape_url(*ptr);
        }
        g_free(*ptr);
        *ptr = escaped_ptr;
    }
    escaped_ptr = g_strjoinv(delimiters[num_delimiters-1], split);
    g_strfreev(split);
    return escaped_ptr;
}

// Like escape_url but don't encode "/" or "+".
char* escape_url_object_name(const char* url) {
    const char *delimiters[1]={"/"};
    return escape_url_extended(url, delimiters, 1);
}

// Like escape_url but don't encode "=".
char* escape_url_range_header(const char* url) {
    const char *delimiters[2]={"=", ","};
    return escape_url_extended(url, delimiters, 2);
}

static unsigned char* _generate_signature_str(http_verb verb, char* resource_name, char* date,
                               char* content_type, char* checksum_value, char* amz_headers) {
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

    return (unsigned char*) g_strconcat(verb_str, "\n", checksum_value, "\n", content_type, "\n", date, "\n", amz_headers, resource_name, NULL);
}

static char* _generate_date_string(void) {
    GDateTime* time  = g_date_time_new_now_local();
    char* date_string = g_date_time_format(time, "%a, %d %b %Y %H:%M:%S %z");

    g_date_time_unref(time);

    return date_string;
}

static char* _net_compute_signature(const ds3_log* log, const ds3_creds* creds, http_verb verb, char* resource_name,
                             char* date, char* content_type, char* checksum_value, char* amz_headers) {
    GHmac* hmac;
    gchar* signature;
    gsize bufSize = 256;
    guint8 buffer[256];

    unsigned char* signature_str = _generate_signature_str(verb, resource_name, date, content_type, checksum_value, amz_headers);
    char* escaped_str = g_strescape((char*) signature_str, NULL);

    ds3_log_message(log, DS3_DEBUG, "signature string: %s", escaped_str);
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

    ds3_log_message(log, DS3_TRACE, "%s: %s", text, g_strchomp(message));

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
            header_signing_value = g_string_new((gchar*)key->data);
            header_signing_value = g_string_append(g_string_ascii_down(header_signing_value), ":");
            header_signing_value = g_string_append(header_signing_value, (gchar*)g_hash_table_lookup(headers, key->data));

            signing_value = g_string_free(header_signing_value, FALSE);
            g_ptr_array_add(signing_strings, signing_value);
        }
        key = key->next;
    }

    g_ptr_array_sort(signing_strings, _gstring_sort);

    for (i = 0; i < signing_strings->len; i++) {
        g_string_append(canonicalized_headers, (gchar*)g_ptr_array_index(signing_strings, i));
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

static size_t _process_header_line(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t to_read;
    char* header_buff;
    char** split_result;
    ds3_str* header_key;
    ds3_str* header_value;
    ds3_response_data* response_data = (ds3_response_data*) user_data;

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

        ds3_string_multimap_insert(response_data->headers, header_key, header_value);
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
        return ds3_load_buffer(buffer, size, nmemb, response_data->body);
    } else { // If we did not get an error, call the user's defined callbacks.
        return response_data->user_func(buffer, size, nmemb, response_data->user_data);
    }
}

ds3_error* net_process_request(const ds3_client* client,
                               const ds3_request* _request,
                               void* read_user_struct,
                               size_t (*read_handler_func)(void*, size_t, size_t, void*),
                               void* write_user_struct,
                               size_t (*write_handler_func)(void*, size_t, size_t, void*),
                               ds3_string_multimap** return_headers) {
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
        handle = (CURL*)ds3_connection_acquire(client->connection_pool);

        if (handle) {
            char* amz_headers;
            char* canonicalized_resource;
            char* date;
            char* date_header;
            char* signature;
            struct curl_slist* headers;
            char* auth_header;
            char* checksum_value;
            ds3_response_data response_data;
            ds3_string_multimap* response_headers = ds3_string_multimap_init();

            ds3_log_message(client->log, DS3_DEBUG, "Preparing to send request");

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

            if (request->checksum == NULL) {
                checksum_value = "";
            } else {
                char* checksum_header;
                checksum_value = request->checksum->value;
                checksum_header = g_strconcat(_get_checksum_type_header(request->checksum_type), checksum_value, NULL);
                headers = curl_slist_append(headers, checksum_header);
                g_free(checksum_header);
            }
            amz_headers = _canonicalize_amz_headers(request->headers);
            canonicalized_resource = _canonicalized_resource(request->path, request->query_params);
            signature = _net_compute_signature(client->log, client->creds, request->verb, canonicalized_resource, date, "", checksum_value, amz_headers);

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
            ds3_connection_release(client->connection_pool, handle);

            //process the response
            if (res != CURLE_OK) {
                char * message = g_strconcat("Request failed: ", curl_easy_strerror(res), NULL);
                ds3_error* error = ds3_create_error(DS3_ERROR_REQUEST_FAILED, message);
                g_free(url);
                g_byte_array_free(response_data.body, TRUE);
                ds3_str_free(response_data.status_message);
                ds3_string_multimap_free(response_headers);
                g_free(message);
                return error;
            }

            ds3_log_message(client->log, DS3_DEBUG, "Request completed with status code of: %d", response_data.status_code);

            if (response_data.status_code == 307) {
                ds3_log_message(client->log, DS3_INFO, "Request encountered a 307 redirect");
                ds3_str_free(response_data.status_message);

                if (response_data.body != NULL) {
                    g_byte_array_free(response_data.body, TRUE);
                }
                ds3_string_multimap_free(response_headers);
                retry_count++;
                ds3_log_message(client->log, DS3_DEBUG, "Retry Attempt: %d | Max Retries: %d", retry_count, client->num_redirects);
                continue;
            }

            if (response_data.status_code < 200 || response_data.status_code >= 300) {
                ds3_error* error = ds3_create_error(DS3_ERROR_BAD_STATUS_CODE, "Got an unexpected status code.");
                error->error = g_new0(ds3_error_response, 1);
                error->error->http_error_code = response_data.status_code;
                error->error->code = ds3_str_init(response_data.status_message->value);
                if (response_data.body != NULL) {
                    error->error->message = ds3_str_init_with_size((char*)response_data.body->data, response_data.body->len);
                    g_byte_array_free(response_data.body, TRUE);
                } else {
                    ds3_log_message(client->log, DS3_ERROR, "The response body for the error is empty");
                    error->error->message = NULL;
                }
                ds3_string_multimap_free(response_headers);
                ds3_str_free(response_data.status_message);
                g_free(url);
                return error;
            }
            g_byte_array_free(response_data.body, TRUE);
            ds3_str_free(response_data.status_message);

            if (return_headers != NULL) {
                *return_headers = response_headers;
            } else {
                ds3_string_multimap_free(response_headers);
            }

            break;
        } else {
            return ds3_create_error(DS3_ERROR_CURL_HANDLE, "Failed to create curl handle");
        }
    }
    g_free(url);

    if (retry_count == client->num_redirects) {
      return ds3_create_error(DS3_ERROR_TOO_MANY_REDIRECTS, "Encountered too many redirects while attempting to fulfill the request");
    }
    return NULL;
}

void net_cleanup(void) {
    curl_global_cleanup();
}

