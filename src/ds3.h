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


/** @file ds3.h
 *  @brief The public definitions for the Spectra S3 C SDK
 */

#ifndef __DS3_HEADER__
#define __DS3_HEADER__

#include <stdint.h>
#include <string.h>
#include <curl/curl.h>
#include "ds3_string.h"
#include "ds3_string_multimap.h"

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

typedef struct {
    int page_truncated;
    int total_result_count;
}ds3_paging;

typedef struct _ds3_request ds3_request;

typedef struct _ds3_connection_pool ds3_connection_pool;

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

typedef enum {
    False, True
}ds3_bool;

typedef enum {
    HTTP_GET,
    HTTP_PUT,
    HTTP_POST,
    HTTP_DELETE,
    HTTP_HEAD
}http_verb;

typedef enum {
    DS3_ERROR,
    DS3_WARN,
    DS3_INFO,
    DS3_DEBUG,
    DS3_TRACE
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

typedef enum {
  DS3_ERROR_INVALID_XML,
  DS3_ERROR_CURL_HANDLE,
  DS3_ERROR_REQUEST_FAILED,
  DS3_ERROR_MISSING_ARGS,
  DS3_ERROR_BAD_STATUS_CODE,
  DS3_ERROR_TOO_MANY_REDIRECTS
}ds3_error_code;

typedef struct {
    ds3_str* etag;
    int      part_number;
}ds3_multipart_upload_part_response;

typedef struct {
    ds3_multipart_upload_part_response** parts;
    int                         num_parts;
}ds3_complete_multipart_upload_response;

typedef struct {
    ds3_str** strings_list;
    int       num_strings;
}ds3_delete_objects_response;

typedef enum {
    DS3_AUTO_INSPECT_MODE_NEVER,
    DS3_AUTO_INSPECT_MODE_MINIMAL,
    DS3_AUTO_INSPECT_MODE_DEFAULT
}ds3_auto_inspect_mode;
typedef enum {
    DS3_PRIORITY_CRITICAL,
    DS3_PRIORITY_URGENT,
    DS3_PRIORITY_HIGH,
    DS3_PRIORITY_NORMAL,
    DS3_PRIORITY_LOW,
    DS3_PRIORITY_BACKGROUND
}ds3_priority;
typedef enum {
    DS3_BUCKET_ACL_PERMISSION_LIST,
    DS3_BUCKET_ACL_PERMISSION_READ,
    DS3_BUCKET_ACL_PERMISSION_WRITE,
    DS3_BUCKET_ACL_PERMISSION_DELETE,
    DS3_BUCKET_ACL_PERMISSION_JOB,
    DS3_BUCKET_ACL_PERMISSION_OWNER
}ds3_bucket_acl_permission;
typedef enum {
    DS3_DATA_ISOLATION_LEVEL_STANDARD,
    DS3_DATA_ISOLATION_LEVEL_BUCKET_ISOLATED
}ds3_data_isolation_level;
typedef enum {
    DS3_DATA_PERSISTENCE_RULE_STATE_NORMAL,
    DS3_DATA_PERSISTENCE_RULE_STATE_INCLUSION_IN_PROGRESS
}ds3_data_persistence_rule_state;
typedef enum {
    DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT,
    DS3_DATA_PERSISTENCE_RULE_TYPE_TEMPORARY,
    DS3_DATA_PERSISTENCE_RULE_TYPE_RETIRED
}ds3_data_persistence_rule_type;
typedef enum {
    DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT,
    DS3_DATA_REPLICATION_RULE_TYPE_RETIRED
}ds3_data_replication_rule_type;
typedef enum {
    DS3_JOB_CHUNK_BLOB_STORE_STATE_PENDING,
    DS3_JOB_CHUNK_BLOB_STORE_STATE_IN_PROGRESS,
    DS3_JOB_CHUNK_BLOB_STORE_STATE_COMPLETED
}ds3_job_chunk_blob_store_state;
typedef enum {
    DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE,
    DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_IN_ORDER
}ds3_job_chunk_client_processing_order_guarantee;
typedef enum {
    DS3_JOB_REQUEST_TYPE_PUT,
    DS3_JOB_REQUEST_TYPE_GET,
    DS3_JOB_REQUEST_TYPE_VERIFY
}ds3_job_request_type;
typedef enum {
    DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME,
    DS3_LTFS_FILE_NAMING_MODE_OBJECT_ID
}ds3_ltfs_file_naming_mode;
typedef enum {
    DS3_S3_OBJECT_TYPE_DATA,
    DS3_S3_OBJECT_TYPE_FOLDER
}ds3_s3_object_type;
typedef enum {
    DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED,
    DS3_STORAGE_DOMAIN_FAILURE_TYPE_MEMBER_BECAME_READ_ONLY,
    DS3_STORAGE_DOMAIN_FAILURE_TYPE_WRITES_STALLED_DUE_TO_NO_FREE_MEDIA_REMAINING
}ds3_storage_domain_failure_type;
typedef enum {
    DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL,
    DS3_STORAGE_DOMAIN_MEMBER_STATE_EXCLUSION_IN_PROGRESS
}ds3_storage_domain_member_state;
typedef enum {
    DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED,
    DS3_SYSTEM_FAILURE_TYPE_RECONCILE_POOL_ENVIRONMENT_FAILED,
    DS3_SYSTEM_FAILURE_TYPE_CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION
}ds3_system_failure_type;
typedef enum {
    DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW,
    DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISCOURAGED,
    DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISALLOW
}ds3_unavailable_media_usage_policy;
typedef enum {
    DS3_VERSIONING_LEVEL_NONE,
    DS3_VERSIONING_LEVEL_KEEP_LATEST
}ds3_versioning_level;
typedef enum {
    DS3_WRITE_OPTIMIZATION_CAPACITY,
    DS3_WRITE_OPTIMIZATION_PERFORMANCE
}ds3_write_optimization;
typedef enum {
    DS3_WRITE_PREFERENCE_LEVEL_HIGH,
    DS3_WRITE_PREFERENCE_LEVEL_NORMAL,
    DS3_WRITE_PREFERENCE_LEVEL_LOW,
    DS3_WRITE_PREFERENCE_LEVEL_NEVER_SELECT
}ds3_write_preference_level;
typedef enum {
    DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED,
    DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE,
    DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_MISSING,
    DS3_POOL_FAILURE_TYPE_FORMAT_FAILED,
    DS3_POOL_FAILURE_TYPE_IMPORT_FAILED,
    DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE,
    DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY,
    DS3_POOL_FAILURE_TYPE_INSPECT_FAILED,
    DS3_POOL_FAILURE_TYPE_QUIESCED,
    DS3_POOL_FAILURE_TYPE_READ_FAILED,
    DS3_POOL_FAILURE_TYPE_VERIFY_FAILED,
    DS3_POOL_FAILURE_TYPE_WRITE_FAILED
}ds3_pool_failure_type;
typedef enum {
    DS3_POOL_HEALTH_OK,
    DS3_POOL_HEALTH_DEGRADED
}ds3_pool_health;
typedef enum {
    DS3_POOL_STATE_BLANK,
    DS3_POOL_STATE_NORMAL,
    DS3_POOL_STATE_LOST,
    DS3_POOL_STATE_FOREIGN,
    DS3_POOL_STATE_IMPORT_PENDING,
    DS3_POOL_STATE_IMPORT_IN_PROGRESS
}ds3_pool_state;
typedef enum {
    DS3_POOL_TYPE_NEARLINE,
    DS3_POOL_TYPE_ONLINE
}ds3_pool_type;
typedef enum {
    DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL,
    DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_MOST_RECENT,
    DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_EXISTING
}ds3_import_conflict_resolution_mode;
typedef enum {
    DS3_QUIESCED_NO,
    DS3_QUIESCED_PENDING,
    DS3_QUIESCED_YES
}ds3_quiesced;
typedef enum {
    DS3_SEVERITY_CRITICAL,
    DS3_SEVERITY_WARNING,
    DS3_SEVERITY_ALERT
}ds3_severity;
typedef enum {
    DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED,
    DS3_IMPORT_EXPORT_CONFIGURATION_NOT_SUPPORTED
}ds3_import_export_configuration;
typedef enum {
    DS3_TAPE_DRIVE_STATE_OFFLINE,
    DS3_TAPE_DRIVE_STATE_NORMAL,
    DS3_TAPE_DRIVE_STATE_ERROR,
    DS3_TAPE_DRIVE_STATE_NOT_COMPATIBLE_IN_PARTITION_DUE_TO_NEWER_TAPE_DRIVES
}ds3_tape_drive_state;
typedef enum {
    DS3_TAPE_DRIVE_TYPE_UNKNOWN,
    DS3_TAPE_DRIVE_TYPE_LTO5,
    DS3_TAPE_DRIVE_TYPE_LTO6,
    DS3_TAPE_DRIVE_TYPE_LTO7,
    DS3_TAPE_DRIVE_TYPE_TS1140,
    DS3_TAPE_DRIVE_TYPE_TS1150
}ds3_tape_drive_type;
typedef enum {
    DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED,
    DS3_TAPE_FAILURE_TYPE_BAR_CODE_DUPLICATE,
    DS3_TAPE_FAILURE_TYPE_BLOB_READ_FAILED,
    DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE,
    DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY,
    DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_MISSING,
    DS3_TAPE_FAILURE_TYPE_DELAYED_OWNERSHIP_FAILURE,
    DS3_TAPE_FAILURE_TYPE_DRIVE_CLEAN_FAILED,
    DS3_TAPE_FAILURE_TYPE_DRIVE_CLEANED,
    DS3_TAPE_FAILURE_TYPE_FORMAT_FAILED,
    DS3_TAPE_FAILURE_TYPE_GET_TAPE_INFORMATION_FAILED,
    DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED,
    DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE,
    DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY,
    DS3_TAPE_FAILURE_TYPE_INSPECT_FAILED,
    DS3_TAPE_FAILURE_TYPE_READ_FAILED,
    DS3_TAPE_FAILURE_TYPE_REIMPORT_REQUIRED,
    DS3_TAPE_FAILURE_TYPE_SERIAL_NUMBER_MISMATCH,
    DS3_TAPE_FAILURE_TYPE_VERIFY_FAILED,
    DS3_TAPE_FAILURE_TYPE_WRITE_FAILED
}ds3_tape_failure_type;
typedef enum {
    DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED,
    DS3_TAPE_PARTITION_FAILURE_TYPE_DUPLICATE_TAPE_BAR_CODES_DETECTED,
    DS3_TAPE_PARTITION_FAILURE_TYPE_EJECT_STALLED_DUE_TO_OFFLINE_TAPES,
    DS3_TAPE_PARTITION_FAILURE_TYPE_MINIMUM_DRIVE_COUNT_NOT_MET,
    DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED,
    DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED_DUE_TO_PREPARE_TAPE_FOR_REMOVAL_FAILURE,
    DS3_TAPE_PARTITION_FAILURE_TYPE_NO_USABLE_DRIVES,
    DS3_TAPE_PARTITION_FAILURE_TYPE_ONLINE_STALLED_DUE_TO_NO_STORAGE_SLOTS,
    DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_IN_ERROR,
    DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_MISSING,
    DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_TYPE_MISMATCH,
    DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_EJECTION_BY_OPERATOR_REQUIRED,
    DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_MEDIA_TYPE_INCOMPATIBLE
}ds3_tape_partition_failure_type;
typedef enum {
    DS3_TAPE_PARTITION_STATE_ONLINE,
    DS3_TAPE_PARTITION_STATE_OFFLINE,
    DS3_TAPE_PARTITION_STATE_ERROR
}ds3_tape_partition_state;
typedef enum {
    DS3_TAPE_STATE_NORMAL,
    DS3_TAPE_STATE_OFFLINE,
    DS3_TAPE_STATE_ONLINE_PENDING,
    DS3_TAPE_STATE_ONLINE_IN_PROGRESS,
    DS3_TAPE_STATE_PENDING_INSPECTION,
    DS3_TAPE_STATE_UNKNOWN,
    DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE,
    DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY,
    DS3_TAPE_STATE_DATA_CHECKPOINT_MISSING,
    DS3_TAPE_STATE_LTFS_WITH_FOREIGN_DATA,
    DS3_TAPE_STATE_FOREIGN,
    DS3_TAPE_STATE_IMPORT_PENDING,
    DS3_TAPE_STATE_IMPORT_IN_PROGRESS,
    DS3_TAPE_STATE_INCOMPATIBLE,
    DS3_TAPE_STATE_LOST,
    DS3_TAPE_STATE_BAD,
    DS3_TAPE_STATE_CANNOT_FORMAT_DUE_TO_WRITE_PROTECTION,
    DS3_TAPE_STATE_SERIAL_NUMBER_MISMATCH,
    DS3_TAPE_STATE_BAR_CODE_MISSING,
    DS3_TAPE_STATE_FORMAT_PENDING,
    DS3_TAPE_STATE_FORMAT_IN_PROGRESS,
    DS3_TAPE_STATE_EJECT_TO_EE_IN_PROGRESS,
    DS3_TAPE_STATE_EJECT_FROM_EE_PENDING,
    DS3_TAPE_STATE_EJECTED
}ds3_tape_state;
typedef enum {
    DS3_TAPE_TYPE_LTO5,
    DS3_TAPE_TYPE_LTO6,
    DS3_TAPE_TYPE_LTO7,
    DS3_TAPE_TYPE_LTO_CLEANING_TAPE,
    DS3_TAPE_TYPE_TS_JC,
    DS3_TAPE_TYPE_TS_JY,
    DS3_TAPE_TYPE_TS_JK,
    DS3_TAPE_TYPE_TS_JD,
    DS3_TAPE_TYPE_TS_JZ,
    DS3_TAPE_TYPE_TS_JL,
    DS3_TAPE_TYPE_TS_CLEANING_TAPE,
    DS3_TAPE_TYPE_UNKNOWN,
    DS3_TAPE_TYPE_FORBIDDEN
}ds3_tape_type;
typedef enum {
    DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE,
    DS3_TARGET_ACCESS_CONTROL_REPLICATION_USERS
}ds3_target_access_control_replication;
typedef enum {
    DS3_TARGET_FAILURE_TYPE_NOT_ONLINE,
    DS3_TARGET_FAILURE_TYPE_WRITE_FAILED,
    DS3_TARGET_FAILURE_TYPE_WRITE_INITIATE_FAILED,
    DS3_TARGET_FAILURE_TYPE_READ_FAILED,
    DS3_TARGET_FAILURE_TYPE_READ_INITIATE_FAILED,
    DS3_TARGET_FAILURE_TYPE_VERIFY_FAILED
}ds3_target_failure_type;
typedef enum {
    DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY,
    DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_ONLINE_POOL,
    DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NEARLINE_POOL,
    DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NON_EJECTABLE_TAPE,
    DS3_TARGET_READ_PREFERENCE_TYPE_LAST_RESORT,
    DS3_TARGET_READ_PREFERENCE_TYPE_NEVER
}ds3_target_read_preference_type;
typedef enum {
    DS3_TARGET_STATE_ONLINE,
    DS3_TARGET_STATE_LIMITED_ACCESS,
    DS3_TARGET_STATE_OFFLINE
}ds3_target_state;
typedef enum {
    DS3_BLOB_STORE_TASK_STATE_NOT_READY,
    DS3_BLOB_STORE_TASK_STATE_READY,
    DS3_BLOB_STORE_TASK_STATE_PENDING_EXECUTION,
    DS3_BLOB_STORE_TASK_STATE_IN_PROGRESS,
    DS3_BLOB_STORE_TASK_STATE_COMPLETED
}ds3_blob_store_task_state;
typedef enum {
    DS3_CACHE_ENTRY_STATE_ALLOCATED,
    DS3_CACHE_ENTRY_STATE_IN_CACHE
}ds3_cache_entry_state;
typedef enum {
    DS3_JOB_STATUS_IN_PROGRESS,
    DS3_JOB_STATUS_COMPLETED,
    DS3_JOB_STATUS_CANCELED
}ds3_job_status;
typedef enum {
    DS3_APPLICATION_S3_SERVER,
    DS3_APPLICATION_DATA_PLANNER
}ds3_application;
typedef enum {
    DS3_REST_ACTION_TYPE_BULK_DELETE,
    DS3_REST_ACTION_TYPE_BULK_MODIFY,
    DS3_REST_ACTION_TYPE_CREATE,
    DS3_REST_ACTION_TYPE_DELETE,
    DS3_REST_ACTION_TYPE_LIST,
    DS3_REST_ACTION_TYPE_MODIFY,
    DS3_REST_ACTION_TYPE_SHOW
}ds3_rest_action_type;
typedef enum {
    DS3_REST_DOMAIN_TYPE_ACTIVE_JOB,
    DS3_REST_DOMAIN_TYPE_BEANS_RETRIEVER,
    DS3_REST_DOMAIN_TYPE_BLOB_PERSISTENCE,
    DS3_REST_DOMAIN_TYPE_BLOB_STORE_TASK,
    DS3_REST_DOMAIN_TYPE_BUCKET,
    DS3_REST_DOMAIN_TYPE_BUCKET_ACL,
    DS3_REST_DOMAIN_TYPE_BUCKET_CAPACITY_SUMMARY,
    DS3_REST_DOMAIN_TYPE_CACHE_FILESYSTEM,
    DS3_REST_DOMAIN_TYPE_CACHE_STATE,
    DS3_REST_DOMAIN_TYPE_CANCELED_JOB,
    DS3_REST_DOMAIN_TYPE_CAPACITY_SUMMARY,
    DS3_REST_DOMAIN_TYPE_COMPLETED_JOB,
    DS3_REST_DOMAIN_TYPE_DATA_PATH,
    DS3_REST_DOMAIN_TYPE_DATA_PATH_BACKEND,
    DS3_REST_DOMAIN_TYPE_DATA_PERSISTENCE_RULE,
    DS3_REST_DOMAIN_TYPE_DATA_POLICY,
    DS3_REST_DOMAIN_TYPE_DATA_POLICY_ACL,
    DS3_REST_DOMAIN_TYPE_DATA_REPLICATION_RULE,
    DS3_REST_DOMAIN_TYPE_DEGRADED_BLOB,
    DS3_REST_DOMAIN_TYPE_DEGRADED_BUCKET,
    DS3_REST_DOMAIN_TYPE_DEGRADED_DATA_PERSISTENCE_RULE,
    DS3_REST_DOMAIN_TYPE_DEGRADED_DATA_REPLICATION_RULE,
    DS3_REST_DOMAIN_TYPE_DS3_TARGET,
    DS3_REST_DOMAIN_TYPE_DS3_TARGET_DATA_POLICIES,
    DS3_REST_DOMAIN_TYPE_DS3_TARGET_FAILURE,
    DS3_REST_DOMAIN_TYPE_DS3_TARGET_FAILURE_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_DS3_TARGET_READ_PREFERENCE,
    DS3_REST_DOMAIN_TYPE_DS3_TARGET_USER_MAPPING,
    DS3_REST_DOMAIN_TYPE_FOLDER,
    DS3_REST_DOMAIN_TYPE_GENERIC_DAO_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_GROUP,
    DS3_REST_DOMAIN_TYPE_GROUP_MEMBER,
    DS3_REST_DOMAIN_TYPE_HEAP_DUMP,
    DS3_REST_DOMAIN_TYPE_INSTANCE_IDENTIFIER,
    DS3_REST_DOMAIN_TYPE_JOB,
    DS3_REST_DOMAIN_TYPE_JOB_CHUNK,
    DS3_REST_DOMAIN_TYPE_JOB_CHUNK_DAO,
    DS3_REST_DOMAIN_TYPE_JOB_COMPLETED_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_JOB_CREATED_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_JOB_CREATION_FAILED_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_NODE,
    DS3_REST_DOMAIN_TYPE_OBJECT,
    DS3_REST_DOMAIN_TYPE_OBJECT_CACHED_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_OBJECT_LOST_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_OBJECT_PERSISTED_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_POOL,
    DS3_REST_DOMAIN_TYPE_POOL_ENVIRONMENT,
    DS3_REST_DOMAIN_TYPE_POOL_FAILURE,
    DS3_REST_DOMAIN_TYPE_POOL_FAILURE_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_POOL_PARTITION,
    DS3_REST_DOMAIN_TYPE_REQUEST_HANDLER,
    DS3_REST_DOMAIN_TYPE_REQUEST_HANDLER_CONTRACT,
    DS3_REST_DOMAIN_TYPE_STORAGE_DOMAIN,
    DS3_REST_DOMAIN_TYPE_STORAGE_DOMAIN_FAILURE,
    DS3_REST_DOMAIN_TYPE_STORAGE_DOMAIN_FAILURE_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_STORAGE_DOMAIN_MEMBER,
    DS3_REST_DOMAIN_TYPE_SUSPECT_BLOB_POOL,
    DS3_REST_DOMAIN_TYPE_SUSPECT_BLOB_TAPE,
    DS3_REST_DOMAIN_TYPE_SUSPECT_BLOB_TARGET,
    DS3_REST_DOMAIN_TYPE_SUSPECT_BUCKET,
    DS3_REST_DOMAIN_TYPE_SUSPECT_OBJECT,
    DS3_REST_DOMAIN_TYPE_SYSTEM_FAILURE,
    DS3_REST_DOMAIN_TYPE_SYSTEM_FAILURE_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_SYSTEM_HEALTH,
    DS3_REST_DOMAIN_TYPE_SYSTEM_INFORMATION,
    DS3_REST_DOMAIN_TYPE_TAPE,
    DS3_REST_DOMAIN_TYPE_TAPE_BUCKET,
    DS3_REST_DOMAIN_TYPE_TAPE_DENSITY_DIRECTIVE,
    DS3_REST_DOMAIN_TYPE_TAPE_DRIVE,
    DS3_REST_DOMAIN_TYPE_TAPE_ENVIRONMENT,
    DS3_REST_DOMAIN_TYPE_TAPE_FAILURE,
    DS3_REST_DOMAIN_TYPE_TAPE_FAILURE_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_TAPE_LIBRARY,
    DS3_REST_DOMAIN_TYPE_TAPE_PARTITION,
    DS3_REST_DOMAIN_TYPE_TAPE_PARTITION_FAILURE,
    DS3_REST_DOMAIN_TYPE_TAPE_PARTITION_FAILURE_NOTIFICATION_REGISTRATION,
    DS3_REST_DOMAIN_TYPE_TARGET_ENVIRONMENT,
    DS3_REST_DOMAIN_TYPE_USER,
    DS3_REST_DOMAIN_TYPE_USER_INTERNAL
}ds3_rest_domain_type;
typedef enum {
    DS3_REST_OPERATION_TYPE_ALLOCATE,
    DS3_REST_OPERATION_TYPE_CANCEL_EJECT,
    DS3_REST_OPERATION_TYPE_CANCEL_FORMAT,
    DS3_REST_OPERATION_TYPE_CANCEL_IMPORT,
    DS3_REST_OPERATION_TYPE_CANCEL_ONLINE,
    DS3_REST_OPERATION_TYPE_CANCEL_VERIFY,
    DS3_REST_OPERATION_TYPE_CLEAN,
    DS3_REST_OPERATION_TYPE_COMPACT,
    DS3_REST_OPERATION_TYPE_DEALLOCATE,
    DS3_REST_OPERATION_TYPE_EJECT,
    DS3_REST_OPERATION_TYPE_FORMAT,
    DS3_REST_OPERATION_TYPE_GET_PHYSICAL_PLACEMENT,
    DS3_REST_OPERATION_TYPE_IMPORT,
    DS3_REST_OPERATION_TYPE_INSPECT,
    DS3_REST_OPERATION_TYPE_ONLINE,
    DS3_REST_OPERATION_TYPE_PAIR_BACK,
    DS3_REST_OPERATION_TYPE_REGENERATE_SECRET_KEY,
    DS3_REST_OPERATION_TYPE_START_BULK_GET,
    DS3_REST_OPERATION_TYPE_START_BULK_PUT,
    DS3_REST_OPERATION_TYPE_START_BULK_VERIFY,
    DS3_REST_OPERATION_TYPE_VERIFY,
    DS3_REST_OPERATION_TYPE_VERIFY_SAFE_TO_START_BULK_PUT,
    DS3_REST_OPERATION_TYPE_VERIFY_PHYSICAL_PLACEMENT
}ds3_rest_operation_type;
typedef enum {
    DS3_REST_RESOURCE_TYPE_SINGLETON,
    DS3_REST_RESOURCE_TYPE_NON_SINGLETON
}ds3_rest_resource_type;
typedef enum {
    DS3_SQL_OPERATION_SELECT,
    DS3_SQL_OPERATION_INSERT,
    DS3_SQL_OPERATION_UPDATE,
    DS3_SQL_OPERATION_DELETE
}ds3_sql_operation;
typedef enum {
    DS3_DATABASE_PHYSICAL_SPACE_STATE_CRITICAL,
    DS3_DATABASE_PHYSICAL_SPACE_STATE_LOW,
    DS3_DATABASE_PHYSICAL_SPACE_STATE_NEAR_LOW,
    DS3_DATABASE_PHYSICAL_SPACE_STATE_NORMAL
}ds3_database_physical_space_state;
typedef enum {
    DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT,
    DS3_HTTP_RESPONSE_FORMAT_TYPE_JSON,
    DS3_HTTP_RESPONSE_FORMAT_TYPE_XML
}ds3_http_response_format_type;
typedef enum {
    DS3_REQUEST_TYPE_DELETE,
    DS3_REQUEST_TYPE_GET,
    DS3_REQUEST_TYPE_HEAD,
    DS3_REQUEST_TYPE_POST,
    DS3_REQUEST_TYPE_PUT
}ds3_request_type;
typedef enum {
    DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE,
    DS3_NAMING_CONVENTION_TYPE_CONSTANT,
    DS3_NAMING_CONVENTION_TYPE_UNDERSCORED,
    DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_UPPERCASE,
    DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_LOWERCASE
}ds3_naming_convention_type;
typedef enum {
    DS3_CHECKSUM_TYPE_CRC_32,
    DS3_CHECKSUM_TYPE_CRC_32C,
    DS3_CHECKSUM_TYPE_MD5,
    DS3_CHECKSUM_TYPE_SHA_256,
    DS3_CHECKSUM_TYPE_SHA_512
}ds3_checksum_type;
typedef struct {
    uint64_t byte_offset;
    ds3_str* checksum;
    ds3_checksum_type checksum_type;
    ds3_str* id;
    uint64_t length;
    ds3_str* object_id;
}ds3_blob_response;
typedef struct {
    ds3_str* creation_date;
    ds3_str* data_policy_id;
    ds3_bool empty;
    ds3_str* id;
    uint64_t last_preferred_chunk_size_in_bytes;
    uint64_t logical_used_capacity;
    ds3_str* name;
    ds3_str* user_id;
}ds3_bucket_response;
typedef struct {
    ds3_str* bucket_id;
    ds3_str* group_id;
    ds3_str* id;
    ds3_bucket_acl_permission permission;
    ds3_str* user_id;
}ds3_bucket_acl_response;
typedef struct {
    ds3_str* bucket_id;
    uint64_t cached_size_in_bytes;
    ds3_bool canceled_due_to_timeout;
    ds3_job_chunk_client_processing_order_guarantee chunk_client_processing_order_guarantee;
    uint64_t completed_size_in_bytes;
    ds3_str* created_at;
    ds3_str* date_canceled;
    ds3_str* error_message;
    ds3_str* id;
    ds3_bool naked;
    ds3_str* name;
    uint64_t original_size_in_bytes;
    ds3_priority priority;
    ds3_str* rechunked;
    ds3_job_request_type request_type;
    ds3_bool truncated;
    ds3_str* user_id;
}ds3_canceled_job_response;
typedef struct {
    ds3_str* bucket_id;
    uint64_t cached_size_in_bytes;
    ds3_job_chunk_client_processing_order_guarantee chunk_client_processing_order_guarantee;
    uint64_t completed_size_in_bytes;
    ds3_str* created_at;
    ds3_str* date_completed;
    ds3_str* error_message;
    ds3_str* id;
    ds3_bool naked;
    ds3_str* name;
    uint64_t original_size_in_bytes;
    ds3_priority priority;
    ds3_str* rechunked;
    ds3_job_request_type request_type;
    ds3_bool truncated;
    ds3_str* user_id;
}ds3_completed_job_response;
typedef struct {
    ds3_bool activated;
    int auto_activate_timeout_in_mins;
    ds3_auto_inspect_mode auto_inspect;
    ds3_import_conflict_resolution_mode default_import_conflict_resolution_mode;
    ds3_priority default_verify_data_after_import;
    ds3_bool default_verify_data_prior_to_import;
    ds3_str* id;
    ds3_str* instance_id;
    ds3_str* last_heartbeat;
    int partially_verify_last_percent_of_tapes;
    ds3_unavailable_media_usage_policy unavailable_media_policy;
    int unavailable_pool_max_job_retry_in_mins;
    int unavailable_tape_partition_max_job_retry_in_mins;
}ds3_data_path_backend_response;
typedef struct {
    ds3_str* data_policy_id;
    ds3_str* id;
    ds3_data_isolation_level isolation_level;
    int minimum_days_to_retain;
    ds3_data_persistence_rule_state state;
    ds3_str* storage_domain_id;
    ds3_data_persistence_rule_type type;
}ds3_data_persistence_rule_response;
typedef struct {
    ds3_bool always_force_put_job_creation;
    ds3_bool always_minimize_spanning_across_media;
    ds3_bool always_replicate_deletes;
    ds3_bool blobbing_enabled;
    ds3_checksum_type checksum_type;
    ds3_str* creation_date;
    uint64_t default_blob_size;
    ds3_priority default_get_job_priority;
    ds3_priority default_put_job_priority;
    ds3_priority default_verify_job_priority;
    ds3_bool end_to_end_crc_required;
    ds3_str* id;
    ds3_bool ltfs_object_naming_allowed;
    ds3_str* name;
    ds3_priority rebuild_priority;
    ds3_versioning_level versioning;
}ds3_data_policy_response;
typedef struct {
    ds3_str* data_policy_id;
    ds3_str* group_id;
    ds3_str* id;
    ds3_str* user_id;
}ds3_data_policy_acl_response;
typedef struct {
    ds3_str* data_policy_id;
    ds3_str* ds3_target_data_policy;
    ds3_str* ds3_target_id;
    ds3_str* id;
    ds3_data_persistence_rule_state state;
    ds3_data_replication_rule_type type;
}ds3_data_replication_rule_response;
typedef struct {
    ds3_str* blob_id;
    ds3_str* bucket_id;
    ds3_str* id;
    ds3_str* persistence_rule_id;
    ds3_str* replication_rule_id;
}ds3_degraded_blob_response;
typedef struct {
    ds3_bool built_in;
    ds3_str* id;
    ds3_str* name;
}ds3_group_response;
typedef struct {
    ds3_str* group_id;
    ds3_str* id;
    ds3_str* member_group_id;
    ds3_str* member_user_id;
}ds3_group_member_response;
typedef struct {
    ds3_bool aggregating;
    ds3_str* bucket_id;
    uint64_t cached_size_in_bytes;
    ds3_job_chunk_client_processing_order_guarantee chunk_client_processing_order_guarantee;
    uint64_t completed_size_in_bytes;
    ds3_str* created_at;
    ds3_str* error_message;
    ds3_str* id;
    ds3_bool minimize_spanning_across_media;
    ds3_bool naked;
    ds3_str* name;
    uint64_t original_size_in_bytes;
    ds3_priority priority;
    ds3_str* rechunked;
    ds3_bool replicating;
    ds3_job_request_type request_type;
    ds3_bool truncated;
    ds3_bool truncated_due_to_timeout;
    ds3_str* user_id;
}ds3_active_job_response;
typedef struct {
    ds3_job_chunk_blob_store_state blob_store_state;
    int chunk_number;
    ds3_str* id;
    ds3_str* job_creation_date;
    ds3_str* job_id;
    ds3_str* node_id;
    ds3_bool pending_target_commit;
    ds3_str* read_from_ds3_target_id;
    ds3_str* read_from_pool_id;
    ds3_str* read_from_tape_id;
}ds3_job_chunk_response;
typedef struct {
    int data_path_http_port;
    int data_path_https_port;
    ds3_str* data_path_ip_address;
    ds3_str* dns_name;
    ds3_str* id;
    ds3_str* last_heartbeat;
    ds3_str* name;
    ds3_str* serial_number;
}ds3_node_response;
typedef struct {
    ds3_str* bucket_id;
    ds3_str* creation_date;
    ds3_str* id;
    ds3_bool latest;
    ds3_str* name;
    ds3_s3_object_type type;
    uint64_t version;
}ds3_s3_object_response;
typedef struct {
    uint64_t auto_eject_media_full_threshold;
    ds3_str* auto_eject_upon_cron;
    ds3_bool auto_eject_upon_job_cancellation;
    ds3_bool auto_eject_upon_job_completion;
    ds3_bool auto_eject_upon_media_full;
    ds3_str* id;
    ds3_ltfs_file_naming_mode ltfs_file_naming;
    int max_tape_fragmentation_percent;
    int maximum_auto_verification_frequency_in_days;
    ds3_bool media_ejection_allowed;
    ds3_str* name;
    ds3_bool secure_media_allocation;
    ds3_priority verify_prior_to_auto_eject;
    ds3_write_optimization write_optimization;
}ds3_storage_domain_response;
typedef struct {
    uint64_t physical_allocated;
    uint64_t physical_free;
    uint64_t physical_used;
}ds3_storage_domain_capacity_summary_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* id;
    ds3_str* storage_domain_id;
    ds3_storage_domain_failure_type type;
}ds3_storage_domain_failure_response;
typedef struct {
    ds3_str* id;
    ds3_str* pool_partition_id;
    ds3_storage_domain_member_state state;
    ds3_str* storage_domain_id;
    ds3_str* tape_partition_id;
    ds3_tape_type tape_type;
    ds3_write_preference_level write_preference;
}ds3_storage_domain_member_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* id;
    ds3_system_failure_type type;
}ds3_system_failure_response;
typedef struct {
    ds3_str* auth_id;
    ds3_str* default_data_policy_id;
    ds3_str* id;
    ds3_str* name;
    ds3_str* secret_key;
}ds3_spectra_user_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_target_failure_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_str* dao_type;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_generic_dao_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* job_id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_job_completed_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_job_created_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_job_creation_failed_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_pool_failure_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* job_id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_s3_object_cached_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_s3_object_lost_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* job_id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_s3_object_persisted_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_storage_domain_failure_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_system_failure_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_tape_failure_notification_registration_response;
typedef struct {
    ds3_str* creation_date;
    ds3_http_response_format_type format;
    ds3_str* id;
    ds3_str* last_failure;
    int last_http_response_code;
    ds3_str* last_notification;
    ds3_naming_convention_type naming_convention;
    ds3_str* notification_end_point;
    ds3_request_type notification_http_method;
    int number_of_failures_since_last_success;
    ds3_str* user_id;
}ds3_tape_partition_failure_notification_registration_response;
typedef struct {
    float auto_reclaim_initiate_threshold;
    float auto_reclaim_terminate_threshold;
    float burst_threshold;
    ds3_str* id;
    uint64_t max_capacity_in_bytes;
    float max_percent_utilization_of_filesystem;
    ds3_str* node_id;
    ds3_str* path;
}ds3_cache_filesystem_response;
typedef struct {
    ds3_bool assigned_to_storage_domain;
    uint64_t available_capacity;
    ds3_str* bucket_id;
    ds3_str* guid;
    ds3_pool_health health;
    ds3_str* id;
    ds3_str* last_accessed;
    ds3_str* last_modified;
    ds3_str* last_verified;
    ds3_str* mountpoint;
    ds3_str* name;
    ds3_str* partition_id;
    ds3_bool powered_on;
    ds3_quiesced quiesced;
    uint64_t reserved_capacity;
    ds3_pool_state state;
    ds3_str* storage_domain_id;
    uint64_t total_capacity;
    ds3_pool_type type;
    uint64_t used_capacity;
}ds3_pool_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* id;
    ds3_str* pool_id;
    ds3_pool_failure_type type;
}ds3_pool_failure_response;
typedef struct {
    ds3_str* id;
    ds3_str* name;
    ds3_pool_type type;
}ds3_pool_partition_response;
typedef struct {
    ds3_str* blob_id;
    ds3_str* bucket_id;
    ds3_str* date_written;
    ds3_str* id;
    ds3_str* last_accessed;
    ds3_str* pool_id;
}ds3_suspect_blob_pool_response;
typedef struct {
    ds3_str* blob_id;
    ds3_str* id;
    int order_index;
    ds3_str* tape_id;
}ds3_suspect_blob_tape_response;
typedef struct {
    ds3_bool assigned_to_storage_domain;
    uint64_t available_raw_capacity;
    ds3_str* bar_code;
    ds3_str* bucket_id;
    ds3_str* description_for_identification;
    ds3_str* eject_date;
    ds3_str* eject_label;
    ds3_str* eject_location;
    ds3_str* eject_pending;
    ds3_bool full_of_data;
    ds3_str* id;
    ds3_str* last_accessed;
    ds3_str* last_checkpoint;
    ds3_str* last_modified;
    ds3_str* last_verified;
    ds3_str* partially_verified_end_of_tape;
    ds3_str* partition_id;
    ds3_tape_state previous_state;
    ds3_str* serial_number;
    ds3_tape_state state;
    ds3_str* storage_domain_id;
    ds3_bool take_ownership_pending;
    uint64_t total_raw_capacity;
    ds3_tape_type type;
    ds3_priority verify_pending;
    ds3_bool write_protected;
}ds3_tape_response;
typedef struct {
    ds3_tape_drive_type density;
    ds3_str* id;
    ds3_str* partition_id;
    ds3_tape_type tape_type;
}ds3_tape_density_directive_response;
typedef struct {
    ds3_str* error_message;
    ds3_bool force_tape_removal;
    ds3_str* id;
    ds3_str* last_cleaned;
    ds3_str* partition_id;
    ds3_str* serial_number;
    ds3_tape_drive_state state;
    ds3_str* tape_id;
    ds3_tape_drive_type type;
}ds3_tape_drive_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* id;
    ds3_str* tape_drive_id;
    ds3_str* tape_id;
    ds3_tape_failure_type type;
}ds3_detailed_tape_failure_response;
typedef struct {
    ds3_str* id;
    ds3_str* management_url;
    ds3_str* name;
    ds3_str* serial_number;
}ds3_tape_library_response;
typedef struct {
    ds3_tape_drive_type drive_type;
    ds3_str* error_message;
    ds3_str* id;
    ds3_import_export_configuration import_export_configuration;
    ds3_str* library_id;
    ds3_str* name;
    ds3_quiesced quiesced;
    ds3_str* serial_number;
    ds3_tape_partition_state state;
}ds3_tape_partition_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* id;
    ds3_str* partition_id;
    ds3_tape_partition_failure_type type;
}ds3_tape_partition_failure_response;
typedef struct {
    ds3_target_access_control_replication access_control_replication;
    ds3_str* admin_auth_id;
    ds3_str* admin_secret_key;
    ds3_str* data_path_end_point;
    ds3_bool data_path_https;
    int data_path_port;
    ds3_str* data_path_proxy;
    ds3_bool data_path_verify_certificate;
    ds3_target_read_preference_type default_read_preference;
    ds3_str* id;
    ds3_str* name;
    ds3_bool permit_going_out_of_sync;
    ds3_quiesced quiesced;
    ds3_str* replicated_user_default_data_policy;
    ds3_target_state state;
}ds3_target_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* id;
    ds3_str* target_id;
    ds3_target_failure_type type;
}ds3_target_failure_response;
typedef struct {
    ds3_str* bucket_id;
    ds3_str* id;
    ds3_target_read_preference_type read_preference;
    ds3_str* target_id;
}ds3_target_read_preference_response;
typedef struct {
    ds3_str* blob_id;
    ds3_str* ds3_target_id;
    ds3_str* id;
}ds3_suspect_blob_target_response;
typedef struct {
    ds3_str* branch;
    ds3_str* revision;
    ds3_str* version;
}ds3_build_information_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* notification_generation_date;
    ds3_str* target_id;
    ds3_target_failure_type type;
}ds3_target_failure_notification_payload_response;
typedef struct {
    ds3_str* dao_type;
    ds3_str** ids;
    size_t num_ids;
    ds3_str* notification_generation_date;
    ds3_sql_operation sql_operation;
}ds3_generic_dao_notification_payload_response;
typedef struct {
    ds3_str* job_id;
    ds3_str* notification_generation_date;
}ds3_job_created_notification_payload_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* notification_generation_date;
    ds3_str* pool_id;
    ds3_pool_failure_type type;
}ds3_pool_failure_notification_payload_response;
typedef struct {
    ds3_str** tape_bar_codes;
    size_t num_tape_bar_codes;
}ds3_set_of_tape_bar_codes_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* notification_generation_date;
    ds3_str* storage_domain_id;
    ds3_storage_domain_failure_type type;
}ds3_storage_domain_failure_notification_payload_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* notification_generation_date;
    ds3_system_failure_type type;
}ds3_system_failure_notification_payload_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* notification_generation_date;
    ds3_str* tape_drive_id;
    ds3_str* tape_id;
    ds3_tape_failure_type type;
}ds3_tape_failure_notification_payload_response;
typedef struct {
    ds3_str* date;
    ds3_str* error_message;
    ds3_str* notification_generation_date;
    ds3_str* partition_id;
    ds3_tape_partition_failure_type type;
}ds3_tape_partition_failure_notification_payload_response;
typedef struct {
    ds3_set_of_tape_bar_codes_response** tapes_to_online;
    size_t num_tapes_to_online;
}ds3_tapes_must_be_onlined_response;
typedef struct {
    ds3_str* date_scheduled;
    ds3_str* date_started;
    ds3_str* description;
    ds3_str* drive_id;
    ds3_str* ds3_target_id;
    uint64_t id;
    ds3_str* name;
    ds3_str* pool_id;
    ds3_priority priority;
    ds3_blob_store_task_state state;
    ds3_str* tape_id;
}ds3_blob_store_task_information_response;
typedef struct {
    ds3_blob_store_task_information_response** tasks;
    size_t num_tasks;
}ds3_blob_store_tasks_information_response;
typedef struct {
    ds3_blob_response* blob;
    ds3_cache_entry_state state;
}ds3_cache_entry_information_response;
typedef struct {
    uint64_t available_capacity_in_bytes;
    ds3_cache_filesystem_response* cache_filesystem;
    ds3_cache_entry_information_response** entries;
    size_t num_entries;
    ds3_str* summary;
    uint64_t total_capacity_in_bytes;
    uint64_t unavailable_capacity_in_bytes;
    uint64_t used_capacity_in_bytes;
}ds3_cache_filesystem_information_response;
typedef struct {
    ds3_cache_filesystem_information_response** filesystems;
    size_t num_filesystems;
}ds3_cache_information_response;
typedef struct {
    ds3_str* creation_date;
    ds3_str* name;
}ds3_bucket_details_response;
typedef struct {
    ds3_str* bucket;
    ds3_str* e_tag;
    ds3_str* key;
    ds3_str* location;
}ds3_complete_multipart_upload_result_response;
typedef struct {
    ds3_str* code;
    ds3_str* key;
    ds3_str* message;
}ds3_delete_object_error_response;
typedef struct {
    ds3_tape_drive_type drive_type;
    ds3_tape_drive_type* drive_types;
    size_t num_drive_types;
    ds3_str* error_message;
    ds3_str* id;
    ds3_import_export_configuration import_export_configuration;
    ds3_str* library_id;
    ds3_str* name;
    ds3_quiesced quiesced;
    ds3_str* serial_number;
    ds3_tape_partition_state state;
    ds3_tape_type* tape_types;
    size_t num_tape_types;
}ds3_detailed_tape_partition_response;
typedef struct {
    ds3_str* code;
    int http_error_code;
    ds3_str* message;
    ds3_str* resource;
    uint64_t resource_id;
}ds3_error_response;
typedef struct {
    ds3_str* bucket;
    ds3_str* key;
    ds3_str* upload_id;
}ds3_initiate_multipart_upload_result_response;
typedef struct {
    ds3_str* e_tag;
    ds3_str* last_modified;
    int part_number;
}ds3_multi_part_upload_part_response;
typedef struct {
    ds3_str* end_point;
    int http_port;
    int https_port;
    ds3_str* id;
}ds3_job_node_response;
typedef struct {
    ds3_str* key;
}ds3_s3_object_to_delete_response;
typedef struct {
    ds3_str* display_name;
    ds3_str* id;
}ds3_user_response;
typedef struct {
    ds3_str* api_version;
    ds3_bool backend_activated;
    ds3_build_information_response* build_information;
    ds3_str* instance_id;
    uint64_t now;
    ds3_str* serial_number;
}ds3_system_information_response;
typedef struct {
    ds3_database_physical_space_state database_filesystem_free_space;
    uint64_t ms_required_to_verify_data_planner_health;
}ds3_health_verification_result_response;
typedef struct {
    ds3_tape_drive_type drive_type;
    ds3_tape_drive_type* drive_types;
    size_t num_drive_types;
    ds3_str* error_message;
    ds3_str* id;
    ds3_import_export_configuration import_export_configuration;
    ds3_str* library_id;
    ds3_str* name;
    ds3_quiesced quiesced;
    ds3_str* serial_number;
    ds3_tape_partition_state state;
    ds3_tape_type* tape_types;
    size_t num_tape_types;
}ds3_named_detailed_tape_partition_response;
typedef struct {
    ds3_str* cause;
    ds3_tape_response* tape;
}ds3_tape_failure_response;
typedef struct {
    ds3_tape_failure_response** failures;
    size_t num_failures;
}ds3_tape_failure_list_response;
typedef struct {
    ds3_application application;
    ds3_str* path;
}ds3_create_heap_dump_params_response;
typedef struct {
    ds3_str* beans_retriever_name;
    ds3_str* domain_name;
    int number_of_type;
}ds3_type_response;
typedef struct {
    uint64_t elapsed_millis;
    int elapsed_minutes;
    uint64_t elapsed_nanos;
    int elapsed_seconds;
}ds3_duration_response;
typedef struct {
    ds3_bucket_acl_response** bucket_acls;
    size_t num_bucket_acls;
    ds3_paging* paging;
}ds3_bucket_acl_list_response;
typedef struct {
    ds3_data_policy_acl_response** data_policy_acls;
    size_t num_data_policy_acls;
    ds3_paging* paging;
}ds3_data_policy_acl_list_response;
typedef struct {
    ds3_bucket_response** buckets;
    size_t num_buckets;
    ds3_paging* paging;
}ds3_bucket_list_response;
typedef struct {
    ds3_cache_filesystem_response** cache_filesystems;
    size_t num_cache_filesystems;
    ds3_paging* paging;
}ds3_cache_filesystem_list_response;
typedef struct {
    ds3_data_persistence_rule_response** data_persistence_rules;
    size_t num_data_persistence_rules;
    ds3_paging* paging;
}ds3_data_persistence_rule_list_response;
typedef struct {
    ds3_data_policy_response** data_policies;
    size_t num_data_policies;
    ds3_paging* paging;
}ds3_data_policy_list_response;
typedef struct {
    ds3_data_replication_rule_response** data_replication_rules;
    size_t num_data_replication_rules;
    ds3_paging* paging;
}ds3_data_replication_rule_list_response;
typedef struct {
    ds3_degraded_blob_response** degraded_blobs;
    size_t num_degraded_blobs;
    ds3_paging* paging;
}ds3_degraded_blob_list_response;
typedef struct {
    ds3_suspect_blob_pool_response** suspect_blob_pools;
    size_t num_suspect_blob_pools;
    ds3_paging* paging;
}ds3_suspect_blob_pool_list_response;
typedef struct {
    ds3_suspect_blob_tape_response** suspect_blob_tapes;
    size_t num_suspect_blob_tapes;
    ds3_paging* paging;
}ds3_suspect_blob_tape_list_response;
typedef struct {
    ds3_suspect_blob_target_response** suspect_blob_targets;
    size_t num_suspect_blob_targets;
    ds3_paging* paging;
}ds3_suspect_blob_target_list_response;
typedef struct {
    ds3_group_member_response** group_members;
    size_t num_group_members;
    ds3_paging* paging;
}ds3_group_member_list_response;
typedef struct {
    ds3_group_response** groups;
    size_t num_groups;
    ds3_paging* paging;
}ds3_group_list_response;
typedef struct {
    ds3_active_job_response** active_jobs;
    size_t num_active_jobs;
    ds3_paging* paging;
}ds3_active_job_list_response;
typedef struct {
    ds3_canceled_job_response** canceled_jobs;
    size_t num_canceled_jobs;
    ds3_paging* paging;
}ds3_canceled_job_list_response;
typedef struct {
    ds3_completed_job_response** completed_jobs;
    size_t num_completed_jobs;
    ds3_paging* paging;
}ds3_completed_job_list_response;
typedef struct {
    ds3_node_response** nodes;
    size_t num_nodes;
    ds3_paging* paging;
}ds3_node_list_response;
typedef struct {
    ds3_target_failure_notification_registration_response** ds3_target_failure_notification_registrations;
    size_t num_ds3_target_failure_notification_registrations;
    ds3_paging* paging;
}ds3_target_failure_notification_registration_list_response;
typedef struct {
    ds3_job_completed_notification_registration_response** job_completed_notification_registrations;
    size_t num_job_completed_notification_registrations;
    ds3_paging* paging;
}ds3_job_completed_notification_registration_list_response;
typedef struct {
    ds3_job_created_notification_registration_response** job_created_notification_registrations;
    size_t num_job_created_notification_registrations;
    ds3_paging* paging;
}ds3_job_created_notification_registration_list_response;
typedef struct {
    ds3_job_creation_failed_notification_registration_response** job_creation_failed_notification_registrations;
    size_t num_job_creation_failed_notification_registrations;
    ds3_paging* paging;
}ds3_job_creation_failed_notification_registration_list_response;
typedef struct {
    ds3_s3_object_cached_notification_registration_response** s3_object_cached_notification_registrations;
    size_t num_s3_object_cached_notification_registrations;
    ds3_paging* paging;
}ds3_s3_object_cached_notification_registration_list_response;
typedef struct {
    ds3_s3_object_lost_notification_registration_response** s3_object_lost_notification_registrations;
    size_t num_s3_object_lost_notification_registrations;
    ds3_paging* paging;
}ds3_s3_object_lost_notification_registration_list_response;
typedef struct {
    ds3_s3_object_persisted_notification_registration_response** s3_object_persisted_notification_registrations;
    size_t num_s3_object_persisted_notification_registrations;
    ds3_paging* paging;
}ds3_s3_object_persisted_notification_registration_list_response;
typedef struct {
    ds3_pool_failure_notification_registration_response** pool_failure_notification_registrations;
    size_t num_pool_failure_notification_registrations;
    ds3_paging* paging;
}ds3_pool_failure_notification_registration_list_response;
typedef struct {
    ds3_storage_domain_failure_notification_registration_response** storage_domain_failure_notification_registrations;
    size_t num_storage_domain_failure_notification_registrations;
    ds3_paging* paging;
}ds3_storage_domain_failure_notification_registration_list_response;
typedef struct {
    ds3_system_failure_notification_registration_response** system_failure_notification_registrations;
    size_t num_system_failure_notification_registrations;
    ds3_paging* paging;
}ds3_system_failure_notification_registration_list_response;
typedef struct {
    ds3_tape_failure_notification_registration_response** tape_failure_notification_registrations;
    size_t num_tape_failure_notification_registrations;
    ds3_paging* paging;
}ds3_tape_failure_notification_registration_list_response;
typedef struct {
    ds3_tape_partition_failure_notification_registration_response** tape_partition_failure_notification_registrations;
    size_t num_tape_partition_failure_notification_registrations;
    ds3_paging* paging;
}ds3_tape_partition_failure_notification_registration_list_response;
typedef struct {
    ds3_s3_object_response** s3_objects;
    size_t num_s3_objects;
    ds3_paging* paging;
}ds3_s3_object_list_response;
typedef struct {
    ds3_pool_failure_response** pool_failures;
    size_t num_pool_failures;
    ds3_paging* paging;
}ds3_pool_failure_list_response;
typedef struct {
    ds3_pool_partition_response** pool_partitions;
    size_t num_pool_partitions;
    ds3_paging* paging;
}ds3_pool_partition_list_response;
typedef struct {
    ds3_pool_response** pools;
    size_t num_pools;
    ds3_paging* paging;
}ds3_pool_list_response;
typedef struct {
    ds3_storage_domain_failure_response** storage_domain_failures;
    size_t num_storage_domain_failures;
    ds3_paging* paging;
}ds3_storage_domain_failure_list_response;
typedef struct {
    ds3_storage_domain_member_response** storage_domain_members;
    size_t num_storage_domain_members;
    ds3_paging* paging;
}ds3_storage_domain_member_list_response;
typedef struct {
    ds3_storage_domain_response** storage_domains;
    size_t num_storage_domains;
    ds3_paging* paging;
}ds3_storage_domain_list_response;
typedef struct {
    ds3_system_failure_response** system_failures;
    size_t num_system_failures;
    ds3_paging* paging;
}ds3_system_failure_list_response;
typedef struct {
    ds3_tape_density_directive_response** tape_density_directives;
    size_t num_tape_density_directives;
    ds3_paging* paging;
}ds3_tape_density_directive_list_response;
typedef struct {
    ds3_tape_drive_response** tape_drives;
    size_t num_tape_drives;
    ds3_paging* paging;
}ds3_tape_drive_list_response;
typedef struct {
    ds3_detailed_tape_failure_response** detailed_tape_failures;
    size_t num_detailed_tape_failures;
    ds3_paging* paging;
}ds3_detailed_tape_failure_list_response;
typedef struct {
    ds3_tape_library_response** tape_libraries;
    size_t num_tape_libraries;
    ds3_paging* paging;
}ds3_tape_library_list_response;
typedef struct {
    ds3_tape_partition_failure_response** tape_partition_failures;
    size_t num_tape_partition_failures;
    ds3_paging* paging;
}ds3_tape_partition_failure_list_response;
typedef struct {
    ds3_tape_partition_response** tape_partitions;
    size_t num_tape_partitions;
    ds3_paging* paging;
}ds3_tape_partition_list_response;
typedef struct {
    ds3_named_detailed_tape_partition_response** named_detailed_tape_partitions;
    size_t num_named_detailed_tape_partitions;
    ds3_paging* paging;
}ds3_named_detailed_tape_partition_list_response;
typedef struct {
    ds3_tape_response** tapes;
    size_t num_tapes;
    ds3_paging* paging;
}ds3_tape_list_response;
typedef struct {
    ds3_target_failure_response** ds3_target_failures;
    size_t num_ds3_target_failures;
    ds3_paging* paging;
}ds3_target_failure_list_response;
typedef struct {
    ds3_target_read_preference_response** ds3_target_read_preferences;
    size_t num_ds3_target_read_preferences;
    ds3_paging* paging;
}ds3_target_read_preference_list_response;
typedef struct {
    ds3_target_response** ds3_targets;
    size_t num_ds3_targets;
    ds3_paging* paging;
}ds3_target_list_response;
typedef struct {
    ds3_spectra_user_response** spectra_users;
    size_t num_spectra_users;
    ds3_paging* paging;
}ds3_spectra_user_list_response;
typedef struct {
    ds3_target_response** ds3_targets;
    size_t num_ds3_targets;
    ds3_pool_response** pools;
    size_t num_pools;
    ds3_tape_response** tapes;
    size_t num_tapes;
}ds3_physical_placement_response;
typedef struct {
    ds3_storage_domain_capacity_summary_response* pool;
    ds3_storage_domain_capacity_summary_response* tape;
}ds3_capacity_summary_container_response;
typedef struct {
    ds3_str* bucket;
    ds3_str* id;
    ds3_bool in_cache;
    ds3_bool latest;
    uint64_t length;
    ds3_str* name;
    uint64_t offset;
    ds3_physical_placement_response* physical_placement;
    uint64_t version;
}ds3_bulk_object_response;
typedef struct {
    ds3_bulk_object_response** objects;
    size_t num_objects;
}ds3_bulk_object_list_response;
typedef struct {
    ds3_bool cancel_occurred;
    ds3_str* job_id;
    ds3_str* notification_generation_date;
    ds3_bulk_object_response** objects_not_persisted;
    size_t num_objects_not_persisted;
}ds3_job_completed_notification_payload_response;
typedef struct {
    ds3_str* notification_generation_date;
    ds3_tapes_must_be_onlined_response* tapes_must_be_onlined;
}ds3_job_creation_failed_notification_payload_response;
typedef struct {
    ds3_str* job_id;
    ds3_str* notification_generation_date;
    ds3_bulk_object_response** objects;
    size_t num_objects;
}ds3_s3_objects_cached_notification_payload_response;
typedef struct {
    ds3_str* notification_generation_date;
    ds3_bulk_object_response** objects;
    size_t num_objects;
}ds3_s3_objects_lost_notification_payload_response;
typedef struct {
    ds3_str* job_id;
    ds3_str* notification_generation_date;
    ds3_bulk_object_response** objects;
    size_t num_objects;
}ds3_s3_objects_persisted_notification_payload_response;
typedef struct {
    ds3_bucket_details_response** buckets;
    size_t num_buckets;
    ds3_user_response* owner;
}ds3_list_all_my_buckets_result_response;
typedef struct {
    ds3_s3_object_to_delete_response** deleted_objects;
    size_t num_deleted_objects;
    ds3_delete_object_error_response** errors;
    size_t num_errors;
}ds3_delete_result_response;
typedef struct {
    ds3_bool aggregating;
    ds3_str* bucket_name;
    uint64_t cached_size_in_bytes;
    ds3_job_chunk_client_processing_order_guarantee chunk_client_processing_order_guarantee;
    uint64_t completed_size_in_bytes;
    ds3_bool entirely_in_cache;
    ds3_str* job_id;
    ds3_bool naked;
    ds3_str* name;
    ds3_job_node_response** nodes;
    size_t num_nodes;
    uint64_t original_size_in_bytes;
    ds3_priority priority;
    ds3_job_request_type request_type;
    ds3_str* start_date;
    ds3_job_status status;
    ds3_str* user_id;
    ds3_str* user_name;
}ds3_job_response;
typedef struct {
    ds3_str* chunk_id;
    int chunk_number;
    ds3_str* node_id;
    ds3_bulk_object_response** objects;
    size_t num_objects;
}ds3_objects_response;
typedef struct {
    ds3_bool aggregating;
    ds3_str* bucket_name;
    uint64_t cached_size_in_bytes;
    ds3_job_chunk_client_processing_order_guarantee chunk_client_processing_order_guarantee;
    uint64_t completed_size_in_bytes;
    ds3_bool entirely_in_cache;
    ds3_str* job_id;
    ds3_bool naked;
    ds3_str* name;
    ds3_job_node_response** nodes;
    size_t num_nodes;
    ds3_objects_response** objects;
    size_t num_objects;
    uint64_t original_size_in_bytes;
    ds3_priority priority;
    ds3_job_request_type request_type;
    ds3_str* start_date;
    ds3_job_status status;
    ds3_str* user_id;
    ds3_str* user_name;
}ds3_master_object_list_response;
typedef struct {
    ds3_job_response** jobs;
    size_t num_jobs;
}ds3_job_list_response;
typedef struct {
    ds3_str* bucket;
    ds3_str* key;
    int max_parts;
    int next_part_number_marker;
    ds3_user_response* owner;
    int part_number_marker;
    ds3_multi_part_upload_part_response** parts;
    size_t num_parts;
    ds3_bool truncated;
    ds3_str* upload_id;
}ds3_list_parts_result_response;
typedef struct {
    ds3_str* initiated;
    ds3_str* key;
    ds3_user_response* owner;
    ds3_str* upload_id;
}ds3_multi_part_upload_response;
typedef struct {
    ds3_str* e_tag;
    ds3_str* key;
    ds3_str* last_modified;
    ds3_user_response* owner;
    uint64_t size;
    ds3_str* storage_class;
}ds3_contents_response;
typedef struct {
    ds3_bulk_object_list_response* blobs;
    int blobs_being_persisted;
    int blobs_degraded;
    int blobs_in_cache;
    int blobs_total;
    ds3_str* bucket_id;
    ds3_str* creation_date;
    ds3_str* e_tag;
    ds3_str* id;
    ds3_bool latest;
    ds3_str* name;
    ds3_str* owner;
    uint64_t size;
    ds3_s3_object_type type;
    uint64_t version;
}ds3_detailed_s3_object_response;
typedef struct {
    ds3_type_response** types;
    size_t num_types;
}ds3_database_contents_response;
typedef struct {
    ds3_detailed_s3_object_response** detailed_s3_objects;
    size_t num_detailed_s3_objects;
    ds3_paging* paging;
}ds3_detailed_s3_object_list_response;
typedef struct {
    ds3_str** common_prefixes;
    size_t num_common_prefixes;
    ds3_str* creation_date;
    ds3_str* delimiter;
    ds3_str* marker;
    int max_keys;
    ds3_str* name;
    ds3_str* next_marker;
    ds3_contents_response** objects;
    size_t num_objects;
    ds3_str* prefix;
    ds3_bool truncated;
}ds3_list_bucket_result_response;
typedef struct {
    ds3_str* bucket;
    ds3_str** common_prefixes;
    size_t num_common_prefixes;
    ds3_str* delimiter;
    ds3_str* key_marker;
    int max_uploads;
    ds3_str* next_key_marker;
    ds3_str* next_upload_id_marker;
    ds3_str* prefix;
    ds3_bool truncated;
    ds3_str* upload_id_marker;
    ds3_multi_part_upload_response** uploads;
    size_t num_uploads;
}ds3_list_multi_part_uploads_result_response;

