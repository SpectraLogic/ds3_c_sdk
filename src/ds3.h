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

#ifndef __DS3_HEADER__
#define __DS3_HEADER__

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// For windows DLL symbol exports.
#ifdef _WIN32
#    ifdef LIBRARY_EXPORTS
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#else
#    define LIBRARY_API
#endif

#define DS3_READFUNC_ABORT CURL_READFUNC_ABORT

typedef enum {
    False, True
}ds3_bool;

typedef enum {
  HTTP_GET, HTTP_PUT, HTTP_POST, HTTP_DELETE, HTTP_HEAD
}http_verb;

typedef enum {
    CRITICAL,
    VERY_HIGH,
    HIGH,
    NORMAL,
    LOW,
    BACKGROUND,
    MINIMIZED_DUE_TO_TOO_MANY_RETRIES
}ds3_job_priority;

typedef enum {
    PUT, GET
}ds3_job_request_type;

typedef enum {
    CAPACITY, PERFORMANCE
}ds3_write_optimization;

typedef enum {
    IN_ORDER, NONE
}ds3_chunk_ordering;

typedef struct{
    char* value;
    size_t size;
}ds3_str;

typedef enum {
    IN_PROGRESS,
    COMPLETED,
    CANCELED
}ds3_job_status;

typedef enum {
    TAPE_STATE_NORMAL,
    TAPE_STATE_OFFLINE,
    TAPE_STATE_ONLINE_PENDING,
    TAPE_STATE_ONLINE_IN_PROGRESS,
    TAPE_STATE_PENDING_INSPECTION,
    TAPE_STATE_UNKNOWN,
    TAPE_STATE_DATA_CHECKPOINT_FAILURE,
    TAPE_STATE_DATA_CHECKPOINT_MISSING,
    TAPE_STATE_LTFS_WITH_FOREIGN_DATA,
    TAPE_STATE_FOREIGN,
    TAPE_STATE_IMPORT_PENDING,
    TAPE_STATE_IMPORT_IN_PROGRESS,
    TAPE_STATE_LOST,
    TAPE_STATE_BAD,
    TAPE_STATE_SERIAL_NUMBER_MISMATCH,
    TAPE_STATE_BAD_CODE_MISSING,
    TAPE_STATE_FORMAT_PENDING,
    TAPE_STATE_FORMAT_IN_PROGRESS,
    TAPE_STATE_EJECT_TO_EE_IN_PROGRESS,
    TAPE_STATE_EJECT_FROM_EE_PENDING,
    TAPE_STATE_EJECTED
}ds3_tape_state;

typedef enum {
    TAPE_TYPE_LTO5,
    TAPE_TYPE_LTO6,
    TAPE_TYPE_LTO7,
    TAPE_TYPE_LTO_CLEANING_TAPE,
    TAPE_TYPE_TS_JC,
    TAPE_TYPE_TS_JY,
    TAPE_TYPE_TS_JK,
    TAPE_TYPE_TS_JD,
    TAPE_TYPE_TS_JZ,
    TAPE_TYPE_TS_JL,
    TAPE_TYPE_TS_CLEANING_TAPE,
    TAPE_TYPE_UNKNOWN,
    TAPE_TYPE_FORBIDDEN
}ds3_tape_type;

typedef enum {
  DATA, FOLDER
}ds3_object_type;

LIBRARY_API ds3_str* ds3_str_init(const char* string);
LIBRARY_API ds3_str* ds3_str_init_with_size(const char* string, size_t size);
LIBRARY_API char* ds3_str_value(const ds3_str* string);
LIBRARY_API size_t ds3_str_size(const ds3_str* string);
LIBRARY_API ds3_str* ds3_str_dup(const ds3_str* string);
LIBRARY_API void ds3_str_free(ds3_str* string);

typedef enum {
  DS3_ERROR, DS3_WARN, DS3_INFO, DS3_DEBUG, DS3_TRACE
}ds3_log_lvl;

