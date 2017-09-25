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
#include <libxml/parser.h>
#include <inttypes.h>

#include "ds3.h"
#include "ds3_request.h"
#include "ds3_net.h"

#ifdef _WIN32
  #include <io.h>
  #ifndef PRIu64
    #define PRIu64 "I64u"
  #endif
#else
  #include <inttypes.h>
#endif

//The max size of an uint32_t is 10 characters + NULL
//The max size of an uint64_t is 20 characters + NULL
#define STRING_BUFFER_SIZE 32

static char* _get_ds3_bucket_acl_permission_str(ds3_bucket_acl_permission input) {
    if (input == DS3_BUCKET_ACL_PERMISSION_LIST) {
        return "LIST";
    } else if (input == DS3_BUCKET_ACL_PERMISSION_READ) {
        return "READ";
    } else if (input == DS3_BUCKET_ACL_PERMISSION_WRITE) {
        return "WRITE";
    } else if (input == DS3_BUCKET_ACL_PERMISSION_DELETE) {
        return "DELETE";
    } else if (input == DS3_BUCKET_ACL_PERMISSION_JOB) {
        return "JOB";
    } else if (input == DS3_BUCKET_ACL_PERMISSION_OWNER) {
        return "OWNER";
    } else {
        return "";
    }

}
static char* _get_ds3_pool_health_str(ds3_pool_health input) {
    if (input == DS3_POOL_HEALTH_OK) {
        return "OK";
    } else if (input == DS3_POOL_HEALTH_DEGRADED) {
        return "DEGRADED";
    } else {
        return "";
    }

}
static char* _get_ds3_pool_state_str(ds3_pool_state input) {
    if (input == DS3_POOL_STATE_BLANK) {
        return "BLANK";
    } else if (input == DS3_POOL_STATE_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_POOL_STATE_LOST) {
        return "LOST";
    } else if (input == DS3_POOL_STATE_FOREIGN) {
        return "FOREIGN";
    } else if (input == DS3_POOL_STATE_IMPORT_PENDING) {
        return "IMPORT_PENDING";
    } else if (input == DS3_POOL_STATE_IMPORT_IN_PROGRESS) {
        return "IMPORT_IN_PROGRESS";
    } else {
        return "";
    }

}
static char* _get_ds3_pool_type_str(ds3_pool_type input) {
    if (input == DS3_POOL_TYPE_NEARLINE) {
        return "NEARLINE";
    } else if (input == DS3_POOL_TYPE_ONLINE) {
        return "ONLINE";
    } else {
        return "";
    }

}
static char* _get_ds3_tape_state_str(ds3_tape_state input) {
    if (input == DS3_TAPE_STATE_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_TAPE_STATE_OFFLINE) {
        return "OFFLINE";
    } else if (input == DS3_TAPE_STATE_ONLINE_PENDING) {
        return "ONLINE_PENDING";
    } else if (input == DS3_TAPE_STATE_ONLINE_IN_PROGRESS) {
        return "ONLINE_IN_PROGRESS";
    } else if (input == DS3_TAPE_STATE_PENDING_INSPECTION) {
        return "PENDING_INSPECTION";
    } else if (input == DS3_TAPE_STATE_UNKNOWN) {
        return "UNKNOWN";
    } else if (input == DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE) {
        return "DATA_CHECKPOINT_FAILURE";
    } else if (input == DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY) {
        return "DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY";
    } else if (input == DS3_TAPE_STATE_DATA_CHECKPOINT_MISSING) {
        return "DATA_CHECKPOINT_MISSING";
    } else if (input == DS3_TAPE_STATE_LTFS_WITH_FOREIGN_DATA) {
        return "LTFS_WITH_FOREIGN_DATA";
    } else if (input == DS3_TAPE_STATE_RAW_IMPORT_PENDING) {
        return "RAW_IMPORT_PENDING";
    } else if (input == DS3_TAPE_STATE_RAW_IMPORT_IN_PROGRESS) {
        return "RAW_IMPORT_IN_PROGRESS";
    } else if (input == DS3_TAPE_STATE_FOREIGN) {
        return "FOREIGN";
    } else if (input == DS3_TAPE_STATE_IMPORT_PENDING) {
        return "IMPORT_PENDING";
    } else if (input == DS3_TAPE_STATE_IMPORT_IN_PROGRESS) {
        return "IMPORT_IN_PROGRESS";
    } else if (input == DS3_TAPE_STATE_INCOMPATIBLE) {
        return "INCOMPATIBLE";
    } else if (input == DS3_TAPE_STATE_LOST) {
        return "LOST";
    } else if (input == DS3_TAPE_STATE_BAD) {
        return "BAD";
    } else if (input == DS3_TAPE_STATE_CANNOT_FORMAT_DUE_TO_WRITE_PROTECTION) {
        return "CANNOT_FORMAT_DUE_TO_WRITE_PROTECTION";
    } else if (input == DS3_TAPE_STATE_SERIAL_NUMBER_MISMATCH) {
        return "SERIAL_NUMBER_MISMATCH";
    } else if (input == DS3_TAPE_STATE_BAR_CODE_MISSING) {
        return "BAR_CODE_MISSING";
    } else if (input == DS3_TAPE_STATE_FORMAT_PENDING) {
        return "FORMAT_PENDING";
    } else if (input == DS3_TAPE_STATE_FORMAT_IN_PROGRESS) {
        return "FORMAT_IN_PROGRESS";
    } else if (input == DS3_TAPE_STATE_EJECT_TO_EE_IN_PROGRESS) {
        return "EJECT_TO_EE_IN_PROGRESS";
    } else if (input == DS3_TAPE_STATE_EJECT_FROM_EE_PENDING) {
        return "EJECT_FROM_EE_PENDING";
    } else if (input == DS3_TAPE_STATE_EJECTED) {
        return "EJECTED";
    } else {
        return "";
    }

}
static char* _get_ds3_tape_type_str(ds3_tape_type input) {
    if (input == DS3_TAPE_TYPE_LTO5) {
        return "LTO5";
    } else if (input == DS3_TAPE_TYPE_LTO6) {
        return "LTO6";
    } else if (input == DS3_TAPE_TYPE_LTO7) {
        return "LTO7";
    } else if (input == DS3_TAPE_TYPE_LTO8) {
        return "LTO8";
    } else if (input == DS3_TAPE_TYPE_LTO_CLEANING_TAPE) {
        return "LTO_CLEANING_TAPE";
    } else if (input == DS3_TAPE_TYPE_TS_JC) {
        return "TS_JC";
    } else if (input == DS3_TAPE_TYPE_TS_JY) {
        return "TS_JY";
    } else if (input == DS3_TAPE_TYPE_TS_JK) {
        return "TS_JK";
    } else if (input == DS3_TAPE_TYPE_TS_JD) {
        return "TS_JD";
    } else if (input == DS3_TAPE_TYPE_TS_JZ) {
        return "TS_JZ";
    } else if (input == DS3_TAPE_TYPE_TS_JL) {
        return "TS_JL";
    } else if (input == DS3_TAPE_TYPE_TS_CLEANING_TAPE) {
        return "TS_CLEANING_TAPE";
    } else if (input == DS3_TAPE_TYPE_UNKNOWN) {
        return "UNKNOWN";
    } else if (input == DS3_TAPE_TYPE_FORBIDDEN) {
        return "FORBIDDEN";
    } else {
        return "";
    }

}
static char* _get_ds3_auto_inspect_mode_str(ds3_auto_inspect_mode input) {
    if (input == DS3_AUTO_INSPECT_MODE_NEVER) {
        return "NEVER";
    } else if (input == DS3_AUTO_INSPECT_MODE_MINIMAL) {
        return "MINIMAL";
    } else if (input == DS3_AUTO_INSPECT_MODE_FULL) {
        return "FULL";
    } else {
        return "";
    }

}
static char* _get_ds3_import_conflict_resolution_mode_str(ds3_import_conflict_resolution_mode input) {
    if (input == DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL) {
        return "CANCEL";
    } else if (input == DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_MOST_RECENT) {
        return "ACCEPT_MOST_RECENT";
    } else if (input == DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_EXISTING) {
        return "ACCEPT_EXISTING";
    } else {
        return "";
    }

}
static char* _get_ds3_priority_str(ds3_priority input) {
    if (input == DS3_PRIORITY_CRITICAL) {
        return "CRITICAL";
    } else if (input == DS3_PRIORITY_URGENT) {
        return "URGENT";
    } else if (input == DS3_PRIORITY_HIGH) {
        return "HIGH";
    } else if (input == DS3_PRIORITY_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_PRIORITY_LOW) {
        return "LOW";
    } else if (input == DS3_PRIORITY_BACKGROUND) {
        return "BACKGROUND";
    } else {
        return "";
    }

}
static char* _get_ds3_unavailable_media_usage_policy_str(ds3_unavailable_media_usage_policy input) {
    if (input == DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW) {
        return "ALLOW";
    } else if (input == DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISCOURAGED) {
        return "DISCOURAGED";
    } else if (input == DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISALLOW) {
        return "DISALLOW";
    } else {
        return "";
    }

}
static char* _get_ds3_data_replication_rule_type_str(ds3_data_replication_rule_type input) {
    if (input == DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT) {
        return "PERMANENT";
    } else if (input == DS3_DATA_REPLICATION_RULE_TYPE_RETIRED) {
        return "RETIRED";
    } else {
        return "";
    }

}
static char* _get_ds3_data_isolation_level_str(ds3_data_isolation_level input) {
    if (input == DS3_DATA_ISOLATION_LEVEL_STANDARD) {
        return "STANDARD";
    } else if (input == DS3_DATA_ISOLATION_LEVEL_BUCKET_ISOLATED) {
        return "BUCKET_ISOLATED";
    } else {
        return "";
    }

}
static char* _get_ds3_data_persistence_rule_type_str(ds3_data_persistence_rule_type input) {
    if (input == DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT) {
        return "PERMANENT";
    } else if (input == DS3_DATA_PERSISTENCE_RULE_TYPE_TEMPORARY) {
        return "TEMPORARY";
    } else if (input == DS3_DATA_PERSISTENCE_RULE_TYPE_RETIRED) {
        return "RETIRED";
    } else {
        return "";
    }

}
static char* _get_ds3_checksum_type_str(ds3_checksum_type input) {
    if (input == DS3_CHECKSUM_TYPE_CRC_32) {
        return "CRC_32";
    } else if (input == DS3_CHECKSUM_TYPE_CRC_32C) {
        return "CRC_32C";
    } else if (input == DS3_CHECKSUM_TYPE_MD5) {
        return "MD5";
    } else if (input == DS3_CHECKSUM_TYPE_SHA_256) {
        return "SHA_256";
    } else if (input == DS3_CHECKSUM_TYPE_SHA_512) {
        return "SHA_512";
    } else {
        return "";
    }

}
static char* _get_ds3_versioning_level_str(ds3_versioning_level input) {
    if (input == DS3_VERSIONING_LEVEL_NONE) {
        return "NONE";
    } else if (input == DS3_VERSIONING_LEVEL_KEEP_LATEST) {
        return "KEEP_LATEST";
    } else {
        return "";
    }

}
static char* _get_ds3_s3_initial_data_placement_policy_str(ds3_s3_initial_data_placement_policy input) {
    if (input == DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_STANDARD) {
        return "STANDARD";
    } else if (input == DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_REDUCED_REDUNDANCY) {
        return "REDUCED_REDUNDANCY";
    } else if (input == DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_STANDARD_IA) {
        return "STANDARD_IA";
    } else if (input == DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_GLACIER) {
        return "GLACIER";
    } else {
        return "";
    }

}
static char* _get_ds3_data_placement_rule_state_str(ds3_data_placement_rule_state input) {
    if (input == DS3_DATA_PLACEMENT_RULE_STATE_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_DATA_PLACEMENT_RULE_STATE_INCLUSION_IN_PROGRESS) {
        return "INCLUSION_IN_PROGRESS";
    } else {
        return "";
    }

}
static char* _get_ds3_job_request_type_str(ds3_job_request_type input) {
    if (input == DS3_JOB_REQUEST_TYPE_PUT) {
        return "PUT";
    } else if (input == DS3_JOB_REQUEST_TYPE_GET) {
        return "GET";
    } else if (input == DS3_JOB_REQUEST_TYPE_VERIFY) {
        return "VERIFY";
    } else {
        return "";
    }

}
static char* _get_ds3_job_chunk_client_processing_order_guarantee_str(ds3_job_chunk_client_processing_order_guarantee input) {
    if (input == DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE) {
        return "NONE";
    } else if (input == DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_IN_ORDER) {
        return "IN_ORDER";
    } else {
        return "";
    }

}
static char* _get_ds3_http_response_format_type_str(ds3_http_response_format_type input) {
    if (input == DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT) {
        return "DEFAULT";
    } else if (input == DS3_HTTP_RESPONSE_FORMAT_TYPE_JSON) {
        return "JSON";
    } else if (input == DS3_HTTP_RESPONSE_FORMAT_TYPE_XML) {
        return "XML";
    } else {
        return "";
    }

}
static char* _get_ds3_naming_convention_type_str(ds3_naming_convention_type input) {
    if (input == DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE) {
        return "CONCAT_LOWERCASE";
    } else if (input == DS3_NAMING_CONVENTION_TYPE_CONSTANT) {
        return "CONSTANT";
    } else if (input == DS3_NAMING_CONVENTION_TYPE_UNDERSCORED) {
        return "UNDERSCORED";
    } else if (input == DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_UPPERCASE) {
        return "CAMEL_CASE_WITH_FIRST_LETTER_UPPERCASE";
    } else if (input == DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_LOWERCASE) {
        return "CAMEL_CASE_WITH_FIRST_LETTER_LOWERCASE";
    } else {
        return "";
    }

}
static char* _get_ds3_request_type_str(ds3_request_type input) {
    if (input == DS3_REQUEST_TYPE_DELETE) {
        return "DELETE";
    } else if (input == DS3_REQUEST_TYPE_GET) {
        return "GET";
    } else if (input == DS3_REQUEST_TYPE_HEAD) {
        return "HEAD";
    } else if (input == DS3_REQUEST_TYPE_POST) {
        return "POST";
    } else if (input == DS3_REQUEST_TYPE_PUT) {
        return "PUT";
    } else {
        return "";
    }

}
static char* _get_ds3_s3_object_type_str(ds3_s3_object_type input) {
    if (input == DS3_S3_OBJECT_TYPE_DATA) {
        return "DATA";
    } else if (input == DS3_S3_OBJECT_TYPE_FOLDER) {
        return "FOLDER";
    } else {
        return "";
    }

}
static char* _get_ds3_pool_failure_type_str(ds3_pool_failure_type input) {
    if (input == DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED) {
        return "BLOB_READ_FAILED";
    } else if (input == DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE) {
        return "DATA_CHECKPOINT_FAILURE";
    } else if (input == DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_MISSING) {
        return "DATA_CHECKPOINT_MISSING";
    } else if (input == DS3_POOL_FAILURE_TYPE_FORMAT_FAILED) {
        return "FORMAT_FAILED";
    } else if (input == DS3_POOL_FAILURE_TYPE_IMPORT_FAILED) {
        return "IMPORT_FAILED";
    } else if (input == DS3_POOL_FAILURE_TYPE_IMPORT_INCOMPLETE) {
        return "IMPORT_INCOMPLETE";
    } else if (input == DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE) {
        return "IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE";
    } else if (input == DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY) {
        return "IMPORT_FAILED_DUE_TO_DATA_INTEGRITY";
    } else if (input == DS3_POOL_FAILURE_TYPE_INSPECT_FAILED) {
        return "INSPECT_FAILED";
    } else if (input == DS3_POOL_FAILURE_TYPE_QUIESCED) {
        return "QUIESCED";
    } else if (input == DS3_POOL_FAILURE_TYPE_READ_FAILED) {
        return "READ_FAILED";
    } else if (input == DS3_POOL_FAILURE_TYPE_VERIFY_FAILED) {
        return "VERIFY_FAILED";
    } else if (input == DS3_POOL_FAILURE_TYPE_WRITE_FAILED) {
        return "WRITE_FAILED";
    } else {
        return "";
    }

}
static char* _get_ds3_quiesced_str(ds3_quiesced input) {
    if (input == DS3_QUIESCED_NO) {
        return "NO";
    } else if (input == DS3_QUIESCED_PENDING) {
        return "PENDING";
    } else if (input == DS3_QUIESCED_YES) {
        return "YES";
    } else {
        return "";
    }

}
static char* _get_ds3_write_preference_level_str(ds3_write_preference_level input) {
    if (input == DS3_WRITE_PREFERENCE_LEVEL_HIGH) {
        return "HIGH";
    } else if (input == DS3_WRITE_PREFERENCE_LEVEL_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_WRITE_PREFERENCE_LEVEL_LOW) {
        return "LOW";
    } else if (input == DS3_WRITE_PREFERENCE_LEVEL_NEVER_SELECT) {
        return "NEVER_SELECT";
    } else {
        return "";
    }

}
static char* _get_ds3_ltfs_file_naming_mode_str(ds3_ltfs_file_naming_mode input) {
    if (input == DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME) {
        return "OBJECT_NAME";
    } else if (input == DS3_LTFS_FILE_NAMING_MODE_OBJECT_ID) {
        return "OBJECT_ID";
    } else {
        return "";
    }

}
static char* _get_ds3_write_optimization_str(ds3_write_optimization input) {
    if (input == DS3_WRITE_OPTIMIZATION_CAPACITY) {
        return "CAPACITY";
    } else if (input == DS3_WRITE_OPTIMIZATION_PERFORMANCE) {
        return "PERFORMANCE";
    } else {
        return "";
    }

}
static char* _get_ds3_storage_domain_failure_type_str(ds3_storage_domain_failure_type input) {
    if (input == DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED) {
        return "ILLEGAL_EJECTION_OCCURRED";
    } else if (input == DS3_STORAGE_DOMAIN_FAILURE_TYPE_MEMBER_BECAME_READ_ONLY) {
        return "MEMBER_BECAME_READ_ONLY";
    } else if (input == DS3_STORAGE_DOMAIN_FAILURE_TYPE_WRITES_STALLED_DUE_TO_NO_FREE_MEDIA_REMAINING) {
        return "WRITES_STALLED_DUE_TO_NO_FREE_MEDIA_REMAINING";
    } else {
        return "";
    }

}
static char* _get_ds3_storage_domain_member_state_str(ds3_storage_domain_member_state input) {
    if (input == DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_STORAGE_DOMAIN_MEMBER_STATE_EXCLUSION_IN_PROGRESS) {
        return "EXCLUSION_IN_PROGRESS";
    } else {
        return "";
    }

}
static char* _get_ds3_feature_key_type_str(ds3_feature_key_type input) {
    if (input == DS3_FEATURE_KEY_TYPE_AWS_S3_CLOUD_OUT) {
        return "AWS_S3_CLOUD_OUT";
    } else if (input == DS3_FEATURE_KEY_TYPE_MICROSOFT_AZURE_CLOUD_OUT) {
        return "MICROSOFT_AZURE_CLOUD_OUT";
    } else {
        return "";
    }

}
static char* _get_ds3_system_failure_type_str(ds3_system_failure_type input) {
    if (input == DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED) {
        return "RECONCILE_TAPE_ENVIRONMENT_FAILED";
    } else if (input == DS3_SYSTEM_FAILURE_TYPE_RECONCILE_POOL_ENVIRONMENT_FAILED) {
        return "RECONCILE_POOL_ENVIRONMENT_FAILED";
    } else if (input == DS3_SYSTEM_FAILURE_TYPE_CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION) {
        return "CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION";
    } else if (input == DS3_SYSTEM_FAILURE_TYPE_MICROSOFT_AZURE_WRITES_REQUIRE_FEATURE_LICENSE) {
        return "MICROSOFT_AZURE_WRITES_REQUIRE_FEATURE_LICENSE";
    } else if (input == DS3_SYSTEM_FAILURE_TYPE_AWS_S3_WRITES_REQUIRE_FEATURE_LICENSE) {
        return "AWS_S3_WRITES_REQUIRE_FEATURE_LICENSE";
    } else if (input == DS3_SYSTEM_FAILURE_TYPE_DATABASE_RUNNING_OUT_OF_SPACE) {
        return "DATABASE_RUNNING_OUT_OF_SPACE";
    } else {
        return "";
    }

}
static char* _get_ds3_tape_drive_type_str(ds3_tape_drive_type input) {
    if (input == DS3_TAPE_DRIVE_TYPE_UNKNOWN) {
        return "UNKNOWN";
    } else if (input == DS3_TAPE_DRIVE_TYPE_LTO5) {
        return "LTO5";
    } else if (input == DS3_TAPE_DRIVE_TYPE_LTO6) {
        return "LTO6";
    } else if (input == DS3_TAPE_DRIVE_TYPE_LTO7) {
        return "LTO7";
    } else if (input == DS3_TAPE_DRIVE_TYPE_LTO8) {
        return "LTO8";
    } else if (input == DS3_TAPE_DRIVE_TYPE_TS1140) {
        return "TS1140";
    } else if (input == DS3_TAPE_DRIVE_TYPE_TS1150) {
        return "TS1150";
    } else if (input == DS3_TAPE_DRIVE_TYPE_TS1155) {
        return "TS1155";
    } else {
        return "";
    }

}
static char* _get_ds3_tape_drive_state_str(ds3_tape_drive_state input) {
    if (input == DS3_TAPE_DRIVE_STATE_OFFLINE) {
        return "OFFLINE";
    } else if (input == DS3_TAPE_DRIVE_STATE_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_TAPE_DRIVE_STATE_ERROR) {
        return "ERROR";
    } else if (input == DS3_TAPE_DRIVE_STATE_NOT_COMPATIBLE_IN_PARTITION_DUE_TO_NEWER_TAPE_DRIVES) {
        return "NOT_COMPATIBLE_IN_PARTITION_DUE_TO_NEWER_TAPE_DRIVES";
    } else {
        return "";
    }

}
static char* _get_ds3_tape_failure_type_str(ds3_tape_failure_type input) {
    if (input == DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED) {
        return "BAR_CODE_CHANGED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_BAR_CODE_DUPLICATE) {
        return "BAR_CODE_DUPLICATE";
    } else if (input == DS3_TAPE_FAILURE_TYPE_BLOB_READ_FAILED) {
        return "BLOB_READ_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE) {
        return "DATA_CHECKPOINT_FAILURE";
    } else if (input == DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY) {
        return "DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY";
    } else if (input == DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_MISSING) {
        return "DATA_CHECKPOINT_MISSING";
    } else if (input == DS3_TAPE_FAILURE_TYPE_DELAYED_OWNERSHIP_FAILURE) {
        return "DELAYED_OWNERSHIP_FAILURE";
    } else if (input == DS3_TAPE_FAILURE_TYPE_DRIVE_CLEAN_FAILED) {
        return "DRIVE_CLEAN_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_DRIVE_CLEANED) {
        return "DRIVE_CLEANED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_FORMAT_FAILED) {
        return "FORMAT_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_GET_TAPE_INFORMATION_FAILED) {
        return "GET_TAPE_INFORMATION_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED) {
        return "IMPORT_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_IMPORT_INCOMPLETE) {
        return "IMPORT_INCOMPLETE";
    } else if (input == DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE) {
        return "IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE";
    } else if (input == DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY) {
        return "IMPORT_FAILED_DUE_TO_DATA_INTEGRITY";
    } else if (input == DS3_TAPE_FAILURE_TYPE_INSPECT_FAILED) {
        return "INSPECT_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_READ_FAILED) {
        return "READ_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_REIMPORT_REQUIRED) {
        return "REIMPORT_REQUIRED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_SERIAL_NUMBER_MISMATCH) {
        return "SERIAL_NUMBER_MISMATCH";
    } else if (input == DS3_TAPE_FAILURE_TYPE_VERIFY_FAILED) {
        return "VERIFY_FAILED";
    } else if (input == DS3_TAPE_FAILURE_TYPE_WRITE_FAILED) {
        return "WRITE_FAILED";
    } else {
        return "";
    }

}
static char* _get_ds3_tape_partition_failure_type_str(ds3_tape_partition_failure_type input) {
    if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED) {
        return "CLEANING_TAPE_REQUIRED";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_DUPLICATE_TAPE_BAR_CODES_DETECTED) {
        return "DUPLICATE_TAPE_BAR_CODES_DETECTED";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_EJECT_STALLED_DUE_TO_OFFLINE_TAPES) {
        return "EJECT_STALLED_DUE_TO_OFFLINE_TAPES";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_MINIMUM_DRIVE_COUNT_NOT_MET) {
        return "MINIMUM_DRIVE_COUNT_NOT_MET";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED) {
        return "MOVE_FAILED";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED_DUE_TO_PREPARE_TAPE_FOR_REMOVAL_FAILURE) {
        return "MOVE_FAILED_DUE_TO_PREPARE_TAPE_FOR_REMOVAL_FAILURE";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_NO_USABLE_DRIVES) {
        return "NO_USABLE_DRIVES";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_ONLINE_STALLED_DUE_TO_NO_STORAGE_SLOTS) {
        return "ONLINE_STALLED_DUE_TO_NO_STORAGE_SLOTS";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_IN_ERROR) {
        return "TAPE_DRIVE_IN_ERROR";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_MISSING) {
        return "TAPE_DRIVE_MISSING";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_QUIESCED) {
        return "TAPE_DRIVE_QUIESCED";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_TYPE_MISMATCH) {
        return "TAPE_DRIVE_TYPE_MISMATCH";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_EJECTION_BY_OPERATOR_REQUIRED) {
        return "TAPE_EJECTION_BY_OPERATOR_REQUIRED";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_MEDIA_TYPE_INCOMPATIBLE) {
        return "TAPE_MEDIA_TYPE_INCOMPATIBLE";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_REMOVAL_UNEXPECTED) {
        return "TAPE_REMOVAL_UNEXPECTED";
    } else {
        return "";
    }

}
static char* _get_ds3_import_export_configuration_str(ds3_import_export_configuration input) {
    if (input == DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED) {
        return "SUPPORTED";
    } else if (input == DS3_IMPORT_EXPORT_CONFIGURATION_NOT_SUPPORTED) {
        return "NOT_SUPPORTED";
    } else {
        return "";
    }

}
static char* _get_ds3_tape_partition_state_str(ds3_tape_partition_state input) {
    if (input == DS3_TAPE_PARTITION_STATE_ONLINE) {
        return "ONLINE";
    } else if (input == DS3_TAPE_PARTITION_STATE_OFFLINE) {
        return "OFFLINE";
    } else if (input == DS3_TAPE_PARTITION_STATE_ERROR) {
        return "ERROR";
    } else {
        return "";
    }

}
static char* _get_ds3_target_read_preference_type_str(ds3_target_read_preference_type input) {
    if (input == DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY) {
        return "MINIMUM_LATENCY";
    } else if (input == DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_ONLINE_POOL) {
        return "AFTER_ONLINE_POOL";
    } else if (input == DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NEARLINE_POOL) {
        return "AFTER_NEARLINE_POOL";
    } else if (input == DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NON_EJECTABLE_TAPE) {
        return "AFTER_NON_EJECTABLE_TAPE";
    } else if (input == DS3_TARGET_READ_PREFERENCE_TYPE_LAST_RESORT) {
        return "LAST_RESORT";
    } else if (input == DS3_TARGET_READ_PREFERENCE_TYPE_NEVER) {
        return "NEVER";
    } else {
        return "";
    }

}
static char* _get_ds3_target_failure_type_str(ds3_target_failure_type input) {
    if (input == DS3_TARGET_FAILURE_TYPE_IMPORT_FAILED) {
        return "IMPORT_FAILED";
    } else if (input == DS3_TARGET_FAILURE_TYPE_IMPORT_INCOMPLETE) {
        return "IMPORT_INCOMPLETE";
    } else if (input == DS3_TARGET_FAILURE_TYPE_NOT_ONLINE) {
        return "NOT_ONLINE";
    } else if (input == DS3_TARGET_FAILURE_TYPE_WRITE_FAILED) {
        return "WRITE_FAILED";
    } else if (input == DS3_TARGET_FAILURE_TYPE_WRITE_INITIATE_FAILED) {
        return "WRITE_INITIATE_FAILED";
    } else if (input == DS3_TARGET_FAILURE_TYPE_READ_FAILED) {
        return "READ_FAILED";
    } else if (input == DS3_TARGET_FAILURE_TYPE_READ_INITIATE_FAILED) {
        return "READ_INITIATE_FAILED";
    } else if (input == DS3_TARGET_FAILURE_TYPE_VERIFY_FAILED) {
        return "VERIFY_FAILED";
    } else {
        return "";
    }

}
static char* _get_ds3_target_state_str(ds3_target_state input) {
    if (input == DS3_TARGET_STATE_ONLINE) {
        return "ONLINE";
    } else if (input == DS3_TARGET_STATE_OFFLINE) {
        return "OFFLINE";
    } else {
        return "";
    }

}
static char* _get_ds3_target_access_control_replication_str(ds3_target_access_control_replication input) {
    if (input == DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE) {
        return "NONE";
    } else if (input == DS3_TARGET_ACCESS_CONTROL_REPLICATION_USERS) {
        return "USERS";
    } else {
        return "";
    }

}
static char* _get_ds3_s3_region_str(ds3_s3_region input) {
    if (input == DS3_S3_REGION_GOV_CLOUD) {
        return "GOV_CLOUD";
    } else if (input == DS3_S3_REGION_US_EAST_1) {
        return "US_EAST_1";
    } else if (input == DS3_S3_REGION_US_WEST_1) {
        return "US_WEST_1";
    } else if (input == DS3_S3_REGION_US_WEST_2) {
        return "US_WEST_2";
    } else if (input == DS3_S3_REGION_EU_WEST_1) {
        return "EU_WEST_1";
    } else if (input == DS3_S3_REGION_EU_CENTRAL_1) {
        return "EU_CENTRAL_1";
    } else if (input == DS3_S3_REGION_AP_SOUTH_1) {
        return "AP_SOUTH_1";
    } else if (input == DS3_S3_REGION_AP_SOUTHEAST_1) {
        return "AP_SOUTHEAST_1";
    } else if (input == DS3_S3_REGION_AP_SOUTHEAST_2) {
        return "AP_SOUTHEAST_2";
    } else if (input == DS3_S3_REGION_AP_NORTHEAST_1) {
        return "AP_NORTHEAST_1";
    } else if (input == DS3_S3_REGION_AP_NORTHEAST_2) {
        return "AP_NORTHEAST_2";
    } else if (input == DS3_S3_REGION_SA_EAST_1) {
        return "SA_EAST_1";
    } else if (input == DS3_S3_REGION_CN_NORTH_1) {
        return "CN_NORTH_1";
    } else {
        return "";
    }

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

static void _set_header(ds3_request* _request, const char* key, const char* value) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    _set_map_value(request->headers, key, value);
}