typedef struct {
    ds3_error_code      code;
    ds3_str*            message;
    ds3_error_response* error;
}ds3_error;

typedef struct _ds3_client {
    ds3_str*      endpoint;
    ds3_str*      proxy;
    uint64_t      num_redirects;
    ds3_creds*    creds;
    ds3_log*      log;
    ds3_error* (* net_callback)(const struct _ds3_client* client,
                                const ds3_request* _request,
                                void* read_user_struct,
                                size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                void* write_user_struct,
                                size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                ds3_string_multimap** return_headers);
    ds3_connection_pool* connection_pool;
}ds3_client;

LIBRARY_API void ds3_blob_response_free(ds3_blob_response* response_data);
LIBRARY_API void ds3_bucket_response_free(ds3_bucket_response* response_data);
LIBRARY_API void ds3_bucket_acl_response_free(ds3_bucket_acl_response* response_data);
LIBRARY_API void ds3_canceled_job_response_free(ds3_canceled_job_response* response_data);
LIBRARY_API void ds3_completed_job_response_free(ds3_completed_job_response* response_data);
LIBRARY_API void ds3_data_path_backend_response_free(ds3_data_path_backend_response* response_data);
LIBRARY_API void ds3_data_persistence_rule_response_free(ds3_data_persistence_rule_response* response_data);
LIBRARY_API void ds3_data_policy_response_free(ds3_data_policy_response* response_data);
LIBRARY_API void ds3_data_policy_acl_response_free(ds3_data_policy_acl_response* response_data);
LIBRARY_API void ds3_data_replication_rule_response_free(ds3_data_replication_rule_response* response_data);
LIBRARY_API void ds3_degraded_blob_response_free(ds3_degraded_blob_response* response_data);
LIBRARY_API void ds3_group_response_free(ds3_group_response* response_data);
LIBRARY_API void ds3_group_member_response_free(ds3_group_member_response* response_data);
LIBRARY_API void ds3_active_job_response_free(ds3_active_job_response* response_data);
LIBRARY_API void ds3_job_chunk_response_free(ds3_job_chunk_response* response_data);
LIBRARY_API void ds3_node_response_free(ds3_node_response* response_data);
LIBRARY_API void ds3_s3_object_response_free(ds3_s3_object_response* response_data);
LIBRARY_API void ds3_storage_domain_response_free(ds3_storage_domain_response* response_data);
LIBRARY_API void ds3_storage_domain_capacity_summary_response_free(ds3_storage_domain_capacity_summary_response* response_data);
LIBRARY_API void ds3_storage_domain_failure_response_free(ds3_storage_domain_failure_response* response_data);
LIBRARY_API void ds3_storage_domain_member_response_free(ds3_storage_domain_member_response* response_data);
LIBRARY_API void ds3_system_failure_response_free(ds3_system_failure_response* response_data);
LIBRARY_API void ds3_spectra_user_response_free(ds3_spectra_user_response* response_data);
LIBRARY_API void ds3_target_failure_notification_registration_response_free(ds3_target_failure_notification_registration_response* response_data);
LIBRARY_API void ds3_generic_dao_notification_registration_response_free(ds3_generic_dao_notification_registration_response* response_data);
LIBRARY_API void ds3_job_completed_notification_registration_response_free(ds3_job_completed_notification_registration_response* response_data);
LIBRARY_API void ds3_job_created_notification_registration_response_free(ds3_job_created_notification_registration_response* response_data);
LIBRARY_API void ds3_job_creation_failed_notification_registration_response_free(ds3_job_creation_failed_notification_registration_response* response_data);
LIBRARY_API void ds3_pool_failure_notification_registration_response_free(ds3_pool_failure_notification_registration_response* response_data);
LIBRARY_API void ds3_s3_object_cached_notification_registration_response_free(ds3_s3_object_cached_notification_registration_response* response_data);
LIBRARY_API void ds3_s3_object_lost_notification_registration_response_free(ds3_s3_object_lost_notification_registration_response* response_data);
LIBRARY_API void ds3_s3_object_persisted_notification_registration_response_free(ds3_s3_object_persisted_notification_registration_response* response_data);
LIBRARY_API void ds3_storage_domain_failure_notification_registration_response_free(ds3_storage_domain_failure_notification_registration_response* response_data);
LIBRARY_API void ds3_system_failure_notification_registration_response_free(ds3_system_failure_notification_registration_response* response_data);
LIBRARY_API void ds3_tape_failure_notification_registration_response_free(ds3_tape_failure_notification_registration_response* response_data);
LIBRARY_API void ds3_tape_partition_failure_notification_registration_response_free(ds3_tape_partition_failure_notification_registration_response* response_data);
LIBRARY_API void ds3_cache_filesystem_response_free(ds3_cache_filesystem_response* response_data);
LIBRARY_API void ds3_pool_response_free(ds3_pool_response* response_data);
LIBRARY_API void ds3_pool_failure_response_free(ds3_pool_failure_response* response_data);
LIBRARY_API void ds3_pool_partition_response_free(ds3_pool_partition_response* response_data);
LIBRARY_API void ds3_suspect_blob_pool_response_free(ds3_suspect_blob_pool_response* response_data);
LIBRARY_API void ds3_suspect_blob_tape_response_free(ds3_suspect_blob_tape_response* response_data);
LIBRARY_API void ds3_tape_response_free(ds3_tape_response* response_data);
LIBRARY_API void ds3_tape_density_directive_response_free(ds3_tape_density_directive_response* response_data);
LIBRARY_API void ds3_tape_drive_response_free(ds3_tape_drive_response* response_data);
LIBRARY_API void ds3_detailed_tape_failure_response_free(ds3_detailed_tape_failure_response* response_data);
LIBRARY_API void ds3_tape_library_response_free(ds3_tape_library_response* response_data);
LIBRARY_API void ds3_tape_partition_response_free(ds3_tape_partition_response* response_data);
LIBRARY_API void ds3_tape_partition_failure_response_free(ds3_tape_partition_failure_response* response_data);
LIBRARY_API void ds3_target_response_free(ds3_target_response* response_data);
LIBRARY_API void ds3_target_failure_response_free(ds3_target_failure_response* response_data);
LIBRARY_API void ds3_target_read_preference_response_free(ds3_target_read_preference_response* response_data);
LIBRARY_API void ds3_suspect_blob_target_response_free(ds3_suspect_blob_target_response* response_data);
LIBRARY_API void ds3_build_information_response_free(ds3_build_information_response* response_data);
LIBRARY_API void ds3_target_failure_notification_payload_response_free(ds3_target_failure_notification_payload_response* response_data);
LIBRARY_API void ds3_generic_dao_notification_payload_response_free(ds3_generic_dao_notification_payload_response* response_data);
LIBRARY_API void ds3_job_created_notification_payload_response_free(ds3_job_created_notification_payload_response* response_data);
LIBRARY_API void ds3_pool_failure_notification_payload_response_free(ds3_pool_failure_notification_payload_response* response_data);
LIBRARY_API void ds3_set_of_tape_bar_codes_response_free(ds3_set_of_tape_bar_codes_response* response_data);
LIBRARY_API void ds3_storage_domain_failure_notification_payload_response_free(ds3_storage_domain_failure_notification_payload_response* response_data);
LIBRARY_API void ds3_system_failure_notification_payload_response_free(ds3_system_failure_notification_payload_response* response_data);
LIBRARY_API void ds3_tape_failure_notification_payload_response_free(ds3_tape_failure_notification_payload_response* response_data);
LIBRARY_API void ds3_tape_partition_failure_notification_payload_response_free(ds3_tape_partition_failure_notification_payload_response* response_data);
LIBRARY_API void ds3_tapes_must_be_onlined_response_free(ds3_tapes_must_be_onlined_response* response_data);
LIBRARY_API void ds3_blob_store_task_information_response_free(ds3_blob_store_task_information_response* response_data);
LIBRARY_API void ds3_blob_store_tasks_information_response_free(ds3_blob_store_tasks_information_response* response_data);
LIBRARY_API void ds3_cache_entry_information_response_free(ds3_cache_entry_information_response* response_data);
LIBRARY_API void ds3_cache_filesystem_information_response_free(ds3_cache_filesystem_information_response* response_data);
LIBRARY_API void ds3_cache_information_response_free(ds3_cache_information_response* response_data);
LIBRARY_API void ds3_bucket_details_response_free(ds3_bucket_details_response* response_data);
LIBRARY_API void ds3_complete_multipart_upload_result_response_free(ds3_complete_multipart_upload_result_response* response_data);
LIBRARY_API void ds3_delete_object_error_response_free(ds3_delete_object_error_response* response_data);
LIBRARY_API void ds3_detailed_tape_partition_response_free(ds3_detailed_tape_partition_response* response_data);
LIBRARY_API void ds3_error_response_free(ds3_error_response* response_data);
LIBRARY_API void ds3_initiate_multipart_upload_result_response_free(ds3_initiate_multipart_upload_result_response* response_data);
LIBRARY_API void ds3_multi_part_upload_part_response_free(ds3_multi_part_upload_part_response* response_data);
LIBRARY_API void ds3_job_node_response_free(ds3_job_node_response* response_data);
LIBRARY_API void ds3_s3_object_to_delete_response_free(ds3_s3_object_to_delete_response* response_data);
LIBRARY_API void ds3_user_response_free(ds3_user_response* response_data);
LIBRARY_API void ds3_system_information_response_free(ds3_system_information_response* response_data);
LIBRARY_API void ds3_health_verification_result_response_free(ds3_health_verification_result_response* response_data);
LIBRARY_API void ds3_named_detailed_tape_partition_response_free(ds3_named_detailed_tape_partition_response* response_data);
LIBRARY_API void ds3_tape_failure_response_free(ds3_tape_failure_response* response_data);
LIBRARY_API void ds3_tape_failure_list_response_free(ds3_tape_failure_list_response* response_data);
LIBRARY_API void ds3_create_heap_dump_params_response_free(ds3_create_heap_dump_params_response* response_data);
LIBRARY_API void ds3_type_response_free(ds3_type_response* response_data);
LIBRARY_API void ds3_duration_response_free(ds3_duration_response* response_data);
LIBRARY_API void ds3_bucket_acl_list_response_free(ds3_bucket_acl_list_response* response_data);
LIBRARY_API void ds3_data_policy_acl_list_response_free(ds3_data_policy_acl_list_response* response_data);
LIBRARY_API void ds3_bucket_list_response_free(ds3_bucket_list_response* response_data);
LIBRARY_API void ds3_cache_filesystem_list_response_free(ds3_cache_filesystem_list_response* response_data);
LIBRARY_API void ds3_data_persistence_rule_list_response_free(ds3_data_persistence_rule_list_response* response_data);
LIBRARY_API void ds3_data_policy_list_response_free(ds3_data_policy_list_response* response_data);
LIBRARY_API void ds3_data_replication_rule_list_response_free(ds3_data_replication_rule_list_response* response_data);
LIBRARY_API void ds3_degraded_blob_list_response_free(ds3_degraded_blob_list_response* response_data);
LIBRARY_API void ds3_suspect_blob_pool_list_response_free(ds3_suspect_blob_pool_list_response* response_data);
LIBRARY_API void ds3_suspect_blob_tape_list_response_free(ds3_suspect_blob_tape_list_response* response_data);
LIBRARY_API void ds3_suspect_blob_target_list_response_free(ds3_suspect_blob_target_list_response* response_data);
LIBRARY_API void ds3_group_member_list_response_free(ds3_group_member_list_response* response_data);
LIBRARY_API void ds3_group_list_response_free(ds3_group_list_response* response_data);
LIBRARY_API void ds3_active_job_list_response_free(ds3_active_job_list_response* response_data);
LIBRARY_API void ds3_canceled_job_list_response_free(ds3_canceled_job_list_response* response_data);
LIBRARY_API void ds3_completed_job_list_response_free(ds3_completed_job_list_response* response_data);
LIBRARY_API void ds3_node_list_response_free(ds3_node_list_response* response_data);
LIBRARY_API void ds3_target_failure_notification_registration_list_response_free(ds3_target_failure_notification_registration_list_response* response_data);
LIBRARY_API void ds3_job_completed_notification_registration_list_response_free(ds3_job_completed_notification_registration_list_response* response_data);
LIBRARY_API void ds3_job_created_notification_registration_list_response_free(ds3_job_created_notification_registration_list_response* response_data);
LIBRARY_API void ds3_job_creation_failed_notification_registration_list_response_free(ds3_job_creation_failed_notification_registration_list_response* response_data);
LIBRARY_API void ds3_s3_object_cached_notification_registration_list_response_free(ds3_s3_object_cached_notification_registration_list_response* response_data);
LIBRARY_API void ds3_s3_object_lost_notification_registration_list_response_free(ds3_s3_object_lost_notification_registration_list_response* response_data);
LIBRARY_API void ds3_s3_object_persisted_notification_registration_list_response_free(ds3_s3_object_persisted_notification_registration_list_response* response_data);
LIBRARY_API void ds3_pool_failure_notification_registration_list_response_free(ds3_pool_failure_notification_registration_list_response* response_data);
LIBRARY_API void ds3_storage_domain_failure_notification_registration_list_response_free(ds3_storage_domain_failure_notification_registration_list_response* response_data);
LIBRARY_API void ds3_system_failure_notification_registration_list_response_free(ds3_system_failure_notification_registration_list_response* response_data);
LIBRARY_API void ds3_tape_failure_notification_registration_list_response_free(ds3_tape_failure_notification_registration_list_response* response_data);
LIBRARY_API void ds3_tape_partition_failure_notification_registration_list_response_free(ds3_tape_partition_failure_notification_registration_list_response* response_data);
LIBRARY_API void ds3_s3_object_list_response_free(ds3_s3_object_list_response* response_data);
LIBRARY_API void ds3_pool_failure_list_response_free(ds3_pool_failure_list_response* response_data);
LIBRARY_API void ds3_pool_partition_list_response_free(ds3_pool_partition_list_response* response_data);
LIBRARY_API void ds3_pool_list_response_free(ds3_pool_list_response* response_data);
LIBRARY_API void ds3_storage_domain_failure_list_response_free(ds3_storage_domain_failure_list_response* response_data);
LIBRARY_API void ds3_storage_domain_member_list_response_free(ds3_storage_domain_member_list_response* response_data);
LIBRARY_API void ds3_storage_domain_list_response_free(ds3_storage_domain_list_response* response_data);
LIBRARY_API void ds3_system_failure_list_response_free(ds3_system_failure_list_response* response_data);
LIBRARY_API void ds3_tape_density_directive_list_response_free(ds3_tape_density_directive_list_response* response_data);
LIBRARY_API void ds3_tape_drive_list_response_free(ds3_tape_drive_list_response* response_data);
LIBRARY_API void ds3_detailed_tape_failure_list_response_free(ds3_detailed_tape_failure_list_response* response_data);
LIBRARY_API void ds3_tape_library_list_response_free(ds3_tape_library_list_response* response_data);
LIBRARY_API void ds3_tape_partition_failure_list_response_free(ds3_tape_partition_failure_list_response* response_data);
LIBRARY_API void ds3_tape_partition_list_response_free(ds3_tape_partition_list_response* response_data);
LIBRARY_API void ds3_named_detailed_tape_partition_list_response_free(ds3_named_detailed_tape_partition_list_response* response_data);
LIBRARY_API void ds3_tape_list_response_free(ds3_tape_list_response* response_data);
LIBRARY_API void ds3_target_failure_list_response_free(ds3_target_failure_list_response* response_data);
LIBRARY_API void ds3_target_read_preference_list_response_free(ds3_target_read_preference_list_response* response_data);
LIBRARY_API void ds3_target_list_response_free(ds3_target_list_response* response_data);
LIBRARY_API void ds3_spectra_user_list_response_free(ds3_spectra_user_list_response* response_data);
LIBRARY_API void ds3_physical_placement_response_free(ds3_physical_placement_response* response_data);
LIBRARY_API void ds3_capacity_summary_container_response_free(ds3_capacity_summary_container_response* response_data);
LIBRARY_API void ds3_bulk_object_response_free(ds3_bulk_object_response* response_data);
LIBRARY_API void ds3_bulk_object_list_response_free(ds3_bulk_object_list_response* response_data);
LIBRARY_API void ds3_job_completed_notification_payload_response_free(ds3_job_completed_notification_payload_response* response_data);
LIBRARY_API void ds3_job_creation_failed_notification_payload_response_free(ds3_job_creation_failed_notification_payload_response* response_data);
LIBRARY_API void ds3_s3_objects_cached_notification_payload_response_free(ds3_s3_objects_cached_notification_payload_response* response_data);
LIBRARY_API void ds3_s3_objects_lost_notification_payload_response_free(ds3_s3_objects_lost_notification_payload_response* response_data);
LIBRARY_API void ds3_s3_objects_persisted_notification_payload_response_free(ds3_s3_objects_persisted_notification_payload_response* response_data);
LIBRARY_API void ds3_list_all_my_buckets_result_response_free(ds3_list_all_my_buckets_result_response* response_data);
LIBRARY_API void ds3_delete_result_response_free(ds3_delete_result_response* response_data);
LIBRARY_API void ds3_job_response_free(ds3_job_response* response_data);
LIBRARY_API void ds3_objects_response_free(ds3_objects_response* response_data);
LIBRARY_API void ds3_master_object_list_response_free(ds3_master_object_list_response* response_data);
LIBRARY_API void ds3_job_list_response_free(ds3_job_list_response* response_data);
LIBRARY_API void ds3_list_parts_result_response_free(ds3_list_parts_result_response* response_data);
LIBRARY_API void ds3_multi_part_upload_response_free(ds3_multi_part_upload_response* response_data);
LIBRARY_API void ds3_contents_response_free(ds3_contents_response* response_data);
LIBRARY_API void ds3_detailed_s3_object_response_free(ds3_detailed_s3_object_response* response_data);
LIBRARY_API void ds3_database_contents_response_free(ds3_database_contents_response* response_data);
LIBRARY_API void ds3_detailed_s3_object_list_response_free(ds3_detailed_s3_object_list_response* response_data);
LIBRARY_API void ds3_list_bucket_result_response_free(ds3_list_bucket_result_response* response_data);
LIBRARY_API void ds3_list_multi_part_uploads_result_response_free(ds3_list_multi_part_uploads_result_response* response_data);