typedef struct {
    void     (* log_callback)(const char* log_message, void* user_data);
    void*       user_data;
    ds3_log_lvl log_lvl;
}ds3_log;

typedef struct {
    ds3_str* access_id;
    ds3_str* secret_key;
}ds3_creds;

typedef struct {
    ds3_str*    endpoint;
    ds3_str*    proxy;
    uint64_t    num_redirects;
    ds3_creds*  creds;
    ds3_log*    log;
}ds3_client;

typedef struct _ds3_request ds3_request;

typedef struct {
    ds3_str* creation_date;
    ds3_str* name;
}ds3_bucket;

typedef struct {
    ds3_str*    name;
    ds3_str*    id;
}ds3_owner;

typedef struct {
    ds3_str*    name;
    ds3_str*    etag;
    uint64_t    size;
    ds3_owner*  owner;
    ds3_str*    last_modified;
    ds3_str*    storage_class;
}ds3_object;

typedef struct {
    ds3_str*    bucket_id;
    ds3_str*    id;
    ds3_str*    name;
    uint64_t    size;
    ds3_owner*  owner;
    ds3_str*    last_modified;
    ds3_str*    storage_class;
    ds3_str*    type;
    ds3_str*    version;
}ds3_search_object;

typedef struct {
    ds3_bucket* buckets;
    size_t      num_buckets;
    ds3_owner*  owner;
}ds3_get_service_response;

typedef struct {
    ds3_object* objects;
    size_t      num_objects;
    ds3_str*    creation_date;
    ds3_bool    is_truncated;
    ds3_str*    marker;
    ds3_str*    delimiter;
    uint32_t    max_keys;
    ds3_str*    name;
    ds3_str*    next_marker;
    ds3_str*    prefix;
    ds3_str**   common_prefixes;
    uint64_t    num_common_prefixes;
}ds3_get_bucket_response;

typedef struct {
    ds3_search_object** objects;
    uint64_t      num_objects;
}ds3_get_objects_response;

typedef struct {
    ds3_str*    name;
    uint64_t    length;
    uint64_t    offset;
    ds3_bool    in_cache;
}ds3_bulk_object;

typedef struct {
    ds3_bulk_object*  list;
    uint64_t          size;
    uint64_t          chunk_number;
    ds3_str*          node_id;
    ds3_str*          server_id;
    ds3_str*          chunk_id;
}ds3_bulk_object_list;

typedef struct {
    ds3_str*    endpoint;
    ds3_str*    id;
    uint16_t    http_port;
    uint16_t    https_port;
}ds3_node;

typedef struct {
    ds3_node**   list;
    uint64_t     size;
}ds3_nodes_list;

typedef struct {
    ds3_str*                bucket_name;
    uint64_t                cached_size_in_bytes;
    ds3_chunk_ordering      chunk_order;
    uint64_t                completed_size_in_bytes;
    ds3_str*                job_id;
    uint64_t                original_size_in_bytes;
    ds3_job_priority        priority;
    ds3_job_request_type    request_type;
    ds3_str*                start_date;
    ds3_str*                user_id;
    ds3_str*                user_name;
    ds3_write_optimization  write_optimization;
    ds3_bulk_object_list**  list;
    size_t                  list_size;
    ds3_job_status          status;
    ds3_nodes_list*         nodes;
}ds3_bulk_response;

typedef struct {
    ds3_bulk_response** jobs;
    size_t   jobs_size;
}ds3_get_jobs_response;

typedef struct {
    ds3_bool assigned_to_bucket;
    uint64_t available_raw_capacity;
    ds3_str* barcode;
    ds3_str* bucket_id;
    ds3_str* description;
    ds3_str* eject_date;
    ds3_str* eject_label;
    ds3_str* eject_location;
    ds3_str* eject_pending; // date that eject was requested
    ds3_bool full_of_data;
    ds3_str* id;
    ds3_str* last_accessed;
    ds3_str* last_checkpoint;
    ds3_str* last_modified;
    ds3_str* last_verified;
    ds3_str* partition_id;
    ds3_tape_state previous_state;
    ds3_str* serial_number;
    ds3_tape_state state;
    uint64_t total_raw_capacity;
    ds3_tape_type type;
    ds3_bool write_protected;
}ds3_tape;

