/*
 * ******************************************************************************
 *   Copyright 2014-2017 Spectra Logic Corporation. All Rights Reserved.
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


#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <libxml/parser.h>
#include <errno.h>

#include "ds3.h"
#include "ds3_net.h"
#include "ds3_request.h"
#include "ds3_string_multimap_impl.h"
#include "ds3_utils.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif


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

ds3_client* ds3_create_client(const char *const endpoint, ds3_creds* creds) {
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

    client->connection_pool = ds3_connection_pool_init();

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

// Allow multiple ds3_clients to share a ds3_connection_pool
ds3_client* ds3_copy_client(const ds3_client const* client) {
    if (client == NULL) {
      return NULL;
    }

    ds3_client* copied_client = g_new0(ds3_client, 1);
    copied_client->endpoint = ds3_str_dup(client->endpoint);
    if (client->proxy) {
        copied_client->proxy = ds3_str_dup(client->proxy);
    }
    copied_client->num_redirects = client->num_redirects;
    copied_client->creds = ds3_create_creds(client->creds->access_id->value, client->creds->secret_key->value);

    ds3_client_register_net( copied_client, net_process_request );
    ds3_client_register_logging(copied_client, client->log->log_lvl, client->log->log_callback, NULL);

    copied_client->connection_pool = client->connection_pool;
    ds3_connection_pool_inc_ref(copied_client->connection_pool);
    return copied_client;
}

void ds3_client_proxy(ds3_client* client, const char* proxy) {
    ds3_str_free(client->proxy);
    client->proxy = ds3_str_init(proxy);
}

void ds3_creds_free(ds3_creds* creds) {
    if (creds == NULL) {
        return;
    }

    ds3_str_free(creds->access_id);
    ds3_str_free(creds->secret_key);
    g_free(creds);
}

void ds3_client_free(ds3_client* client) {
    if (client == NULL) {
      return;
    }

    // free client->connection_pool only if there are no remaining references
    ds3_connection_pool_dec_ref(client->connection_pool);

    ds3_str_free(client->endpoint);
    ds3_str_free(client->proxy);
    g_free(client->log);
    g_free(client);
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

#ifdef _WIN32
static void get_file_size_windows(const char* file_path, uint64_t* file_size) {
    BY_HANDLE_FILE_INFORMATION info;
    HANDLE file_handle;
    char * file_path_windows;
    gunichar2 * file_path_windows_wide;
    BOOL res;

    file_path_windows = g_strdup(file_path);

    /* Convert the path slashes to windows */
    for (char * ch = file_path_windows; *ch != '\0'; ++ch) {
        switch(*ch) {
        case '/': *ch = '\\'; break;
        }
    }

    *file_size = 0;

    /* Convert the path to UTF16 to use in windows native function */
    file_path_windows_wide = g_utf8_to_utf16(file_path_windows, -1, NULL, NULL, NULL);
    if (file_path_windows_wide == NULL) {
        fprintf(stderr, "error converting file name to wide char '%s'\n", file_path_windows);
        g_free(file_path_windows);
        return;
    }


    /* Open the file for attributes read only */
    file_handle = CreateFileW(
        (wchar_t*)file_path_windows_wide,
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    g_free(file_path_windows_wide);

    if (file_handle == INVALID_HANDLE_VALUE) {
        g_free(file_path_windows);
        fprintf(stderr, "error opening file '%s'\n", file_path_windows);
        return;
    }

    res = GetFileInformationByHandle(file_handle, &info);
    CloseHandle(file_handle);

    if (!res) {
        fprintf(stderr, "error getting file attributes\n");
        g_free(file_path_windows);
        return;
    }

    /* If entry is file get the file size */
    if (!(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        ULARGE_INTEGER size;
        size.LowPart = info.nFileSizeLow;
        size.HighPart = info.nFileSizeHigh;

        *file_size =  size.QuadPart;
    } else {
        *file_size = 0;
    }

    g_free(file_path_windows);

    return;
}
#endif

static void get_file_size_posix(const char* file_path, uint64_t* file_size) {
    struct stat file_info;
    int result;

    result = stat(file_path, &file_info);
    if (result != 0) {
        fprintf(stderr, "Failed to get file info for '%s' res=%d errno=%d: %s\n", file_path, result, errno, strerror(errno));
        *file_size = 0;
        return;
    }

    if (S_ISDIR(file_info.st_mode)) {
        *file_size = 0;
    } else {
        *file_size = file_info.st_size;
    }
}

static ds3_bulk_object_response* _ds3_bulk_object_from_file(const char* file_name, const char* base_path) {
    struct stat file_info;
    int result;
    ds3_bulk_object_response* obj = g_new0(ds3_bulk_object_response, 1);
    char* file_to_stat;
    memset(&file_info, 0, sizeof(struct stat));

    if (base_path != NULL) {
        file_to_stat = g_strconcat(base_path, file_name, NULL);
    } else {
        file_to_stat = g_strdup(file_name);
    }

    obj->name = ds3_str_init(file_name);

#ifdef _WIN32
    get_file_size_windows(file_to_stat, &obj->length);
#else
    get_file_size_posix(file_to_stat, &obj->length);
#endif

    g_free(file_to_stat);

    return obj;
}

ds3_bulk_object_list_response* ds3_convert_file_list(const char** file_list, uint64_t num_files) {
    return ds3_convert_file_list_with_basepath(file_list, num_files, NULL);
}

ds3_bulk_object_list_response* ds3_convert_file_list_with_basepath(const char** file_list, uint64_t num_files, const char* base_path) {
    size_t file_index;
    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();

    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();
    for (file_index = 0; file_index < num_files; file_index++) {
        g_ptr_array_add(ds3_bulk_object_response_array, _ds3_bulk_object_from_file(file_list[file_index], base_path));
    }

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    return obj_list;
}

ds3_bulk_object_list_response* ds3_convert_object_list(const ds3_contents_response** objects, uint64_t num_objects) {
    size_t object_index;
    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();

    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();

    for (object_index = 0; object_index < num_objects; object_index++) {
        ds3_bulk_object_response* response = g_new0(ds3_bulk_object_response, 1);
        response->name = ds3_str_dup(objects[object_index]->key);
        g_ptr_array_add(ds3_bulk_object_response_array, response);
    }

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    return obj_list;
}

ds3_bulk_object_list_response* ds3_convert_object_list_from_strings(const char** objects, uint64_t num_objects) {
    size_t object_index;
    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();

    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();

    for (object_index = 0; object_index < num_objects; object_index++) {
        ds3_bulk_object_response* response = g_new0(ds3_bulk_object_response, 1);
        response->name = ds3_str_init(objects[object_index]);
        g_ptr_array_add(ds3_bulk_object_response_array, response);
    }

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    return obj_list;
}

ds3_bulk_object_list_response* ds3_init_bulk_object_list() {
    return g_new0(ds3_bulk_object_list_response, 1);
}

ds3_bulk_object_list_response* ds3_init_bulk_object_list_with_size(size_t num_objects) {
    size_t object_index;
    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();

    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();

    for (object_index = 0; object_index < num_objects; object_index++) {
        ds3_bulk_object_response* response = g_new0(ds3_bulk_object_response, 1);
        g_ptr_array_add(ds3_bulk_object_response_array, response);
    }

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    return obj_list;
}
void ds3_multipart_upload_part_response_free(ds3_multipart_upload_part_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->etag);

    g_free(response);
}