void ds3_request_set_custom_header(ds3_request* _request, const char* header_name, const char* header_value) {
   _set_header(_request, header_name, header_value);
}

void ds3_request_set_metadata(ds3_request* _request, const char* name, const char* value) {
    if ((value == NULL)
     || (strlen(value) == 0)) {
        fprintf(stderr, "Ignoring metadata key \"%s\" which has a NULL or empty value.\n", name);
        return;
    }

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
    if (header_value != NULL) {
        range_value = g_strdup_printf("%s,%ld-%ld", (char*)header_value, rangeStart, rangeEnd);
    } else {
        range_value = g_strdup_printf("bytes=%ld-%ld", rangeStart, rangeEnd);
    }

    _set_header(_request, "Range", range_value);
    g_free(range_value);
}

void ds3_request_set_md5(ds3_request* _request, const char* md5) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_CHECKSUM_TYPE_MD5;
    request->checksum = ds3_str_init(md5);
}

void ds3_request_set_sha256(ds3_request* _request, const char* sha256) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_CHECKSUM_TYPE_SHA_256;
    request->checksum = ds3_str_init(sha256);
}

void ds3_request_set_sha512(ds3_request* _request, const char* sha512) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_CHECKSUM_TYPE_SHA_512;
    request->checksum = ds3_str_init(sha512);
}