LIBRARY_API void ds3_request_free(ds3_request* request);
LIBRARY_API void ds3_error_free(ds3_error* error);
LIBRARY_API void ds3_multipart_upload_part_response_free(ds3_multipart_upload_part_response* response);
LIBRARY_API void ds3_complete_multipart_upload_response_free(ds3_complete_multipart_upload_response* response);
LIBRARY_API void ds3_delete_objects_response_free(ds3_delete_objects_response* response);

LIBRARY_API ds3_metadata_entry* ds3_metadata_get_entry(const ds3_metadata* metadata, const char* name);
LIBRARY_API unsigned int ds3_metadata_size(const ds3_metadata* metadata);
LIBRARY_API ds3_metadata_keys_result* ds3_metadata_keys(const ds3_metadata* metadata);

LIBRARY_API void ds3_metadata_free(ds3_metadata* _metadata);
LIBRARY_API void ds3_metadata_entry_free(ds3_metadata_entry* entry);
LIBRARY_API void ds3_metadata_keys_free(ds3_metadata_keys_result* metadata_keys);

LIBRARY_API ds3_creds*  ds3_create_creds(const char* access_id, const char* secret_key);
LIBRARY_API ds3_client* ds3_create_client(const char* endpoint, ds3_creds* creds);
LIBRARY_API ds3_error*  ds3_create_client_from_env(ds3_client** client);
LIBRARY_API void        ds3_client_register_logging(ds3_client* client, ds3_log_lvl log_lvl, void (* log_callback)(const char* log_message, void* user_data), void* user_data);
LIBRARY_API void        ds3_client_register_net(ds3_client* client, ds3_error* (* net_callback)(const ds3_client* client,
                                                                                                const ds3_request* _request,
                                                                                                void* read_user_struct,
                                                                                                size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                                                                                void* write_user_struct,
                                                                                                size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                                                                                ds3_string_multimap** return_headers));