typedef struct {
    ds3_tape*		tapes;
    uint64_t		num_tapes;
}ds3_get_physical_placement_response;

typedef enum {
  DS3_ERROR_INVALID_XML, DS3_ERROR_CURL_HANDLE, DS3_ERROR_REQUEST_FAILED,
  DS3_ERROR_MISSING_ARGS, DS3_ERROR_BAD_STATUS_CODE, DS3_ERROR_TOO_MANY_REDIRECTS
}ds3_error_code;

typedef struct {
    ds3_bulk_object_list*   objects;
    uint64_t                retry_after;
}ds3_allocate_chunk_response;

typedef struct {
    ds3_bulk_response* 	  object_list;
    uint64_t              retry_after;
}ds3_get_available_chunks_response;

typedef struct {
    uint64_t  status_code;
    ds3_str*  status_message;
    ds3_str*  error_body;
}ds3_error_response;

typedef struct {
    ds3_error_code      code;
    ds3_str*            message;
    ds3_error_response* error;
}ds3_error;

typedef struct {
    ds3_str*    name;
    ds3_str**   values;
    uint64_t    num_values;
}ds3_metadata_entry;

typedef struct {
    ds3_str**  keys;
    uint64_t   num_keys;
}ds3_metadata_keys_result;

typedef struct _ds3_metadata ds3_metadata;

typedef struct {
    ds3_str*  branch;
    ds3_str*  revision;
    ds3_str*  version;
}ds3_build_information;

typedef struct {
    ds3_str* api_version;
    ds3_str* serial_number;
    ds3_build_information* build_information;
}ds3_get_system_information_response;

typedef struct {
    uint64_t ms_required_to_verify_data_planner_health;
}ds3_verify_system_health_response;

LIBRARY_API ds3_metadata_entry* ds3_metadata_get_entry(const ds3_metadata* metadata, const char* name);
LIBRARY_API unsigned int ds3_metadata_size(const ds3_metadata* metadata);
LIBRARY_API ds3_metadata_keys_result* ds3_metadata_keys(const ds3_metadata* metadata);

LIBRARY_API ds3_creds*  ds3_create_creds(const char* access_id, const char* secret_key);
LIBRARY_API ds3_client* ds3_create_client(const char* endpoint, ds3_creds* creds);
LIBRARY_API ds3_error*  ds3_create_client_from_env(ds3_client** client);
LIBRARY_API void        ds3_client_register_logging(ds3_client* client, ds3_log_lvl log_lvl, void (* log_callback)(const char* log_message, void* user_data), void* user_data);

LIBRARY_API ds3_request* ds3_init_get_system_information(void);
LIBRARY_API ds3_request* ds3_init_get_service(void);
LIBRARY_API ds3_request* ds3_init_get_bucket(const char* bucket_name);
LIBRARY_API ds3_request* ds3_init_head_object(const char* bucket_name, const char* object_name);
LIBRARY_API ds3_request* ds3_init_head_bucket(const char* bucket_name);
LIBRARY_API ds3_request* ds3_init_get_object_for_job(const char* bucket_name, const char* object_name, uint64_t offset, const char* job_id);
LIBRARY_API ds3_request* ds3_init_put_bucket(const char* bucket_name);
LIBRARY_API ds3_request* ds3_init_put_object_for_job(const char* bucket_name, const char* object_name, uint64_t offset, uint64_t length, const char* job_id);
LIBRARY_API ds3_request* ds3_init_delete_bucket(const char* bucket_name);
LIBRARY_API ds3_request* ds3_init_delete_object(const char* bucket_name, const char* object_name);
LIBRARY_API ds3_request* ds3_init_delete_objects(const char* bucket_name);
LIBRARY_API ds3_request* ds3_init_delete_folder(const char* bucket_name, const char* folder_name);
LIBRARY_API ds3_request* ds3_init_allocate_chunk(const char* chunk_id);
LIBRARY_API ds3_request* ds3_init_get_available_chunks(const char* job_id);
LIBRARY_API ds3_request* ds3_init_get_jobs(void);
LIBRARY_API ds3_request* ds3_init_get_job(const char* job_id);
LIBRARY_API ds3_request* ds3_init_put_job(const char* job_id);
LIBRARY_API ds3_request* ds3_init_delete_job(const char* job_id);
LIBRARY_API ds3_request* ds3_init_get_objects();
LIBRARY_API ds3_request* ds3_init_verify_system_health(void);