void ds3_request_set_crc32(ds3_request* _request, const char* crc32) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_CHECKSUM_TYPE_CRC_32;
    request->checksum = ds3_str_init(crc32);
}

void ds3_request_set_crc32c(ds3_request* _request, const char* crc32c) {
    struct _ds3_request* request = (struct _ds3_request*) _request;
    request->checksum_type = DS3_CHECKSUM_TYPE_CRC_32C;
    request->checksum = ds3_str_init(crc32c);
}

static void _set_query_param(const ds3_request* _request, const char* key, const char* value) {
    const struct _ds3_request* request = (const struct _ds3_request*) _request;
    _set_map_value(request->query_params, key, value);
}

static void _set_query_param_flag(const ds3_request* _request, const char* key, ds3_bool value) {
    if (value == False) {
        g_hash_table_remove(_request->headers, key);
    } else {
        _set_query_param(_request, key, NULL);
    }
}

static void _set_query_param_uint64_t(const ds3_request* _request, const char* key, uint64_t value) {
    char string_buffer[STRING_BUFFER_SIZE];
    memset(string_buffer, 0, sizeof(string_buffer));
    g_snprintf(string_buffer, sizeof(string_buffer), "%" PRIu64, value);
    _set_query_param(_request, key, string_buffer);
}