LIBRARY_API void ds3_client_proxy(ds3_client* client, const char* proxy);

// Set optional request query parameters
LIBRARY_API void ds3_request_set_byte_range(ds3_request* _request, int64_t rangeStart, int64_t rangeEnd);
LIBRARY_API void ds3_request_reset_byte_range(ds3_request* _request);
LIBRARY_API void ds3_request_set_access_control_replication_ds3_target_access_control_replication(const ds3_request* request, const ds3_target_access_control_replication value);
LIBRARY_API void ds3_request_set_activated(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_admin_auth_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_admin_secret_key(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_aggregating(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_always_force_put_job_creation(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_always_minimize_spanning_across_media(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_always_replicate_deletes(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_assigned_to_storage_domain(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_auth_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_auto_activate_timeout_in_mins(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_auto_eject_media_full_threshold(const ds3_request* request, const uint64_t value);
LIBRARY_API void ds3_request_set_auto_eject_upon_cron(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_auto_eject_upon_job_cancellation(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_auto_eject_upon_job_completion(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_auto_eject_upon_media_full(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_auto_inspect_ds3_auto_inspect_mode(const ds3_request* request, const ds3_auto_inspect_mode value);
LIBRARY_API void ds3_request_set_auto_reclaim_initiate_threshold(const ds3_request* request, const float value);
LIBRARY_API void ds3_request_set_auto_reclaim_terminate_threshold(const ds3_request* request, const float value);
LIBRARY_API void ds3_request_set_bar_code(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_blob_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_blobbing_enabled(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_bucket_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_built_in(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_burst_threshold(const ds3_request* request, const float value);
LIBRARY_API void ds3_request_set_canceled_due_to_timeout(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_checksum_type_ds3_checksum_type(const ds3_request* request, const ds3_checksum_type value);
LIBRARY_API void ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(const ds3_request* request, const ds3_job_chunk_client_processing_order_guarantee value);
LIBRARY_API void ds3_request_set_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value);
LIBRARY_API void ds3_request_set_created_at(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_data_path_end_point(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_data_path_https(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_data_path_port(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_data_path_proxy(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_data_path_verify_certificate(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_data_policy_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_default_blob_size(const ds3_request* request, const uint64_t value);
LIBRARY_API void ds3_request_set_default_data_policy_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_default_get_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_default_import_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value);
LIBRARY_API void ds3_request_set_default_put_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_default_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value);
LIBRARY_API void ds3_request_set_default_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_default_verify_data_prior_to_import(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_default_verify_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_delimiter(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_density_ds3_tape_drive_type(const ds3_request* request, const ds3_tape_drive_type value);
LIBRARY_API void ds3_request_set_dns_name(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_ds3_target_data_policy(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_ds3_target_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_eject_label(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_eject_location(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_end_to_end_crc_required(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_error_message(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_folder(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_force(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value);
LIBRARY_API void ds3_request_set_full_details(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_full_of_data(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_group_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value);
LIBRARY_API void ds3_request_set_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_ignore_naming_conflicts(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_import_export_configuration_ds3_import_export_configuration(const ds3_request* request, const ds3_import_export_configuration value);
LIBRARY_API void ds3_request_set_include_physical_placement(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_isolation_level_ds3_data_isolation_level(const ds3_request* request, const ds3_data_isolation_level value);
LIBRARY_API void ds3_request_set_job(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_job_chunk(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_job_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_key_marker(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_last_page(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_latest(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_library_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_ltfs_file_naming_ds3_ltfs_file_naming_mode(const ds3_request* request, const ds3_ltfs_file_naming_mode value);
LIBRARY_API void ds3_request_set_management_url(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_marker(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_max_capacity_in_bytes(const ds3_request* request, const uint64_t value);
LIBRARY_API void ds3_request_set_max_keys(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_max_parts(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_max_tape_fragmentation_percent(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_max_upload_size(const ds3_request* request, const uint64_t value);
LIBRARY_API void ds3_request_set_max_uploads(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_maximum_auto_verification_frequency_in_days(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_media_ejection_allowed(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_member_group_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_member_user_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_minimize_spanning_across_media(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_minimum_days_to_retain(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_name(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value);
LIBRARY_API void ds3_request_set_node_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value);
LIBRARY_API void ds3_request_set_offset(const ds3_request* request, const uint64_t value);
LIBRARY_API void ds3_request_set_page_length(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_page_offset(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_page_start_marker(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_part_number_marker(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_partially_verified_end_of_tape(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_partially_verify_last_percent_of_tapes(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_partition_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_permission_ds3_bucket_acl_permission(const ds3_request* request, const ds3_bucket_acl_permission value);
LIBRARY_API void ds3_request_set_permit_going_out_of_sync(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_persistence_rule_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_pool_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value);
LIBRARY_API void ds3_request_set_pool_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_pool_partition_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_pool_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value);
LIBRARY_API void ds3_request_set_pool_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value);
LIBRARY_API void ds3_request_set_powered_on(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_preferred_number_of_chunks(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_prefix(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_previous_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value);
LIBRARY_API void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value);
LIBRARY_API void ds3_request_set_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value);
LIBRARY_API void ds3_request_set_rebuild_priority_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_rechunked(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_replicate(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_replicated_user_default_data_policy(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_replication_rule_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value);
LIBRARY_API void ds3_request_set_roll_back(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_secret_key(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_secure_media_allocation(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_serial_number(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_state_ds3_data_persistence_rule_state(const ds3_request* request, const ds3_data_persistence_rule_state value);
LIBRARY_API void ds3_request_set_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value);
LIBRARY_API void ds3_request_set_state_ds3_storage_domain_member_state(const ds3_request* request, const ds3_storage_domain_member_state value);
LIBRARY_API void ds3_request_set_state_ds3_tape_drive_state(const ds3_request* request, const ds3_tape_drive_state value);
LIBRARY_API void ds3_request_set_state_ds3_tape_partition_state(const ds3_request* request, const ds3_tape_partition_state value);
LIBRARY_API void ds3_request_set_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value);
LIBRARY_API void ds3_request_set_state_ds3_target_state(const ds3_request* request, const ds3_target_state value);
LIBRARY_API void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_tape_drive_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_tape_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_tape_partition_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_tape_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value);
LIBRARY_API void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value);
LIBRARY_API void ds3_request_set_target_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_task_priority_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_truncated(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_type_ds3_data_persistence_rule_type(const ds3_request* request, const ds3_data_persistence_rule_type value);
LIBRARY_API void ds3_request_set_type_ds3_data_replication_rule_type(const ds3_request* request, const ds3_data_replication_rule_type value);
LIBRARY_API void ds3_request_set_type_ds3_s3_object_type(const ds3_request* request, const ds3_s3_object_type value);
LIBRARY_API void ds3_request_set_type_ds3_pool_failure_type(const ds3_request* request, const ds3_pool_failure_type value);
LIBRARY_API void ds3_request_set_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value);
LIBRARY_API void ds3_request_set_type_ds3_storage_domain_failure_type(const ds3_request* request, const ds3_storage_domain_failure_type value);
LIBRARY_API void ds3_request_set_type_ds3_system_failure_type(const ds3_request* request, const ds3_system_failure_type value);
LIBRARY_API void ds3_request_set_type_ds3_tape_drive_type(const ds3_request* request, const ds3_tape_drive_type value);
LIBRARY_API void ds3_request_set_type_ds3_tape_failure_type(const ds3_request* request, const ds3_tape_failure_type value);
LIBRARY_API void ds3_request_set_type_ds3_tape_partition_failure_type(const ds3_request* request, const ds3_tape_partition_failure_type value);
LIBRARY_API void ds3_request_set_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value);
LIBRARY_API void ds3_request_set_type_ds3_target_failure_type(const ds3_request* request, const ds3_target_failure_type value);
LIBRARY_API void ds3_request_set_unavailable_media_policy_ds3_unavailable_media_usage_policy(const ds3_request* request, const ds3_unavailable_media_usage_policy value);
LIBRARY_API void ds3_request_set_unavailable_pool_max_job_retry_in_mins(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_unavailable_tape_partition_max_job_retry_in_mins(const ds3_request* request, const int value);
LIBRARY_API void ds3_request_set_upload_id_marker(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_user_id(const ds3_request* request, const char* value);
LIBRARY_API void ds3_request_set_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_verify_data_prior_to_import(const ds3_request* request, ds3_bool value);
LIBRARY_API void ds3_request_set_verify_prior_to_auto_eject_ds3_priority(const ds3_request* request, const ds3_priority value);
LIBRARY_API void ds3_request_set_version(const ds3_request* request, const uint64_t value);
LIBRARY_API void ds3_request_set_versioning_ds3_versioning_level(const ds3_request* request, const ds3_versioning_level value);
LIBRARY_API void ds3_request_set_write_optimization_ds3_write_optimization(const ds3_request* request, const ds3_write_optimization value);
LIBRARY_API void ds3_request_set_write_preference_ds3_write_preference_level(const ds3_request* request, const ds3_write_preference_level value);
LIBRARY_API void ds3_request_set_write_protected(const ds3_request* request, ds3_bool value);

// Set headers / metadata
LIBRARY_API void ds3_request_set_custom_header(ds3_request* request, const char* header_name, const char* header_value);
LIBRARY_API void ds3_request_set_md5(ds3_request* request, const char* md5);
LIBRARY_API void ds3_request_set_sha256(ds3_request* request, const char* sha256);
LIBRARY_API void ds3_request_set_sha512(ds3_request* request, const char* sha512);
LIBRARY_API void ds3_request_set_crc32(ds3_request* request, const char* crc32);
LIBRARY_API void ds3_request_set_crc32c(ds3_request* request, const char* crc32c);
// Any attempt to set a key with an empty or NULL value will be ignored.
LIBRARY_API void ds3_request_set_metadata(ds3_request* request, const char* name, const char* value);

LIBRARY_API void ds3_creds_free(ds3_creds* client);
LIBRARY_API void ds3_client_free(ds3_client* client);

LIBRARY_API ds3_error* ds3_get_object_with_metadata(const ds3_client* client,
                                                    const ds3_request* request,
                                                    void* user_data,
                                                    size_t (* callback)(void*, size_t, size_t, void*),
                                                    ds3_metadata** _metadata);


LIBRARY_API ds3_request* ds3_init_abort_multi_part_upload_request(const char* bucket_name, const char* object_name, const char* upload_id);
LIBRARY_API ds3_error* ds3_abort_multi_part_upload_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_complete_multi_part_upload_request(const char* bucket_name, const char* object_name, const char* upload_id, const ds3_complete_multipart_upload_response* mpu_list);
LIBRARY_API ds3_error* ds3_complete_multi_part_upload_request(const ds3_client* client, const ds3_request* request, ds3_complete_multipart_upload_result_response** response);
LIBRARY_API ds3_request* ds3_init_put_bucket_request(const char* bucket_name);
LIBRARY_API ds3_error* ds3_put_bucket_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_put_multi_part_upload_part_request(const char* bucket_name, const char* object_name, const int part_number, const char* upload_id);
LIBRARY_API ds3_error* ds3_put_multi_part_upload_part_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*));
/**
 * Optional Request Modifiers for ds3_init_put_object_request
 *
 *   void ds3_request_set_job(const ds3_request* request, const char* value)
 *   void ds3_request_set_offset(const ds3_request* request, const uint64_t value)
 */
LIBRARY_API ds3_request* ds3_init_put_object_request(const char* bucket_name, const char* object_name, const uint64_t length);
LIBRARY_API ds3_error* ds3_put_object_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*));
LIBRARY_API ds3_request* ds3_init_delete_bucket_request(const char* bucket_name);
LIBRARY_API ds3_error* ds3_delete_bucket_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_delete_object_request
 *
 *   void ds3_request_set_replicate(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_roll_back(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_delete_object_request(const char* bucket_name, const char* object_name);
LIBRARY_API ds3_error* ds3_delete_object_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_delete_objects_request
 *
 *   void ds3_request_set_replicate(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_roll_back(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_delete_objects_request(const char* bucket_name, const ds3_delete_objects_response* objects_list);
LIBRARY_API ds3_error* ds3_delete_objects_request(const ds3_client* client, const ds3_request* request, ds3_delete_result_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_bucket_request
 *
 *   void ds3_request_set_delimiter(const ds3_request* request, const char* value)
 *   void ds3_request_set_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_max_keys(const ds3_request* request, const int value)
 *   void ds3_request_set_prefix(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_bucket_request(const char* bucket_name);
LIBRARY_API ds3_error* ds3_get_bucket_request(const ds3_client* client, const ds3_request* request, ds3_list_bucket_result_response** response);
LIBRARY_API ds3_request* ds3_init_get_service_request(void);
LIBRARY_API ds3_error* ds3_get_service_request(const ds3_client* client, const ds3_request* request, ds3_list_all_my_buckets_result_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_object_request
 *
 *   void ds3_request_set_job(const ds3_request* request, const char* value)
 *   void ds3_request_set_offset(const ds3_request* request, const uint64_t value)
 */
LIBRARY_API ds3_request* ds3_init_get_object_request(const char* bucket_name, const char* object_name, const uint64_t length);
LIBRARY_API ds3_error* ds3_get_object_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*));
LIBRARY_API ds3_request* ds3_init_head_bucket_request(const char* bucket_name);
LIBRARY_API ds3_error* ds3_head_bucket_request(const ds3_client* client, const ds3_request* request);

LIBRARY_API ds3_request* ds3_init_head_object_request(const char* bucket_name, const char* object_name);
LIBRARY_API ds3_error* ds3_head_object_request(const ds3_client* client, const ds3_request* request, ds3_metadata** _metadata);

LIBRARY_API ds3_request* ds3_init_initiate_multi_part_upload_request(const char* bucket_name, const char* object_name);
LIBRARY_API ds3_error* ds3_initiate_multi_part_upload_request(const ds3_client* client, const ds3_request* request, ds3_initiate_multipart_upload_result_response** response);
/**
 * Optional Request Modifiers for ds3_init_list_multi_part_upload_parts_request
 *
 *   void ds3_request_set_max_parts(const ds3_request* request, const int value)
 *   void ds3_request_set_part_number_marker(const ds3_request* request, const int value)
 */
LIBRARY_API ds3_request* ds3_init_list_multi_part_upload_parts_request(const char* bucket_name, const char* object_name, const char* upload_id);
LIBRARY_API ds3_error* ds3_list_multi_part_upload_parts_request(const ds3_client* client, const ds3_request* request, ds3_list_parts_result_response** response);
/**
 * Optional Request Modifiers for ds3_init_list_multi_part_uploads_request
 *
 *   void ds3_request_set_delimiter(const ds3_request* request, const char* value)
 *   void ds3_request_set_key_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_max_uploads(const ds3_request* request, const int value)
 *   void ds3_request_set_prefix(const ds3_request* request, const char* value)
 *   void ds3_request_set_upload_id_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_list_multi_part_uploads_request(const char* bucket_name);
LIBRARY_API ds3_error* ds3_list_multi_part_uploads_request(const ds3_client* client, const ds3_request* request, ds3_list_multi_part_uploads_result_response** response);
LIBRARY_API ds3_request* ds3_init_put_bucket_acl_for_group_spectra_s3_request(const char* bucket_id, const char* group_id, const ds3_bucket_acl_permission permission);
LIBRARY_API ds3_error* ds3_put_bucket_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response);
LIBRARY_API ds3_request* ds3_init_put_bucket_acl_for_user_spectra_s3_request(const char* bucket_id, const ds3_bucket_acl_permission permission, const char* user_id);
LIBRARY_API ds3_error* ds3_put_bucket_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response);
LIBRARY_API ds3_request* ds3_init_put_data_policy_acl_for_group_spectra_s3_request(const char* data_policy_id, const char* group_id);
LIBRARY_API ds3_error* ds3_put_data_policy_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response);
LIBRARY_API ds3_request* ds3_init_put_data_policy_acl_for_user_spectra_s3_request(const char* data_policy_id, const char* user_id);
LIBRARY_API ds3_error* ds3_put_data_policy_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response);
LIBRARY_API ds3_request* ds3_init_put_global_bucket_acl_for_group_spectra_s3_request(const char* group_id, const ds3_bucket_acl_permission permission);
LIBRARY_API ds3_error* ds3_put_global_bucket_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response);
LIBRARY_API ds3_request* ds3_init_put_global_bucket_acl_for_user_spectra_s3_request(const ds3_bucket_acl_permission permission, const char* user_id);
LIBRARY_API ds3_error* ds3_put_global_bucket_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response);
LIBRARY_API ds3_request* ds3_init_put_global_data_policy_acl_for_group_spectra_s3_request(const char* group_id);
LIBRARY_API ds3_error* ds3_put_global_data_policy_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response);
LIBRARY_API ds3_request* ds3_init_put_global_data_policy_acl_for_user_spectra_s3_request(const char* user_id);
LIBRARY_API ds3_error* ds3_put_global_data_policy_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response);
LIBRARY_API ds3_request* ds3_init_delete_bucket_acl_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_bucket_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_data_policy_acl_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_data_policy_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_bucket_acl_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_bucket_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_bucket_acls_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_group_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_permission_ds3_bucket_acl_permission(const ds3_request* request, const ds3_bucket_acl_permission value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_bucket_acls_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_bucket_acls_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_data_policy_acl_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_data_policy_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_data_policy_acls_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_group_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_data_policy_acls_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_data_policy_acls_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_bucket_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_put_bucket_spectra_s3_request(const char* name);
LIBRARY_API ds3_error* ds3_put_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response);
/**
 * Optional Request Modifiers for ds3_init_delete_bucket_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_replicate(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_delete_bucket_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_bucket_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_buckets_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_buckets_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_bucket_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_modify_bucket_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response);
LIBRARY_API ds3_request* ds3_init_force_full_cache_reclaim_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_force_full_cache_reclaim_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_cache_filesystem_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_cache_filesystem_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_cache_filesystems_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_node_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_cache_filesystems_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_cache_filesystems_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_cache_state_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_cache_state_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_information_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_cache_filesystem_spectra_s3_request
 *
 *   void ds3_request_set_auto_reclaim_initiate_threshold(const ds3_request* request, const float value)
 *   void ds3_request_set_auto_reclaim_terminate_threshold(const ds3_request* request, const float value)
 *   void ds3_request_set_burst_threshold(const ds3_request* request, const float value)
 *   void ds3_request_set_max_capacity_in_bytes(const ds3_request* request, const uint64_t value)
 */
LIBRARY_API ds3_request* ds3_init_modify_cache_filesystem_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_cache_filesystem_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_bucket_capacity_summary_spectra_s3_request
 *
 *   void ds3_request_set_pool_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value)
 *   void ds3_request_set_pool_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value)
 *   void ds3_request_set_pool_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value)
 *   void ds3_request_set_tape_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value)
 *   void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_bucket_capacity_summary_spectra_s3_request(const char* bucket_id, const char* storage_domain_id);
LIBRARY_API ds3_error* ds3_get_bucket_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_storage_domain_capacity_summary_spectra_s3_request
 *
 *   void ds3_request_set_pool_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value)
 *   void ds3_request_set_pool_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value)
 *   void ds3_request_set_pool_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value)
 *   void ds3_request_set_tape_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value)
 *   void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_storage_domain_capacity_summary_spectra_s3_request(const char* storage_domain_id);
LIBRARY_API ds3_error* ds3_get_storage_domain_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_system_capacity_summary_spectra_s3_request
 *
 *   void ds3_request_set_pool_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value)
 *   void ds3_request_set_pool_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value)
 *   void ds3_request_set_pool_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value)
 *   void ds3_request_set_tape_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value)
 *   void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_system_capacity_summary_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_system_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response);
LIBRARY_API ds3_request* ds3_init_get_data_path_backend_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_data_path_backend_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_data_planner_blob_store_tasks_spectra_s3_request
 *
 *   void ds3_request_set_full_details(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_get_data_planner_blob_store_tasks_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_data_planner_blob_store_tasks_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_blob_store_tasks_information_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_data_path_backend_spectra_s3_request
 *
 *   void ds3_request_set_activated(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_auto_activate_timeout_in_mins(const ds3_request* request, const int value)
 *   void ds3_request_set_auto_inspect_ds3_auto_inspect_mode(const ds3_request* request, const ds3_auto_inspect_mode value)
 *   void ds3_request_set_default_import_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value)
 *   void ds3_request_set_default_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_default_verify_data_prior_to_import(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_partially_verify_last_percent_of_tapes(const ds3_request* request, const int value)
 *   void ds3_request_set_unavailable_media_policy_ds3_unavailable_media_usage_policy(const ds3_request* request, const ds3_unavailable_media_usage_policy value)
 *   void ds3_request_set_unavailable_pool_max_job_retry_in_mins(const ds3_request* request, const int value)
 *   void ds3_request_set_unavailable_tape_partition_max_job_retry_in_mins(const ds3_request* request, const int value)
 */
LIBRARY_API ds3_request* ds3_init_modify_data_path_backend_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_modify_data_path_backend_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_data_persistence_rule_spectra_s3_request
 *
 *   void ds3_request_set_minimum_days_to_retain(const ds3_request* request, const int value)
 */
LIBRARY_API ds3_request* ds3_init_put_data_persistence_rule_spectra_s3_request(const char* data_policy_id, const ds3_data_isolation_level isolation_level, const char* storage_domain_id, const ds3_data_persistence_rule_type type);
LIBRARY_API ds3_error* ds3_put_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_data_policy_spectra_s3_request
 *
 *   void ds3_request_set_always_force_put_job_creation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_always_minimize_spanning_across_media(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_always_replicate_deletes(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_blobbing_enabled(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_checksum_type_ds3_checksum_type(const ds3_request* request, const ds3_checksum_type value)
 *   void ds3_request_set_default_blob_size(const ds3_request* request, const uint64_t value)
 *   void ds3_request_set_default_get_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_default_put_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_default_verify_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_end_to_end_crc_required(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_rebuild_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_versioning_ds3_versioning_level(const ds3_request* request, const ds3_versioning_level value)
 */
LIBRARY_API ds3_request* ds3_init_put_data_policy_spectra_s3_request(const char* name);
LIBRARY_API ds3_error* ds3_put_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_data_replication_rule_spectra_s3_request
 *
 *   void ds3_request_set_ds3_target_data_policy(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_put_data_replication_rule_spectra_s3_request(const char* data_policy_id, const char* ds3_target_id, const ds3_data_replication_rule_type type);
LIBRARY_API ds3_error* ds3_put_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response);
LIBRARY_API ds3_request* ds3_init_delete_data_persistence_rule_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_data_policy_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_data_replication_rule_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_data_persistence_rule_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_data_persistence_rules_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_isolation_level_ds3_data_isolation_level(const ds3_request* request, const ds3_data_isolation_level value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_data_persistence_rule_state(const ds3_request* request, const ds3_data_persistence_rule_state value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_data_persistence_rule_type(const ds3_request* request, const ds3_data_persistence_rule_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_data_persistence_rules_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_data_persistence_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_data_policies_spectra_s3_request
 *
 *   void ds3_request_set_always_force_put_job_creation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_always_minimize_spanning_across_media(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_always_replicate_deletes(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_checksum_type_ds3_checksum_type(const ds3_request* request, const ds3_checksum_type value)
 *   void ds3_request_set_end_to_end_crc_required(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_data_policies_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_data_policies_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_data_policy_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response);
LIBRARY_API ds3_request* ds3_init_get_data_replication_rule_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_data_replication_rules_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_ds3_target_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_data_persistence_rule_state(const ds3_request* request, const ds3_data_persistence_rule_state value)
 *   void ds3_request_set_type_ds3_data_replication_rule_type(const ds3_request* request, const ds3_data_replication_rule_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_data_replication_rules_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_data_persistence_rule_spectra_s3_request
 *
 *   void ds3_request_set_isolation_level_ds3_data_isolation_level(const ds3_request* request, const ds3_data_isolation_level value)
 *   void ds3_request_set_minimum_days_to_retain(const ds3_request* request, const int value)
 *   void ds3_request_set_type_ds3_data_persistence_rule_type(const ds3_request* request, const ds3_data_persistence_rule_type value)
 */
LIBRARY_API ds3_request* ds3_init_modify_data_persistence_rule_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_data_policy_spectra_s3_request
 *
 *   void ds3_request_set_always_force_put_job_creation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_always_minimize_spanning_across_media(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_always_replicate_deletes(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_blobbing_enabled(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_checksum_type_ds3_checksum_type(const ds3_request* request, const ds3_checksum_type value)
 *   void ds3_request_set_default_blob_size(const ds3_request* request, const uint64_t value)
 *   void ds3_request_set_default_get_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_default_put_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_default_verify_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_end_to_end_crc_required(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_rebuild_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_versioning_ds3_versioning_level(const ds3_request* request, const ds3_versioning_level value)
 */
LIBRARY_API ds3_request* ds3_init_modify_data_policy_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_data_replication_rule_spectra_s3_request
 *
 *   void ds3_request_set_ds3_target_data_policy(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_data_replication_rule_type(const ds3_request* request, const ds3_data_replication_rule_type value)
 */
LIBRARY_API ds3_request* ds3_init_modify_data_replication_rule_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response);
/**
 * Optional Request Modifiers for ds3_init_clear_suspect_blob_pools_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_clear_suspect_blob_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_clear_suspect_blob_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_clear_suspect_blob_tapes_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_clear_suspect_blob_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_clear_suspect_blob_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_clear_suspect_blob_targets_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_clear_suspect_blob_targets_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_clear_suspect_blob_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_get_degraded_blobs_spectra_s3_request
 *
 *   void ds3_request_set_blob_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_persistence_rule_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_replication_rule_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_degraded_blobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_degraded_blobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_degraded_blob_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_degraded_buckets_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_degraded_buckets_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_degraded_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_degraded_data_persistence_rules_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_isolation_level_ds3_data_isolation_level(const ds3_request* request, const ds3_data_isolation_level value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_data_persistence_rule_state(const ds3_request* request, const ds3_data_persistence_rule_state value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_data_persistence_rule_type(const ds3_request* request, const ds3_data_persistence_rule_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_degraded_data_persistence_rules_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_degraded_data_persistence_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_degraded_data_replication_rules_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_ds3_target_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_data_persistence_rule_state(const ds3_request* request, const ds3_data_persistence_rule_state value)
 *   void ds3_request_set_type_ds3_data_replication_rule_type(const ds3_request* request, const ds3_data_replication_rule_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_degraded_data_replication_rules_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_degraded_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_suspect_blob_pools_spectra_s3_request
 *
 *   void ds3_request_set_blob_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_pool_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_suspect_blob_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_suspect_blob_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_pool_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_suspect_blob_tapes_spectra_s3_request
 *
 *   void ds3_request_set_blob_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_tape_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_suspect_blob_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_suspect_blob_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_tape_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_suspect_blob_targets_spectra_s3_request
 *
 *   void ds3_request_set_blob_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_ds3_target_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_suspect_blob_targets_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_suspect_blob_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_target_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_suspect_buckets_spectra_s3_request
 *
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_suspect_buckets_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_suspect_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_suspect_objects_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_suspect_objects_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_suspect_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_suspect_objects_with_full_details_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_suspect_objects_with_full_details_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_suspect_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_mark_suspect_blob_pools_as_degraded_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_mark_suspect_blob_pools_as_degraded_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_mark_suspect_blob_pools_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_mark_suspect_blob_tapes_as_degraded_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_mark_suspect_blob_tapes_as_degraded_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_mark_suspect_blob_tapes_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_mark_suspect_blob_targets_as_degraded_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_mark_suspect_blob_targets_as_degraded_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_mark_suspect_blob_targets_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_put_group_group_member_spectra_s3_request(const char* group_id, const char* member_group_id);
LIBRARY_API ds3_error* ds3_put_group_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response);
LIBRARY_API ds3_request* ds3_init_put_group_spectra_s3_request(const char* name);
LIBRARY_API ds3_error* ds3_put_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response);
LIBRARY_API ds3_request* ds3_init_put_user_group_member_spectra_s3_request(const char* group_id, const char* member_user_id);
LIBRARY_API ds3_error* ds3_put_user_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response);
LIBRARY_API ds3_request* ds3_init_delete_group_member_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_group_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_group_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_group_member_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_group_members_spectra_s3_request
 *
 *   void ds3_request_set_group_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_member_group_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_member_user_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_group_members_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_group_members_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_group_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_groups_spectra_s3_request
 *
 *   void ds3_request_set_built_in(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_groups_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_groups_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_group_spectra_s3_request
 *
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_modify_group_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_user_is_member_of_group_spectra_s3_request
 *
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_verify_user_is_member_of_group_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_verify_user_is_member_of_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response);
LIBRARY_API ds3_request* ds3_init_allocate_job_chunk_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_allocate_job_chunk_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_objects_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_active_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_cancel_all_active_jobs_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_cancel_all_active_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_all_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_cancel_all_jobs_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_cancel_all_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_all_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_cancel_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_job_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_clear_all_canceled_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_clear_all_canceled_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_clear_all_completed_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_clear_all_completed_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_get_bulk_job_spectra_s3_request
 *
 *   void ds3_request_set_aggregating(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(const ds3_request* request, const ds3_job_chunk_client_processing_order_guarantee value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_get_bulk_job_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_get_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_bulk_job_spectra_s3_request
 *
 *   void ds3_request_set_aggregating(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_ignore_naming_conflicts(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_max_upload_size(const ds3_request* request, const uint64_t value)
 *   void ds3_request_set_minimize_spanning_across_media(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_put_bulk_job_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_put_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_bulk_job_spectra_s3_request
 *
 *   void ds3_request_set_aggregating(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_verify_bulk_job_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_verify_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_active_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_active_job_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_active_jobs_spectra_s3_request
 *
 *   void ds3_request_set_aggregating(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(const ds3_request* request, const ds3_job_chunk_client_processing_order_guarantee value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_rechunked(const ds3_request* request, const char* value)
 *   void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value)
 *   void ds3_request_set_truncated(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_active_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_active_job_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_canceled_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_canceled_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_canceled_job_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_canceled_jobs_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_canceled_due_to_timeout(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(const ds3_request* request, const ds3_job_chunk_client_processing_order_guarantee value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_rechunked(const ds3_request* request, const char* value)
 *   void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value)
 *   void ds3_request_set_truncated(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_canceled_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_canceled_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_canceled_job_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_completed_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_completed_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_completed_job_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_completed_jobs_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(const ds3_request* request, const ds3_job_chunk_client_processing_order_guarantee value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_rechunked(const ds3_request* request, const char* value)
 *   void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value)
 *   void ds3_request_set_truncated(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_completed_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_completed_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_completed_job_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_job_chunk_dao_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_job_chunk_dao_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_chunk_response** response);
LIBRARY_API ds3_request* ds3_init_get_job_chunk_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_job_chunk_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_objects_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_job_chunks_ready_for_client_processing_spectra_s3_request
 *
 *   void ds3_request_set_job_chunk(const ds3_request* request, const char* value)
 *   void ds3_request_set_preferred_number_of_chunks(const ds3_request* request, const int value)
 */
LIBRARY_API ds3_request* ds3_init_get_job_chunks_ready_for_client_processing_spectra_s3_request(const char* job);
LIBRARY_API ds3_error* ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_job_to_replicate_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_job_to_replicate_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_str** response);
/**
 * Optional Request Modifiers for ds3_init_get_jobs_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_full_details(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_get_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_active_job_spectra_s3_request
 *
 *   void ds3_request_set_created_at(const ds3_request* request, const char* value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_modify_active_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_job_spectra_s3_request
 *
 *   void ds3_request_set_created_at(const ds3_request* request, const char* value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_modify_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_replicate_put_job_spectra_s3_request
 *
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_replicate_put_job_spectra_s3_request(const char* resource_id, const char* payload);
LIBRARY_API ds3_error* ds3_replicate_put_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response);
LIBRARY_API ds3_request* ds3_init_truncate_active_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_truncate_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_truncate_all_active_jobs_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_truncate_all_active_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_truncate_all_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_truncate_all_jobs_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_truncate_all_jobs_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_truncate_all_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_truncate_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_truncate_job_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_verify_safe_to_create_put_job_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_verify_safe_to_create_put_job_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_node_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_node_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_nodes_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_nodes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_nodes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_node_spectra_s3_request
 *
 *   void ds3_request_set_dns_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_modify_node_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_node_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_ds3_target_failure_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_ds3_target_failure_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_job_completed_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_job_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_job_completed_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_job_created_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_job_created_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_job_creation_failed_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_job_creation_failed_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_object_cached_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_job_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_object_cached_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_object_lost_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_object_lost_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_object_persisted_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_job_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_object_persisted_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_pool_failure_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_pool_failure_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_storage_domain_failure_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_storage_domain_failure_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_system_failure_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_system_failure_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_tape_failure_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_tape_failure_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_tape_partition_failure_notification_registration_spectra_s3_request
 *
 *   void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value)
 *   void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value)
 *   void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value)
 */
LIBRARY_API ds3_request* ds3_init_put_tape_partition_failure_notification_registration_spectra_s3_request(const char* notification_end_point);
LIBRARY_API ds3_error* ds3_put_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** response);
LIBRARY_API ds3_request* ds3_init_delete_ds3_target_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_job_completed_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_job_created_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_job_creation_failed_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_object_cached_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_object_lost_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_object_persisted_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_pool_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_storage_domain_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_system_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_tape_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_tape_partition_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_delete_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_ds3_target_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_ds3_target_failure_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_ds3_target_failure_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_ds3_target_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_job_completed_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_job_completed_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_job_completed_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_job_completed_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_job_created_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_job_created_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_job_created_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_job_created_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_job_creation_failed_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_job_creation_failed_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_job_creation_failed_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_job_creation_failed_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_object_cached_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_object_cached_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_object_cached_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_object_cached_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_object_lost_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_object_lost_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_object_lost_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_object_lost_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_object_persisted_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_object_persisted_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_object_persisted_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_object_persisted_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_pool_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_pool_failure_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_pool_failure_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_pool_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_storage_domain_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_storage_domain_failure_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_storage_domain_failure_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_storage_domain_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_system_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_system_failure_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_system_failure_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_system_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_failure_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_failure_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_partition_failure_notification_registration_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_partition_failure_notification_registrations_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_partition_failure_notification_registrations_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_partition_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_delete_folder_recursively_spectra_s3_request
 *
 *   void ds3_request_set_replicate(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_roll_back(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_delete_folder_recursively_spectra_s3_request(const char* resource_id, const char* bucket_id);
LIBRARY_API ds3_error* ds3_delete_folder_recursively_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_blob_persistence_spectra_s3_request(const char* payload);
LIBRARY_API ds3_error* ds3_get_blob_persistence_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_str** response);
LIBRARY_API ds3_request* ds3_init_get_object_details_spectra_s3_request(const char* resource_id, const char* bucket_id);
LIBRARY_API ds3_error* ds3_get_object_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_objects_details_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_folder(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_latest(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_s3_object_type(const ds3_request* request, const ds3_s3_object_type value)
 *   void ds3_request_set_version(const ds3_request* request, const uint64_t value)
 */
LIBRARY_API ds3_request* ds3_init_get_objects_details_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_objects_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_objects_with_full_details_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_folder(const ds3_request* request, const char* value)
 *   void ds3_request_set_include_physical_placement(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_latest(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_s3_object_type(const ds3_request* request, const ds3_s3_object_type value)
 *   void ds3_request_set_version(const ds3_request* request, const uint64_t value)
 */
LIBRARY_API ds3_request* ds3_init_get_objects_with_full_details_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_s3_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_physical_placement_for_objects_spectra_s3_request
 *
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_physical_placement_for_objects_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_get_physical_placement_for_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_physical_placement_for_objects_with_full_details_spectra_s3_request
 *
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_physical_placement_for_objects_with_full_details_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_get_physical_placement_for_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_physical_placement_for_objects_spectra_s3_request
 *
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_verify_physical_placement_for_objects_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_verify_physical_placement_for_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_physical_placement_for_objects_with_full_details_spectra_s3_request
 *
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_verify_physical_placement_for_objects_with_full_details_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_verify_physical_placement_for_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_import_on_all_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_import_on_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_cancel_import_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_import_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response);
/**
 * Optional Request Modifiers for ds3_init_compact_all_pools_spectra_s3_request
 *
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_compact_all_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_compact_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_compact_pool_spectra_s3_request
 *
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_compact_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_compact_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response);
LIBRARY_API ds3_request* ds3_init_put_pool_partition_spectra_s3_request(const char* name, const ds3_pool_type type);
LIBRARY_API ds3_error* ds3_put_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response);
LIBRARY_API ds3_request* ds3_init_deallocate_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_deallocate_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_permanently_lost_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_permanently_lost_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_pool_failure_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_pool_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_pool_partition_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_force_pool_environment_refresh_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_force_pool_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_format_all_foreign_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_format_all_foreign_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_format_foreign_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_format_foreign_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response);
LIBRARY_API ds3_request* ds3_init_get_blobs_on_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_blobs_on_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_pool_failures_spectra_s3_request
 *
 *   void ds3_request_set_error_message(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_pool_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_pool_failure_type(const ds3_request* request, const ds3_pool_failure_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_pool_failures_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_pool_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_pool_partition_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_pool_partitions_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_pool_partitions_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_pool_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_pools_spectra_s3_request
 *
 *   void ds3_request_set_assigned_to_storage_domain(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_powered_on(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_import_all_pools_spectra_s3_request
 *
 *   void ds3_request_set_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value)
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_verify_data_prior_to_import(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_import_all_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_import_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_import_pool_spectra_s3_request
 *
 *   void ds3_request_set_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value)
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_verify_data_prior_to_import(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_import_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_import_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response);
LIBRARY_API ds3_request* ds3_init_modify_all_pools_spectra_s3_request(const ds3_quiesced quiesced);
LIBRARY_API ds3_error* ds3_modify_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_modify_pool_partition_spectra_s3_request
 *
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_modify_pool_partition_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_pool_spectra_s3_request
 *
 *   void ds3_request_set_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value)
 */
LIBRARY_API ds3_request* ds3_init_modify_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_all_pools_spectra_s3_request
 *
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_verify_all_pools_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_verify_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_verify_pool_spectra_s3_request
 *
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_verify_pool_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_verify_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response);
LIBRARY_API ds3_request* ds3_init_convert_storage_domain_to_ds3_target_spectra_s3_request(const char* resource_id, const char* convert_to_ds3_target);
LIBRARY_API ds3_error* ds3_convert_storage_domain_to_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_put_pool_storage_domain_member_spectra_s3_request
 *
 *   void ds3_request_set_write_preference_ds3_write_preference_level(const ds3_request* request, const ds3_write_preference_level value)
 */
LIBRARY_API ds3_request* ds3_init_put_pool_storage_domain_member_spectra_s3_request(const char* pool_partition_id, const char* storage_domain_id);
LIBRARY_API ds3_error* ds3_put_pool_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_storage_domain_spectra_s3_request
 *
 *   void ds3_request_set_auto_eject_media_full_threshold(const ds3_request* request, const uint64_t value)
 *   void ds3_request_set_auto_eject_upon_cron(const ds3_request* request, const char* value)
 *   void ds3_request_set_auto_eject_upon_job_cancellation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_auto_eject_upon_job_completion(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_auto_eject_upon_media_full(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_ltfs_file_naming_ds3_ltfs_file_naming_mode(const ds3_request* request, const ds3_ltfs_file_naming_mode value)
 *   void ds3_request_set_max_tape_fragmentation_percent(const ds3_request* request, const int value)
 *   void ds3_request_set_maximum_auto_verification_frequency_in_days(const ds3_request* request, const int value)
 *   void ds3_request_set_media_ejection_allowed(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_secure_media_allocation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_verify_prior_to_auto_eject_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_write_optimization_ds3_write_optimization(const ds3_request* request, const ds3_write_optimization value)
 */
LIBRARY_API ds3_request* ds3_init_put_storage_domain_spectra_s3_request(const char* name);
LIBRARY_API ds3_error* ds3_put_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response);
/**
 * Optional Request Modifiers for ds3_init_put_tape_storage_domain_member_spectra_s3_request
 *
 *   void ds3_request_set_write_preference_ds3_write_preference_level(const ds3_request* request, const ds3_write_preference_level value)
 */
LIBRARY_API ds3_request* ds3_init_put_tape_storage_domain_member_spectra_s3_request(const char* storage_domain_id, const char* tape_partition_id, const ds3_tape_type tape_type);
LIBRARY_API ds3_error* ds3_put_tape_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response);
LIBRARY_API ds3_request* ds3_init_delete_storage_domain_failure_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_storage_domain_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_storage_domain_member_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_storage_domain_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_get_storage_domain_failures_spectra_s3_request
 *
 *   void ds3_request_set_error_message(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_storage_domain_failure_type(const ds3_request* request, const ds3_storage_domain_failure_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_storage_domain_failures_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_storage_domain_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_storage_domain_member_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_storage_domain_members_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_pool_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_storage_domain_member_state(const ds3_request* request, const ds3_storage_domain_member_state value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_tape_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value)
 *   void ds3_request_set_write_preference_ds3_write_preference_level(const ds3_request* request, const ds3_write_preference_level value)
 */
LIBRARY_API ds3_request* ds3_init_get_storage_domain_members_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_storage_domain_members_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_storage_domain_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_storage_domains_spectra_s3_request
 *
 *   void ds3_request_set_auto_eject_upon_cron(const ds3_request* request, const char* value)
 *   void ds3_request_set_auto_eject_upon_job_cancellation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_auto_eject_upon_job_completion(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_auto_eject_upon_media_full(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_media_ejection_allowed(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_secure_media_allocation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_write_optimization_ds3_write_optimization(const ds3_request* request, const ds3_write_optimization value)
 */
LIBRARY_API ds3_request* ds3_init_get_storage_domains_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_storage_domains_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_storage_domain_member_spectra_s3_request
 *
 *   void ds3_request_set_write_preference_ds3_write_preference_level(const ds3_request* request, const ds3_write_preference_level value)
 */
LIBRARY_API ds3_request* ds3_init_modify_storage_domain_member_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_storage_domain_spectra_s3_request
 *
 *   void ds3_request_set_auto_eject_media_full_threshold(const ds3_request* request, const uint64_t value)
 *   void ds3_request_set_auto_eject_upon_cron(const ds3_request* request, const char* value)
 *   void ds3_request_set_auto_eject_upon_job_cancellation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_auto_eject_upon_job_completion(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_auto_eject_upon_media_full(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_ltfs_file_naming_ds3_ltfs_file_naming_mode(const ds3_request* request, const ds3_ltfs_file_naming_mode value)
 *   void ds3_request_set_max_tape_fragmentation_percent(const ds3_request* request, const int value)
 *   void ds3_request_set_maximum_auto_verification_frequency_in_days(const ds3_request* request, const int value)
 *   void ds3_request_set_media_ejection_allowed(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_secure_media_allocation(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_verify_prior_to_auto_eject_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_write_optimization_ds3_write_optimization(const ds3_request* request, const ds3_write_optimization value)
 */
LIBRARY_API ds3_request* ds3_init_modify_storage_domain_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_system_failures_spectra_s3_request
 *
 *   void ds3_request_set_error_message(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_system_failure_type(const ds3_request* request, const ds3_system_failure_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_system_failures_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_system_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_system_information_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_system_information_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_information_response** response);
LIBRARY_API ds3_request* ds3_init_reset_instance_identifier_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_reset_instance_identifier_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response);
LIBRARY_API ds3_request* ds3_init_verify_system_health_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_verify_system_health_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_health_verification_result_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_eject_on_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_eject_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_eject_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_eject_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_format_on_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_format_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_format_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_format_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_import_on_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_import_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_import_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_import_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_online_on_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_online_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_online_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_online_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_verify_on_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_cancel_verify_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_cancel_verify_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_cancel_verify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_clean_tape_drive_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_clean_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** response);
LIBRARY_API ds3_request* ds3_init_put_tape_density_directive_spectra_s3_request(const ds3_tape_drive_type density, const char* partition_id, const ds3_tape_type tape_type);
LIBRARY_API ds3_error* ds3_put_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** response);
LIBRARY_API ds3_request* ds3_init_delete_permanently_lost_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_permanently_lost_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_tape_density_directive_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_tape_drive_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_tape_failure_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_tape_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_tape_partition_failure_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_tape_partition_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_tape_partition_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_eject_all_tapes_spectra_s3_request
 *
 *   void ds3_request_set_eject_label(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_location(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_eject_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_eject_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_eject_storage_domain_blobs_spectra_s3_request
 *
 *   void ds3_request_set_eject_label(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_location(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_eject_storage_domain_blobs_spectra_s3_request(const char* bucket_id, const char* storage_domain_id, const ds3_bulk_object_list_response* object_list);
LIBRARY_API ds3_error* ds3_eject_storage_domain_blobs_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_eject_storage_domain_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_label(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_location(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_eject_storage_domain_spectra_s3_request(const char* storage_domain_id);
LIBRARY_API ds3_error* ds3_eject_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_eject_tape_spectra_s3_request
 *
 *   void ds3_request_set_eject_label(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_location(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_eject_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_eject_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_force_tape_environment_refresh_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_force_tape_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_format_all_tapes_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_format_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_format_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_format_tape_spectra_s3_request
 *
 *   void ds3_request_set_force(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_format_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_format_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_get_blobs_on_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_blobs_on_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_density_directive_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_density_directives_spectra_s3_request
 *
 *   void ds3_request_set_density_ds3_tape_drive_type(const ds3_request* request, const ds3_tape_drive_type value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_density_directives_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_density_directives_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_drive_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_drives_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_serial_number(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_tape_drive_state(const ds3_request* request, const ds3_tape_drive_state value)
 *   void ds3_request_set_type_ds3_tape_drive_type(const ds3_request* request, const ds3_tape_drive_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_drives_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_drives_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_failures_spectra_s3_request
 *
 *   void ds3_request_set_error_message(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_tape_drive_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_tape_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_tape_failure_type(const ds3_request* request, const ds3_tape_failure_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_failures_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_failure_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_libraries_spectra_s3_request
 *
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_management_url(const ds3_request* request, const char* value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_serial_number(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_libraries_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_libraries_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_library_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_library_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_tape_library_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_library_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_partition_failures_spectra_s3_request
 *
 *   void ds3_request_set_error_message(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_tape_partition_failure_type(const ds3_request* request, const ds3_tape_partition_failure_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_partition_failures_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_partition_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_partition_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_partition_with_full_details_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_tape_partition_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_partition_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_partitions_spectra_s3_request
 *
 *   void ds3_request_set_import_export_configuration_ds3_import_export_configuration(const ds3_request* request, const ds3_import_export_configuration value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_library_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value)
 *   void ds3_request_set_serial_number(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_tape_partition_state(const ds3_request* request, const ds3_tape_partition_state value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_partitions_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tape_partitions_with_full_details_spectra_s3_request
 *
 *   void ds3_request_set_import_export_configuration_ds3_import_export_configuration(const ds3_request* request, const ds3_import_export_configuration value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_library_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value)
 *   void ds3_request_set_serial_number(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_tape_partition_state(const ds3_request* request, const ds3_tape_partition_state value)
 */
LIBRARY_API ds3_request* ds3_init_get_tape_partitions_with_full_details_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tape_partitions_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_named_detailed_tape_partition_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_tapes_spectra_s3_request
 *
 *   void ds3_request_set_assigned_to_storage_domain(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_bar_code(const ds3_request* request, const char* value)
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_label(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_location(const ds3_request* request, const char* value)
 *   void ds3_request_set_full_of_data(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_partially_verified_end_of_tape(const ds3_request* request, const char* value)
 *   void ds3_request_set_partition_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_previous_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value)
 *   void ds3_request_set_serial_number(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value)
 *   void ds3_request_set_write_protected(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_get_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_import_all_tapes_spectra_s3_request
 *
 *   void ds3_request_set_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value)
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_verify_data_prior_to_import(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_import_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_import_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_import_tape_spectra_s3_request
 *
 *   void ds3_request_set_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value)
 *   void ds3_request_set_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_user_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value)
 *   void ds3_request_set_verify_data_prior_to_import(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_import_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_import_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
/**
 * Optional Request Modifiers for ds3_init_inspect_all_tapes_spectra_s3_request
 *
 *   void ds3_request_set_task_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_inspect_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_inspect_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_inspect_tape_spectra_s3_request
 *
 *   void ds3_request_set_task_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_inspect_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_inspect_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_modify_all_tape_partitions_spectra_s3_request(const ds3_quiesced quiesced);
LIBRARY_API ds3_error* ds3_modify_all_tape_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_modify_tape_partition_spectra_s3_request
 *
 *   void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value)
 */
LIBRARY_API ds3_request* ds3_init_modify_tape_partition_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_tape_spectra_s3_request
 *
 *   void ds3_request_set_eject_label(const ds3_request* request, const char* value)
 *   void ds3_request_set_eject_location(const ds3_request* request, const char* value)
 *   void ds3_request_set_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value)
 */
LIBRARY_API ds3_request* ds3_init_modify_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_online_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_online_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_online_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_online_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_all_tapes_spectra_s3_request
 *
 *   void ds3_request_set_task_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_verify_all_tapes_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_verify_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_tape_spectra_s3_request
 *
 *   void ds3_request_set_task_priority_ds3_priority(const ds3_request* request, const ds3_priority value)
 */
LIBRARY_API ds3_request* ds3_init_verify_tape_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_verify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response);
LIBRARY_API ds3_request* ds3_init_put_ds3_target_read_preference_spectra_s3_request(const char* bucket_id, const ds3_target_read_preference_type read_preference, const char* target_id);
LIBRARY_API ds3_error* ds3_put_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** response);
LIBRARY_API ds3_request* ds3_init_delete_ds3_target_failure_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_ds3_target_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_ds3_target_read_preference_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_delete_ds3_target_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delete_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_force_target_environment_refresh_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_force_target_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_blobs_on_ds3_target_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_blobs_on_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_ds3_target_data_policies_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_ds3_target_data_policies_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_ds3_target_failures_spectra_s3_request
 *
 *   void ds3_request_set_error_message(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_target_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_type_ds3_target_failure_type(const ds3_request* request, const ds3_target_failure_type value)
 */
LIBRARY_API ds3_request* ds3_init_get_ds3_target_failures_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_ds3_target_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_ds3_target_read_preference_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_ds3_target_read_preferences_spectra_s3_request
 *
 *   void ds3_request_set_bucket_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value)
 *   void ds3_request_set_target_id(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_ds3_target_read_preferences_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_ds3_target_read_preferences_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_list_response** response);
LIBRARY_API ds3_request* ds3_init_get_ds3_target_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_ds3_targets_spectra_s3_request
 *
 *   void ds3_request_set_admin_auth_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_end_point(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_https(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_data_path_port(const ds3_request* request, const int value)
 *   void ds3_request_set_data_path_proxy(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_verify_certificate(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_default_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 *   void ds3_request_set_permit_going_out_of_sync(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value)
 *   void ds3_request_set_state_ds3_target_state(const ds3_request* request, const ds3_target_state value)
 */
LIBRARY_API ds3_request* ds3_init_get_ds3_targets_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_list_response** response);
LIBRARY_API ds3_request* ds3_init_modify_all_ds3_targets_spectra_s3_request(const ds3_quiesced quiesced);
LIBRARY_API ds3_error* ds3_modify_all_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_modify_ds3_target_spectra_s3_request
 *
 *   void ds3_request_set_access_control_replication_ds3_target_access_control_replication(const ds3_request* request, const ds3_target_access_control_replication value)
 *   void ds3_request_set_admin_auth_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_admin_secret_key(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_end_point(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_https(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_data_path_port(const ds3_request* request, const int value)
 *   void ds3_request_set_data_path_proxy(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_verify_certificate(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_default_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_permit_going_out_of_sync(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value)
 *   void ds3_request_set_replicated_user_default_data_policy(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_modify_ds3_target_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response);
/**
 * Optional Request Modifiers for ds3_init_pair_back_registered_ds3_target_spectra_s3_request
 *
 *   void ds3_request_set_access_control_replication_ds3_target_access_control_replication(const ds3_request* request, const ds3_target_access_control_replication value)
 *   void ds3_request_set_admin_auth_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_admin_secret_key(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_end_point(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_https(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_data_path_port(const ds3_request* request, const int value)
 *   void ds3_request_set_data_path_proxy(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_verify_certificate(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_default_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_permit_going_out_of_sync(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_replicated_user_default_data_policy(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_pair_back_registered_ds3_target_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_pair_back_registered_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request);
/**
 * Optional Request Modifiers for ds3_init_register_ds3_target_spectra_s3_request
 *
 *   void ds3_request_set_access_control_replication_ds3_target_access_control_replication(const ds3_request* request, const ds3_target_access_control_replication value)
 *   void ds3_request_set_data_path_https(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_data_path_port(const ds3_request* request, const int value)
 *   void ds3_request_set_data_path_proxy(const ds3_request* request, const char* value)
 *   void ds3_request_set_data_path_verify_certificate(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_default_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value)
 *   void ds3_request_set_permit_going_out_of_sync(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_replicated_user_default_data_policy(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_register_ds3_target_spectra_s3_request(const char* admin_auth_id, const char* admin_secret_key, const char* data_path_end_point, const char* name);
LIBRARY_API ds3_error* ds3_register_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response);
/**
 * Optional Request Modifiers for ds3_init_verify_ds3_target_spectra_s3_request
 *
 *   void ds3_request_set_full_details(const ds3_request* request, ds3_bool value)
 */
LIBRARY_API ds3_request* ds3_init_verify_ds3_target_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_verify_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response);
/**
 * Optional Request Modifiers for ds3_init_delegate_create_user_spectra_s3_request
 *
 *   void ds3_request_set_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_secret_key(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_delegate_create_user_spectra_s3_request(const char* name);
LIBRARY_API ds3_error* ds3_delegate_create_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response);
LIBRARY_API ds3_request* ds3_init_delegate_delete_user_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_delegate_delete_user_spectra_s3_request(const ds3_client* client, const ds3_request* request);
LIBRARY_API ds3_request* ds3_init_get_user_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_get_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response);
/**
 * Optional Request Modifiers for ds3_init_get_users_spectra_s3_request
 *
 *   void ds3_request_set_auth_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_default_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_last_page(const ds3_request* request, ds3_bool value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_page_length(const ds3_request* request, const int value)
 *   void ds3_request_set_page_offset(const ds3_request* request, const int value)
 *   void ds3_request_set_page_start_marker(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_get_users_spectra_s3_request(void);
LIBRARY_API ds3_error* ds3_get_users_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_list_response** response);
/**
 * Optional Request Modifiers for ds3_init_modify_user_spectra_s3_request
 *
 *   void ds3_request_set_default_data_policy_id(const ds3_request* request, const char* value)
 *   void ds3_request_set_name(const ds3_request* request, const char* value)
 *   void ds3_request_set_secret_key(const ds3_request* request, const char* value)
 */
LIBRARY_API ds3_request* ds3_init_modify_user_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_modify_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response);
LIBRARY_API ds3_request* ds3_init_regenerate_user_secret_key_spectra_s3_request(const char* resource_id);
LIBRARY_API ds3_error* ds3_regenerate_user_secret_key_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response);

LIBRARY_API void ds3_cleanup(void);

// provided helpers
LIBRARY_API size_t ds3_write_to_file(void* buffer, size_t size, size_t nmemb, void* user_data);
LIBRARY_API size_t ds3_read_from_file(void* buffer, size_t size, size_t nmemb, void* user_data);
LIBRARY_API size_t ds3_write_to_fd(void* buffer, size_t size, size_t nmemb, void* user_data);
LIBRARY_API size_t ds3_read_from_fd(void* buffer, size_t size, size_t nmemb, void* user_data);

LIBRARY_API ds3_bulk_object_list_response* ds3_convert_file_list(const char** file_list, uint64_t num_files);
LIBRARY_API ds3_bulk_object_list_response* ds3_convert_file_list_with_basepath(const char** file_list, uint64_t num_files, const char* base_path);
LIBRARY_API ds3_bulk_object_list_response* ds3_convert_object_list(const ds3_contents_response** objects, uint64_t num_objects);
LIBRARY_API ds3_bulk_object_list_response* ds3_init_bulk_object_list();


#ifdef __cplusplus
}
#endif
#endif