LIBRARY_API ds3_request* ds3_init_put_bulk(const char* bucket_name, ds3_bulk_object_list* object_list);
LIBRARY_API ds3_request* ds3_init_get_bulk(const char* bucket_name, ds3_bulk_object_list* object_list, ds3_chunk_ordering order);

LIBRARY_API ds3_request* ds3_init_get_physical_placement(const char* bucket_name, ds3_bulk_object_list* object_list);
LIBRARY_API ds3_request* ds3_init_get_physical_placement_full_details(const char* bucket_name, ds3_bulk_object_list* object_list);

LIBRARY_API void ds3_client_proxy(ds3_client* client, const char* proxy);

LIBRARY_API void ds3_request_set_custom_header(ds3_request* request, const char* header_name, const char* header_value);
LIBRARY_API void ds3_request_set_bucket_name(ds3_request* request, const char* bucket_name);
LIBRARY_API void ds3_request_set_prefix(ds3_request* request, const char* prefix);
LIBRARY_API void ds3_request_set_delimiter(ds3_request* request, const char* delimiter);
LIBRARY_API void ds3_request_set_marker(ds3_request* request, const char* marker);
LIBRARY_API void ds3_request_set_max_keys(ds3_request* request, uint32_t max_keys);
LIBRARY_API void ds3_request_set_md5(ds3_request* request, const char* md5);
LIBRARY_API void ds3_request_set_metadata(ds3_request* request, const char* name, const char* value);
LIBRARY_API void ds3_request_set_name(ds3_request* request, const char* name);
LIBRARY_API void ds3_request_set_id(ds3_request* request, const char* id);
LIBRARY_API void ds3_request_set_type(ds3_request* request, ds3_object_type type);
LIBRARY_API void ds3_request_set_version(ds3_request* request, const char* version);

LIBRARY_API ds3_error* ds3_get_system_information(const ds3_client* client, const ds3_request* request, ds3_get_system_information_response** response);
LIBRARY_API ds3_error* ds3_verify_system_health(const ds3_client* client, const ds3_request* request, ds3_verify_system_health_response** response);
LIBRARY_API ds3_error* ds3_get_service(const ds3_client* client, const ds3_request* request, ds3_get_service_response** response);
LIBRARY_API ds3_error* ds3_get_bucket(const ds3_client* client, const ds3_request* request, ds3_get_bucket_response** response);
LIBRARY_API ds3_error* ds3_bulk(const ds3_client* client, const ds3_request* request, ds3_bulk_response** response);
LIBRARY_API ds3_error* ds3_allocate_chunk(const ds3_client* client, const ds3_request* request, ds3_allocate_chunk_response** response);
LIBRARY_API ds3_error* ds3_get_available_chunks(const ds3_client* client, const ds3_request* request, ds3_get_available_chunks_response** response);