static void _set_query_param_int(const ds3_request* _request, const char* key, int value) {
    char string_buffer[STRING_BUFFER_SIZE];
    memset(string_buffer, 0, sizeof(string_buffer));
    g_snprintf(string_buffer, sizeof(string_buffer), "%d", value);
    _set_query_param(_request, key, string_buffer);
}

static void _set_query_param_float(const ds3_request* _request, const char* key, float value) {
    char string_buffer[STRING_BUFFER_SIZE];
    memset(string_buffer, 0, sizeof(string_buffer));
    g_snprintf(string_buffer, sizeof(string_buffer), "%f", value);
    _set_query_param(_request, key, string_buffer);
}

void ds3_request_set_access_control_replication_ds3_target_access_control_replication(const ds3_request* request, const ds3_target_access_control_replication value) {
    _set_query_param(request, "access_control_replication", (const char*)_get_ds3_target_access_control_replication_str(value));

}
void ds3_request_set_access_key(const ds3_request* request, const char * const value) {
    _set_query_param(request, "access_key", value);

}
void ds3_request_set_account_key(const ds3_request* request, const char * const value) {
    _set_query_param(request, "account_key", value);

}
void ds3_request_set_account_name(const ds3_request* request, const char * const value) {
    _set_query_param(request, "account_name", value);

}
void ds3_request_set_activated(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "activated", value);

}
void ds3_request_set_admin_auth_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "admin_auth_id", value);

}
void ds3_request_set_admin_secret_key(const ds3_request* request, const char * const value) {
    _set_query_param(request, "admin_secret_key", value);

}
void ds3_request_set_aggregating(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "aggregating", value);

}
void ds3_request_set_always_force_put_job_creation(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "always_force_put_job_creation", value);

}
void ds3_request_set_always_minimize_spanning_across_media(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "always_minimize_spanning_across_media", value);

}
void ds3_request_set_assigned_to_storage_domain(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "assigned_to_storage_domain", value);

}
void ds3_request_set_auth_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "auth_id", value);

}
void ds3_request_set_auto_activate_timeout_in_mins(const ds3_request* request, const int value) {
    _set_query_param_int(request, "auto_activate_timeout_in_mins", value);

}
void ds3_request_set_auto_eject_media_full_threshold(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "auto_eject_media_full_threshold", value);

}
void ds3_request_set_auto_eject_upon_cron(const ds3_request* request, const char * const value) {
    _set_query_param(request, "auto_eject_upon_cron", value);

}
void ds3_request_set_auto_eject_upon_job_cancellation(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "auto_eject_upon_job_cancellation", value);

}
void ds3_request_set_auto_eject_upon_job_completion(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "auto_eject_upon_job_completion", value);

}
void ds3_request_set_auto_eject_upon_media_full(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "auto_eject_upon_media_full", value);

}
void ds3_request_set_auto_inspect_ds3_auto_inspect_mode(const ds3_request* request, const ds3_auto_inspect_mode value) {
    _set_query_param(request, "auto_inspect", (const char*)_get_ds3_auto_inspect_mode_str(value));

}
void ds3_request_set_auto_reclaim_initiate_threshold(const ds3_request* request, const float value) {
    _set_query_param_float(request, "auto_reclaim_initiate_threshold", value);

}
void ds3_request_set_auto_reclaim_terminate_threshold(const ds3_request* request, const float value) {
    _set_query_param_float(request, "auto_reclaim_terminate_threshold", value);

}
void ds3_request_set_auto_verify_frequency_in_days(const ds3_request* request, const int value) {
    _set_query_param_int(request, "auto_verify_frequency_in_days", value);

}
void ds3_request_set_available_raw_capacity(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "available_raw_capacity", value);

}
void ds3_request_set_bar_code(const ds3_request* request, const char * const value) {
    _set_query_param(request, "bar_code", value);

}
void ds3_request_set_blob_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "blob_id", value);

}
void ds3_request_set_blobbing_enabled(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "blobbing_enabled", value);

}
void ds3_request_set_bucket_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "bucket_id", value);

}
void ds3_request_set_built_in(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "built_in", value);

}
void ds3_request_set_burst_threshold(const ds3_request* request, const float value) {
    _set_query_param_float(request, "burst_threshold", value);

}
void ds3_request_set_canceled_due_to_timeout(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "canceled_due_to_timeout", value);

}
void ds3_request_set_checksum_type_ds3_checksum_type(const ds3_request* request, const ds3_checksum_type value) {
    _set_query_param(request, "checksum_type", (const char*)_get_ds3_checksum_type_str(value));

}
void ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(const ds3_request* request, const ds3_job_chunk_client_processing_order_guarantee value) {
    _set_query_param(request, "chunk_client_processing_order_guarantee", (const char*)_get_ds3_job_chunk_client_processing_order_guarantee_str(value));

}
void ds3_request_set_cloud_bucket_prefix(const ds3_request* request, const char * const value) {
    _set_query_param(request, "cloud_bucket_prefix", value);

}
void ds3_request_set_cloud_bucket_suffix(const ds3_request* request, const char * const value) {
    _set_query_param(request, "cloud_bucket_suffix", value);

}
void ds3_request_set_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value) {
    _set_query_param(request, "conflict_resolution_mode", (const char*)_get_ds3_import_conflict_resolution_mode_str(value));

}
void ds3_request_set_created_at(const ds3_request* request, const char * const value) {
    _set_query_param(request, "created_at", value);

}
void ds3_request_set_data_path_end_point(const ds3_request* request, const char * const value) {
    _set_query_param(request, "data_path_end_point", value);

}
void ds3_request_set_data_path_https(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "data_path_https", value);

}
void ds3_request_set_data_path_port(const ds3_request* request, const int value) {
    _set_query_param_int(request, "data_path_port", value);

}
void ds3_request_set_data_path_proxy(const ds3_request* request, const char * const value) {
    _set_query_param(request, "data_path_proxy", value);

}
void ds3_request_set_data_path_verify_certificate(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "data_path_verify_certificate", value);

}
void ds3_request_set_data_policy_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "data_policy_id", value);

}
void ds3_request_set_default_blob_size(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "default_blob_size", value);

}
void ds3_request_set_default_data_policy_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "default_data_policy_id", value);

}
void ds3_request_set_default_get_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "default_get_job_priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_default_import_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value) {
    _set_query_param(request, "default_import_conflict_resolution_mode", (const char*)_get_ds3_import_conflict_resolution_mode_str(value));

}
void ds3_request_set_default_put_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "default_put_job_priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_default_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value) {
    _set_query_param(request, "default_read_preference", (const char*)_get_ds3_target_read_preference_type_str(value));

}
void ds3_request_set_default_verify_after_write(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "default_verify_after_write", value);

}
void ds3_request_set_default_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "default_verify_data_after_import", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_default_verify_data_prior_to_import(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "default_verify_data_prior_to_import", value);

}
void ds3_request_set_default_verify_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "default_verify_job_priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_delimiter(const ds3_request* request, const char * const value) {
    _set_query_param(request, "delimiter", value);

}
void ds3_request_set_density_ds3_tape_drive_type(const ds3_request* request, const ds3_tape_drive_type value) {
    _set_query_param(request, "density", (const char*)_get_ds3_tape_drive_type_str(value));

}
void ds3_request_set_dns_name(const ds3_request* request, const char * const value) {
    _set_query_param(request, "dns_name", value);

}
void ds3_request_set_ds3_replication_rule_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "ds3_replication_rule_id", value);

}
void ds3_request_set_eject_label(const ds3_request* request, const char * const value) {
    _set_query_param(request, "eject_label", value);

}
void ds3_request_set_eject_location(const ds3_request* request, const char * const value) {
    _set_query_param(request, "eject_location", value);

}
void ds3_request_set_end_to_end_crc_required(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "end_to_end_crc_required", value);

}
void ds3_request_set_error_message(const ds3_request* request, const char * const value) {
    _set_query_param(request, "error_message", value);

}
void ds3_request_set_expiration_date(const ds3_request* request, const char * const value) {
    _set_query_param(request, "expiration_date", value);

}
void ds3_request_set_force(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "force", value);

}
void ds3_request_set_format_ds3_http_response_format_type(const ds3_request* request, const ds3_http_response_format_type value) {
    _set_query_param(request, "format", (const char*)_get_ds3_http_response_format_type_str(value));

}
void ds3_request_set_full_details(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "full_details", value);

}
void ds3_request_set_full_of_data(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "full_of_data", value);

}
void ds3_request_set_group_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "group_id", value);

}
void ds3_request_set_guid(const ds3_request* request, const char * const value) {
    _set_query_param(request, "guid", value);

}
void ds3_request_set_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value) {
    _set_query_param(request, "health", (const char*)_get_ds3_pool_health_str(value));

}
void ds3_request_set_https(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "https", value);

}
void ds3_request_set_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "id", value);

}
void ds3_request_set_ignore_naming_conflicts(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "ignore_naming_conflicts", value);

}
void ds3_request_set_implicit_job_id_resolution(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "implicit_job_id_resolution", value);

}
void ds3_request_set_import_export_configuration_ds3_import_export_configuration(const ds3_request* request, const ds3_import_export_configuration value) {
    _set_query_param(request, "import_export_configuration", (const char*)_get_ds3_import_export_configuration_str(value));

}
void ds3_request_set_include_physical_placement(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "include_physical_placement", value);

}
void ds3_request_set_initial_data_placement_ds3_s3_initial_data_placement_policy(const ds3_request* request, const ds3_s3_initial_data_placement_policy value) {
    _set_query_param(request, "initial_data_placement", (const char*)_get_ds3_s3_initial_data_placement_policy_str(value));

}
void ds3_request_set_isolation_level_ds3_data_isolation_level(const ds3_request* request, const ds3_data_isolation_level value) {
    _set_query_param(request, "isolation_level", (const char*)_get_ds3_data_isolation_level_str(value));

}
void ds3_request_set_job(const ds3_request* request, const char * const value) {
    _set_query_param(request, "job", value);

}
void ds3_request_set_job_chunk(const ds3_request* request, const char * const value) {
    _set_query_param(request, "job_chunk", value);

}
void ds3_request_set_job_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "job_id", value);

}
void ds3_request_set_key_ds3_feature_key_type(const ds3_request* request, const ds3_feature_key_type value) {
    _set_query_param(request, "key", (const char*)_get_ds3_feature_key_type_str(value));

}
void ds3_request_set_key_marker(const ds3_request* request, const char * const value) {
    _set_query_param(request, "key_marker", value);

}
void ds3_request_set_last_page(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "last_page", value);

}
void ds3_request_set_last_verified(const ds3_request* request, const char * const value) {
    _set_query_param(request, "last_verified", value);

}
void ds3_request_set_latest(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "latest", value);

}
void ds3_request_set_library_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "library_id", value);

}
void ds3_request_set_ltfs_file_naming_ds3_ltfs_file_naming_mode(const ds3_request* request, const ds3_ltfs_file_naming_mode value) {
    _set_query_param(request, "ltfs_file_naming", (const char*)_get_ds3_ltfs_file_naming_mode_str(value));

}
void ds3_request_set_management_url(const ds3_request* request, const char * const value) {
    _set_query_param(request, "management_url", value);

}
void ds3_request_set_marker(const ds3_request* request, const char * const value) {
    _set_query_param(request, "marker", value);

}
void ds3_request_set_max_blob_part_size_in_bytes(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "max_blob_part_size_in_bytes", value);

}
void ds3_request_set_max_capacity_in_bytes(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "max_capacity_in_bytes", value);

}
void ds3_request_set_max_keys(const ds3_request* request, const int value) {
    _set_query_param_int(request, "max_keys", value);

}
void ds3_request_set_max_parts(const ds3_request* request, const int value) {
    _set_query_param_int(request, "max_parts", value);

}
void ds3_request_set_max_tape_fragmentation_percent(const ds3_request* request, const int value) {
    _set_query_param_int(request, "max_tape_fragmentation_percent", value);

}
void ds3_request_set_max_upload_size(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "max_upload_size", value);

}
void ds3_request_set_max_uploads(const ds3_request* request, const int value) {
    _set_query_param_int(request, "max_uploads", value);

}
void ds3_request_set_maximum_auto_verification_frequency_in_days(const ds3_request* request, const int value) {
    _set_query_param_int(request, "maximum_auto_verification_frequency_in_days", value);

}
void ds3_request_set_media_ejection_allowed(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "media_ejection_allowed", value);

}
void ds3_request_set_member_group_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "member_group_id", value);

}
void ds3_request_set_member_user_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "member_user_id", value);

}
void ds3_request_set_minimize_spanning_across_media(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "minimize_spanning_across_media", value);

}
void ds3_request_set_minimum_days_to_retain(const ds3_request* request, const int value) {
    _set_query_param_int(request, "minimum_days_to_retain", value);

}
void ds3_request_set_name(const ds3_request* request, const char * const value) {
    _set_query_param(request, "name", value);

}
void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value) {
    _set_query_param(request, "naming_convention", (const char*)_get_ds3_naming_convention_type_str(value));

}
void ds3_request_set_node_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "node_id", value);

}
void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value) {
    _set_query_param(request, "notification_http_method", (const char*)_get_ds3_request_type_str(value));

}
void ds3_request_set_offline_data_staging_window_in_tb(const ds3_request* request, const int value) {
    _set_query_param_int(request, "offline_data_staging_window_in_tb", value);

}
void ds3_request_set_offset(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "offset", value);

}
void ds3_request_set_page_length(const ds3_request* request, const int value) {
    _set_query_param_int(request, "page_length", value);

}
void ds3_request_set_page_offset(const ds3_request* request, const int value) {
    _set_query_param_int(request, "page_offset", value);

}
void ds3_request_set_page_start_marker(const ds3_request* request, const char * const value) {
    _set_query_param(request, "page_start_marker", value);

}
void ds3_request_set_part_number_marker(const ds3_request* request, const int value) {
    _set_query_param_int(request, "part_number_marker", value);

}
void ds3_request_set_partially_verified_end_of_tape(const ds3_request* request, const char * const value) {
    _set_query_param(request, "partially_verified_end_of_tape", value);

}
void ds3_request_set_partially_verify_last_percent_of_tapes(const ds3_request* request, const int value) {
    _set_query_param_int(request, "partially_verify_last_percent_of_tapes", value);

}
void ds3_request_set_partition_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "partition_id", value);

}
void ds3_request_set_permission_ds3_bucket_acl_permission(const ds3_request* request, const ds3_bucket_acl_permission value) {
    _set_query_param(request, "permission", (const char*)_get_ds3_bucket_acl_permission_str(value));

}
void ds3_request_set_permit_going_out_of_sync(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "permit_going_out_of_sync", value);

}
void ds3_request_set_persistence_rule_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "persistence_rule_id", value);

}
void ds3_request_set_pool_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value) {
    _set_query_param(request, "pool_health", (const char*)_get_ds3_pool_health_str(value));

}
void ds3_request_set_pool_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "pool_id", value);

}
void ds3_request_set_pool_partition_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "pool_partition_id", value);

}
void ds3_request_set_pool_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value) {
    _set_query_param(request, "pool_state", (const char*)_get_ds3_pool_state_str(value));

}
void ds3_request_set_pool_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value) {
    _set_query_param(request, "pool_type", (const char*)_get_ds3_pool_type_str(value));

}
void ds3_request_set_powered_on(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "powered_on", value);

}
void ds3_request_set_preferred_number_of_chunks(const ds3_request* request, const int value) {
    _set_query_param_int(request, "preferred_number_of_chunks", value);

}
void ds3_request_set_prefix(const ds3_request* request, const char * const value) {
    _set_query_param(request, "prefix", value);

}
void ds3_request_set_previous_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value) {
    _set_query_param(request, "previous_state", (const char*)_get_ds3_tape_state_str(value));

}
void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_proxy_domain(const ds3_request* request, const char * const value) {
    _set_query_param(request, "proxy_domain", value);

}
void ds3_request_set_proxy_host(const ds3_request* request, const char * const value) {
    _set_query_param(request, "proxy_host", value);

}
void ds3_request_set_proxy_password(const ds3_request* request, const char * const value) {
    _set_query_param(request, "proxy_password", value);

}
void ds3_request_set_proxy_port(const ds3_request* request, const int value) {
    _set_query_param_int(request, "proxy_port", value);

}
void ds3_request_set_proxy_username(const ds3_request* request, const char * const value) {
    _set_query_param(request, "proxy_username", value);

}
void ds3_request_set_quiesced_ds3_quiesced(const ds3_request* request, const ds3_quiesced value) {
    _set_query_param(request, "quiesced", (const char*)_get_ds3_quiesced_str(value));

}
void ds3_request_set_read_preference_ds3_target_read_preference_type(const ds3_request* request, const ds3_target_read_preference_type value) {
    _set_query_param(request, "read_preference", (const char*)_get_ds3_target_read_preference_type_str(value));

}
void ds3_request_set_rebuild_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "rebuild_priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_rechunked(const ds3_request* request, const char * const value) {
    _set_query_param(request, "rechunked", value);

}
void ds3_request_set_region_ds3_s3_region(const ds3_request* request, const ds3_s3_region value) {
    _set_query_param(request, "region", (const char*)_get_ds3_s3_region_str(value));

}
void ds3_request_set_replicate_deletes(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "replicate_deletes", value);

}
void ds3_request_set_replicated_user_default_data_policy(const ds3_request* request, const char * const value) {
    _set_query_param(request, "replicated_user_default_data_policy", value);

}
void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value) {
    _set_query_param(request, "request_type", (const char*)_get_ds3_job_request_type_str(value));

}
void ds3_request_set_roll_back(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "roll_back", value);

}
void ds3_request_set_secret_key(const ds3_request* request, const char * const value) {
    _set_query_param(request, "secret_key", value);

}
void ds3_request_set_secure_media_allocation(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "secure_media_allocation", value);

}
void ds3_request_set_serial_number(const ds3_request* request, const char * const value) {
    _set_query_param(request, "serial_number", value);

}
void ds3_request_set_sort_by(const ds3_request* request, const char * const value) {
    _set_query_param(request, "sort_by", value);

}
void ds3_request_set_staged_data_expiration_in_days(const ds3_request* request, const int value) {
    _set_query_param_int(request, "staged_data_expiration_in_days", value);

}
void ds3_request_set_state_ds3_data_placement_rule_state(const ds3_request* request, const ds3_data_placement_rule_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_data_placement_rule_state_str(value));

}
void ds3_request_set_state_ds3_pool_state(const ds3_request* request, const ds3_pool_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_pool_state_str(value));

}
void ds3_request_set_state_ds3_storage_domain_member_state(const ds3_request* request, const ds3_storage_domain_member_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_storage_domain_member_state_str(value));

}
void ds3_request_set_state_ds3_tape_drive_state(const ds3_request* request, const ds3_tape_drive_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_tape_drive_state_str(value));

}
void ds3_request_set_state_ds3_tape_partition_state(const ds3_request* request, const ds3_tape_partition_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_tape_partition_state_str(value));

}
void ds3_request_set_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_tape_state_str(value));

}
void ds3_request_set_state_ds3_target_state(const ds3_request* request, const ds3_target_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_target_state_str(value));

}
void ds3_request_set_storage_domain_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "storage_domain_id", value);

}
void ds3_request_set_tape_drive_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "tape_drive_id", value);

}
void ds3_request_set_tape_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "tape_id", value);

}
void ds3_request_set_tape_partition_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "tape_partition_id", value);

}
void ds3_request_set_tape_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value) {
    _set_query_param(request, "tape_state", (const char*)_get_ds3_tape_state_str(value));

}
void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value) {
    _set_query_param(request, "tape_type", (const char*)_get_ds3_tape_type_str(value));

}
void ds3_request_set_target_data_policy(const ds3_request* request, const char * const value) {
    _set_query_param(request, "target_data_policy", value);

}
void ds3_request_set_target_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "target_id", value);

}
void ds3_request_set_task_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "task_priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_truncated(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "truncated", value);

}
void ds3_request_set_type_ds3_data_replication_rule_type(const ds3_request* request, const ds3_data_replication_rule_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_data_replication_rule_type_str(value));

}
void ds3_request_set_type_ds3_data_persistence_rule_type(const ds3_request* request, const ds3_data_persistence_rule_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_data_persistence_rule_type_str(value));

}
void ds3_request_set_type_ds3_s3_object_type(const ds3_request* request, const ds3_s3_object_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_s3_object_type_str(value));

}
void ds3_request_set_type_ds3_pool_failure_type(const ds3_request* request, const ds3_pool_failure_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_pool_failure_type_str(value));

}
void ds3_request_set_type_ds3_pool_type(const ds3_request* request, const ds3_pool_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_pool_type_str(value));

}
void ds3_request_set_type_ds3_storage_domain_failure_type(const ds3_request* request, const ds3_storage_domain_failure_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_storage_domain_failure_type_str(value));

}
void ds3_request_set_type_ds3_system_failure_type(const ds3_request* request, const ds3_system_failure_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_system_failure_type_str(value));

}
void ds3_request_set_type_ds3_tape_drive_type(const ds3_request* request, const ds3_tape_drive_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_tape_drive_type_str(value));

}
void ds3_request_set_type_ds3_tape_failure_type(const ds3_request* request, const ds3_tape_failure_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_tape_failure_type_str(value));

}
void ds3_request_set_type_ds3_tape_partition_failure_type(const ds3_request* request, const ds3_tape_partition_failure_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_tape_partition_failure_type_str(value));

}
void ds3_request_set_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_tape_type_str(value));

}
void ds3_request_set_type_ds3_target_failure_type(const ds3_request* request, const ds3_target_failure_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_target_failure_type_str(value));

}
void ds3_request_set_unavailable_media_policy_ds3_unavailable_media_usage_policy(const ds3_request* request, const ds3_unavailable_media_usage_policy value) {
    _set_query_param(request, "unavailable_media_policy", (const char*)_get_ds3_unavailable_media_usage_policy_str(value));

}
void ds3_request_set_unavailable_pool_max_job_retry_in_mins(const ds3_request* request, const int value) {
    _set_query_param_int(request, "unavailable_pool_max_job_retry_in_mins", value);

}
void ds3_request_set_unavailable_tape_partition_max_job_retry_in_mins(const ds3_request* request, const int value) {
    _set_query_param_int(request, "unavailable_tape_partition_max_job_retry_in_mins", value);

}
void ds3_request_set_upload_id_marker(const ds3_request* request, const char * const value) {
    _set_query_param(request, "upload_id_marker", value);

}
void ds3_request_set_user_id(const ds3_request* request, const char * const value) {
    _set_query_param(request, "user_id", value);

}
void ds3_request_set_verify_after_write(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "verify_after_write", value);

}
void ds3_request_set_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "verify_data_after_import", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_verify_data_prior_to_import(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "verify_data_prior_to_import", value);

}
void ds3_request_set_verify_pending_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "verify_pending", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_verify_prior_to_auto_eject_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "verify_prior_to_auto_eject", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_version(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "version", value);

}
void ds3_request_set_versioning_ds3_versioning_level(const ds3_request* request, const ds3_versioning_level value) {
    _set_query_param(request, "versioning", (const char*)_get_ds3_versioning_level_str(value));

}
void ds3_request_set_write_optimization_ds3_write_optimization(const ds3_request* request, const ds3_write_optimization value) {
    _set_query_param(request, "write_optimization", (const char*)_get_ds3_write_optimization_str(value));

}
void ds3_request_set_write_preference_ds3_write_preference_level(const ds3_request* request, const ds3_write_preference_level value) {
    _set_query_param(request, "write_preference", (const char*)_get_ds3_write_preference_level_str(value));

}
void ds3_request_set_write_protected(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "write_protected", value);

}