void ds3_complete_multipart_upload_response_free(ds3_complete_multipart_upload_response* response) {
    if (response == NULL) {
        return;
    }

    int index;
    for (index = 0; index < response->num_parts; index++) {
        ds3_multipart_upload_part_response_free(response->parts[index]);
    }
    g_free(response->parts);
    g_free(response);
}

void ds3_delete_objects_response_free(ds3_delete_objects_response* response) {
    if (response == NULL) {
        return;
    }

    int index;
    for (index = 0; index < response->num_strings; index++) {
        ds3_str_free(response->strings_list[index]);
    }
    g_free(response->strings_list);
    g_free(response);
}

void ds3_paging_free(ds3_paging* paging) {
    g_free(paging);
}

void ds3_request_free(ds3_request* _request) {
    struct _ds3_request* request;
    if (_request == NULL) {
        return;
    }

    request = (struct _ds3_request*) _request;

    ds3_str_free(request->path);
    ds3_str_free(request->checksum);

    if (request->headers != NULL) {
        g_hash_table_destroy(request->headers);
    }
    if (request->query_params != NULL) {
        g_hash_table_destroy(request->query_params);
    }

    g_free(request);
}

void ds3_error_free(ds3_error* error) {
    if (error == NULL) {
        return;
    }

    ds3_str_free(error->message);

    ds3_error_response_free(error->error);

    g_free(error);
}

void ds3_cleanup(void) {
    net_cleanup();
    xmlCleanupParser();
}