LIBRARY_API ds3_error* ds3_put_bucket(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_error* ds3_delete_bucket(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_error* ds3_head_object(const ds3_client* client, const ds3_request* request, ds3_metadata** metadata);
LIBRARY_API ds3_error* ds3_head_bucket(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_error* ds3_get_object(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*));
LIBRARY_API ds3_error* ds3_get_object_with_metadata(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*), ds3_metadata** metadata);
LIBRARY_API ds3_error* ds3_put_object(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*));
LIBRARY_API ds3_error* ds3_delete_object(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_error* ds3_delete_objects(const ds3_client* client, const ds3_request* _request, ds3_bulk_object_list *bulkObjList);
LIBRARY_API ds3_error* ds3_delete_folder(const ds3_client* client, const ds3_request* _request);
LIBRARY_API ds3_error* ds3_get_jobs(const ds3_client* client, const ds3_request* request, ds3_get_jobs_response** response);
LIBRARY_API ds3_error* ds3_get_job(const ds3_client* client, const ds3_request* request, ds3_bulk_response** response);
LIBRARY_API ds3_error* ds3_put_job(const ds3_client* client, const ds3_request* request, ds3_bulk_response** response);
LIBRARY_API ds3_error* ds3_delete_job(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_error* ds3_get_physical_placement(const ds3_client* client, const ds3_request* request, ds3_get_physical_placement_response** response);
LIBRARY_API ds3_error* ds3_get_objects(const ds3_client* client, const ds3_request* request, ds3_get_objects_response** response);

LIBRARY_API void ds3_free_service_response(ds3_get_service_response* response);
LIBRARY_API void ds3_free_bucket_response(ds3_get_bucket_response* response);
LIBRARY_API void ds3_free_bulk_response(ds3_bulk_response* response);
LIBRARY_API void ds3_free_error(ds3_error* error);
LIBRARY_API void ds3_free_allocate_chunk_response(ds3_allocate_chunk_response* response);
LIBRARY_API void ds3_free_available_chunks_response(ds3_get_available_chunks_response* response);
LIBRARY_API void ds3_free_get_physical_placement_response(ds3_get_physical_placement_response* response);
LIBRARY_API void ds3_free_owner(ds3_owner* owner);
LIBRARY_API void ds3_free_creds(ds3_creds* client);
LIBRARY_API void ds3_free_client(ds3_client* client);
LIBRARY_API void ds3_free_request(ds3_request* request);
LIBRARY_API void ds3_free_metadata(ds3_metadata* metadata);
LIBRARY_API void ds3_free_metadata_entry(ds3_metadata_entry* metadata_entry);
LIBRARY_API void ds3_free_metadata_keys(ds3_metadata_keys_result* metadata_keys);
LIBRARY_API void ds3_free_objects_response(ds3_get_objects_response* response);
LIBRARY_API void ds3_free_get_jobs_response(ds3_get_jobs_response* response);
LIBRARY_API void ds3_free_build_information(ds3_build_information* build_info);
LIBRARY_API void ds3_free_get_system_information(ds3_get_system_information_response* system_info);
LIBRARY_API void ds3_free_verify_system_health(ds3_verify_system_health_response* response);
LIBRARY_API void ds3_free_nodes_list(ds3_nodes_list* nodes_list);
LIBRARY_API void ds3_cleanup(void);

LIBRARY_API void ds3_print_request(const ds3_request* request);

// provided helpers
LIBRARY_API size_t ds3_write_to_file(void* buffer, size_t size, size_t nmemb, void* user_data);
LIBRARY_API size_t ds3_read_from_file(void* buffer, size_t size, size_t nmemb, void* user_data);
LIBRARY_API size_t ds3_write_to_fd(void* buffer, size_t size, size_t nmemb, void* user_data);
LIBRARY_API size_t ds3_read_from_fd(void* buffer, size_t size, size_t nmemb, void* user_data);

LIBRARY_API ds3_bulk_object_list* ds3_convert_file_list(const char** file_list, uint64_t num_files);
LIBRARY_API ds3_bulk_object_list* ds3_convert_file_list_with_basepath(const char** file_list, uint64_t num_files, const char* base_path);
LIBRARY_API ds3_bulk_object_list* ds3_convert_object_list(const ds3_object* objects, uint64_t num_objects);
LIBRARY_API ds3_bulk_object_list* ds3_init_bulk_object_list(uint64_t num_files);
LIBRARY_API void ds3_free_bulk_object_list(ds3_bulk_object_list* object_list);

#ifdef __cplusplus
}
#endif
#endif