static void _cleanup_hash_value(gpointer value) {
    g_free(value);
}

static GHashTable* _create_hash_table(void) {
    GHashTable* hash =  g_hash_table_new_full(g_str_hash, g_str_equal, _cleanup_hash_value, _cleanup_hash_value);
    return hash;
}

static struct _ds3_request* _common_request_init(http_verb verb, ds3_str* path) {
    struct _ds3_request* request = g_new0(struct _ds3_request, 1);
    request->headers = _create_hash_table();
    request->query_params = _create_hash_table();
    request->verb = verb;
    request->path = path;
    return request;
}

static ds3_str* _build_path(const char *const path_prefix, const char *const bucket_name, const char *const object_name) {
    ds3_str* buildPathArgs = NULL;
    char* escaped_bucket_name = NULL;
    char* escaped_object_name = NULL;
    char* joined_path = NULL;
    char* full_path = NULL;

    if (bucket_name != NULL) {
        if (g_str_has_suffix(bucket_name, "/") == (gboolean)TRUE) {
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

    buildPathArgs = ds3_str_init(full_path);
    g_free(full_path);

    if (escaped_bucket_name != NULL) {
        g_free(escaped_bucket_name);
    }
    if (escaped_object_name != NULL) {
        g_free(escaped_object_name);
    }

    return buildPathArgs;
}

ds3_request* ds3_init_abort_multi_part_upload_request(const char *const bucket_name, const char *const object_name, const char* upload_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, object_name));
    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_complete_multi_part_upload_request(const char *const bucket_name, const char *const object_name, const char* upload_id, const ds3_complete_multipart_upload_response* mpu_list) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/", bucket_name, object_name));
    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    request->mpu_list = (ds3_complete_multipart_upload_response*) mpu_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_bucket_request(const char *const bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_multi_part_upload_part_request(const char *const bucket_name, const char *const object_name, const int part_number, const char* upload_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, object_name));
    char tmp_buff[32];
    sprintf(tmp_buff, "%d", part_number);
    _set_query_param((ds3_request*) request, "part_number", tmp_buff);

    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_object_request(const char *const bucket_name, const char *const object_name, const uint64_t length) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, object_name));
    request->length = length;

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_bucket_request(const char *const bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_object_request(const char *const bucket_name, const char *const object_name) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, object_name));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_objects_request(const char *const bucket_name, const ds3_delete_objects_response* objects_list) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "delete", NULL);

    request->delete_objects = (ds3_delete_objects_response*) objects_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_request(const char *const bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_service_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_request(const char *const bucket_name, const char *const object_name, const uint64_t length) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, object_name));
    request->length = length;

    return (ds3_request*) request;
}
ds3_request* ds3_init_head_bucket_request(const char *const bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_HEAD, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_head_object_request(const char *const bucket_name, const char *const object_name) {
    struct _ds3_request* request = _common_request_init(HTTP_HEAD, _build_path("/", bucket_name, object_name));
    return (ds3_request*) request;
}
ds3_request* ds3_init_initiate_multi_part_upload_request(const char *const bucket_name, const char *const object_name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/", bucket_name, object_name));
    _set_query_param((ds3_request*) request, "uploads", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_list_multi_part_upload_parts_request(const char *const bucket_name, const char *const object_name, const char* upload_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, object_name));
    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_list_multi_part_uploads_request(const char *const bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "uploads", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_bucket_acl_for_group_spectra_s3_request(const char* bucket_id, const char* group_id, const ds3_bucket_acl_permission permission) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/bucket_acl/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    if (group_id != NULL) {
        _set_query_param((ds3_request*) request, "group_id", group_id);
    }
    _set_query_param((ds3_request*) request, "permission", _get_ds3_bucket_acl_permission_str(permission));

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_bucket_acl_for_user_spectra_s3_request(const char* bucket_id, const ds3_bucket_acl_permission permission, const char* user_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/bucket_acl/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "permission", _get_ds3_bucket_acl_permission_str(permission));

    if (user_id != NULL) {
        _set_query_param((ds3_request*) request, "user_id", user_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_data_policy_acl_for_group_spectra_s3_request(const char* data_policy_id, const char* group_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/data_policy_acl/", NULL, NULL));
    if (data_policy_id != NULL) {
        _set_query_param((ds3_request*) request, "data_policy_id", data_policy_id);
    }
    if (group_id != NULL) {
        _set_query_param((ds3_request*) request, "group_id", group_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_data_policy_acl_for_user_spectra_s3_request(const char* data_policy_id, const char* user_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/data_policy_acl/", NULL, NULL));
    if (data_policy_id != NULL) {
        _set_query_param((ds3_request*) request, "data_policy_id", data_policy_id);
    }
    if (user_id != NULL) {
        _set_query_param((ds3_request*) request, "user_id", user_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_global_bucket_acl_for_group_spectra_s3_request(const char* group_id, const ds3_bucket_acl_permission permission) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/bucket_acl/", NULL, NULL));
    if (group_id != NULL) {
        _set_query_param((ds3_request*) request, "group_id", group_id);
    }
    _set_query_param((ds3_request*) request, "permission", _get_ds3_bucket_acl_permission_str(permission));

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_global_bucket_acl_for_user_spectra_s3_request(const ds3_bucket_acl_permission permission, const char* user_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/bucket_acl/", NULL, NULL));
    _set_query_param((ds3_request*) request, "permission", _get_ds3_bucket_acl_permission_str(permission));

    if (user_id != NULL) {
        _set_query_param((ds3_request*) request, "user_id", user_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_global_data_policy_acl_for_group_spectra_s3_request(const char* group_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/data_policy_acl/", NULL, NULL));
    if (group_id != NULL) {
        _set_query_param((ds3_request*) request, "group_id", group_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_global_data_policy_acl_for_user_spectra_s3_request(const char* user_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/data_policy_acl/", NULL, NULL));
    if (user_id != NULL) {
        _set_query_param((ds3_request*) request, "user_id", user_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_bucket_acl_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/bucket_acl/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_data_policy_acl_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/data_policy_acl/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_acl_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket_acl/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_acls_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket_acl/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_policy_acl_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_policy_acl/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_policy_acls_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_policy_acl/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_bucket_spectra_s3_request(const char* name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/bucket/", NULL, NULL));
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_bucket_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/bucket/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_buckets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_bucket_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_force_full_cache_reclaim_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/cache_filesystem/", NULL, NULL));
    _set_query_param((ds3_request*) request, "reclaim", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_cache_filesystem_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/cache_filesystem/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_cache_filesystems_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/cache_filesystem/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_cache_state_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/cache_state/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_cache_filesystem_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/cache_filesystem/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_capacity_summary_spectra_s3_request(const char* bucket_id, const char* storage_domain_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/capacity_summary/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    if (storage_domain_id != NULL) {
        _set_query_param((ds3_request*) request, "storage_domain_id", storage_domain_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_capacity_summary_spectra_s3_request(const char* storage_domain_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/capacity_summary/", NULL, NULL));
    if (storage_domain_id != NULL) {
        _set_query_param((ds3_request*) request, "storage_domain_id", storage_domain_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_system_capacity_summary_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/capacity_summary/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_path_backend_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_path_backend/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_planner_blob_store_tasks_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/blob_store_task/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_data_path_backend_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/data_path_backend/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_azure_data_replication_rule_spectra_s3_request(const char* data_policy_id, const char* target_id, const ds3_data_replication_rule_type type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/azure_data_replication_rule/", NULL, NULL));
    if (data_policy_id != NULL) {
        _set_query_param((ds3_request*) request, "data_policy_id", data_policy_id);
    }
    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    _set_query_param((ds3_request*) request, "type", _get_ds3_data_replication_rule_type_str(type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_data_persistence_rule_spectra_s3_request(const char* data_policy_id, const ds3_data_isolation_level isolation_level, const char* storage_domain_id, const ds3_data_persistence_rule_type type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/data_persistence_rule/", NULL, NULL));
    if (data_policy_id != NULL) {
        _set_query_param((ds3_request*) request, "data_policy_id", data_policy_id);
    }
    _set_query_param((ds3_request*) request, "isolation_level", _get_ds3_data_isolation_level_str(isolation_level));

    if (storage_domain_id != NULL) {
        _set_query_param((ds3_request*) request, "storage_domain_id", storage_domain_id);
    }
    _set_query_param((ds3_request*) request, "type", _get_ds3_data_persistence_rule_type_str(type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_data_policy_spectra_s3_request(const char* name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/data_policy/", NULL, NULL));
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_ds3_data_replication_rule_spectra_s3_request(const char* data_policy_id, const char* target_id, const ds3_data_replication_rule_type type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/ds3_data_replication_rule/", NULL, NULL));
    if (data_policy_id != NULL) {
        _set_query_param((ds3_request*) request, "data_policy_id", data_policy_id);
    }
    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    _set_query_param((ds3_request*) request, "type", _get_ds3_data_replication_rule_type_str(type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_s3_data_replication_rule_spectra_s3_request(const char* data_policy_id, const char* target_id, const ds3_data_replication_rule_type type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/s3_data_replication_rule/", NULL, NULL));
    if (data_policy_id != NULL) {
        _set_query_param((ds3_request*) request, "data_policy_id", data_policy_id);
    }
    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    _set_query_param((ds3_request*) request, "type", _get_ds3_data_replication_rule_type_str(type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_azure_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/azure_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_data_persistence_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/data_persistence_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_data_policy_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/data_policy/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_ds3_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_s3_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/s3_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_persistence_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_persistence_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_persistence_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_persistence_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_policies_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_policy/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_policy_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_policy/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_azure_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/azure_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_data_persistence_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/data_persistence_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_data_policy_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/data_policy/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_ds3_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/ds3_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_s3_data_replication_rule_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/s3_data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_azure_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_azure_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_ds3_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_ds3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_pool/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_s3_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_s3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_tape/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_degraded_azure_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/degraded_azure_data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_degraded_blobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/degraded_blob/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_degraded_buckets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/degraded_bucket/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_degraded_data_persistence_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/degraded_data_persistence_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_degraded_ds3_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/degraded_ds3_data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_degraded_s3_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/degraded_s3_data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_azure_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_azure_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_ds3_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_ds3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_pool/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_s3_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_s3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_tape/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_buckets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_bucket/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_objects_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_object/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_objects_with_full_details_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_object/", NULL, NULL));
    _set_query_param((ds3_request*) request, "full_details", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_mark_suspect_blob_azure_targets_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_azure_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_mark_suspect_blob_ds3_targets_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_ds3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_mark_suspect_blob_pools_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_pool/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_mark_suspect_blob_s3_targets_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_s3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_mark_suspect_blob_tapes_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_tape/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_group_group_member_spectra_s3_request(const char* group_id, const char* member_group_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/group_member/", NULL, NULL));
    if (group_id != NULL) {
        _set_query_param((ds3_request*) request, "group_id", group_id);
    }
    if (member_group_id != NULL) {
        _set_query_param((ds3_request*) request, "member_group_id", member_group_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_group_spectra_s3_request(const char* name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/group/", NULL, NULL));
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_user_group_member_spectra_s3_request(const char* group_id, const char* member_user_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/group_member/", NULL, NULL));
    if (group_id != NULL) {
        _set_query_param((ds3_request*) request, "group_id", group_id);
    }
    if (member_user_id != NULL) {
        _set_query_param((ds3_request*) request, "member_user_id", member_user_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_group_member_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/group_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_group_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/group/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_group_member_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_group_members_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group_member/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_group_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_groups_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_group_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/group/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_user_is_member_of_group_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/group/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_allocate_job_chunk_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/job_chunk/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "ALLOCATE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_active_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/active_job/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "force", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_all_active_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/active_job/", NULL, NULL));
    _set_query_param((ds3_request*) request, "force", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_all_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job/", NULL, NULL));
    _set_query_param((ds3_request*) request, "force", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "force", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_all_canceled_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/canceled_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_all_completed_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/completed_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_close_aggregating_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/job/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "close_aggregating_job", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bulk_job_spectra_s3_request(const char *const resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_GET");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_bulk_job_spectra_s3_request(const char *const resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_PUT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_bulk_job_spectra_s3_request(const char *const resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_VERIFY");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_active_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/active_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_active_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/active_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_canceled_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/canceled_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_canceled_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/canceled_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_completed_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/completed_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_completed_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/completed_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_chunk_dao_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_chunk_dao/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_chunk_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_chunk/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_chunks_ready_for_client_processing_spectra_s3_request(const char* job) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_chunk/", NULL, NULL));
    if (job != NULL) {
        _set_query_param((ds3_request*) request, "job", job);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_to_replicate_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "replicate", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_active_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/active_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_replicate_put_job_spectra_s3_request(const char *const resource_id, const char* payload) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_PUT");

    _set_query_param((ds3_request*) request, "replicate", NULL);

    if (payload != NULL) {
        request->delete_objects->strings_list[0]->value = (char*) payload;
        request->delete_objects->strings_list[0]->size = strlen(payload);    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_truncate_active_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/active_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_truncate_all_active_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/active_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_truncate_all_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_truncate_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_safe_to_create_put_job_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY_SAFE_TO_START_BULK_PUT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_node_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/node/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_nodes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/node/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_node_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/node/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_azure_target_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/azure_target_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_ds3_target_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/ds3_target_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_job_completed_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/job_completed_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_job_created_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/job_created_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_job_creation_failed_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/job_creation_failed_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_object_cached_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/object_cached_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_object_lost_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/object_lost_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_object_persisted_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/object_persisted_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_pool_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/pool_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_s3_target_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/s3_target_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_storage_domain_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/storage_domain_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_system_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/system_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_tape_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/tape_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_tape_partition_failure_notification_registration_spectra_s3_request(const char* notification_end_point) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/tape_partition_failure_notification_registration/", NULL, NULL));
    if (notification_end_point != NULL) {
        _set_query_param((ds3_request*) request, "notification_end_point", notification_end_point);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_azure_target_failure_notification_registration_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/azure_target_failure_notification_registration/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_ds3_target_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_target_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_job_completed_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job_completed_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_job_created_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job_created_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_job_creation_failed_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job_creation_failed_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_object_cached_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/object_cached_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_object_lost_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/object_lost_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_object_persisted_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/object_persisted_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_pool_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/pool_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_s3_target_failure_notification_registration_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/s3_target_failure_notification_registration/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_storage_domain_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/storage_domain_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_system_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/system_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_partition_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_partition_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_target_failure_notification_registration_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target_failure_notification_registration/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_target_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_completed_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_completed_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_completed_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_completed_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_created_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_created_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_created_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_created_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_creation_failed_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_creation_failed_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_creation_failed_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_creation_failed_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_cached_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object_cached_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_cached_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object_cached_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_lost_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object_lost_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_lost_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object_lost_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_persisted_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object_persisted_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_persisted_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object_persisted_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_target_failure_notification_registration_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target_failure_notification_registration/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_target_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_system_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/system_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_system_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/system_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_failure_notification_registration_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_failure_notification_registrations_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition_failure_notification_registration/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_folder_recursively_spectra_s3_request(const char *const resource_id, const char* bucket_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/folder/", resource_id, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "recursive", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blob_persistence_spectra_s3_request(const char* payload) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/blob_persistence/", NULL, NULL));
    if (payload != NULL) {
        request->delete_objects->strings_list[0]->value = (char*) payload;
        request->delete_objects->strings_list[0]->size = strlen(payload);    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_details_spectra_s3_request(const char *const resource_id, const char* bucket_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object/", resource_id, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_objects_details_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_objects_with_full_details_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/object/", NULL, NULL));
    _set_query_param((ds3_request*) request, "full_details", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_physical_placement_for_objects_spectra_s3_request(const char *const resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_physical_placement_for_objects_with_full_details_spectra_s3_request(const char *const resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "full_details", NULL);

    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_physical_placement_for_objects_spectra_s3_request(const char *const resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY_PHYSICAL_PLACEMENT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_physical_placement_for_objects_with_full_details_spectra_s3_request(const char *const resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "full_details", NULL);

    _set_query_param((ds3_request*) request, "operation", "VERIFY_PHYSICAL_PLACEMENT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_import_on_all_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_import_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_verify_on_all_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_verify_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_compact_all_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "COMPACT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_compact_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "COMPACT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_pool_partition_spectra_s3_request(const char* name, const ds3_pool_type type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/pool_partition/", NULL, NULL));
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    _set_query_param((ds3_request*) request, "type", _get_ds3_pool_type_str(type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_deallocate_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "DEALLOCATE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_permanently_lost_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/pool/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_pool_failure_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/pool_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_pool_partition_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/pool_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_force_pool_environment_refresh_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool_environment/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_format_all_foreign_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_format_foreign_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_partition_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_partitions_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_partition/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_import_all_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_import_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_all_pools_spectra_s3_request(const ds3_quiesced quiesced) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "quiesced", _get_ds3_quiesced_str(quiesced));

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_pool_partition_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_all_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_pool_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_convert_storage_domain_to_ds3_target_spectra_s3_request(const char *const resource_id, const char* convert_to_ds3_target) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/storage_domain/", resource_id, NULL));
    if (convert_to_ds3_target != NULL) {
        _set_query_param((ds3_request*) request, "convert_to_ds3_target", convert_to_ds3_target);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_pool_storage_domain_member_spectra_s3_request(const char* pool_partition_id, const char* storage_domain_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/storage_domain_member/", NULL, NULL));
    if (pool_partition_id != NULL) {
        _set_query_param((ds3_request*) request, "pool_partition_id", pool_partition_id);
    }
    if (storage_domain_id != NULL) {
        _set_query_param((ds3_request*) request, "storage_domain_id", storage_domain_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_storage_domain_spectra_s3_request(const char* name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/storage_domain/", NULL, NULL));
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_tape_storage_domain_member_spectra_s3_request(const char* storage_domain_id, const char* tape_partition_id, const ds3_tape_type tape_type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/storage_domain_member/", NULL, NULL));
    if (storage_domain_id != NULL) {
        _set_query_param((ds3_request*) request, "storage_domain_id", storage_domain_id);
    }
    if (tape_partition_id != NULL) {
        _set_query_param((ds3_request*) request, "tape_partition_id", tape_partition_id);
    }
    _set_query_param((ds3_request*) request, "tape_type", _get_ds3_tape_type_str(tape_type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_storage_domain_failure_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/storage_domain_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_storage_domain_member_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/storage_domain_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_storage_domain_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/storage_domain/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_member_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_members_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_member/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domains_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_storage_domain_member_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/storage_domain_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_storage_domain_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/storage_domain/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_force_feature_key_validation_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/feature_key/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_feature_keys_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/feature_key/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_system_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/system_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_system_information_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/system_information/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_reset_instance_identifier_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/instance_identifier/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_system_health_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/system_health/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_eject_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_EJECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_eject_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_EJECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_format_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_format_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_import_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_import_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_online_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_online_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_verify_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_verify_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_clean_tape_drive_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape_drive/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CLEAN");

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_tape_density_directive_spectra_s3_request(const ds3_tape_drive_type density, const char* partition_id, const ds3_tape_type tape_type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/tape_density_directive/", NULL, NULL));
    _set_query_param((ds3_request*) request, "density", _get_ds3_tape_drive_type_str(density));

    if (partition_id != NULL) {
        _set_query_param((ds3_request*) request, "partition_id", partition_id);
    }
    _set_query_param((ds3_request*) request, "tape_type", _get_ds3_tape_type_str(tape_type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_permanently_lost_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_density_directive_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_density_directive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_drive_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_drive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_failure_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_partition_failure_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_partition_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_partition_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_eject_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "EJECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_eject_storage_domain_blobs_spectra_s3_request(const char* bucket_id, const char* storage_domain_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "blobs", NULL);

    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "operation", "EJECT");

    if (storage_domain_id != NULL) {
        _set_query_param((ds3_request*) request, "storage_domain_id", storage_domain_id);
    }
    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_eject_storage_domain_spectra_s3_request(const char* storage_domain_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "EJECT");

    if (storage_domain_id != NULL) {
        _set_query_param((ds3_request*) request, "storage_domain_id", storage_domain_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_eject_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "EJECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_force_tape_environment_refresh_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape_environment/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_format_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_format_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_density_directive_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_density_directive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_density_directives_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_density_directive/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_drive_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_drive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_drives_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_drive/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_libraries_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_library/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_library_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_library/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_with_full_details_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "full_details", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partitions_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partitions_with_full_details_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition/", NULL, NULL));
    _set_query_param((ds3_request*) request, "full_details", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_import_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_import_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_inspect_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "INSPECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_inspect_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "INSPECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_all_tape_partitions_spectra_s3_request(const ds3_quiesced quiesced) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape_partition/", NULL, NULL));
    _set_query_param((ds3_request*) request, "quiesced", _get_ds3_quiesced_str(quiesced));

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_tape_drive_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape_drive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_tape_partition_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_online_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_online_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_raw_import_all_tapes_spectra_s3_request(const char* bucket_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_raw_import_tape_spectra_s3_request(const char *const resource_id, const char* bucket_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_tape_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_force_target_environment_refresh_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/target_environment/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_azure_target_bucket_name_spectra_s3_request(const char* bucket_id, const char* name, const char* target_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/azure_target_bucket_name/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_azure_target_read_preference_spectra_s3_request(const char* bucket_id, const ds3_target_read_preference_type read_preference, const char* target_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/azure_target_read_preference/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "read_preference", _get_ds3_target_read_preference_type_str(read_preference));

    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_azure_target_bucket_name_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/azure_target_bucket_name/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_azure_target_failure_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/azure_target_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_azure_target_read_preference_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/azure_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_azure_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/azure_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_target_bucket_names_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target_bucket_name/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_target_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_target_read_preference_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_target_read_preferences_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target_read_preference/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_azure_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_azure_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/azure_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_import_azure_target_spectra_s3_request(const char *const resource_id, const char* cloud_bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/azure_target/", resource_id, NULL));
    if (cloud_bucket_name != NULL) {
        _set_query_param((ds3_request*) request, "cloud_bucket_name", cloud_bucket_name);
    }
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_all_azure_targets_spectra_s3_request(const ds3_quiesced quiesced) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/azure_target/", NULL, NULL));
    _set_query_param((ds3_request*) request, "quiesced", _get_ds3_quiesced_str(quiesced));

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_azure_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/azure_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_register_azure_target_spectra_s3_request(const char* account_key, const char* account_name, const char* name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/azure_target/", NULL, NULL));
    if (account_key != NULL) {
        _set_query_param((ds3_request*) request, "account_key", account_key);
    }
    if (account_name != NULL) {
        _set_query_param((ds3_request*) request, "account_name", account_name);
    }
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_azure_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/azure_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_ds3_target_read_preference_spectra_s3_request(const char* bucket_id, const ds3_target_read_preference_type read_preference, const char* target_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/ds3_target_read_preference/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "read_preference", _get_ds3_target_read_preference_type_str(read_preference));

    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_ds3_target_failure_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_target_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_ds3_target_read_preference_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_ds3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_ds3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_data_policies_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_data_policies/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_read_preference_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_read_preferences_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_read_preference/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_all_ds3_targets_spectra_s3_request(const ds3_quiesced quiesced) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/ds3_target/", NULL, NULL));
    _set_query_param((ds3_request*) request, "quiesced", _get_ds3_quiesced_str(quiesced));

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_ds3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_pair_back_registered_ds3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "PAIR_BACK");

    return (ds3_request*) request;
}
ds3_request* ds3_init_register_ds3_target_spectra_s3_request(const char* admin_auth_id, const char* admin_secret_key, const char* data_path_end_point, const char* name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/ds3_target/", NULL, NULL));
    if (admin_auth_id != NULL) {
        _set_query_param((ds3_request*) request, "admin_auth_id", admin_auth_id);
    }
    if (admin_secret_key != NULL) {
        _set_query_param((ds3_request*) request, "admin_secret_key", admin_secret_key);
    }
    if (data_path_end_point != NULL) {
        _set_query_param((ds3_request*) request, "data_path_end_point", data_path_end_point);
    }
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_ds3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_s3_target_bucket_name_spectra_s3_request(const char* bucket_id, const char* name, const char* target_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/s3_target_bucket_name/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_s3_target_read_preference_spectra_s3_request(const char* bucket_id, const ds3_target_read_preference_type read_preference, const char* target_id) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/s3_target_read_preference/", NULL, NULL));
    if (bucket_id != NULL) {
        _set_query_param((ds3_request*) request, "bucket_id", bucket_id);
    }
    _set_query_param((ds3_request*) request, "read_preference", _get_ds3_target_read_preference_type_str(read_preference));

    if (target_id != NULL) {
        _set_query_param((ds3_request*) request, "target_id", target_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_s3_target_bucket_name_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/s3_target_bucket_name/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_s3_target_failure_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/s3_target_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_s3_target_read_preference_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/s3_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_s3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/s3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_s3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_target_bucket_names_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target_bucket_name/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_target_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_target_read_preference_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_target_read_preferences_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target_read_preference/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_s3_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/s3_target/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_import_s3_target_spectra_s3_request(const char *const resource_id, const char* cloud_bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/s3_target/", resource_id, NULL));
    if (cloud_bucket_name != NULL) {
        _set_query_param((ds3_request*) request, "cloud_bucket_name", cloud_bucket_name);
    }
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_all_s3_targets_spectra_s3_request(const ds3_quiesced quiesced) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/s3_target/", NULL, NULL));
    _set_query_param((ds3_request*) request, "quiesced", _get_ds3_quiesced_str(quiesced));

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_s3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/s3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_register_s3_target_spectra_s3_request(const char* access_key, const char* name, const char* secret_key) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/s3_target/", NULL, NULL));
    if (access_key != NULL) {
        _set_query_param((ds3_request*) request, "access_key", access_key);
    }
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    if (secret_key != NULL) {
        _set_query_param((ds3_request*) request, "secret_key", secret_key);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_s3_target_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/s3_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_delegate_create_user_spectra_s3_request(const char* name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/user/", NULL, NULL));
    if (name != NULL) {
        _set_query_param((ds3_request*) request, "name", name);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_delegate_delete_user_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/user/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_user_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/user/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_users_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/user/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_user_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/user/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_regenerate_user_secret_key_spectra_s3_request(const char *const resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/user/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "REGENERATE_SECRET_KEY");

    return (ds3_request*) request;
}