void ds3_azure_data_replication_rule_response_free(ds3_azure_data_replication_rule_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->data_policy_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_blob_response_free(ds3_blob_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->checksum);
    ds3_str_free(response->id);
    ds3_str_free(response->object_id);

    g_free(response);
}
void ds3_bucket_response_free(ds3_bucket_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->data_policy_id);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_bucket_acl_response_free(ds3_bucket_acl_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->group_id);
    ds3_str_free(response->id);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_canceled_job_response_free(ds3_canceled_job_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->created_at);
    ds3_str_free(response->date_canceled);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->rechunked);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_completed_job_response_free(ds3_completed_job_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->created_at);
    ds3_str_free(response->date_completed);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->rechunked);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_data_path_backend_response_free(ds3_data_path_backend_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->instance_id);
    ds3_str_free(response->last_heartbeat);

    g_free(response);
}
void ds3_data_persistence_rule_response_free(ds3_data_persistence_rule_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->data_policy_id);
    ds3_str_free(response->id);
    ds3_str_free(response->storage_domain_id);

    g_free(response);
}
void ds3_data_policy_response_free(ds3_data_policy_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->name);

    g_free(response);
}
void ds3_data_policy_acl_response_free(ds3_data_policy_acl_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->data_policy_id);
    ds3_str_free(response->group_id);
    ds3_str_free(response->id);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_degraded_blob_response_free(ds3_degraded_blob_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->azure_replication_rule_id);
    ds3_str_free(response->blob_id);
    ds3_str_free(response->bucket_id);
    ds3_str_free(response->ds3_replication_rule_id);
    ds3_str_free(response->id);
    ds3_str_free(response->persistence_rule_id);
    ds3_str_free(response->s3_replication_rule_id);

    g_free(response);
}
void ds3_data_replication_rule_response_free(ds3_data_replication_rule_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->data_policy_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_data_policy);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_feature_key_response_free(ds3_feature_key_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->error_message);
    ds3_str_free(response->expiration_date);
    ds3_str_free(response->id);

    g_free(response);
}
void ds3_group_response_free(ds3_group_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->name);

    g_free(response);
}
void ds3_group_member_response_free(ds3_group_member_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->group_id);
    ds3_str_free(response->id);
    ds3_str_free(response->member_group_id);
    ds3_str_free(response->member_user_id);

    g_free(response);
}
void ds3_active_job_response_free(ds3_active_job_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->created_at);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->rechunked);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_job_chunk_response_free(ds3_job_chunk_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->job_creation_date);
    ds3_str_free(response->job_id);
    ds3_str_free(response->node_id);
    ds3_str_free(response->read_from_azure_target_id);
    ds3_str_free(response->read_from_ds3_target_id);
    ds3_str_free(response->read_from_pool_id);
    ds3_str_free(response->read_from_s3_target_id);
    ds3_str_free(response->read_from_tape_id);

    g_free(response);
}
void ds3_node_response_free(ds3_node_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->data_path_ip_address);
    ds3_str_free(response->dns_name);
    ds3_str_free(response->id);
    ds3_str_free(response->last_heartbeat);
    ds3_str_free(response->name);
    ds3_str_free(response->serial_number);

    g_free(response);
}
void ds3_s3_data_replication_rule_response_free(ds3_s3_data_replication_rule_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->data_policy_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_s3_object_response_free(ds3_s3_object_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->name);

    g_free(response);
}
void ds3_storage_domain_response_free(ds3_storage_domain_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->auto_eject_upon_cron);
    ds3_str_free(response->id);
    ds3_str_free(response->name);

    g_free(response);
}
void ds3_storage_domain_capacity_summary_response_free(ds3_storage_domain_capacity_summary_response* response) {
    if (response == NULL) {
        return;
    }


    g_free(response);
}
void ds3_storage_domain_failure_response_free(ds3_storage_domain_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->storage_domain_id);

    g_free(response);
}
void ds3_storage_domain_member_response_free(ds3_storage_domain_member_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->pool_partition_id);
    ds3_str_free(response->storage_domain_id);
    ds3_str_free(response->tape_partition_id);

    g_free(response);
}
void ds3_system_failure_response_free(ds3_system_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);

    g_free(response);
}
void ds3_spectra_user_response_free(ds3_spectra_user_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->auth_id);
    ds3_str_free(response->default_data_policy_id);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->secret_key);

    g_free(response);
}
void ds3_azure_target_failure_notification_registration_response_free(ds3_azure_target_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_target_failure_notification_registration_response_free(ds3_target_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_generic_dao_notification_registration_response_free(ds3_generic_dao_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->dao_type);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_job_completed_notification_registration_response_free(ds3_job_completed_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->job_id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_job_created_notification_registration_response_free(ds3_job_created_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_job_creation_failed_notification_registration_response_free(ds3_job_creation_failed_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_pool_failure_notification_registration_response_free(ds3_pool_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_s3_object_cached_notification_registration_response_free(ds3_s3_object_cached_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->job_id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_s3_object_lost_notification_registration_response_free(ds3_s3_object_lost_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_s3_object_persisted_notification_registration_response_free(ds3_s3_object_persisted_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->job_id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_s3_target_failure_notification_registration_response_free(ds3_s3_target_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_storage_domain_failure_notification_registration_response_free(ds3_storage_domain_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_system_failure_notification_registration_response_free(ds3_system_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_tape_failure_notification_registration_response_free(ds3_tape_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_tape_partition_failure_notification_registration_response_free(ds3_tape_partition_failure_notification_registration_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->id);
    ds3_str_free(response->last_failure);
    ds3_str_free(response->last_notification);
    ds3_str_free(response->notification_end_point);
    ds3_str_free(response->user_id);

    g_free(response);
}
void ds3_cache_filesystem_response_free(ds3_cache_filesystem_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->node_id);
    ds3_str_free(response->path);

    g_free(response);
}
void ds3_pool_response_free(ds3_pool_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->guid);
    ds3_str_free(response->id);
    ds3_str_free(response->last_accessed);
    ds3_str_free(response->last_modified);
    ds3_str_free(response->last_verified);
    ds3_str_free(response->mountpoint);
    ds3_str_free(response->name);
    ds3_str_free(response->partition_id);
    ds3_str_free(response->storage_domain_id);

    g_free(response);
}
void ds3_pool_failure_response_free(ds3_pool_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->pool_id);

    g_free(response);
}
void ds3_pool_partition_response_free(ds3_pool_partition_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->name);

    g_free(response);
}
void ds3_suspect_blob_pool_response_free(ds3_suspect_blob_pool_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->blob_id);
    ds3_str_free(response->bucket_id);
    ds3_str_free(response->date_written);
    ds3_str_free(response->id);
    ds3_str_free(response->last_accessed);
    ds3_str_free(response->pool_id);

    g_free(response);
}
void ds3_suspect_blob_tape_response_free(ds3_suspect_blob_tape_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->blob_id);
    ds3_str_free(response->id);
    ds3_str_free(response->tape_id);

    g_free(response);
}
void ds3_tape_response_free(ds3_tape_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bar_code);
    ds3_str_free(response->bucket_id);
    ds3_str_free(response->description_for_identification);
    ds3_str_free(response->eject_date);
    ds3_str_free(response->eject_label);
    ds3_str_free(response->eject_location);
    ds3_str_free(response->eject_pending);
    ds3_str_free(response->id);
    ds3_str_free(response->last_accessed);
    ds3_str_free(response->last_checkpoint);
    ds3_str_free(response->last_modified);
    ds3_str_free(response->last_verified);
    ds3_str_free(response->partially_verified_end_of_tape);
    ds3_str_free(response->partition_id);
    ds3_str_free(response->serial_number);
    ds3_str_free(response->storage_domain_id);

    g_free(response);
}
void ds3_tape_density_directive_response_free(ds3_tape_density_directive_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->partition_id);

    g_free(response);
}
void ds3_tape_drive_response_free(ds3_tape_drive_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->last_cleaned);
    ds3_str_free(response->mfg_serial_number);
    ds3_str_free(response->partition_id);
    ds3_str_free(response->serial_number);
    ds3_str_free(response->tape_id);

    g_free(response);
}
void ds3_detailed_tape_failure_response_free(ds3_detailed_tape_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->tape_drive_id);
    ds3_str_free(response->tape_id);

    g_free(response);
}
void ds3_tape_library_response_free(ds3_tape_library_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->id);
    ds3_str_free(response->management_url);
    ds3_str_free(response->name);
    ds3_str_free(response->serial_number);

    g_free(response);
}
void ds3_tape_partition_response_free(ds3_tape_partition_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->library_id);
    ds3_str_free(response->name);
    ds3_str_free(response->serial_id);
    ds3_str_free(response->serial_number);

    g_free(response);
}
void ds3_tape_partition_failure_response_free(ds3_tape_partition_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->partition_id);

    g_free(response);
}
void ds3_azure_target_response_free(ds3_azure_target_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->account_key);
    ds3_str_free(response->account_name);
    ds3_str_free(response->cloud_bucket_prefix);
    ds3_str_free(response->cloud_bucket_suffix);
    ds3_str_free(response->id);
    ds3_str_free(response->last_fully_verified);
    ds3_str_free(response->name);

    g_free(response);
}
void ds3_azure_target_bucket_name_response_free(ds3_azure_target_bucket_name_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_azure_target_failure_response_free(ds3_azure_target_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_azure_target_read_preference_response_free(ds3_azure_target_read_preference_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_target_response_free(ds3_target_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->admin_auth_id);
    ds3_str_free(response->admin_secret_key);
    ds3_str_free(response->data_path_end_point);
    ds3_str_free(response->data_path_proxy);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->replicated_user_default_data_policy);

    g_free(response);
}
void ds3_target_failure_response_free(ds3_target_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_target_read_preference_response_free(ds3_target_read_preference_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_s3_target_response_free(ds3_s3_target_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->access_key);
    ds3_str_free(response->cloud_bucket_prefix);
    ds3_str_free(response->cloud_bucket_suffix);
    ds3_str_free(response->data_path_end_point);
    ds3_str_free(response->id);
    ds3_str_free(response->last_fully_verified);
    ds3_str_free(response->name);
    ds3_str_free(response->proxy_domain);
    ds3_str_free(response->proxy_host);
    ds3_str_free(response->proxy_password);
    ds3_str_free(response->proxy_username);
    ds3_str_free(response->secret_key);

    g_free(response);
}
void ds3_s3_target_bucket_name_response_free(ds3_s3_target_bucket_name_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_s3_target_failure_response_free(ds3_s3_target_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_s3_target_read_preference_response_free(ds3_s3_target_read_preference_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_suspect_blob_azure_target_response_free(ds3_suspect_blob_azure_target_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->blob_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_suspect_blob_ds3_target_response_free(ds3_suspect_blob_ds3_target_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->blob_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_suspect_blob_s3_target_response_free(ds3_suspect_blob_s3_target_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->blob_id);
    ds3_str_free(response->id);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_build_information_response_free(ds3_build_information_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->branch);
    ds3_str_free(response->revision);
    ds3_str_free(response->version);

    g_free(response);
}
void ds3_azure_target_failure_notification_payload_response_free(ds3_azure_target_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_target_failure_notification_payload_response_free(ds3_target_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_generic_dao_notification_payload_response_free(ds3_generic_dao_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->dao_type);
    for (index = 0; index < response->num_ids; index++) {
        ds3_str_free(response->ids[index]);
    }
    g_free(response->ids);
    ds3_str_free(response->notification_generation_date);

    g_free(response);
}
void ds3_job_created_notification_payload_response_free(ds3_job_created_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->job_id);
    ds3_str_free(response->notification_generation_date);

    g_free(response);
}
void ds3_pool_failure_notification_payload_response_free(ds3_pool_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);
    ds3_str_free(response->pool_id);

    g_free(response);
}
void ds3_s3_target_failure_notification_payload_response_free(ds3_s3_target_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);
    ds3_str_free(response->target_id);

    g_free(response);
}
void ds3_set_of_tape_bar_codes_response_free(ds3_set_of_tape_bar_codes_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_bar_codes; index++) {
        ds3_str_free(response->tape_bar_codes[index]);
    }
    g_free(response->tape_bar_codes);

    g_free(response);
}
void ds3_storage_domain_failure_notification_payload_response_free(ds3_storage_domain_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);
    ds3_str_free(response->storage_domain_id);

    g_free(response);
}
void ds3_system_failure_notification_payload_response_free(ds3_system_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);

    g_free(response);
}
void ds3_tape_failure_notification_payload_response_free(ds3_tape_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);
    ds3_str_free(response->tape_drive_id);
    ds3_str_free(response->tape_id);

    g_free(response);
}
void ds3_tape_partition_failure_notification_payload_response_free(ds3_tape_partition_failure_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date);
    ds3_str_free(response->error_message);
    ds3_str_free(response->notification_generation_date);
    ds3_str_free(response->partition_id);

    g_free(response);
}
void ds3_tapes_must_be_onlined_response_free(ds3_tapes_must_be_onlined_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tapes_to_online; index++) {
        ds3_set_of_tape_bar_codes_response_free(response->tapes_to_online[index]);
    }
    g_free(response->tapes_to_online);

    g_free(response);
}
void ds3_blob_store_task_information_response_free(ds3_blob_store_task_information_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->date_scheduled);
    ds3_str_free(response->date_started);
    ds3_str_free(response->description);
    ds3_str_free(response->drive_id);
    ds3_str_free(response->name);
    ds3_str_free(response->pool_id);
    ds3_str_free(response->tape_id);
    ds3_str_free(response->target_id);
    ds3_str_free(response->target_type);

    g_free(response);
}
void ds3_blob_store_tasks_information_response_free(ds3_blob_store_tasks_information_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tasks; index++) {
        ds3_blob_store_task_information_response_free(response->tasks[index]);
    }
    g_free(response->tasks);

    g_free(response);
}
void ds3_cache_entry_information_response_free(ds3_cache_entry_information_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_blob_response_free(response->blob);

    g_free(response);
}
void ds3_cache_filesystem_information_response_free(ds3_cache_filesystem_information_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_cache_filesystem_response_free(response->cache_filesystem);
    for (index = 0; index < response->num_entries; index++) {
        ds3_cache_entry_information_response_free(response->entries[index]);
    }
    g_free(response->entries);
    ds3_str_free(response->summary);

    g_free(response);
}
void ds3_cache_information_response_free(ds3_cache_information_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_filesystems; index++) {
        ds3_cache_filesystem_information_response_free(response->filesystems[index]);
    }
    g_free(response->filesystems);

    g_free(response);
}
void ds3_bucket_details_response_free(ds3_bucket_details_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->creation_date);
    ds3_str_free(response->name);

    g_free(response);
}
void ds3_complete_multipart_upload_result_response_free(ds3_complete_multipart_upload_result_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket);
    ds3_str_free(response->e_tag);
    ds3_str_free(response->key);
    ds3_str_free(response->location);

    g_free(response);
}
void ds3_delete_object_error_response_free(ds3_delete_object_error_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->code);
    ds3_str_free(response->key);
    ds3_str_free(response->message);

    g_free(response);
}
void ds3_detailed_tape_partition_response_free(ds3_detailed_tape_partition_response* response) {
    if (response == NULL) {
        return;
    }

    g_free(response->drive_types);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->library_id);
    ds3_str_free(response->name);
    ds3_str_free(response->serial_id);
    ds3_str_free(response->serial_number);
    g_free(response->tape_types);

    g_free(response);
}
void ds3_error_response_free(ds3_error_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->code);
    ds3_str_free(response->message);
    ds3_str_free(response->resource);

    g_free(response);
}
void ds3_initiate_multipart_upload_result_response_free(ds3_initiate_multipart_upload_result_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket);
    ds3_str_free(response->key);
    ds3_str_free(response->upload_id);

    g_free(response);
}
void ds3_multi_part_upload_part_response_free(ds3_multi_part_upload_part_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->e_tag);
    ds3_str_free(response->last_modified);

    g_free(response);
}
void ds3_job_node_response_free(ds3_job_node_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->end_point);
    ds3_str_free(response->id);

    g_free(response);
}
void ds3_s3_object_to_delete_response_free(ds3_s3_object_to_delete_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->key);

    g_free(response);
}
void ds3_user_response_free(ds3_user_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->display_name);
    ds3_str_free(response->id);

    g_free(response);
}
void ds3_system_information_response_free(ds3_system_information_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->api_version);
    ds3_build_information_response_free(response->build_information);
    ds3_str_free(response->instance_id);
    ds3_str_free(response->serial_number);

    g_free(response);
}
void ds3_health_verification_result_response_free(ds3_health_verification_result_response* response) {
    if (response == NULL) {
        return;
    }


    g_free(response);
}
void ds3_named_detailed_tape_partition_response_free(ds3_named_detailed_tape_partition_response* response) {
    if (response == NULL) {
        return;
    }

    g_free(response->drive_types);
    ds3_str_free(response->error_message);
    ds3_str_free(response->id);
    ds3_str_free(response->library_id);
    ds3_str_free(response->name);
    ds3_str_free(response->serial_id);
    ds3_str_free(response->serial_number);
    g_free(response->tape_types);

    g_free(response);
}
void ds3_tape_failure_response_free(ds3_tape_failure_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->cause);
    ds3_tape_response_free(response->tape);

    g_free(response);
}
void ds3_tape_failure_list_response_free(ds3_tape_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_failures; index++) {
        ds3_tape_failure_response_free(response->failures[index]);
    }
    g_free(response->failures);

    g_free(response);
}
void ds3_create_heap_dump_params_response_free(ds3_create_heap_dump_params_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->path);

    g_free(response);
}
void ds3_type_response_free(ds3_type_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->beans_retriever_name);
    ds3_str_free(response->domain_name);

    g_free(response);
}
void ds3_duration_response_free(ds3_duration_response* response) {
    if (response == NULL) {
        return;
    }


    g_free(response);
}
void ds3_bucket_acl_list_response_free(ds3_bucket_acl_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_bucket_acls; index++) {
        ds3_bucket_acl_response_free(response->bucket_acls[index]);
    }
    g_free(response->bucket_acls);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_data_policy_acl_list_response_free(ds3_data_policy_acl_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_data_policy_acls; index++) {
        ds3_data_policy_acl_response_free(response->data_policy_acls[index]);
    }
    g_free(response->data_policy_acls);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_bucket_list_response_free(ds3_bucket_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_buckets; index++) {
        ds3_bucket_response_free(response->buckets[index]);
    }
    g_free(response->buckets);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_cache_filesystem_list_response_free(ds3_cache_filesystem_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_cache_filesystems; index++) {
        ds3_cache_filesystem_response_free(response->cache_filesystems[index]);
    }
    g_free(response->cache_filesystems);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_azure_data_replication_rule_list_response_free(ds3_azure_data_replication_rule_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_azure_data_replication_rules; index++) {
        ds3_azure_data_replication_rule_response_free(response->azure_data_replication_rules[index]);
    }
    g_free(response->azure_data_replication_rules);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_data_persistence_rule_list_response_free(ds3_data_persistence_rule_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_data_persistence_rules; index++) {
        ds3_data_persistence_rule_response_free(response->data_persistence_rules[index]);
    }
    g_free(response->data_persistence_rules);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_data_policy_list_response_free(ds3_data_policy_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_data_policies; index++) {
        ds3_data_policy_response_free(response->data_policies[index]);
    }
    g_free(response->data_policies);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_data_replication_rule_list_response_free(ds3_data_replication_rule_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_ds3_data_replication_rules; index++) {
        ds3_data_replication_rule_response_free(response->ds3_data_replication_rules[index]);
    }
    g_free(response->ds3_data_replication_rules);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_data_replication_rule_list_response_free(ds3_s3_data_replication_rule_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_data_replication_rules; index++) {
        ds3_s3_data_replication_rule_response_free(response->s3_data_replication_rules[index]);
    }
    g_free(response->s3_data_replication_rules);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_degraded_blob_list_response_free(ds3_degraded_blob_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_degraded_blobs; index++) {
        ds3_degraded_blob_response_free(response->degraded_blobs[index]);
    }
    g_free(response->degraded_blobs);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_suspect_blob_azure_target_list_response_free(ds3_suspect_blob_azure_target_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_suspect_blob_azure_targets; index++) {
        ds3_suspect_blob_azure_target_response_free(response->suspect_blob_azure_targets[index]);
    }
    g_free(response->suspect_blob_azure_targets);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_suspect_blob_ds3_target_list_response_free(ds3_suspect_blob_ds3_target_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_suspect_blob_ds3_targets; index++) {
        ds3_suspect_blob_ds3_target_response_free(response->suspect_blob_ds3_targets[index]);
    }
    g_free(response->suspect_blob_ds3_targets);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_suspect_blob_pool_list_response_free(ds3_suspect_blob_pool_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_suspect_blob_pools; index++) {
        ds3_suspect_blob_pool_response_free(response->suspect_blob_pools[index]);
    }
    g_free(response->suspect_blob_pools);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_suspect_blob_s3_target_list_response_free(ds3_suspect_blob_s3_target_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_suspect_blob_s3_targets; index++) {
        ds3_suspect_blob_s3_target_response_free(response->suspect_blob_s3_targets[index]);
    }
    g_free(response->suspect_blob_s3_targets);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_suspect_blob_tape_list_response_free(ds3_suspect_blob_tape_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_suspect_blob_tapes; index++) {
        ds3_suspect_blob_tape_response_free(response->suspect_blob_tapes[index]);
    }
    g_free(response->suspect_blob_tapes);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_object_list_response_free(ds3_s3_object_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_objects; index++) {
        ds3_s3_object_response_free(response->s3_objects[index]);
    }
    g_free(response->s3_objects);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_group_member_list_response_free(ds3_group_member_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_group_members; index++) {
        ds3_group_member_response_free(response->group_members[index]);
    }
    g_free(response->group_members);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_group_list_response_free(ds3_group_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_groups; index++) {
        ds3_group_response_free(response->groups[index]);
    }
    g_free(response->groups);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_active_job_list_response_free(ds3_active_job_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_active_jobs; index++) {
        ds3_active_job_response_free(response->active_jobs[index]);
    }
    g_free(response->active_jobs);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_canceled_job_list_response_free(ds3_canceled_job_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_canceled_jobs; index++) {
        ds3_canceled_job_response_free(response->canceled_jobs[index]);
    }
    g_free(response->canceled_jobs);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_completed_job_list_response_free(ds3_completed_job_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_completed_jobs; index++) {
        ds3_completed_job_response_free(response->completed_jobs[index]);
    }
    g_free(response->completed_jobs);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_node_list_response_free(ds3_node_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_nodes; index++) {
        ds3_node_response_free(response->nodes[index]);
    }
    g_free(response->nodes);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_azure_target_failure_notification_registration_list_response_free(ds3_azure_target_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_azure_target_failure_notification_registrations; index++) {
        ds3_azure_target_failure_notification_registration_response_free(response->azure_target_failure_notification_registrations[index]);
    }
    g_free(response->azure_target_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_target_failure_notification_registration_list_response_free(ds3_target_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_ds3_target_failure_notification_registrations; index++) {
        ds3_target_failure_notification_registration_response_free(response->ds3_target_failure_notification_registrations[index]);
    }
    g_free(response->ds3_target_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_job_completed_notification_registration_list_response_free(ds3_job_completed_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_job_completed_notification_registrations; index++) {
        ds3_job_completed_notification_registration_response_free(response->job_completed_notification_registrations[index]);
    }
    g_free(response->job_completed_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_job_created_notification_registration_list_response_free(ds3_job_created_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_job_created_notification_registrations; index++) {
        ds3_job_created_notification_registration_response_free(response->job_created_notification_registrations[index]);
    }
    g_free(response->job_created_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_job_creation_failed_notification_registration_list_response_free(ds3_job_creation_failed_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_job_creation_failed_notification_registrations; index++) {
        ds3_job_creation_failed_notification_registration_response_free(response->job_creation_failed_notification_registrations[index]);
    }
    g_free(response->job_creation_failed_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_object_cached_notification_registration_list_response_free(ds3_s3_object_cached_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_object_cached_notification_registrations; index++) {
        ds3_s3_object_cached_notification_registration_response_free(response->s3_object_cached_notification_registrations[index]);
    }
    g_free(response->s3_object_cached_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_object_lost_notification_registration_list_response_free(ds3_s3_object_lost_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_object_lost_notification_registrations; index++) {
        ds3_s3_object_lost_notification_registration_response_free(response->s3_object_lost_notification_registrations[index]);
    }
    g_free(response->s3_object_lost_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_object_persisted_notification_registration_list_response_free(ds3_s3_object_persisted_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_object_persisted_notification_registrations; index++) {
        ds3_s3_object_persisted_notification_registration_response_free(response->s3_object_persisted_notification_registrations[index]);
    }
    g_free(response->s3_object_persisted_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_pool_failure_notification_registration_list_response_free(ds3_pool_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_pool_failure_notification_registrations; index++) {
        ds3_pool_failure_notification_registration_response_free(response->pool_failure_notification_registrations[index]);
    }
    g_free(response->pool_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_target_failure_notification_registration_list_response_free(ds3_s3_target_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_target_failure_notification_registrations; index++) {
        ds3_s3_target_failure_notification_registration_response_free(response->s3_target_failure_notification_registrations[index]);
    }
    g_free(response->s3_target_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_storage_domain_failure_notification_registration_list_response_free(ds3_storage_domain_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_storage_domain_failure_notification_registrations; index++) {
        ds3_storage_domain_failure_notification_registration_response_free(response->storage_domain_failure_notification_registrations[index]);
    }
    g_free(response->storage_domain_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_system_failure_notification_registration_list_response_free(ds3_system_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_system_failure_notification_registrations; index++) {
        ds3_system_failure_notification_registration_response_free(response->system_failure_notification_registrations[index]);
    }
    g_free(response->system_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_failure_notification_registration_list_response_free(ds3_tape_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_failure_notification_registrations; index++) {
        ds3_tape_failure_notification_registration_response_free(response->tape_failure_notification_registrations[index]);
    }
    g_free(response->tape_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_partition_failure_notification_registration_list_response_free(ds3_tape_partition_failure_notification_registration_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_partition_failure_notification_registrations; index++) {
        ds3_tape_partition_failure_notification_registration_response_free(response->tape_partition_failure_notification_registrations[index]);
    }
    g_free(response->tape_partition_failure_notification_registrations);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_pool_failure_list_response_free(ds3_pool_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_pool_failures; index++) {
        ds3_pool_failure_response_free(response->pool_failures[index]);
    }
    g_free(response->pool_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_pool_partition_list_response_free(ds3_pool_partition_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_pool_partitions; index++) {
        ds3_pool_partition_response_free(response->pool_partitions[index]);
    }
    g_free(response->pool_partitions);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_pool_list_response_free(ds3_pool_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_pools; index++) {
        ds3_pool_response_free(response->pools[index]);
    }
    g_free(response->pools);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_storage_domain_failure_list_response_free(ds3_storage_domain_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_storage_domain_failures; index++) {
        ds3_storage_domain_failure_response_free(response->storage_domain_failures[index]);
    }
    g_free(response->storage_domain_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_storage_domain_member_list_response_free(ds3_storage_domain_member_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_storage_domain_members; index++) {
        ds3_storage_domain_member_response_free(response->storage_domain_members[index]);
    }
    g_free(response->storage_domain_members);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_storage_domain_list_response_free(ds3_storage_domain_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_storage_domains; index++) {
        ds3_storage_domain_response_free(response->storage_domains[index]);
    }
    g_free(response->storage_domains);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_feature_key_list_response_free(ds3_feature_key_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_feature_keys; index++) {
        ds3_feature_key_response_free(response->feature_keys[index]);
    }
    g_free(response->feature_keys);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_system_failure_list_response_free(ds3_system_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_system_failures; index++) {
        ds3_system_failure_response_free(response->system_failures[index]);
    }
    g_free(response->system_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_density_directive_list_response_free(ds3_tape_density_directive_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_density_directives; index++) {
        ds3_tape_density_directive_response_free(response->tape_density_directives[index]);
    }
    g_free(response->tape_density_directives);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_drive_list_response_free(ds3_tape_drive_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_drives; index++) {
        ds3_tape_drive_response_free(response->tape_drives[index]);
    }
    g_free(response->tape_drives);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_detailed_tape_failure_list_response_free(ds3_detailed_tape_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_detailed_tape_failures; index++) {
        ds3_detailed_tape_failure_response_free(response->detailed_tape_failures[index]);
    }
    g_free(response->detailed_tape_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_library_list_response_free(ds3_tape_library_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_libraries; index++) {
        ds3_tape_library_response_free(response->tape_libraries[index]);
    }
    g_free(response->tape_libraries);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_partition_failure_list_response_free(ds3_tape_partition_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_partition_failures; index++) {
        ds3_tape_partition_failure_response_free(response->tape_partition_failures[index]);
    }
    g_free(response->tape_partition_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_partition_list_response_free(ds3_tape_partition_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tape_partitions; index++) {
        ds3_tape_partition_response_free(response->tape_partitions[index]);
    }
    g_free(response->tape_partitions);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_named_detailed_tape_partition_list_response_free(ds3_named_detailed_tape_partition_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_named_detailed_tape_partitions; index++) {
        ds3_named_detailed_tape_partition_response_free(response->named_detailed_tape_partitions[index]);
    }
    g_free(response->named_detailed_tape_partitions);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_tape_list_response_free(ds3_tape_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_tapes; index++) {
        ds3_tape_response_free(response->tapes[index]);
    }
    g_free(response->tapes);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_azure_target_bucket_name_list_response_free(ds3_azure_target_bucket_name_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_azure_target_bucket_names; index++) {
        ds3_azure_target_bucket_name_response_free(response->azure_target_bucket_names[index]);
    }
    g_free(response->azure_target_bucket_names);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_azure_target_failure_list_response_free(ds3_azure_target_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_azure_target_failures; index++) {
        ds3_azure_target_failure_response_free(response->azure_target_failures[index]);
    }
    g_free(response->azure_target_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_azure_target_read_preference_list_response_free(ds3_azure_target_read_preference_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_azure_target_read_preferences; index++) {
        ds3_azure_target_read_preference_response_free(response->azure_target_read_preferences[index]);
    }
    g_free(response->azure_target_read_preferences);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_azure_target_list_response_free(ds3_azure_target_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_azure_targets; index++) {
        ds3_azure_target_response_free(response->azure_targets[index]);
    }
    g_free(response->azure_targets);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_target_failure_list_response_free(ds3_target_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_ds3_target_failures; index++) {
        ds3_target_failure_response_free(response->ds3_target_failures[index]);
    }
    g_free(response->ds3_target_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_target_read_preference_list_response_free(ds3_target_read_preference_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_ds3_target_read_preferences; index++) {
        ds3_target_read_preference_response_free(response->ds3_target_read_preferences[index]);
    }
    g_free(response->ds3_target_read_preferences);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_target_list_response_free(ds3_target_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_ds3_targets; index++) {
        ds3_target_response_free(response->ds3_targets[index]);
    }
    g_free(response->ds3_targets);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_target_bucket_name_list_response_free(ds3_s3_target_bucket_name_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_target_bucket_names; index++) {
        ds3_s3_target_bucket_name_response_free(response->s3_target_bucket_names[index]);
    }
    g_free(response->s3_target_bucket_names);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_target_failure_list_response_free(ds3_s3_target_failure_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_target_failures; index++) {
        ds3_s3_target_failure_response_free(response->s3_target_failures[index]);
    }
    g_free(response->s3_target_failures);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_target_read_preference_list_response_free(ds3_s3_target_read_preference_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_target_read_preferences; index++) {
        ds3_s3_target_read_preference_response_free(response->s3_target_read_preferences[index]);
    }
    g_free(response->s3_target_read_preferences);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_s3_target_list_response_free(ds3_s3_target_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_s3_targets; index++) {
        ds3_s3_target_response_free(response->s3_targets[index]);
    }
    g_free(response->s3_targets);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_spectra_user_list_response_free(ds3_spectra_user_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_spectra_users; index++) {
        ds3_spectra_user_response_free(response->spectra_users[index]);
    }
    g_free(response->spectra_users);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_physical_placement_response_free(ds3_physical_placement_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_azure_targets; index++) {
        ds3_azure_target_response_free(response->azure_targets[index]);
    }
    g_free(response->azure_targets);
    for (index = 0; index < response->num_ds3_targets; index++) {
        ds3_target_response_free(response->ds3_targets[index]);
    }
    g_free(response->ds3_targets);
    for (index = 0; index < response->num_pools; index++) {
        ds3_pool_response_free(response->pools[index]);
    }
    g_free(response->pools);
    for (index = 0; index < response->num_s3_targets; index++) {
        ds3_s3_target_response_free(response->s3_targets[index]);
    }
    g_free(response->s3_targets);
    for (index = 0; index < response->num_tapes; index++) {
        ds3_tape_response_free(response->tapes[index]);
    }
    g_free(response->tapes);

    g_free(response);
}
void ds3_capacity_summary_container_response_free(ds3_capacity_summary_container_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_storage_domain_capacity_summary_response_free(response->pool);
    ds3_storage_domain_capacity_summary_response_free(response->tape);

    g_free(response);
}
void ds3_bulk_object_response_free(ds3_bulk_object_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->bucket);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_physical_placement_response_free(response->physical_placement);

    g_free(response);
}
void ds3_bulk_object_list_response_free(ds3_bulk_object_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_objects; index++) {
        ds3_bulk_object_response_free(response->objects[index]);
    }
    g_free(response->objects);

    g_free(response);
}
void ds3_job_completed_notification_payload_response_free(ds3_job_completed_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->job_id);
    ds3_str_free(response->notification_generation_date);
    for (index = 0; index < response->num_objects_not_persisted; index++) {
        ds3_bulk_object_response_free(response->objects_not_persisted[index]);
    }
    g_free(response->objects_not_persisted);

    g_free(response);
}
void ds3_job_creation_failed_notification_payload_response_free(ds3_job_creation_failed_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->notification_generation_date);
    ds3_tapes_must_be_onlined_response_free(response->tapes_must_be_onlined);

    g_free(response);
}
void ds3_s3_objects_cached_notification_payload_response_free(ds3_s3_objects_cached_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->job_id);
    ds3_str_free(response->notification_generation_date);
    for (index = 0; index < response->num_objects; index++) {
        ds3_bulk_object_response_free(response->objects[index]);
    }
    g_free(response->objects);

    g_free(response);
}
void ds3_s3_objects_lost_notification_payload_response_free(ds3_s3_objects_lost_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->notification_generation_date);
    for (index = 0; index < response->num_objects; index++) {
        ds3_bulk_object_response_free(response->objects[index]);
    }
    g_free(response->objects);

    g_free(response);
}
void ds3_s3_objects_persisted_notification_payload_response_free(ds3_s3_objects_persisted_notification_payload_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->job_id);
    ds3_str_free(response->notification_generation_date);
    for (index = 0; index < response->num_objects; index++) {
        ds3_bulk_object_response_free(response->objects[index]);
    }
    g_free(response->objects);

    g_free(response);
}
void ds3_list_all_my_buckets_result_response_free(ds3_list_all_my_buckets_result_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_buckets; index++) {
        ds3_bucket_details_response_free(response->buckets[index]);
    }
    g_free(response->buckets);
    ds3_user_response_free(response->owner);

    g_free(response);
}
void ds3_delete_result_response_free(ds3_delete_result_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_deleted_objects; index++) {
        ds3_s3_object_to_delete_response_free(response->deleted_objects[index]);
    }
    g_free(response->deleted_objects);
    for (index = 0; index < response->num_errors; index++) {
        ds3_delete_object_error_response_free(response->errors[index]);
    }
    g_free(response->errors);

    g_free(response);
}
void ds3_job_response_free(ds3_job_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->bucket_name);
    ds3_str_free(response->job_id);
    ds3_str_free(response->name);
    for (index = 0; index < response->num_nodes; index++) {
        ds3_job_node_response_free(response->nodes[index]);
    }
    g_free(response->nodes);
    ds3_str_free(response->start_date);
    ds3_str_free(response->user_id);
    ds3_str_free(response->user_name);

    g_free(response);
}
void ds3_objects_response_free(ds3_objects_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->chunk_id);
    ds3_str_free(response->node_id);
    for (index = 0; index < response->num_objects; index++) {
        ds3_bulk_object_response_free(response->objects[index]);
    }
    g_free(response->objects);

    g_free(response);
}
void ds3_master_object_list_response_free(ds3_master_object_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->bucket_name);
    ds3_str_free(response->job_id);
    ds3_str_free(response->name);
    for (index = 0; index < response->num_nodes; index++) {
        ds3_job_node_response_free(response->nodes[index]);
    }
    g_free(response->nodes);
    for (index = 0; index < response->num_objects; index++) {
        ds3_objects_response_free(response->objects[index]);
    }
    g_free(response->objects);
    ds3_str_free(response->start_date);
    ds3_str_free(response->user_id);
    ds3_str_free(response->user_name);

    g_free(response);
}
void ds3_job_list_response_free(ds3_job_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_jobs; index++) {
        ds3_job_response_free(response->jobs[index]);
    }
    g_free(response->jobs);

    g_free(response);
}
void ds3_list_parts_result_response_free(ds3_list_parts_result_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->bucket);
    ds3_str_free(response->key);
    ds3_user_response_free(response->owner);
    for (index = 0; index < response->num_parts; index++) {
        ds3_multi_part_upload_part_response_free(response->parts[index]);
    }
    g_free(response->parts);
    ds3_str_free(response->upload_id);

    g_free(response);
}
void ds3_multi_part_upload_response_free(ds3_multi_part_upload_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->initiated);
    ds3_str_free(response->key);
    ds3_user_response_free(response->owner);
    ds3_str_free(response->upload_id);

    g_free(response);
}
void ds3_contents_response_free(ds3_contents_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->e_tag);
    ds3_str_free(response->key);
    ds3_str_free(response->last_modified);
    ds3_user_response_free(response->owner);
    ds3_str_free(response->storage_class);

    g_free(response);
}
void ds3_detailed_s3_object_response_free(ds3_detailed_s3_object_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_bulk_object_list_response_free(response->blobs);
    ds3_str_free(response->bucket_id);
    ds3_str_free(response->creation_date);
    ds3_str_free(response->e_tag);
    ds3_str_free(response->id);
    ds3_str_free(response->name);
    ds3_str_free(response->owner);

    g_free(response);
}
void ds3_database_contents_response_free(ds3_database_contents_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_types; index++) {
        ds3_type_response_free(response->types[index]);
    }
    g_free(response->types);

    g_free(response);
}
void ds3_detailed_s3_object_list_response_free(ds3_detailed_s3_object_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_detailed_s3_objects; index++) {
        ds3_detailed_s3_object_response_free(response->detailed_s3_objects[index]);
    }
    g_free(response->detailed_s3_objects);
    ds3_paging_free(response->paging);

    g_free(response);
}
void ds3_list_bucket_result_response_free(ds3_list_bucket_result_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_common_prefixes; index++) {
        ds3_str_free(response->common_prefixes[index]);
    }
    g_free(response->common_prefixes);
    ds3_str_free(response->creation_date);
    ds3_str_free(response->delimiter);
    ds3_str_free(response->marker);
    ds3_str_free(response->name);
    ds3_str_free(response->next_marker);
    for (index = 0; index < response->num_objects; index++) {
        ds3_contents_response_free(response->objects[index]);
    }
    g_free(response->objects);
    ds3_str_free(response->prefix);

    g_free(response);
}
void ds3_list_multi_part_uploads_result_response_free(ds3_list_multi_part_uploads_result_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    ds3_str_free(response->bucket);
    for (index = 0; index < response->num_common_prefixes; index++) {
        ds3_str_free(response->common_prefixes[index]);
    }
    g_free(response->common_prefixes);
    ds3_str_free(response->delimiter);
    ds3_str_free(response->key_marker);
    ds3_str_free(response->next_key_marker);
    ds3_str_free(response->next_upload_id_marker);
    ds3_str_free(response->prefix);
    ds3_str_free(response->upload_id_marker);
    for (index = 0; index < response->num_uploads; index++) {
        ds3_multi_part_upload_response_free(response->uploads[index]);
    }
    g_free(response->uploads);

    g_free(response);
}
