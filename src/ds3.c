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
#include "ds3_connection.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

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
    } else if (input == DS3_AUTO_INSPECT_MODE_DEFAULT) {
        return "DEFAULT";
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
static char* _get_ds3_data_replication_rule_type_str(ds3_data_replication_rule_type input) {
    if (input == DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT) {
        return "PERMANENT";
    } else if (input == DS3_DATA_REPLICATION_RULE_TYPE_RETIRED) {
        return "RETIRED";
    } else {
        return "";
    }

}
static char* _get_ds3_data_persistence_rule_state_str(ds3_data_persistence_rule_state input) {
    if (input == DS3_DATA_PERSISTENCE_RULE_STATE_NORMAL) {
        return "NORMAL";
    } else if (input == DS3_DATA_PERSISTENCE_RULE_STATE_INCLUSION_IN_PROGRESS) {
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
static char* _get_ds3_system_failure_type_str(ds3_system_failure_type input) {
    if (input == DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED) {
        return "RECONCILE_TAPE_ENVIRONMENT_FAILED";
    } else if (input == DS3_SYSTEM_FAILURE_TYPE_RECONCILE_POOL_ENVIRONMENT_FAILED) {
        return "RECONCILE_POOL_ENVIRONMENT_FAILED";
    } else if (input == DS3_SYSTEM_FAILURE_TYPE_CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION) {
        return "CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION";
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
    } else if (input == DS3_TAPE_DRIVE_TYPE_TS1140) {
        return "TS1140";
    } else if (input == DS3_TAPE_DRIVE_TYPE_TS1150) {
        return "TS1150";
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
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_TYPE_MISMATCH) {
        return "TAPE_DRIVE_TYPE_MISMATCH";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_EJECTION_BY_OPERATOR_REQUIRED) {
        return "TAPE_EJECTION_BY_OPERATOR_REQUIRED";
    } else if (input == DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_MEDIA_TYPE_INCOMPATIBLE) {
        return "TAPE_MEDIA_TYPE_INCOMPATIBLE";
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
    if (input == DS3_TARGET_FAILURE_TYPE_NOT_ONLINE) {
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
    } else if (input == DS3_TARGET_STATE_LIMITED_ACCESS) {
        return "LIMITED_ACCESS";
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


struct _ds3_metadata {
    GHashTable* metadata;
};

static void _ds3_metadata_entry_free(gpointer pointer) {
    ds3_metadata_entry* entry;
    if (pointer == NULL) {
        return; // do nothing
    }

    entry = (ds3_metadata_entry*) pointer;

    ds3_metadata_entry_free(entry);
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
    metadata->metadata = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, _ds3_metadata_entry_free);

    if (response_headers == NULL) {
        fprintf(stderr, "response headers was null\n");
    }

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
    metadata_copy = g_new0(ds3_str*, (gsize)orig->num_values);

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

static void _cleanup_hash_value(gpointer value) {
    g_free(value);
}

static GHashTable* _create_hash_table(void) {
    GHashTable* hash =  g_hash_table_new_full(g_str_hash, g_str_equal, _cleanup_hash_value, _cleanup_hash_value);
    return hash;
}

void ds3_metadata_free(ds3_metadata* _metadata) {
    struct _ds3_metadata* metadata;
    if (_metadata == NULL) return;

    metadata = (struct _ds3_metadata*) _metadata;

    if (metadata->metadata == NULL) return;

    g_hash_table_destroy(metadata->metadata);

    g_free(metadata);
}

void ds3_metadata_entry_free(ds3_metadata_entry* entry) {
    if (entry == NULL) {
        return;
    }

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

void ds3_metadata_keys_free(ds3_metadata_keys_result* metadata_keys) {
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

void ds3_client_proxy(ds3_client* client, const char* proxy) {
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

    ds3_str_free(client->endpoint);
    ds3_str_free(client->proxy);
    g_free(client->log);
    ds3_connection_pool_clear(client->connection_pool);
    g_free(client->connection_pool);
    g_free(client);
}


static const char UNSIGNED_LONG_BASE_10[] = "4294967296";
static const unsigned int UNSIGNED_LONG_BASE_10_STR_LEN = sizeof(UNSIGNED_LONG_BASE_10);

typedef struct {
    char* buff;
    size_t size;
    size_t total_read;
}ds3_xml_send_buff;

typedef enum {
    BULK_PUT,
    BULK_GET,
    BULK_DELETE,
    GET_PHYSICAL_PLACEMENT,
    COMPLETE_MPU,
    STRING,
    STRING_LIST,
    DATA
}object_list_type;

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
    char string_buffer[UNSIGNED_LONG_BASE_10_STR_LEN];
    memset(string_buffer, 0, sizeof(string_buffer));
    snprintf(string_buffer, sizeof(string_buffer), "%lu", value);
    _set_query_param(_request, key, string_buffer);
}

static void _set_query_param_int(const ds3_request* _request, const char* key, int value) {
    char string_buffer[UNSIGNED_LONG_BASE_10_STR_LEN];
    memset(string_buffer, 0, sizeof(string_buffer));
    snprintf(string_buffer, sizeof(string_buffer), "%d", value);
    _set_query_param(_request, key, string_buffer);
}

static void _set_query_param_float(const ds3_request* _request, const char* key, float value) {
    char string_buffer[UNSIGNED_LONG_BASE_10_STR_LEN];
    memset(string_buffer, 0, sizeof(string_buffer));
    snprintf(string_buffer, sizeof(string_buffer), "%f", value);
    _set_query_param(_request, key, string_buffer);
}
void ds3_request_set_access_control_replication_ds3_target_access_control_replication(const ds3_request* request, const ds3_target_access_control_replication value) {
    _set_query_param(request, "access_control_replication", (const char*)_get_ds3_target_access_control_replication_str(value));

}
void ds3_request_set_activated(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "activated", value);

}
void ds3_request_set_admin_auth_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "admin_auth_id", value);

}
void ds3_request_set_admin_secret_key(const ds3_request* request, const char* value) {
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
void ds3_request_set_always_replicate_deletes(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "always_replicate_deletes", value);

}
void ds3_request_set_assigned_to_storage_domain(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "assigned_to_storage_domain", value);

}
void ds3_request_set_auth_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "auth_id", value);

}
void ds3_request_set_auto_activate_timeout_in_mins(const ds3_request* request, const int value) {
    _set_query_param_int(request, "auto_activate_timeout_in_mins", value);

}
void ds3_request_set_auto_eject_media_full_threshold(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "auto_eject_media_full_threshold", value);

}
void ds3_request_set_auto_eject_upon_cron(const ds3_request* request, const char* value) {
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
void ds3_request_set_bar_code(const ds3_request* request, const char* value) {
    _set_query_param(request, "bar_code", value);

}
void ds3_request_set_blob_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "blob_id", value);

}
void ds3_request_set_blobbing_enabled(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "blobbing_enabled", value);

}
void ds3_request_set_bucket_id(const ds3_request* request, const char* value) {
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
void ds3_request_set_conflict_resolution_mode_ds3_import_conflict_resolution_mode(const ds3_request* request, const ds3_import_conflict_resolution_mode value) {
    _set_query_param(request, "conflict_resolution_mode", (const char*)_get_ds3_import_conflict_resolution_mode_str(value));

}
void ds3_request_set_created_at(const ds3_request* request, const char* value) {
    _set_query_param(request, "created_at", value);

}
void ds3_request_set_data_path_end_point(const ds3_request* request, const char* value) {
    _set_query_param(request, "data_path_end_point", value);

}
void ds3_request_set_data_path_https(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "data_path_https", value);

}
void ds3_request_set_data_path_port(const ds3_request* request, const int value) {
    _set_query_param_int(request, "data_path_port", value);

}
void ds3_request_set_data_path_proxy(const ds3_request* request, const char* value) {
    _set_query_param(request, "data_path_proxy", value);

}
void ds3_request_set_data_path_verify_certificate(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "data_path_verify_certificate", value);

}
void ds3_request_set_data_policy_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "data_policy_id", value);

}
void ds3_request_set_default_blob_size(const ds3_request* request, const uint64_t value) {
    _set_query_param_uint64_t(request, "default_blob_size", value);

}
void ds3_request_set_default_data_policy_id(const ds3_request* request, const char* value) {
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
void ds3_request_set_default_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "default_verify_data_after_import", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_default_verify_data_prior_to_import(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "default_verify_data_prior_to_import", value);

}
void ds3_request_set_default_verify_job_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "default_verify_job_priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_delimiter(const ds3_request* request, const char* value) {
    _set_query_param(request, "delimiter", value);

}
void ds3_request_set_density_ds3_tape_drive_type(const ds3_request* request, const ds3_tape_drive_type value) {
    _set_query_param(request, "density", (const char*)_get_ds3_tape_drive_type_str(value));

}
void ds3_request_set_dns_name(const ds3_request* request, const char* value) {
    _set_query_param(request, "dns_name", value);

}
void ds3_request_set_ds3_target_data_policy(const ds3_request* request, const char* value) {
    _set_query_param(request, "ds3_target_data_policy", value);

}
void ds3_request_set_ds3_target_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "ds3_target_id", value);

}
void ds3_request_set_eject_label(const ds3_request* request, const char* value) {
    _set_query_param(request, "eject_label", value);

}
void ds3_request_set_eject_location(const ds3_request* request, const char* value) {
    _set_query_param(request, "eject_location", value);

}
void ds3_request_set_end_to_end_crc_required(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "end_to_end_crc_required", value);

}
void ds3_request_set_error_message(const ds3_request* request, const char* value) {
    _set_query_param(request, "error_message", value);

}
void ds3_request_set_folder(const ds3_request* request, const char* value) {
    _set_query_param(request, "folder", value);

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
void ds3_request_set_group_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "group_id", value);

}
void ds3_request_set_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value) {
    _set_query_param(request, "health", (const char*)_get_ds3_pool_health_str(value));

}
void ds3_request_set_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "id", value);

}
void ds3_request_set_ignore_naming_conflicts(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "ignore_naming_conflicts", value);

}
void ds3_request_set_import_export_configuration_ds3_import_export_configuration(const ds3_request* request, const ds3_import_export_configuration value) {
    _set_query_param(request, "import_export_configuration", (const char*)_get_ds3_import_export_configuration_str(value));

}
void ds3_request_set_include_physical_placement(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "include_physical_placement", value);

}
void ds3_request_set_isolation_level_ds3_data_isolation_level(const ds3_request* request, const ds3_data_isolation_level value) {
    _set_query_param(request, "isolation_level", (const char*)_get_ds3_data_isolation_level_str(value));

}
void ds3_request_set_job(const ds3_request* request, const char* value) {
    _set_query_param(request, "job", value);

}
void ds3_request_set_job_chunk(const ds3_request* request, const char* value) {
    _set_query_param(request, "job_chunk", value);

}
void ds3_request_set_job_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "job_id", value);

}
void ds3_request_set_key_marker(const ds3_request* request, const char* value) {
    _set_query_param(request, "key_marker", value);

}
void ds3_request_set_last_page(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "last_page", value);

}
void ds3_request_set_latest(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "latest", value);

}
void ds3_request_set_library_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "library_id", value);

}
void ds3_request_set_ltfs_file_naming_ds3_ltfs_file_naming_mode(const ds3_request* request, const ds3_ltfs_file_naming_mode value) {
    _set_query_param(request, "ltfs_file_naming", (const char*)_get_ds3_ltfs_file_naming_mode_str(value));

}
void ds3_request_set_management_url(const ds3_request* request, const char* value) {
    _set_query_param(request, "management_url", value);

}
void ds3_request_set_marker(const ds3_request* request, const char* value) {
    _set_query_param(request, "marker", value);

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
void ds3_request_set_member_group_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "member_group_id", value);

}
void ds3_request_set_member_user_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "member_user_id", value);

}
void ds3_request_set_minimize_spanning_across_media(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "minimize_spanning_across_media", value);

}
void ds3_request_set_minimum_days_to_retain(const ds3_request* request, const int value) {
    _set_query_param_int(request, "minimum_days_to_retain", value);

}
void ds3_request_set_name(const ds3_request* request, const char* value) {
    _set_query_param(request, "name", value);

}
void ds3_request_set_naming_convention_ds3_naming_convention_type(const ds3_request* request, const ds3_naming_convention_type value) {
    _set_query_param(request, "naming_convention", (const char*)_get_ds3_naming_convention_type_str(value));

}
void ds3_request_set_node_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "node_id", value);

}
void ds3_request_set_notification_http_method_ds3_request_type(const ds3_request* request, const ds3_request_type value) {
    _set_query_param(request, "notification_http_method", (const char*)_get_ds3_request_type_str(value));

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
void ds3_request_set_page_start_marker(const ds3_request* request, const char* value) {
    _set_query_param(request, "page_start_marker", value);

}
void ds3_request_set_part_number_marker(const ds3_request* request, const int value) {
    _set_query_param_int(request, "part_number_marker", value);

}
void ds3_request_set_partially_verified_end_of_tape(const ds3_request* request, const char* value) {
    _set_query_param(request, "partially_verified_end_of_tape", value);

}
void ds3_request_set_partially_verify_last_percent_of_tapes(const ds3_request* request, const int value) {
    _set_query_param_int(request, "partially_verify_last_percent_of_tapes", value);

}
void ds3_request_set_partition_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "partition_id", value);

}
void ds3_request_set_permission_ds3_bucket_acl_permission(const ds3_request* request, const ds3_bucket_acl_permission value) {
    _set_query_param(request, "permission", (const char*)_get_ds3_bucket_acl_permission_str(value));

}
void ds3_request_set_permit_going_out_of_sync(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "permit_going_out_of_sync", value);

}
void ds3_request_set_persistence_rule_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "persistence_rule_id", value);

}
void ds3_request_set_pool_health_ds3_pool_health(const ds3_request* request, const ds3_pool_health value) {
    _set_query_param(request, "pool_health", (const char*)_get_ds3_pool_health_str(value));

}
void ds3_request_set_pool_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "pool_id", value);

}
void ds3_request_set_pool_partition_id(const ds3_request* request, const char* value) {
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
void ds3_request_set_prefix(const ds3_request* request, const char* value) {
    _set_query_param(request, "prefix", value);

}
void ds3_request_set_previous_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value) {
    _set_query_param(request, "previous_state", (const char*)_get_ds3_tape_state_str(value));

}
void ds3_request_set_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "priority", (const char*)_get_ds3_priority_str(value));

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
void ds3_request_set_rechunked(const ds3_request* request, const char* value) {
    _set_query_param(request, "rechunked", value);

}
void ds3_request_set_replicate(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "replicate", value);

}
void ds3_request_set_replicated_user_default_data_policy(const ds3_request* request, const char* value) {
    _set_query_param(request, "replicated_user_default_data_policy", value);

}
void ds3_request_set_replication_rule_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "replication_rule_id", value);

}
void ds3_request_set_request_type_ds3_job_request_type(const ds3_request* request, const ds3_job_request_type value) {
    _set_query_param(request, "request_type", (const char*)_get_ds3_job_request_type_str(value));

}
void ds3_request_set_roll_back(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "roll_back", value);

}
void ds3_request_set_secret_key(const ds3_request* request, const char* value) {
    _set_query_param(request, "secret_key", value);

}
void ds3_request_set_secure_media_allocation(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "secure_media_allocation", value);

}
void ds3_request_set_serial_number(const ds3_request* request, const char* value) {
    _set_query_param(request, "serial_number", value);

}
void ds3_request_set_state_ds3_data_persistence_rule_state(const ds3_request* request, const ds3_data_persistence_rule_state value) {
    _set_query_param(request, "state", (const char*)_get_ds3_data_persistence_rule_state_str(value));

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
void ds3_request_set_storage_domain_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "storage_domain_id", value);

}
void ds3_request_set_tape_drive_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "tape_drive_id", value);

}
void ds3_request_set_tape_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "tape_id", value);

}
void ds3_request_set_tape_partition_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "tape_partition_id", value);

}
void ds3_request_set_tape_state_ds3_tape_state(const ds3_request* request, const ds3_tape_state value) {
    _set_query_param(request, "tape_state", (const char*)_get_ds3_tape_state_str(value));

}
void ds3_request_set_tape_type_ds3_tape_type(const ds3_request* request, const ds3_tape_type value) {
    _set_query_param(request, "tape_type", (const char*)_get_ds3_tape_type_str(value));

}
void ds3_request_set_target_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "target_id", value);

}
void ds3_request_set_task_priority_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "task_priority", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_truncated(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "truncated", value);

}
void ds3_request_set_type_ds3_data_persistence_rule_type(const ds3_request* request, const ds3_data_persistence_rule_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_data_persistence_rule_type_str(value));

}
void ds3_request_set_type_ds3_data_replication_rule_type(const ds3_request* request, const ds3_data_replication_rule_type value) {
    _set_query_param(request, "type", (const char*)_get_ds3_data_replication_rule_type_str(value));

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
void ds3_request_set_upload_id_marker(const ds3_request* request, const char* value) {
    _set_query_param(request, "upload_id_marker", value);

}
void ds3_request_set_user_id(const ds3_request* request, const char* value) {
    _set_query_param(request, "user_id", value);

}
void ds3_request_set_verify_data_after_import_ds3_priority(const ds3_request* request, const ds3_priority value) {
    _set_query_param(request, "verify_data_after_import", (const char*)_get_ds3_priority_str(value));

}
void ds3_request_set_verify_data_prior_to_import(const ds3_request* request, ds3_bool value) {
    _set_query_param_flag(request, "verify_data_prior_to_import", value);

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

static struct _ds3_request* _common_request_init(http_verb verb, ds3_str* path) {
    struct _ds3_request* request = g_new0(struct _ds3_request, 1);
    request->headers = _create_hash_table();
    request->query_params = _create_hash_table();
    request->verb = verb;
    request->path = path;
    return request;
}

static ds3_str* _build_path(const char* path_prefix, const char* bucket_name, const char* object_name) {
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

ds3_request* ds3_init_abort_multi_part_upload_request(const char* bucket_name, const char* object_name, const char* upload_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, object_name));
    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_complete_multi_part_upload_request(const char* bucket_name, const char* object_name, const char* upload_id, const ds3_complete_multipart_upload_response* mpu_list) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/", bucket_name, object_name));
    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    request->mpu_list = (ds3_complete_multipart_upload_response*) mpu_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_bucket_request(const char* bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_multi_part_upload_part_request(const char* bucket_name, const char* object_name, const int part_number, const char* upload_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, object_name));
    char tmp_buff[32];
    sprintf(tmp_buff, "%d", part_number);
    _set_query_param((ds3_request*) request, "part_number", tmp_buff);

    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_put_object_request(const char* bucket_name, const char* object_name, const uint64_t length) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/", bucket_name, object_name));
    request->length = length;

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_bucket_request(const char* bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_object_request(const char* bucket_name, const char* object_name) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/", bucket_name, object_name));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_objects_request(const char* bucket_name, const ds3_delete_objects_response* objects_list) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/", bucket_name, NULL));
    _set_query_param((ds3_request*) request, "delete", NULL);

    request->delete_objects = (ds3_delete_objects_response*) objects_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_request(const char* bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_service_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_object_request(const char* bucket_name, const char* object_name, const uint64_t length) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, object_name));
    request->length = length;

    return (ds3_request*) request;
}
ds3_request* ds3_init_head_bucket_request(const char* bucket_name) {
    struct _ds3_request* request = _common_request_init(HTTP_HEAD, _build_path("/", bucket_name, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_head_object_request(const char* bucket_name, const char* object_name) {
    struct _ds3_request* request = _common_request_init(HTTP_HEAD, _build_path("/", bucket_name, object_name));
    return (ds3_request*) request;
}
ds3_request* ds3_init_initiate_multi_part_upload_request(const char* bucket_name, const char* object_name) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/", bucket_name, object_name));
    _set_query_param((ds3_request*) request, "uploads", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_list_multi_part_upload_parts_request(const char* bucket_name, const char* object_name, const char* upload_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/", bucket_name, object_name));
    if (upload_id != NULL) {
        _set_query_param((ds3_request*) request, "upload_id", upload_id);
    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_list_multi_part_uploads_request(const char* bucket_name) {
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
ds3_request* ds3_init_delete_bucket_acl_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/bucket_acl/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_data_policy_acl_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/data_policy_acl/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_acl_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket_acl/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_acls_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket_acl/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_policy_acl_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_delete_bucket_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/bucket/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_bucket_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_buckets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_bucket_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_force_full_cache_reclaim_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/cache_filesystem/", NULL, NULL));
    _set_query_param((ds3_request*) request, "reclaim", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_cache_filesystem_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_modify_cache_filesystem_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_put_data_replication_rule_spectra_s3_request(const char* data_policy_id, const char* ds3_target_id, const ds3_data_replication_rule_type type) {
    struct _ds3_request* request = _common_request_init(HTTP_POST, _build_path("/_rest_/data_replication_rule/", NULL, NULL));
    if (data_policy_id != NULL) {
        _set_query_param((ds3_request*) request, "data_policy_id", data_policy_id);
    }
    if (ds3_target_id != NULL) {
        _set_query_param((ds3_request*) request, "ds3_target_id", ds3_target_id);
    }
    _set_query_param((ds3_request*) request, "type", _get_ds3_data_replication_rule_type_str(type));

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_data_persistence_rule_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/data_persistence_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_data_policy_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/data_policy/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_data_replication_rule_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_persistence_rule_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_get_data_policy_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_policy/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_replication_rule_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_data_persistence_rule_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/data_persistence_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_data_policy_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/data_policy/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_data_replication_rule_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/data_replication_rule/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_pool/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_tape/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_clear_suspect_blob_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/suspect_blob_target/", NULL, NULL));
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
ds3_request* ds3_init_get_degraded_data_replication_rules_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/degraded_data_replication_rule/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_pool/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_tape/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_suspect_blob_targets_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/suspect_blob_target/", NULL, NULL));
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
ds3_request* ds3_init_mark_suspect_blob_pools_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_pool/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_mark_suspect_blob_tapes_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_tape/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_mark_suspect_blob_targets_as_degraded_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/suspect_blob_target/", NULL, NULL));
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
ds3_request* ds3_init_delete_group_member_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/group_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_group_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/group/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_group_member_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_group_members_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group_member/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_group_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_groups_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/group/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_group_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/group/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_user_is_member_of_group_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/group/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_allocate_job_chunk_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/job_chunk/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "ALLOCATE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_active_job_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_cancel_job_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_get_bulk_job_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_GET");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_put_bulk_job_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_PUT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_bulk_job_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_VERIFY");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_active_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/active_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_active_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/active_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_canceled_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/canceled_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_canceled_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/canceled_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_completed_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/completed_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_completed_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/completed_job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_chunk_dao_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job_chunk_dao/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_chunk_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_get_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_job_to_replicate_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "replicate", NULL);

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_jobs_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/job/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_active_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/active_job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_replicate_put_job_spectra_s3_request(const char* resource_id, const char* payload) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "START_BULK_PUT");

    _set_query_param((ds3_request*) request, "replicate", NULL);

    if (payload != NULL) {
        request->delete_objects->strings_list[0]->value = (char*) payload;
        request->delete_objects->strings_list[0]->size = strlen(payload);    }
    return (ds3_request*) request;
}
ds3_request* ds3_init_truncate_active_job_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_truncate_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/job/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_safe_to_create_put_job_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY_SAFE_TO_START_BULK_PUT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_node_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/node/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_nodes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/node/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_node_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/node/", resource_id, NULL));
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
ds3_request* ds3_init_delete_folder_recursively_spectra_s3_request(const char* resource_id, const char* bucket_id) {
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
ds3_request* ds3_init_get_object_details_spectra_s3_request(const char* resource_id, const char* bucket_id) {
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
ds3_request* ds3_init_get_physical_placement_for_objects_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_physical_placement_for_objects_with_full_details_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "full_details", NULL);

    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_physical_placement_for_objects_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/bucket/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY_PHYSICAL_PLACEMENT");

    request->object_list = (ds3_bulk_object_list_response*) object_list;

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_physical_placement_for_objects_with_full_details_spectra_s3_request(const char* resource_id, const ds3_bulk_object_list_response* object_list) {
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
ds3_request* ds3_init_cancel_import_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_compact_all_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "COMPACT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_compact_pool_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_deallocate_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "DEALLOCATE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_permanently_lost_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/pool/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_pool_failure_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/pool_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_pool_partition_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_format_foreign_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_partition_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_partitions_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/pool_partition/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_pool_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_import_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_all_pools_spectra_s3_request(const ds3_quiesced quiesced) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "quiesced", _get_ds3_quiesced_str(quiesced));

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_pool_partition_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_all_pools_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_pool_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/pool/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_convert_storage_domain_to_ds3_target_spectra_s3_request(const char* resource_id, const char* convert_to_ds3_target) {
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
ds3_request* ds3_init_delete_storage_domain_failure_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/storage_domain_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_storage_domain_member_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/storage_domain_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_storage_domain_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/storage_domain/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_member_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_members_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain_member/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domain_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_storage_domains_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/storage_domain/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_storage_domain_member_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/storage_domain_member/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_storage_domain_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/storage_domain/", resource_id, NULL));
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
ds3_request* ds3_init_cancel_eject_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_EJECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_format_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_format_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_import_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_import_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_online_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_online_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_verify_on_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_cancel_verify_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "CANCEL_VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_clean_tape_drive_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_delete_permanently_lost_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_density_directive_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_density_directive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_drive_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_drive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_failure_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_partition_failure_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/tape_partition_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_tape_partition_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_eject_tape_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_format_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "FORMAT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_density_directive_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_density_directive/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_density_directives_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_density_directive/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_drive_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_get_tape_library_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_library/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/tape_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_tape_partition_with_full_details_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_get_tape_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_import_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "IMPORT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_inspect_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "INSPECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_inspect_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "INSPECT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_all_tape_partitions_spectra_s3_request(const ds3_quiesced quiesced) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape_partition/", NULL, NULL));
    _set_query_param((ds3_request*) request, "quiesced", _get_ds3_quiesced_str(quiesced));

    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_tape_partition_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape_partition/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_online_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_online_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "ONLINE");

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_all_tapes_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", NULL, NULL));
    _set_query_param((ds3_request*) request, "operation", "VERIFY");

    return (ds3_request*) request;
}
ds3_request* ds3_init_verify_tape_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/tape/", resource_id, NULL));
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
ds3_request* ds3_init_delete_ds3_target_failure_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_target_failure/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_ds3_target_read_preference_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_delete_ds3_target_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_force_target_environment_refresh_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/target_environment/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_blobs_on_ds3_target_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "GET_PHYSICAL_PLACEMENT");

    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_data_policies_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_data_policies/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_failures_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_failure/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_read_preference_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_read_preference/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_read_preferences_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/ds3_target_read_preference/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_ds3_target_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_modify_ds3_target_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/ds3_target/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_pair_back_registered_ds3_target_spectra_s3_request(const char* resource_id) {
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
ds3_request* ds3_init_verify_ds3_target_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/ds3_target/", resource_id, NULL));
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
ds3_request* ds3_init_delegate_delete_user_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_DELETE, _build_path("/_rest_/user/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_user_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/user/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_get_users_spectra_s3_request(void) {
    struct _ds3_request* request = _common_request_init(HTTP_GET, _build_path("/_rest_/user/", NULL, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_modify_user_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/user/", resource_id, NULL));
    return (ds3_request*) request;
}
ds3_request* ds3_init_regenerate_user_secret_key_spectra_s3_request(const char* resource_id) {
    struct _ds3_request* request = _common_request_init(HTTP_PUT, _build_path("/_rest_/user/", resource_id, NULL));
    _set_query_param((ds3_request*) request, "operation", "REGENERATE_SECRET_KEY");

    return (ds3_request*) request;
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

static ds3_bool xml_get_bool_from_attribute(const ds3_log* log, xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_bool(log, doc, (xmlNodePtr) attribute);
}

static ds3_error* _internal_request_dispatcher(
        const ds3_client* client,
        const ds3_request* request,
        void* read_user_struct,
        size_t (*read_handler_func)(void*, size_t, size_t, void*),
        void* write_user_struct,
        size_t (*write_handler_func)(void*, size_t, size_t, void*),
        ds3_string_multimap** return_headers) {
    if (client == NULL || request == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }
    return net_process_request(client, request, read_user_struct, read_handler_func, write_user_struct, write_handler_func, return_headers);
}

static int num_chars_in_ds3_str(const ds3_str* str, char ch) {
    int num_matches = 0;
    size_t index;

    for (index = 0; index < str->size; index++) {
        if (str->value[index] == '/') {
            num_matches++;
        }
    }

    return num_matches;
}

static ds3_error* _get_request_xml_nodes(
        GByteArray* xml_blob,
        xmlDocPtr* _doc,
        xmlNodePtr* _root,
        char* root_element_name) {
    xmlNodePtr root;

    xmlDocPtr doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, root_element_name) == false) {
        char* message = g_strconcat("Expected the root element to be '", root_element_name, "'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    *_doc = doc;
    *_root = root;

    g_byte_array_free(xml_blob, TRUE);
    return NULL;
}

#define LENGTH_BUFF_SIZE 21

static xmlDocPtr _generate_xml_bulk_objects_list(const ds3_bulk_object_list_response* obj_list, object_list_type list_type, ds3_job_chunk_client_processing_order_guarantee order) {
    char size_buff[LENGTH_BUFF_SIZE]; //The max size of an uint64_t should be 20 characters
    xmlDocPtr doc;
    ds3_bulk_object_response* obj;
    xmlNodePtr objects_node, object_node;
    size_t obj_index;

    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    objects_node = xmlNewNode(NULL, (xmlChar*) "Objects");

    if (list_type == BULK_GET) {
        xmlSetProp(objects_node, (xmlChar*) "ChunkClientProcessingOrderGuarantee", (xmlChar*) _get_ds3_job_chunk_client_processing_order_guarantee_str(order));
    }

    for (obj_index = 0; obj_index < obj_list->num_objects; obj_index++) {
        obj = obj_list->objects[obj_index];
        g_snprintf(size_buff, sizeof(char) * LENGTH_BUFF_SIZE, "%llu", (unsigned long long int) obj->length);

        object_node = xmlNewNode(NULL, (xmlChar*) "Object");
        xmlAddChild(objects_node, object_node);

        xmlSetProp(object_node, (xmlChar*) "Name", (xmlChar*) obj->name->value);
        if (list_type == BULK_PUT) {
            xmlSetProp(object_node, (xmlChar*) "Size", (xmlChar*) size_buff);
        }
    }

    xmlDocSetRootElement(doc, objects_node);

    return doc;
}

static xmlDocPtr _generate_xml_complete_mpu(const ds3_complete_multipart_upload_response* mpu_list) {
    char size_buff[LENGTH_BUFF_SIZE]; //The max size of an uint64_t should be 20 characters
    xmlDocPtr doc;
    ds3_multipart_upload_part_response* part;
    xmlNodePtr parts_node, part_node;
    int part_num;

    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    parts_node = xmlNewNode(NULL, (xmlChar*) "CompleteMultipartUpload");

    for (part_num = 0; part_num < mpu_list->num_parts; part_num++) {
        part = mpu_list->parts[part_num];

        part_node = xmlNewNode(NULL, (xmlChar*) "Part");
        xmlAddChild(parts_node, part_node);

        g_snprintf(size_buff, sizeof(char) * LENGTH_BUFF_SIZE, "%d", part->part_number);
        xmlNewTextChild(part_node, NULL, (xmlChar*) "PartNumber", (xmlChar*) size_buff);

        xmlNewTextChild(part_node, NULL, (xmlChar*) "ETag", (xmlChar*) part->etag->value);
    }

    xmlDocSetRootElement(doc, parts_node);
    return doc;
}

static xmlDocPtr _generate_xml_delete_objects(ds3_delete_objects_response* keys_list) {
    xmlDocPtr doc;
    ds3_str* key;
    xmlNodePtr del_node, obj_node;
    int key_num;

    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    del_node = xmlNewNode(NULL, (xmlChar*) "Delete");

    for (key_num = 0; key_num < keys_list->num_strings; key_num++) {
        key = keys_list->strings_list[key_num];

        obj_node = xmlNewNode(NULL, (xmlChar*) "Object");
        xmlAddChild(del_node, obj_node);

        xmlNewTextChild(obj_node, NULL, (xmlChar*) "Key", (xmlChar*) key->value);
    }

    xmlDocSetRootElement(doc, del_node);
    return doc;
}

static ds3_error* _init_request_payload(const ds3_request* _request,
                                        ds3_xml_send_buff* send_buff,
                                        const object_list_type operation_type) {
    xmlDocPtr doc;

    struct _ds3_request* request = (struct _ds3_request*) _request;

    // Clear send_buff
    memset(send_buff, 0, sizeof(ds3_xml_send_buff));

    switch(operation_type) {
        case BULK_PUT:
        case BULK_GET:
        case GET_PHYSICAL_PLACEMENT:
            if (request->object_list == NULL || request->object_list->num_objects == 0) {
                return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
            }
            doc = _generate_xml_bulk_objects_list(request->object_list, operation_type, request->chunk_ordering);
            break;

        case COMPLETE_MPU:
            if (request->mpu_list == NULL || request->mpu_list->num_parts == 0) {
                return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The complete multipart upload command requires a list of objects to process");
            }
            doc = _generate_xml_complete_mpu(request->mpu_list);
            break;

        case BULK_DELETE:
        case STRING_LIST:
            if (request->delete_objects == NULL || request->delete_objects->num_strings == 0) {
                return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The delete objects command requires a list of objects to process");
            }
            doc = _generate_xml_delete_objects(request->delete_objects);
            break;

        case STRING: // *** not XML - do not interpret
            send_buff->buff = request->delete_objects->strings_list[0]->value;
            send_buff->size = request->delete_objects->strings_list[0]->size;
            request->length = send_buff->size;
            return NULL;
            break;

        default:
            return ds3_create_error(DS3_ERROR_INVALID_XML, "Unknown request payload type");
    }

    xmlDocDumpFormatMemory(doc, (xmlChar**) &send_buff->buff, (int*) &send_buff->size, 1);
    request->length = send_buff->size; // make sure to set the size of the request.

    xmlFreeDoc(doc);

    return NULL;
}


static ds3_auto_inspect_mode _match_ds3_auto_inspect_mode(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NEVER") == 0) {
        return DS3_AUTO_INSPECT_MODE_NEVER;
    } else if (xmlStrcmp(text, (const xmlChar*) "MINIMAL") == 0) {
        return DS3_AUTO_INSPECT_MODE_MINIMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "DEFAULT") == 0) {
        return DS3_AUTO_INSPECT_MODE_DEFAULT;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_AUTO_INSPECT_MODE_NEVER for safety.", text);
        return DS3_AUTO_INSPECT_MODE_NEVER;
    }
}
static ds3_priority _match_ds3_priority(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CRITICAL") == 0) {
        return DS3_PRIORITY_CRITICAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "URGENT") == 0) {
        return DS3_PRIORITY_URGENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "HIGH") == 0) {
        return DS3_PRIORITY_HIGH;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_PRIORITY_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOW") == 0) {
        return DS3_PRIORITY_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "BACKGROUND") == 0) {
        return DS3_PRIORITY_BACKGROUND;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_PRIORITY_CRITICAL for safety.", text);
        return DS3_PRIORITY_CRITICAL;
    }
}
static ds3_bucket_acl_permission _match_ds3_bucket_acl_permission(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "LIST") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_LIST;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_READ;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_WRITE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DELETE") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_DELETE;
    } else if (xmlStrcmp(text, (const xmlChar*) "JOB") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_JOB;
    } else if (xmlStrcmp(text, (const xmlChar*) "OWNER") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_OWNER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_BUCKET_ACL_PERMISSION_LIST for safety.", text);
        return DS3_BUCKET_ACL_PERMISSION_LIST;
    }
}
static ds3_data_isolation_level _match_ds3_data_isolation_level(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "STANDARD") == 0) {
        return DS3_DATA_ISOLATION_LEVEL_STANDARD;
    } else if (xmlStrcmp(text, (const xmlChar*) "BUCKET_ISOLATED") == 0) {
        return DS3_DATA_ISOLATION_LEVEL_BUCKET_ISOLATED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_ISOLATION_LEVEL_STANDARD for safety.", text);
        return DS3_DATA_ISOLATION_LEVEL_STANDARD;
    }
}
static ds3_data_persistence_rule_state _match_ds3_data_persistence_rule_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "INCLUSION_IN_PROGRESS") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_STATE_INCLUSION_IN_PROGRESS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_PERSISTENCE_RULE_STATE_NORMAL for safety.", text);
        return DS3_DATA_PERSISTENCE_RULE_STATE_NORMAL;
    }
}
static ds3_data_persistence_rule_type _match_ds3_data_persistence_rule_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PERMANENT") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "TEMPORARY") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_TYPE_TEMPORARY;
    } else if (xmlStrcmp(text, (const xmlChar*) "RETIRED") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_TYPE_RETIRED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT for safety.", text);
        return DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT;
    }
}
static ds3_data_replication_rule_type _match_ds3_data_replication_rule_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PERMANENT") == 0) {
        return DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "RETIRED") == 0) {
        return DS3_DATA_REPLICATION_RULE_TYPE_RETIRED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT for safety.", text);
        return DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT;
    }
}
static ds3_job_chunk_blob_store_state _match_ds3_job_chunk_blob_store_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PENDING") == 0) {
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_PROGRESS") == 0) {
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "COMPLETED") == 0) {
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_COMPLETED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_CHUNK_BLOB_STORE_STATE_PENDING for safety.", text);
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_PENDING;
    }
}
static ds3_job_chunk_client_processing_order_guarantee _match_ds3_job_chunk_client_processing_order_guarantee(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_ORDER") == 0) {
        return DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_IN_ORDER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE for safety.", text);
        return DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    }
}
static ds3_job_request_type _match_ds3_job_request_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PUT") == 0) {
        return DS3_JOB_REQUEST_TYPE_PUT;
    } else if (xmlStrcmp(text, (const xmlChar*) "GET") == 0) {
        return DS3_JOB_REQUEST_TYPE_GET;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY") == 0) {
        return DS3_JOB_REQUEST_TYPE_VERIFY;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_REQUEST_TYPE_PUT for safety.", text);
        return DS3_JOB_REQUEST_TYPE_PUT;
    }
}
static ds3_ltfs_file_naming_mode _match_ds3_ltfs_file_naming_mode(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "OBJECT_NAME") == 0) {
        return DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME;
    } else if (xmlStrcmp(text, (const xmlChar*) "OBJECT_ID") == 0) {
        return DS3_LTFS_FILE_NAMING_MODE_OBJECT_ID;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME for safety.", text);
        return DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME;
    }
}
static ds3_s3_object_type _match_ds3_s3_object_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "DATA") == 0) {
        return DS3_S3_OBJECT_TYPE_DATA;
    } else if (xmlStrcmp(text, (const xmlChar*) "FOLDER") == 0) {
        return DS3_S3_OBJECT_TYPE_FOLDER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_S3_OBJECT_TYPE_DATA for safety.", text);
        return DS3_S3_OBJECT_TYPE_DATA;
    }
}
static ds3_storage_domain_failure_type _match_ds3_storage_domain_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ILLEGAL_EJECTION_OCCURRED") == 0) {
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "MEMBER_BECAME_READ_ONLY") == 0) {
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_MEMBER_BECAME_READ_ONLY;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITES_STALLED_DUE_TO_NO_FREE_MEDIA_REMAINING") == 0) {
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_WRITES_STALLED_DUE_TO_NO_FREE_MEDIA_REMAINING;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED for safety.", text);
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED;
    }
}
static ds3_storage_domain_member_state _match_ds3_storage_domain_member_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "EXCLUSION_IN_PROGRESS") == 0) {
        return DS3_STORAGE_DOMAIN_MEMBER_STATE_EXCLUSION_IN_PROGRESS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL for safety.", text);
        return DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL;
    }
}
static ds3_system_failure_type _match_ds3_system_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "RECONCILE_TAPE_ENVIRONMENT_FAILED") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "RECONCILE_POOL_ENVIRONMENT_FAILED") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_RECONCILE_POOL_ENVIRONMENT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED for safety.", text);
        return DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED;
    }
}
static ds3_unavailable_media_usage_policy _match_ds3_unavailable_media_usage_policy(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ALLOW") == 0) {
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "DISCOURAGED") == 0) {
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISCOURAGED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DISALLOW") == 0) {
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISALLOW;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW for safety.", text);
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW;
    }
}
static ds3_versioning_level _match_ds3_versioning_level(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return DS3_VERSIONING_LEVEL_NONE;
    } else if (xmlStrcmp(text, (const xmlChar*) "KEEP_LATEST") == 0) {
        return DS3_VERSIONING_LEVEL_KEEP_LATEST;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_VERSIONING_LEVEL_NONE for safety.", text);
        return DS3_VERSIONING_LEVEL_NONE;
    }
}
static ds3_write_optimization _match_ds3_write_optimization(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CAPACITY") == 0) {
        return DS3_WRITE_OPTIMIZATION_CAPACITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "PERFORMANCE") == 0) {
        return DS3_WRITE_OPTIMIZATION_PERFORMANCE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_WRITE_OPTIMIZATION_CAPACITY for safety.", text);
        return DS3_WRITE_OPTIMIZATION_CAPACITY;
    }
}
static ds3_write_preference_level _match_ds3_write_preference_level(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "HIGH") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_HIGH;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOW") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "NEVER_SELECT") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_NEVER_SELECT;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_WRITE_PREFERENCE_LEVEL_HIGH for safety.", text);
        return DS3_WRITE_PREFERENCE_LEVEL_HIGH;
    }
}
static ds3_pool_failure_type _match_ds3_pool_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "BLOB_READ_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE") == 0) {
        return DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_MISSING") == 0) {
        return DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_FORMAT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_IMPORT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE") == 0) {
        return DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_DATA_INTEGRITY") == 0) {
        return DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "INSPECT_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_INSPECT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "QUIESCED") == 0) {
        return DS3_POOL_FAILURE_TYPE_QUIESCED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_VERIFY_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_WRITE_FAILED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED for safety.", text);
        return DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED;
    }
}
static ds3_pool_health _match_ds3_pool_health(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "OK") == 0) {
        return DS3_POOL_HEALTH_OK;
    } else if (xmlStrcmp(text, (const xmlChar*) "DEGRADED") == 0) {
        return DS3_POOL_HEALTH_DEGRADED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_HEALTH_OK for safety.", text);
        return DS3_POOL_HEALTH_OK;
    }
}
static ds3_pool_state _match_ds3_pool_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "BLANK") == 0) {
        return DS3_POOL_STATE_BLANK;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_POOL_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOST") == 0) {
        return DS3_POOL_STATE_LOST;
    } else if (xmlStrcmp(text, (const xmlChar*) "FOREIGN") == 0) {
        return DS3_POOL_STATE_FOREIGN;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_PENDING") == 0) {
        return DS3_POOL_STATE_IMPORT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_IN_PROGRESS") == 0) {
        return DS3_POOL_STATE_IMPORT_IN_PROGRESS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_STATE_BLANK for safety.", text);
        return DS3_POOL_STATE_BLANK;
    }
}
static ds3_pool_type _match_ds3_pool_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NEARLINE") == 0) {
        return DS3_POOL_TYPE_NEARLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE") == 0) {
        return DS3_POOL_TYPE_ONLINE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_TYPE_NEARLINE for safety.", text);
        return DS3_POOL_TYPE_NEARLINE;
    }
}
static ds3_import_conflict_resolution_mode _match_ds3_import_conflict_resolution_mode(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CANCEL") == 0) {
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL;
    } else if (xmlStrcmp(text, (const xmlChar*) "ACCEPT_MOST_RECENT") == 0) {
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_MOST_RECENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "ACCEPT_EXISTING") == 0) {
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_EXISTING;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL for safety.", text);
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL;
    }
}
static ds3_quiesced _match_ds3_quiesced(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NO") == 0) {
        return DS3_QUIESCED_NO;
    } else if (xmlStrcmp(text, (const xmlChar*) "PENDING") == 0) {
        return DS3_QUIESCED_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "YES") == 0) {
        return DS3_QUIESCED_YES;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_QUIESCED_NO for safety.", text);
        return DS3_QUIESCED_NO;
    }
}
static ds3_import_export_configuration _match_ds3_import_export_configuration(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "SUPPORTED") == 0) {
        return DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED;
    } else if (xmlStrcmp(text, (const xmlChar*) "NOT_SUPPORTED") == 0) {
        return DS3_IMPORT_EXPORT_CONFIGURATION_NOT_SUPPORTED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED for safety.", text);
        return DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED;
    }
}
static ds3_tape_drive_state _match_ds3_tape_drive_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TAPE_DRIVE_STATE_OFFLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_TAPE_DRIVE_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "ERROR") == 0) {
        return DS3_TAPE_DRIVE_STATE_ERROR;
    } else if (xmlStrcmp(text, (const xmlChar*) "NOT_COMPATIBLE_IN_PARTITION_DUE_TO_NEWER_TAPE_DRIVES") == 0) {
        return DS3_TAPE_DRIVE_STATE_NOT_COMPATIBLE_IN_PARTITION_DUE_TO_NEWER_TAPE_DRIVES;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_DRIVE_STATE_OFFLINE for safety.", text);
        return DS3_TAPE_DRIVE_STATE_OFFLINE;
    }
}
static ds3_tape_drive_type _match_ds3_tape_drive_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return DS3_TAPE_DRIVE_TYPE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO5") == 0) {
        return DS3_TAPE_DRIVE_TYPE_LTO5;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO6") == 0) {
        return DS3_TAPE_DRIVE_TYPE_LTO6;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO7") == 0) {
        return DS3_TAPE_DRIVE_TYPE_LTO7;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS1140") == 0) {
        return DS3_TAPE_DRIVE_TYPE_TS1140;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS1150") == 0) {
        return DS3_TAPE_DRIVE_TYPE_TS1150;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_DRIVE_TYPE_UNKNOWN for safety.", text);
        return DS3_TAPE_DRIVE_TYPE_UNKNOWN;
    }
}
static ds3_tape_failure_type _match_ds3_tape_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "BAR_CODE_CHANGED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAR_CODE_DUPLICATE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_BAR_CODE_DUPLICATE;
    } else if (xmlStrcmp(text, (const xmlChar*) "BLOB_READ_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_BLOB_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_MISSING") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "DELAYED_OWNERSHIP_FAILURE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DELAYED_OWNERSHIP_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DRIVE_CLEAN_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DRIVE_CLEAN_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DRIVE_CLEANED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DRIVE_CLEANED;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_FORMAT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "GET_TAPE_INFORMATION_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_GET_TAPE_INFORMATION_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_DATA_INTEGRITY") == 0) {
        return DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "INSPECT_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_INSPECT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "REIMPORT_REQUIRED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_REIMPORT_REQUIRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "SERIAL_NUMBER_MISMATCH") == 0) {
        return DS3_TAPE_FAILURE_TYPE_SERIAL_NUMBER_MISMATCH;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_VERIFY_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_WRITE_FAILED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED for safety.", text);
        return DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED;
    }
}
static ds3_tape_partition_failure_type _match_ds3_tape_partition_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CLEANING_TAPE_REQUIRED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DUPLICATE_TAPE_BAR_CODES_DETECTED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_DUPLICATE_TAPE_BAR_CODES_DETECTED;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_STALLED_DUE_TO_OFFLINE_TAPES") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_EJECT_STALLED_DUE_TO_OFFLINE_TAPES;
    } else if (xmlStrcmp(text, (const xmlChar*) "MINIMUM_DRIVE_COUNT_NOT_MET") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_MINIMUM_DRIVE_COUNT_NOT_MET;
    } else if (xmlStrcmp(text, (const xmlChar*) "MOVE_FAILED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "MOVE_FAILED_DUE_TO_PREPARE_TAPE_FOR_REMOVAL_FAILURE") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED_DUE_TO_PREPARE_TAPE_FOR_REMOVAL_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "NO_USABLE_DRIVES") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_NO_USABLE_DRIVES;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_STALLED_DUE_TO_NO_STORAGE_SLOTS") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_ONLINE_STALLED_DUE_TO_NO_STORAGE_SLOTS;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_DRIVE_IN_ERROR") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_IN_ERROR;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_DRIVE_MISSING") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_DRIVE_TYPE_MISMATCH") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_TYPE_MISMATCH;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_EJECTION_BY_OPERATOR_REQUIRED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_EJECTION_BY_OPERATOR_REQUIRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_MEDIA_TYPE_INCOMPATIBLE") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_MEDIA_TYPE_INCOMPATIBLE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED for safety.", text);
        return DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED;
    }
}
static ds3_tape_partition_state _match_ds3_tape_partition_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ONLINE") == 0) {
        return DS3_TAPE_PARTITION_STATE_ONLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TAPE_PARTITION_STATE_OFFLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "ERROR") == 0) {
        return DS3_TAPE_PARTITION_STATE_ERROR;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_PARTITION_STATE_ONLINE for safety.", text);
        return DS3_TAPE_PARTITION_STATE_ONLINE;
    }
}
static ds3_tape_state _match_ds3_tape_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_TAPE_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TAPE_STATE_OFFLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_PENDING") == 0) {
        return DS3_TAPE_STATE_ONLINE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_ONLINE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "PENDING_INSPECTION") == 0) {
        return DS3_TAPE_STATE_PENDING_INSPECTION;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return DS3_TAPE_STATE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE") == 0) {
        return DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY") == 0) {
        return DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_MISSING") == 0) {
        return DS3_TAPE_STATE_DATA_CHECKPOINT_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTFS_WITH_FOREIGN_DATA") == 0) {
        return DS3_TAPE_STATE_LTFS_WITH_FOREIGN_DATA;
    } else if (xmlStrcmp(text, (const xmlChar*) "FOREIGN") == 0) {
        return DS3_TAPE_STATE_FOREIGN;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_PENDING") == 0) {
        return DS3_TAPE_STATE_IMPORT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_IMPORT_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "INCOMPATIBLE") == 0) {
        return DS3_TAPE_STATE_INCOMPATIBLE;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOST") == 0) {
        return DS3_TAPE_STATE_LOST;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAD") == 0) {
        return DS3_TAPE_STATE_BAD;
    } else if (xmlStrcmp(text, (const xmlChar*) "CANNOT_FORMAT_DUE_TO_WRITE_PROTECTION") == 0) {
        return DS3_TAPE_STATE_CANNOT_FORMAT_DUE_TO_WRITE_PROTECTION;
    } else if (xmlStrcmp(text, (const xmlChar*) "SERIAL_NUMBER_MISMATCH") == 0) {
        return DS3_TAPE_STATE_SERIAL_NUMBER_MISMATCH;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAR_CODE_MISSING") == 0) {
        return DS3_TAPE_STATE_BAR_CODE_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_PENDING") == 0) {
        return DS3_TAPE_STATE_FORMAT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_FORMAT_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_TO_EE_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_EJECT_TO_EE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_FROM_EE_PENDING") == 0) {
        return DS3_TAPE_STATE_EJECT_FROM_EE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECTED") == 0) {
        return DS3_TAPE_STATE_EJECTED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_STATE_NORMAL for safety.", text);
        return DS3_TAPE_STATE_NORMAL;
    }
}
static ds3_tape_type _match_ds3_tape_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "LTO5") == 0) {
        return DS3_TAPE_TYPE_LTO5;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO6") == 0) {
        return DS3_TAPE_TYPE_LTO6;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO7") == 0) {
        return DS3_TAPE_TYPE_LTO7;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO_CLEANING_TAPE") == 0) {
        return DS3_TAPE_TYPE_LTO_CLEANING_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JC") == 0) {
        return DS3_TAPE_TYPE_TS_JC;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JY") == 0) {
        return DS3_TAPE_TYPE_TS_JY;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JK") == 0) {
        return DS3_TAPE_TYPE_TS_JK;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JD") == 0) {
        return DS3_TAPE_TYPE_TS_JD;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JZ") == 0) {
        return DS3_TAPE_TYPE_TS_JZ;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JL") == 0) {
        return DS3_TAPE_TYPE_TS_JL;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_CLEANING_TAPE") == 0) {
        return DS3_TAPE_TYPE_TS_CLEANING_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return DS3_TAPE_TYPE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORBIDDEN") == 0) {
        return DS3_TAPE_TYPE_FORBIDDEN;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_TYPE_LTO5 for safety.", text);
        return DS3_TAPE_TYPE_LTO5;
    }
}
static ds3_target_access_control_replication _match_ds3_target_access_control_replication(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE;
    } else if (xmlStrcmp(text, (const xmlChar*) "USERS") == 0) {
        return DS3_TARGET_ACCESS_CONTROL_REPLICATION_USERS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE for safety.", text);
        return DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE;
    }
}
static ds3_target_failure_type _match_ds3_target_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NOT_ONLINE") == 0) {
        return DS3_TARGET_FAILURE_TYPE_NOT_ONLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_WRITE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_INITIATE_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_WRITE_INITIATE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_INITIATE_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_READ_INITIATE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_VERIFY_FAILED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_FAILURE_TYPE_NOT_ONLINE for safety.", text);
        return DS3_TARGET_FAILURE_TYPE_NOT_ONLINE;
    }
}
static ds3_target_read_preference_type _match_ds3_target_read_preference_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "MINIMUM_LATENCY") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY;
    } else if (xmlStrcmp(text, (const xmlChar*) "AFTER_ONLINE_POOL") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_ONLINE_POOL;
    } else if (xmlStrcmp(text, (const xmlChar*) "AFTER_NEARLINE_POOL") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NEARLINE_POOL;
    } else if (xmlStrcmp(text, (const xmlChar*) "AFTER_NON_EJECTABLE_TAPE") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NON_EJECTABLE_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "LAST_RESORT") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_LAST_RESORT;
    } else if (xmlStrcmp(text, (const xmlChar*) "NEVER") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_NEVER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY for safety.", text);
        return DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY;
    }
}
static ds3_target_state _match_ds3_target_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ONLINE") == 0) {
        return DS3_TARGET_STATE_ONLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "LIMITED_ACCESS") == 0) {
        return DS3_TARGET_STATE_LIMITED_ACCESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TARGET_STATE_OFFLINE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_STATE_ONLINE for safety.", text);
        return DS3_TARGET_STATE_ONLINE;
    }
}
static ds3_blob_store_task_state _match_ds3_blob_store_task_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NOT_READY") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_NOT_READY;
    } else if (xmlStrcmp(text, (const xmlChar*) "READY") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_READY;
    } else if (xmlStrcmp(text, (const xmlChar*) "PENDING_EXECUTION") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_PENDING_EXECUTION;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_PROGRESS") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "COMPLETED") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_COMPLETED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_BLOB_STORE_TASK_STATE_NOT_READY for safety.", text);
        return DS3_BLOB_STORE_TASK_STATE_NOT_READY;
    }
}
static ds3_cache_entry_state _match_ds3_cache_entry_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ALLOCATED") == 0) {
        return DS3_CACHE_ENTRY_STATE_ALLOCATED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_CACHE") == 0) {
        return DS3_CACHE_ENTRY_STATE_IN_CACHE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_CACHE_ENTRY_STATE_ALLOCATED for safety.", text);
        return DS3_CACHE_ENTRY_STATE_ALLOCATED;
    }
}
static ds3_job_status _match_ds3_job_status(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "IN_PROGRESS") == 0) {
        return DS3_JOB_STATUS_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "COMPLETED") == 0) {
        return DS3_JOB_STATUS_COMPLETED;
    } else if (xmlStrcmp(text, (const xmlChar*) "CANCELED") == 0) {
        return DS3_JOB_STATUS_CANCELED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_STATUS_IN_PROGRESS for safety.", text);
        return DS3_JOB_STATUS_IN_PROGRESS;
    }
}
static ds3_database_physical_space_state _match_ds3_database_physical_space_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CRITICAL") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_CRITICAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOW") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "NEAR_LOW") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_NEAR_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_NORMAL;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATABASE_PHYSICAL_SPACE_STATE_CRITICAL for safety.", text);
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_CRITICAL;
    }
}
static ds3_http_response_format_type _match_ds3_http_response_format_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "DEFAULT") == 0) {
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT;
    } else if (xmlStrcmp(text, (const xmlChar*) "JSON") == 0) {
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_JSON;
    } else if (xmlStrcmp(text, (const xmlChar*) "XML") == 0) {
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_XML;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT for safety.", text);
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT;
    }
}
static ds3_request_type _match_ds3_request_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "DELETE") == 0) {
        return DS3_REQUEST_TYPE_DELETE;
    } else if (xmlStrcmp(text, (const xmlChar*) "GET") == 0) {
        return DS3_REQUEST_TYPE_GET;
    } else if (xmlStrcmp(text, (const xmlChar*) "HEAD") == 0) {
        return DS3_REQUEST_TYPE_HEAD;
    } else if (xmlStrcmp(text, (const xmlChar*) "POST") == 0) {
        return DS3_REQUEST_TYPE_POST;
    } else if (xmlStrcmp(text, (const xmlChar*) "PUT") == 0) {
        return DS3_REQUEST_TYPE_PUT;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_REQUEST_TYPE_DELETE for safety.", text);
        return DS3_REQUEST_TYPE_DELETE;
    }
}
static ds3_naming_convention_type _match_ds3_naming_convention_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CONCAT_LOWERCASE") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE;
    } else if (xmlStrcmp(text, (const xmlChar*) "CONSTANT") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CONSTANT;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNDERSCORED") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_UNDERSCORED;
    } else if (xmlStrcmp(text, (const xmlChar*) "CAMEL_CASE_WITH_FIRST_LETTER_UPPERCASE") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_UPPERCASE;
    } else if (xmlStrcmp(text, (const xmlChar*) "CAMEL_CASE_WITH_FIRST_LETTER_LOWERCASE") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_LOWERCASE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE for safety.", text);
        return DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE;
    }
}
static ds3_checksum_type _match_ds3_checksum_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CRC_32") == 0) {
        return DS3_CHECKSUM_TYPE_CRC_32;
    } else if (xmlStrcmp(text, (const xmlChar*) "CRC_32C") == 0) {
        return DS3_CHECKSUM_TYPE_CRC_32C;
    } else if (xmlStrcmp(text, (const xmlChar*) "MD5") == 0) {
        return DS3_CHECKSUM_TYPE_MD5;
    } else if (xmlStrcmp(text, (const xmlChar*) "SHA_256") == 0) {
        return DS3_CHECKSUM_TYPE_SHA_256;
    } else if (xmlStrcmp(text, (const xmlChar*) "SHA_512") == 0) {
        return DS3_CHECKSUM_TYPE_SHA_512;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_CHECKSUM_TYPE_CRC_32 for safety.", text);
        return DS3_CHECKSUM_TYPE_CRC_32;
    }
}

//************ STRUCT PARSERS **************
static ds3_error* _parse_ds3_blob_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_blob_response** _response) {
    ds3_blob_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_blob_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ByteOffset")) {
            response->byte_offset = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Checksum")) {
            response->checksum = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ChecksumType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->checksum_type = _match_ds3_checksum_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Length")) {
            response->length = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ObjectId")) {
            response->object_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_blob_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_bucket_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bucket_response** _response) {
    ds3_bucket_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bucket_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Empty")) {
            response->empty = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastPreferredChunkSizeInBytes")) {
            response->last_preferred_chunk_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "LogicalUsedCapacity")) {
            response->logical_used_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_bucket_acl_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bucket_acl_response** _response) {
    ds3_bucket_acl_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bucket_acl_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Permission")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->permission = _match_ds3_bucket_acl_permission(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_canceled_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_canceled_job_response** _response) {
    ds3_canceled_job_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_canceled_job_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CanceledDueToTimeout")) {
            response->canceled_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCanceled")) {
            response->date_canceled = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_canceled_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_completed_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_completed_job_response** _response) {
    ds3_completed_job_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_completed_job_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCompleted")) {
            response->date_completed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_completed_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_persistence_rule_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_persistence_rule_response** _response) {
    ds3_data_persistence_rule_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_persistence_rule_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsolationLevel")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->isolation_level = _match_ds3_data_isolation_level(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MinimumDaysToRetain")) {
            response->minimum_days_to_retain = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_persistence_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_persistence_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_persistence_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_policy_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_policy_response** _response) {
    ds3_data_policy_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_policy_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AlwaysForcePutJobCreation")) {
            response->always_force_put_job_creation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysMinimizeSpanningAcrossMedia")) {
            response->always_minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysReplicateDeletes")) {
            response->always_replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BlobbingEnabled")) {
            response->blobbing_enabled = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChecksumType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->checksum_type = _match_ds3_checksum_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultBlobSize")) {
            response->default_blob_size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "DefaultGetJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_get_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultPutJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_put_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_verify_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "EndToEndCrcRequired")) {
            response->end_to_end_crc_required = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsObjectNamingAllowed")) {
            response->ltfs_object_naming_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RebuildPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->rebuild_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Versioning")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->versioning = _match_ds3_versioning_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_policy_acl_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_policy_acl_response** _response) {
    ds3_data_policy_acl_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_policy_acl_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_replication_rule_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_replication_rule_response** _response) {
    ds3_data_replication_rule_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_replication_rule_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Ds3TargetDataPolicy")) {
            response->ds3_target_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Ds3TargetId")) {
            response->ds3_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_persistence_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_degraded_blob_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_degraded_blob_response** _response) {
    ds3_degraded_blob_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_degraded_blob_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PersistenceRuleId")) {
            response->persistence_rule_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReplicationRuleId")) {
            response->replication_rule_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_degraded_blob_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_group_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_group_response** _response) {
    ds3_group_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_group_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BuiltIn")) {
            response->built_in = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_group_member_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_group_member_response** _response) {
    ds3_group_member_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_group_member_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberGroupId")) {
            response->member_group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberUserId")) {
            response->member_user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_active_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_active_job_response** _response) {
    ds3_active_job_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_active_job_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Aggregating")) {
            response->aggregating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MinimizeSpanningAcrossMedia")) {
            response->minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Replicating")) {
            response->replicating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TruncatedDueToTimeout")) {
            response->truncated_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_active_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_node_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_node_response** _response) {
    ds3_node_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_node_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPathHttpPort")) {
            response->data_path_http_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttpsPort")) {
            response->data_path_https_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathIpAddress")) {
            response->data_path_ip_address = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DnsName")) {
            response->dns_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHeartbeat")) {
            response->last_heartbeat = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_node_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_response** _response) {
    ds3_s3_object_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Latest")) {
            response->latest = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_s3_object_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_response** _response) {
    ds3_storage_domain_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoEjectMediaFullThreshold")) {
            response->auto_eject_media_full_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponCron")) {
            response->auto_eject_upon_cron = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCancellation")) {
            response->auto_eject_upon_job_cancellation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCompletion")) {
            response->auto_eject_upon_job_completion = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponMediaFull")) {
            response->auto_eject_upon_media_full = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsFileNaming")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->ltfs_file_naming = _match_ds3_ltfs_file_naming_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MaxTapeFragmentationPercent")) {
            response->max_tape_fragmentation_percent = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MaximumAutoVerificationFrequencyInDays")) {
            response->maximum_auto_verification_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MediaEjectionAllowed")) {
            response->media_ejection_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecureMediaAllocation")) {
            response->secure_media_allocation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "VerifyPriorToAutoEject")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_prior_to_auto_eject = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteOptimization")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_optimization = _match_ds3_write_optimization(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_capacity_summary_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_capacity_summary_response** _response) {
    ds3_storage_domain_capacity_summary_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_capacity_summary_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PhysicalAllocated")) {
            response->physical_allocated = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "PhysicalFree")) {
            response->physical_free = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "PhysicalUsed")) {
            response->physical_used = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_capacity_summary_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_failure_response** _response) {
    ds3_storage_domain_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_storage_domain_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_member_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_member_response** _response) {
    ds3_storage_domain_member_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_member_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolPartitionId")) {
            response->pool_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_storage_domain_member_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapePartitionId")) {
            response->tape_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->tape_type = _match_ds3_tape_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WritePreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_preference = _match_ds3_write_preference_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_system_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_system_failure_response** _response) {
    ds3_system_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_system_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_system_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_spectra_user_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_spectra_user_response** _response) {
    ds3_spectra_user_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_spectra_user_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AuthId")) {
            response->auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultDataPolicyId")) {
            response->default_data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecretKey")) {
            response->secret_key = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_spectra_user_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_failure_notification_registration_response** _response) {
    ds3_target_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_completed_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_completed_notification_registration_response** _response) {
    ds3_job_completed_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_completed_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_completed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_created_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_created_notification_registration_response** _response) {
    ds3_job_created_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_created_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_created_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_creation_failed_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_creation_failed_notification_registration_response** _response) {
    ds3_job_creation_failed_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_creation_failed_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_creation_failed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_failure_notification_registration_response** _response) {
    ds3_pool_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_cached_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_cached_notification_registration_response** _response) {
    ds3_s3_object_cached_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_cached_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_cached_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_lost_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_lost_notification_registration_response** _response) {
    ds3_s3_object_lost_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_lost_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_lost_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_persisted_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_persisted_notification_registration_response** _response) {
    ds3_s3_object_persisted_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_persisted_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_persisted_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_failure_notification_registration_response** _response) {
    ds3_storage_domain_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_system_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_system_failure_notification_registration_response** _response) {
    ds3_system_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_system_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_failure_notification_registration_response** _response) {
    ds3_tape_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_partition_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_partition_failure_notification_registration_response** _response) {
    ds3_tape_partition_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_partition_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_cache_filesystem_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_cache_filesystem_response** _response) {
    ds3_cache_filesystem_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_cache_filesystem_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoReclaimInitiateThreshold")) {
            response->auto_reclaim_initiate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoReclaimTerminateThreshold")) {
            response->auto_reclaim_terminate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BurstThreshold")) {
            response->burst_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxCapacityInBytes")) {
            response->max_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "MaxPercentUtilizationOfFilesystem")) {
            response->max_percent_utilization_of_filesystem = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "NodeId")) {
            response->node_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Path")) {
            response->path = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_response** _response) {
    ds3_pool_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableCapacity")) {
            response->available_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Guid")) {
            response->guid = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Health")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->health = _match_ds3_pool_health(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Mountpoint")) {
            response->mountpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoweredOn")) {
            response->powered_on = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReservedCapacity")) {
            response->reserved_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_pool_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TotalCapacity")) {
            response->total_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UsedCapacity")) {
            response->used_capacity = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_pool_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_pool_response* response = NULL;
        error = _parse_ds3_pool_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_pool_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_pool_response_array;

    return error;
}
static ds3_error* _parse_ds3_pool_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_failure_response** _response) {
    ds3_pool_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolId")) {
            response->pool_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_partition_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_partition_response** _response) {
    ds3_pool_partition_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_partition_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_pool_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_pool_response** _response) {
    ds3_suspect_blob_pool_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_pool_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateWritten")) {
            response->date_written = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolId")) {
            response->pool_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_pool_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_tape_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_tape_response** _response) {
    ds3_suspect_blob_tape_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_tape_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OrderIndex")) {
            response->order_index = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_tape_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_response** _response) {
    ds3_tape_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableRawCapacity")) {
            response->available_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BarCode")) {
            response->bar_code = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DescriptionForIdentification")) {
            response->description_for_identification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectDate")) {
            response->eject_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLabel")) {
            response->eject_label = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLocation")) {
            response->eject_location = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectPending")) {
            response->eject_pending = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "FullOfData")) {
            response->full_of_data = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCheckpoint")) {
            response->last_checkpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartiallyVerifiedEndOfTape")) {
            response->partially_verified_end_of_tape = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PreviousState")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->previous_state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TakeOwnershipPending")) {
            response->take_ownership_pending = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TotalRawCapacity")) {
            response->total_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "VerifyPending")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_pending = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteProtected")) {
            response->write_protected = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_tape_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_tape_response* response = NULL;
        error = _parse_ds3_tape_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_tape_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_tape_response_array;

    return error;
}
static ds3_error* _parse_ds3_tape_density_directive_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_density_directive_response** _response) {
    ds3_tape_density_directive_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_density_directive_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Density")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->density = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->tape_type = _match_ds3_tape_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_density_directive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_drive_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_drive_response** _response) {
    ds3_tape_drive_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_drive_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ForceTapeRemoval")) {
            response->force_tape_removal = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCleaned")) {
            response->last_cleaned = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_drive_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_drive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_detailed_tape_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_detailed_tape_failure_response** _response) {
    ds3_detailed_tape_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_detailed_tape_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeDriveId")) {
            response->tape_drive_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_tape_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_library_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_library_response** _response) {
    ds3_tape_library_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_library_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ManagementUrl")) {
            response->management_url = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_library_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_partition_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_partition_response** _response) {
    ds3_tape_partition_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_partition_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_partition_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_partition_failure_response** _response) {
    ds3_tape_partition_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_partition_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_partition_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_response** _response) {
    ds3_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccessControlReplication")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->access_control_replication = _match_ds3_target_access_control_replication(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "AdminAuthId")) {
            response->admin_auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AdminSecretKey")) {
            response->admin_secret_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathEndPoint")) {
            response->data_path_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttps")) {
            response->data_path_https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DataPathPort")) {
            response->data_path_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathProxy")) {
            response->data_path_proxy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathVerifyCertificate")) {
            response->data_path_verify_certificate = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReplicatedUserDefaultDataPolicy")) {
            response->replicated_user_default_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_target_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_target_response* response = NULL;
        error = _parse_ds3_target_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_target_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_target_response_array;

    return error;
}
static ds3_error* _parse_ds3_target_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_failure_response** _response) {
    ds3_target_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_target_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_read_preference_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_read_preference_response** _response) {
    ds3_target_read_preference_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_read_preference_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_target_response** _response) {
    ds3_suspect_blob_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Ds3TargetId")) {
            response->ds3_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_build_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_build_information_response** _response) {
    ds3_build_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_build_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Branch")) {
            response->branch = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Revision")) {
            response->revision = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_build_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_set_of_tape_bar_codes_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_set_of_tape_bar_codes_response** _response) {
    ds3_set_of_tape_bar_codes_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* tape_bar_codes_array = g_ptr_array_new();

    response = g_new0(ds3_set_of_tape_bar_codes_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeBarCodes")) {
            xmlNodePtr loop_node;
            GPtrArray* tape_bar_codes_array = g_ptr_array_new();
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* tape_bar_codes = xml_get_string(doc, loop_node);
                g_ptr_array_add(tape_bar_codes_array, tape_bar_codes);
            }
            response->tape_bar_codes = (ds3_str**)tape_bar_codes_array->pdata;
            response->num_tape_bar_codes = tape_bar_codes_array->len;
            g_ptr_array_free(tape_bar_codes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_set_of_tape_bar_codes_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_bar_codes = (ds3_str**)tape_bar_codes_array->pdata;
    response->num_tape_bar_codes = tape_bar_codes_array->len;
    g_ptr_array_free(tape_bar_codes_array, FALSE);

    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tapes_must_be_onlined_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tapes_must_be_onlined_response** _response) {
    ds3_tapes_must_be_onlined_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* tapes_to_online_array = g_ptr_array_new();

    response = g_new0(ds3_tapes_must_be_onlined_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapesToOnline")) {
            ds3_set_of_tape_bar_codes_response* tapes_to_online_response = NULL;
            error = _parse_ds3_set_of_tape_bar_codes_response(client, doc, child_node, &tapes_to_online_response);
            response->tapes_to_online = (ds3_set_of_tape_bar_codes_response**)tapes_to_online_array->pdata;
            g_ptr_array_add(tapes_to_online_array, tapes_to_online_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tapes_must_be_onlined_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tapes_to_online = (ds3_set_of_tape_bar_codes_response**)tapes_to_online_array->pdata;
    response->num_tapes_to_online = tapes_to_online_array->len;
    g_ptr_array_free(tapes_to_online_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tapes_must_be_onlined_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_blob_store_task_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_blob_store_task_information_response** _response) {
    ds3_blob_store_task_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_blob_store_task_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DateScheduled")) {
            response->date_scheduled = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateStarted")) {
            response->date_started = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Description")) {
            response->description = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DriveId")) {
            response->drive_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Ds3TargetId")) {
            response->ds3_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolId")) {
            response->pool_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_blob_store_task_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_blob_store_task_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_cache_entry_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_cache_entry_information_response** _response) {
    ds3_cache_entry_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_cache_entry_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Blob")) {
            error = _parse_ds3_blob_response(client, doc, child_node, &response->blob);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_cache_entry_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_entry_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_entry_information_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_cache_filesystem_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_cache_filesystem_information_response** _response) {
    ds3_cache_filesystem_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* entries_array = g_ptr_array_new();

    response = g_new0(ds3_cache_filesystem_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AvailableCapacityInBytes")) {
            response->available_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CacheFilesystem")) {
            error = _parse_ds3_cache_filesystem_response(client, doc, child_node, &response->cache_filesystem);
        } else if (element_equal(child_node, "Entries")) {
            ds3_cache_entry_information_response* entries_response = NULL;
            error = _parse_ds3_cache_entry_information_response(client, doc, child_node, &entries_response);
            response->entries = (ds3_cache_entry_information_response**)entries_array->pdata;
            g_ptr_array_add(entries_array, entries_response);
        } else if (element_equal(child_node, "Summary")) {
            response->summary = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TotalCapacityInBytes")) {
            response->total_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "UnavailableCapacityInBytes")) {
            response->unavailable_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "UsedCapacityInBytes")) {
            response->used_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->entries = (ds3_cache_entry_information_response**)entries_array->pdata;
    response->num_entries = entries_array->len;
    g_ptr_array_free(entries_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_filesystem_information_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_bucket_details_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bucket_details_response** _response) {
    ds3_bucket_details_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bucket_details_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_details_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_bucket_details_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_bucket_details_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_bucket_details_response* response = NULL;
        error = _parse_ds3_bucket_details_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_bucket_details_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_bucket_details_response_array;

    return error;
}
static ds3_error* _parse_ds3_delete_object_error_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_delete_object_error_response** _response) {
    ds3_delete_object_error_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_delete_object_error_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Code")) {
            response->code = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Message")) {
            response->message = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_delete_object_error_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_multi_part_upload_part_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_multi_part_upload_part_response** _response) {
    ds3_multi_part_upload_part_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_multi_part_upload_part_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartNumber")) {
            response->part_number = xml_get_uint16(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_multi_part_upload_part_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_node_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_node_response** _response) {
    ds3_job_node_response* response;
    struct _xmlAttr* attribute;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_node_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "EndPoint") == true) {
            response->end_point = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "HttpPort") == true) {
            response->http_port = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "HttpsPort") == true) {
            response->https_port = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Id") == true) {
            response->id = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_job_node_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }



    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_node_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_job_node_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_job_node_response* response = NULL;
        error = _parse_ds3_job_node_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_job_node_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_job_node_response_array;

    return error;
}
static ds3_error* _parse_ds3_s3_object_to_delete_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_to_delete_response** _response) {
    ds3_s3_object_to_delete_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_to_delete_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_to_delete_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_user_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_user_response** _response) {
    ds3_user_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_user_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DisplayName")) {
            response->display_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ID")) {
            response->id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_user_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_named_detailed_tape_partition_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_named_detailed_tape_partition_response** _response) {
    ds3_named_detailed_tape_partition_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_named_detailed_tape_partition_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DriveTypes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            GByteArray* enum_array = g_byte_array_new();
            ds3_tape_drive_type drive_types;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                xmlChar* text = xmlNodeListGetString(doc, loop_node, 1);
                if (text == NULL) {
                    continue;
                }
                drive_types = _match_ds3_tape_drive_type(client->log, text);
                g_byte_array_append(enum_array, (const guint8*) &drive_types, sizeof(ds3_tape_drive_type));
            }
            response->drive_types = (ds3_tape_drive_type*)enum_array->data;
            response->num_drive_types = enum_array->len;
            g_byte_array_free(enum_array, FALSE);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeTypes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            GByteArray* enum_array = g_byte_array_new();
            ds3_tape_type tape_types;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                xmlChar* text = xmlNodeListGetString(doc, loop_node, 1);
                if (text == NULL) {
                    continue;
                }
                tape_types = _match_ds3_tape_type(client->log, text);
                g_byte_array_append(enum_array, (const guint8*) &tape_types, sizeof(ds3_tape_type));
            }
            response->tape_types = (ds3_tape_type*)enum_array->data;
            response->num_tape_types = enum_array->len;
            g_byte_array_free(enum_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_named_detailed_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_named_detailed_tape_partition_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_tape_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_failure_response** _response) {
    ds3_tape_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Cause")) {
            response->cause = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Tape")) {
            error = _parse_ds3_tape_response(client, doc, child_node, &response->tape);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_type_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_type_response** _response) {
    ds3_type_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_type_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BeansRetrieverName")) {
            response->beans_retriever_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DomainName")) {
            response->domain_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NumberOfType")) {
            response->number_of_type = xml_get_uint16(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_type_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_physical_placement_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_physical_placement_response** _response) {
    ds3_physical_placement_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_physical_placement_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3Targets")) {
            GPtrArray* ds3_targets_array;
            error = _parse_ds3_target_response_array(client, doc, child_node, &ds3_targets_array);
            response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
            response->num_ds3_targets = ds3_targets_array->len;
            g_ptr_array_free(ds3_targets_array, FALSE);
        } else if (element_equal(child_node, "Pools")) {
            GPtrArray* pools_array;
            error = _parse_ds3_pool_response_array(client, doc, child_node, &pools_array);
            response->pools = (ds3_pool_response**)pools_array->pdata;
            response->num_pools = pools_array->len;
            g_ptr_array_free(pools_array, FALSE);
        } else if (element_equal(child_node, "Tapes")) {
            GPtrArray* tapes_array;
            error = _parse_ds3_tape_response_array(client, doc, child_node, &tapes_array);
            response->tapes = (ds3_tape_response**)tapes_array->pdata;
            response->num_tapes = tapes_array->len;
            g_ptr_array_free(tapes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_physical_placement_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_physical_placement_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_bulk_object_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bulk_object_response** _response) {
    ds3_bulk_object_response* response;
    struct _xmlAttr* attribute;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bulk_object_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Bucket") == true) {
            response->bucket = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Id") == true) {
            response->id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "InCache") == true) {
            response->in_cache = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Latest") == true) {
            response->latest = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Length") == true) {
            response->length = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Name") == true) {
            response->name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Offset") == true) {
            response->offset = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Version") == true) {
            response->version = xml_get_uint64_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_bulk_object_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PhysicalPlacement")) {
            error = _parse_ds3_physical_placement_response(client, doc, child_node, &response->physical_placement);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bulk_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bulk_object_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_bulk_object_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_bulk_object_response* response = NULL;
        error = _parse_ds3_bulk_object_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_bulk_object_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_bulk_object_response_array;

    return error;
}
static ds3_error* _parse_ds3_bulk_object_list_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bulk_object_list_response** _response) {
    ds3_bulk_object_list_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    response = g_new0(ds3_bulk_object_list_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bulk_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bulk_object_list_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_response** _response) {
    ds3_job_response* response;
    struct _xmlAttr* attribute;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Aggregating") == true) {
            response->aggregating = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "BucketName") == true) {
            response->bucket_name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "CachedSizeInBytes") == true) {
            response->cached_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkClientProcessingOrderGuarantee") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "CompletedSizeInBytes") == true) {
            response->completed_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "EntirelyInCache") == true) {
            response->entirely_in_cache = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "JobId") == true) {
            response->job_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Naked") == true) {
            response->naked = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Name") == true) {
            response->name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "OriginalSizeInBytes") == true) {
            response->original_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Priority") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "RequestType") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "StartDate") == true) {
            response->start_date = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Status") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->status = _match_ds3_job_status(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "UserId") == true) {
            response->user_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "UserName") == true) {
            response->user_name = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_job_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Nodes")) {
            GPtrArray* nodes_array;
            error = _parse_ds3_job_node_response_array(client, doc, child_node, &nodes_array);
            response->nodes = (ds3_job_node_response**)nodes_array->pdata;
            response->num_nodes = nodes_array->len;
            g_ptr_array_free(nodes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_job_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_job_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_job_response* response = NULL;
        error = _parse_ds3_job_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_job_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_job_response_array;

    return error;
}
static ds3_error* _parse_ds3_objects_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_objects_response** _response) {
    ds3_objects_response* response;
    struct _xmlAttr* attribute;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    response = g_new0(ds3_objects_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "ChunkId") == true) {
            response->chunk_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkNumber") == true) {
            response->chunk_number = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "NodeId") == true) {
            response->node_id = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_objects_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_objects_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_objects_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_multi_part_upload_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_multi_part_upload_response** _response) {
    ds3_multi_part_upload_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_multi_part_upload_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Initiated")) {
            response->initiated = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else if (element_equal(child_node, "UploadId")) {
            response->upload_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_multi_part_upload_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_multi_part_upload_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_contents_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_contents_response** _response) {
    ds3_contents_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_contents_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else if (element_equal(child_node, "Size")) {
            response->size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "StorageClass")) {
            response->storage_class = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_contents_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_contents_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_detailed_s3_object_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_detailed_s3_object_response** _response) {
    ds3_detailed_s3_object_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_detailed_s3_object_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Blobs")) {
            error = _parse_ds3_bulk_object_list_response(client, doc, child_node, &response->blobs);
        } else if (element_equal(child_node, "BlobsBeingPersisted")) {
            response->blobs_being_persisted = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BlobsDegraded")) {
            response->blobs_degraded = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BlobsInCache")) {
            response->blobs_in_cache = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BlobsTotal")) {
            response->blobs_total = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Latest")) {
            response->latest = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            response->owner = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Size")) {
            response->size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_s3_object_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_s3_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_s3_object_response_free(response);
    }

    return error;
}


//************ TOP LEVEL STRUCT PARSERS **************
static ds3_error* _parse_top_level_ds3_bucket_response(const ds3_client* client, const ds3_request* request, ds3_bucket_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Empty")) {
            response->empty = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastPreferredChunkSizeInBytes")) {
            response->last_preferred_chunk_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "LogicalUsedCapacity")) {
            response->logical_used_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bucket_acl_response(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_acl_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_acl_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Permission")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->permission = _match_ds3_bucket_acl_permission(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_acl_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_canceled_job_response(const ds3_client* client, const ds3_request* request, ds3_canceled_job_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_canceled_job_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_canceled_job_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CanceledDueToTimeout")) {
            response->canceled_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCanceled")) {
            response->date_canceled = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_canceled_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_canceled_job_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_completed_job_response(const ds3_client* client, const ds3_request* request, ds3_completed_job_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_completed_job_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_completed_job_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCompleted")) {
            response->date_completed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_completed_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_completed_job_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_path_backend_response(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_path_backend_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_path_backend_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Activated")) {
            response->activated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoActivateTimeoutInMins")) {
            response->auto_activate_timeout_in_mins = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "AutoInspect")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->auto_inspect = _match_ds3_auto_inspect_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultImportConflictResolutionMode")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_import_conflict_resolution_mode = _match_ds3_import_conflict_resolution_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyDataAfterImport")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_verify_data_after_import = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyDataPriorToImport")) {
            response->default_verify_data_prior_to_import = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "InstanceId")) {
            response->instance_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHeartbeat")) {
            response->last_heartbeat = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartiallyVerifyLastPercentOfTapes")) {
            response->partially_verify_last_percent_of_tapes = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UnavailableMediaPolicy")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->unavailable_media_policy = _match_ds3_unavailable_media_usage_policy(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UnavailablePoolMaxJobRetryInMins")) {
            response->unavailable_pool_max_job_retry_in_mins = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UnavailableTapePartitionMaxJobRetryInMins")) {
            response->unavailable_tape_partition_max_job_retry_in_mins = xml_get_uint16(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_path_backend_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_path_backend_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_persistence_rule_response(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_persistence_rule_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_persistence_rule_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsolationLevel")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->isolation_level = _match_ds3_data_isolation_level(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MinimumDaysToRetain")) {
            response->minimum_days_to_retain = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_persistence_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_persistence_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_persistence_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_persistence_rule_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AlwaysForcePutJobCreation")) {
            response->always_force_put_job_creation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysMinimizeSpanningAcrossMedia")) {
            response->always_minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysReplicateDeletes")) {
            response->always_replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BlobbingEnabled")) {
            response->blobbing_enabled = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChecksumType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->checksum_type = _match_ds3_checksum_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultBlobSize")) {
            response->default_blob_size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "DefaultGetJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_get_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultPutJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_put_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_verify_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "EndToEndCrcRequired")) {
            response->end_to_end_crc_required = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsObjectNamingAllowed")) {
            response->ltfs_object_naming_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RebuildPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->rebuild_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Versioning")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->versioning = _match_ds3_versioning_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_acl_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_acl_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_acl_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_acl_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_replication_rule_response(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_replication_rule_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_replication_rule_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Ds3TargetDataPolicy")) {
            response->ds3_target_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Ds3TargetId")) {
            response->ds3_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_persistence_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_replication_rule_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_response(const ds3_client* client, const ds3_request* request, ds3_group_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BuiltIn")) {
            response->built_in = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_member_response(const ds3_client* client, const ds3_request* request, ds3_group_member_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_member_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_member_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberGroupId")) {
            response->member_group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberUserId")) {
            response->member_user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_member_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_active_job_response(const ds3_client* client, const ds3_request* request, ds3_active_job_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_active_job_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_active_job_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Aggregating")) {
            response->aggregating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MinimizeSpanningAcrossMedia")) {
            response->minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Replicating")) {
            response->replicating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TruncatedDueToTimeout")) {
            response->truncated_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_active_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_active_job_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_chunk_response(const ds3_client* client, const ds3_request* request, ds3_job_chunk_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_chunk_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_chunk_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobStoreState")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->blob_store_state = _match_ds3_job_chunk_blob_store_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ChunkNumber")) {
            response->chunk_number = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobCreationDate")) {
            response->job_creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NodeId")) {
            response->node_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PendingTargetCommit")) {
            response->pending_target_commit = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ReadFromDs3TargetId")) {
            response->read_from_ds3_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadFromPoolId")) {
            response->read_from_pool_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadFromTapeId")) {
            response->read_from_tape_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_chunk_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_chunk_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_node_response(const ds3_client* client, const ds3_request* request, ds3_node_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_node_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_node_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPathHttpPort")) {
            response->data_path_http_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttpsPort")) {
            response->data_path_https_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathIpAddress")) {
            response->data_path_ip_address = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DnsName")) {
            response->dns_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHeartbeat")) {
            response->last_heartbeat = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_node_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_node_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Latest")) {
            response->latest = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_s3_object_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoEjectMediaFullThreshold")) {
            response->auto_eject_media_full_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponCron")) {
            response->auto_eject_upon_cron = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCancellation")) {
            response->auto_eject_upon_job_cancellation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCompletion")) {
            response->auto_eject_upon_job_completion = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponMediaFull")) {
            response->auto_eject_upon_media_full = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsFileNaming")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->ltfs_file_naming = _match_ds3_ltfs_file_naming_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MaxTapeFragmentationPercent")) {
            response->max_tape_fragmentation_percent = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MaximumAutoVerificationFrequencyInDays")) {
            response->maximum_auto_verification_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MediaEjectionAllowed")) {
            response->media_ejection_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecureMediaAllocation")) {
            response->secure_media_allocation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "VerifyPriorToAutoEject")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_prior_to_auto_eject = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteOptimization")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_optimization = _match_ds3_write_optimization(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_member_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_member_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_member_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolPartitionId")) {
            response->pool_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_storage_domain_member_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapePartitionId")) {
            response->tape_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->tape_type = _match_ds3_tape_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WritePreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_preference = _match_ds3_write_preference_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_member_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_spectra_user_response(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_spectra_user_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_spectra_user_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AuthId")) {
            response->auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultDataPolicyId")) {
            response->default_data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecretKey")) {
            response->secret_key = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_spectra_user_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_spectra_user_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_completed_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_completed_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_completed_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_completed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_completed_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_created_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_created_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_created_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_created_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_created_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_creation_failed_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_creation_failed_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_creation_failed_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_creation_failed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_creation_failed_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_cached_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_cached_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_cached_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_cached_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_cached_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_lost_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_lost_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_lost_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_lost_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_lost_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_persisted_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_persisted_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_persisted_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_persisted_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_persisted_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_cache_filesystem_response(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_cache_filesystem_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_cache_filesystem_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoReclaimInitiateThreshold")) {
            response->auto_reclaim_initiate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoReclaimTerminateThreshold")) {
            response->auto_reclaim_terminate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BurstThreshold")) {
            response->burst_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxCapacityInBytes")) {
            response->max_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "MaxPercentUtilizationOfFilesystem")) {
            response->max_percent_utilization_of_filesystem = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "NodeId")) {
            response->node_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Path")) {
            response->path = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_filesystem_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_response(const ds3_client* client, const ds3_request* request, ds3_pool_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableCapacity")) {
            response->available_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Guid")) {
            response->guid = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Health")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->health = _match_ds3_pool_health(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Mountpoint")) {
            response->mountpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoweredOn")) {
            response->powered_on = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReservedCapacity")) {
            response->reserved_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_pool_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TotalCapacity")) {
            response->total_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UsedCapacity")) {
            response->used_capacity = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_partition_response(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_partition_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_partition_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_partition_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_response(const ds3_client* client, const ds3_request* request, ds3_tape_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableRawCapacity")) {
            response->available_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BarCode")) {
            response->bar_code = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DescriptionForIdentification")) {
            response->description_for_identification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectDate")) {
            response->eject_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLabel")) {
            response->eject_label = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLocation")) {
            response->eject_location = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectPending")) {
            response->eject_pending = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "FullOfData")) {
            response->full_of_data = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCheckpoint")) {
            response->last_checkpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartiallyVerifiedEndOfTape")) {
            response->partially_verified_end_of_tape = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PreviousState")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->previous_state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TakeOwnershipPending")) {
            response->take_ownership_pending = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TotalRawCapacity")) {
            response->total_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "VerifyPending")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_pending = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteProtected")) {
            response->write_protected = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_density_directive_response(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_density_directive_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_density_directive_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Density")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->density = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->tape_type = _match_ds3_tape_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_density_directive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_density_directive_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_drive_response(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_drive_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_drive_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ForceTapeRemoval")) {
            response->force_tape_removal = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCleaned")) {
            response->last_cleaned = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_drive_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_drive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_drive_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_library_response(const ds3_client* client, const ds3_request* request, ds3_tape_library_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_library_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_library_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ManagementUrl")) {
            response->management_url = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_library_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_library_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_response(const ds3_client* client, const ds3_request* request, ds3_target_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccessControlReplication")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->access_control_replication = _match_ds3_target_access_control_replication(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "AdminAuthId")) {
            response->admin_auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AdminSecretKey")) {
            response->admin_secret_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathEndPoint")) {
            response->data_path_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttps")) {
            response->data_path_https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DataPathPort")) {
            response->data_path_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathProxy")) {
            response->data_path_proxy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathVerifyCertificate")) {
            response->data_path_verify_certificate = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReplicatedUserDefaultDataPolicy")) {
            response->replicated_user_default_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_read_preference_response(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_read_preference_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_read_preference_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_read_preference_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_blob_store_tasks_information_response(const ds3_client* client, const ds3_request* request, ds3_blob_store_tasks_information_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_blob_store_tasks_information_response* response;
    ds3_error* error = NULL;
    GPtrArray* tasks_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_blob_store_tasks_information_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Tasks")) {
            ds3_blob_store_task_information_response* tasks_response = NULL;
            error = _parse_ds3_blob_store_task_information_response(client, doc, child_node, &tasks_response);
            response->tasks = (ds3_blob_store_task_information_response**)tasks_array->pdata;
            g_ptr_array_add(tasks_array, tasks_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_blob_store_tasks_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tasks = (ds3_blob_store_task_information_response**)tasks_array->pdata;
    response->num_tasks = tasks_array->len;
    g_ptr_array_free(tasks_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_blob_store_tasks_information_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_cache_information_response(const ds3_client* client, const ds3_request* request, ds3_cache_information_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_cache_information_response* response;
    ds3_error* error = NULL;
    GPtrArray* filesystems_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_cache_information_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Filesystems")) {
            ds3_cache_filesystem_information_response* filesystems_response = NULL;
            error = _parse_ds3_cache_filesystem_information_response(client, doc, child_node, &filesystems_response);
            response->filesystems = (ds3_cache_filesystem_information_response**)filesystems_array->pdata;
            g_ptr_array_add(filesystems_array, filesystems_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->filesystems = (ds3_cache_filesystem_information_response**)filesystems_array->pdata;
    response->num_filesystems = filesystems_array->len;
    g_ptr_array_free(filesystems_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_information_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_complete_multipart_upload_result_response(const ds3_client* client, const ds3_request* request, ds3_complete_multipart_upload_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_complete_multipart_upload_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "CompleteMultipartUploadResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_complete_multipart_upload_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Location")) {
            response->location = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_complete_multipart_upload_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_complete_multipart_upload_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_detailed_tape_partition_response(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_partition_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_detailed_tape_partition_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_detailed_tape_partition_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DriveTypes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            GByteArray* enum_array = g_byte_array_new();
            ds3_tape_drive_type drive_types;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                xmlChar* text = xmlNodeListGetString(doc, loop_node, 1);
                if (text == NULL) {
                    continue;
                }
                drive_types = _match_ds3_tape_drive_type(client->log, text);
                g_byte_array_append(enum_array, (const guint8*) &drive_types, sizeof(ds3_tape_drive_type));
            }
            response->drive_types = (ds3_tape_drive_type*)enum_array->data;
            response->num_drive_types = enum_array->len;
            g_byte_array_free(enum_array, FALSE);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeTypes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            GByteArray* enum_array = g_byte_array_new();
            ds3_tape_type tape_types;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                xmlChar* text = xmlNodeListGetString(doc, loop_node, 1);
                if (text == NULL) {
                    continue;
                }
                tape_types = _match_ds3_tape_type(client->log, text);
                g_byte_array_append(enum_array, (const guint8*) &tape_types, sizeof(ds3_tape_type));
            }
            response->tape_types = (ds3_tape_type*)enum_array->data;
            response->num_tape_types = enum_array->len;
            g_byte_array_free(enum_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_tape_partition_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_initiate_multipart_upload_result_response(const ds3_client* client, const ds3_request* request, ds3_initiate_multipart_upload_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_initiate_multipart_upload_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "InitiateMultipartUploadResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_initiate_multipart_upload_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UploadId")) {
            response->upload_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_initiate_multipart_upload_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_initiate_multipart_upload_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_information_response(const ds3_client* client, const ds3_request* request, ds3_system_information_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_information_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_information_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ApiVersion")) {
            response->api_version = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BackendActivated")) {
            response->backend_activated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BuildInformation")) {
            error = _parse_ds3_build_information_response(client, doc, child_node, &response->build_information);
        } else if (element_equal(child_node, "InstanceId")) {
            response->instance_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Now")) {
            response->now = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_information_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_health_verification_result_response(const ds3_client* client, const ds3_request* request, ds3_health_verification_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_health_verification_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_health_verification_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DatabaseFilesystemFreeSpace")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->database_filesystem_free_space = _match_ds3_database_physical_space_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MsRequiredToVerifyDataPlannerHealth")) {
            response->ms_required_to_verify_data_planner_health = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_health_verification_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_health_verification_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Failure")) {
            ds3_tape_failure_response* failures_response = NULL;
            error = _parse_ds3_tape_failure_response(client, doc, child_node, &failures_response);
            response->failures = (ds3_tape_failure_response**)failures_array->pdata;
            g_ptr_array_add(failures_array, failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->failures = (ds3_tape_failure_response**)failures_array->pdata;
    response->num_failures = failures_array->len;
    g_ptr_array_free(failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bucket_acl_list_response(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_acl_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* bucket_acls_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_acl_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketAcl")) {
            ds3_bucket_acl_response* bucket_acls_response = NULL;
            error = _parse_ds3_bucket_acl_response(client, doc, child_node, &bucket_acls_response);
            response->bucket_acls = (ds3_bucket_acl_response**)bucket_acls_array->pdata;
            g_ptr_array_add(bucket_acls_array, bucket_acls_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_acl_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->bucket_acls = (ds3_bucket_acl_response**)bucket_acls_array->pdata;
    response->num_bucket_acls = bucket_acls_array->len;
    g_ptr_array_free(bucket_acls_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_acl_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_acl_list_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_acl_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* data_policy_acls_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_acl_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyAcl")) {
            ds3_data_policy_acl_response* data_policy_acls_response = NULL;
            error = _parse_ds3_data_policy_acl_response(client, doc, child_node, &data_policy_acls_response);
            response->data_policy_acls = (ds3_data_policy_acl_response**)data_policy_acls_array->pdata;
            g_ptr_array_add(data_policy_acls_array, data_policy_acls_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_acl_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->data_policy_acls = (ds3_data_policy_acl_response**)data_policy_acls_array->pdata;
    response->num_data_policy_acls = data_policy_acls_array->len;
    g_ptr_array_free(data_policy_acls_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_acl_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bucket_list_response(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* buckets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            ds3_bucket_response* buckets_response = NULL;
            error = _parse_ds3_bucket_response(client, doc, child_node, &buckets_response);
            response->buckets = (ds3_bucket_response**)buckets_array->pdata;
            g_ptr_array_add(buckets_array, buckets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->buckets = (ds3_bucket_response**)buckets_array->pdata;
    response->num_buckets = buckets_array->len;
    g_ptr_array_free(buckets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_cache_filesystem_list_response(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_cache_filesystem_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* cache_filesystems_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_cache_filesystem_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CacheFilesystem")) {
            ds3_cache_filesystem_response* cache_filesystems_response = NULL;
            error = _parse_ds3_cache_filesystem_response(client, doc, child_node, &cache_filesystems_response);
            response->cache_filesystems = (ds3_cache_filesystem_response**)cache_filesystems_array->pdata;
            g_ptr_array_add(cache_filesystems_array, cache_filesystems_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->cache_filesystems = (ds3_cache_filesystem_response**)cache_filesystems_array->pdata;
    response->num_cache_filesystems = cache_filesystems_array->len;
    g_ptr_array_free(cache_filesystems_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_filesystem_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_persistence_rule_list_response(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_persistence_rule_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* data_persistence_rules_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_persistence_rule_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPersistenceRule")) {
            ds3_data_persistence_rule_response* data_persistence_rules_response = NULL;
            error = _parse_ds3_data_persistence_rule_response(client, doc, child_node, &data_persistence_rules_response);
            response->data_persistence_rules = (ds3_data_persistence_rule_response**)data_persistence_rules_array->pdata;
            g_ptr_array_add(data_persistence_rules_array, data_persistence_rules_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_persistence_rule_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->data_persistence_rules = (ds3_data_persistence_rule_response**)data_persistence_rules_array->pdata;
    response->num_data_persistence_rules = data_persistence_rules_array->len;
    g_ptr_array_free(data_persistence_rules_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_persistence_rule_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_list_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* data_policies_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicy")) {
            ds3_data_policy_response* data_policies_response = NULL;
            error = _parse_ds3_data_policy_response(client, doc, child_node, &data_policies_response);
            response->data_policies = (ds3_data_policy_response**)data_policies_array->pdata;
            g_ptr_array_add(data_policies_array, data_policies_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->data_policies = (ds3_data_policy_response**)data_policies_array->pdata;
    response->num_data_policies = data_policies_array->len;
    g_ptr_array_free(data_policies_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_replication_rule_list_response(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_replication_rule_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* data_replication_rules_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_replication_rule_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataReplicationRule")) {
            ds3_data_replication_rule_response* data_replication_rules_response = NULL;
            error = _parse_ds3_data_replication_rule_response(client, doc, child_node, &data_replication_rules_response);
            response->data_replication_rules = (ds3_data_replication_rule_response**)data_replication_rules_array->pdata;
            g_ptr_array_add(data_replication_rules_array, data_replication_rules_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_replication_rule_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->data_replication_rules = (ds3_data_replication_rule_response**)data_replication_rules_array->pdata;
    response->num_data_replication_rules = data_replication_rules_array->len;
    g_ptr_array_free(data_replication_rules_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_replication_rule_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_degraded_blob_list_response(const ds3_client* client, const ds3_request* request, ds3_degraded_blob_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_degraded_blob_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* degraded_blobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_degraded_blob_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DegradedBlob")) {
            ds3_degraded_blob_response* degraded_blobs_response = NULL;
            error = _parse_ds3_degraded_blob_response(client, doc, child_node, &degraded_blobs_response);
            response->degraded_blobs = (ds3_degraded_blob_response**)degraded_blobs_array->pdata;
            g_ptr_array_add(degraded_blobs_array, degraded_blobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_degraded_blob_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->degraded_blobs = (ds3_degraded_blob_response**)degraded_blobs_array->pdata;
    response->num_degraded_blobs = degraded_blobs_array->len;
    g_ptr_array_free(degraded_blobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_degraded_blob_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_pool_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_pool_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_pool_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_pools_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_pool_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobPool")) {
            ds3_suspect_blob_pool_response* suspect_blob_pools_response = NULL;
            error = _parse_ds3_suspect_blob_pool_response(client, doc, child_node, &suspect_blob_pools_response);
            response->suspect_blob_pools = (ds3_suspect_blob_pool_response**)suspect_blob_pools_array->pdata;
            g_ptr_array_add(suspect_blob_pools_array, suspect_blob_pools_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_pool_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_pools = (ds3_suspect_blob_pool_response**)suspect_blob_pools_array->pdata;
    response->num_suspect_blob_pools = suspect_blob_pools_array->len;
    g_ptr_array_free(suspect_blob_pools_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_pool_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_tape_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_tape_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_tape_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_tapes_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_tape_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobTape")) {
            ds3_suspect_blob_tape_response* suspect_blob_tapes_response = NULL;
            error = _parse_ds3_suspect_blob_tape_response(client, doc, child_node, &suspect_blob_tapes_response);
            response->suspect_blob_tapes = (ds3_suspect_blob_tape_response**)suspect_blob_tapes_array->pdata;
            g_ptr_array_add(suspect_blob_tapes_array, suspect_blob_tapes_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_tape_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_tapes = (ds3_suspect_blob_tape_response**)suspect_blob_tapes_array->pdata;
    response->num_suspect_blob_tapes = suspect_blob_tapes_array->len;
    g_ptr_array_free(suspect_blob_tapes_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_tape_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_target_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobTarget")) {
            ds3_suspect_blob_target_response* suspect_blob_targets_response = NULL;
            error = _parse_ds3_suspect_blob_target_response(client, doc, child_node, &suspect_blob_targets_response);
            response->suspect_blob_targets = (ds3_suspect_blob_target_response**)suspect_blob_targets_array->pdata;
            g_ptr_array_add(suspect_blob_targets_array, suspect_blob_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_targets = (ds3_suspect_blob_target_response**)suspect_blob_targets_array->pdata;
    response->num_suspect_blob_targets = suspect_blob_targets_array->len;
    g_ptr_array_free(suspect_blob_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_member_list_response(const ds3_client* client, const ds3_request* request, ds3_group_member_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_member_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* group_members_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_member_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "GroupMember")) {
            ds3_group_member_response* group_members_response = NULL;
            error = _parse_ds3_group_member_response(client, doc, child_node, &group_members_response);
            response->group_members = (ds3_group_member_response**)group_members_array->pdata;
            g_ptr_array_add(group_members_array, group_members_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_member_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->group_members = (ds3_group_member_response**)group_members_array->pdata;
    response->num_group_members = group_members_array->len;
    g_ptr_array_free(group_members_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_member_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_list_response(const ds3_client* client, const ds3_request* request, ds3_group_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* groups_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Group")) {
            ds3_group_response* groups_response = NULL;
            error = _parse_ds3_group_response(client, doc, child_node, &groups_response);
            response->groups = (ds3_group_response**)groups_array->pdata;
            g_ptr_array_add(groups_array, groups_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->groups = (ds3_group_response**)groups_array->pdata;
    response->num_groups = groups_array->len;
    g_ptr_array_free(groups_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_active_job_list_response(const ds3_client* client, const ds3_request* request, ds3_active_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_active_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* active_jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_active_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Job")) {
            ds3_active_job_response* active_jobs_response = NULL;
            error = _parse_ds3_active_job_response(client, doc, child_node, &active_jobs_response);
            response->active_jobs = (ds3_active_job_response**)active_jobs_array->pdata;
            g_ptr_array_add(active_jobs_array, active_jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_active_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->active_jobs = (ds3_active_job_response**)active_jobs_array->pdata;
    response->num_active_jobs = active_jobs_array->len;
    g_ptr_array_free(active_jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_active_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_canceled_job_list_response(const ds3_client* client, const ds3_request* request, ds3_canceled_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_canceled_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* canceled_jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_canceled_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CanceledJob")) {
            ds3_canceled_job_response* canceled_jobs_response = NULL;
            error = _parse_ds3_canceled_job_response(client, doc, child_node, &canceled_jobs_response);
            response->canceled_jobs = (ds3_canceled_job_response**)canceled_jobs_array->pdata;
            g_ptr_array_add(canceled_jobs_array, canceled_jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_canceled_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->canceled_jobs = (ds3_canceled_job_response**)canceled_jobs_array->pdata;
    response->num_canceled_jobs = canceled_jobs_array->len;
    g_ptr_array_free(canceled_jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_canceled_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_completed_job_list_response(const ds3_client* client, const ds3_request* request, ds3_completed_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_completed_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* completed_jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_completed_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CompletedJob")) {
            ds3_completed_job_response* completed_jobs_response = NULL;
            error = _parse_ds3_completed_job_response(client, doc, child_node, &completed_jobs_response);
            response->completed_jobs = (ds3_completed_job_response**)completed_jobs_array->pdata;
            g_ptr_array_add(completed_jobs_array, completed_jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_completed_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->completed_jobs = (ds3_completed_job_response**)completed_jobs_array->pdata;
    response->num_completed_jobs = completed_jobs_array->len;
    g_ptr_array_free(completed_jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_completed_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_node_list_response(const ds3_client* client, const ds3_request* request, ds3_node_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_node_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* nodes_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_node_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Node")) {
            ds3_node_response* nodes_response = NULL;
            error = _parse_ds3_node_response(client, doc, child_node, &nodes_response);
            response->nodes = (ds3_node_response**)nodes_array->pdata;
            g_ptr_array_add(nodes_array, nodes_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_node_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->nodes = (ds3_node_response**)nodes_array->pdata;
    response->num_nodes = nodes_array->len;
    g_ptr_array_free(nodes_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_node_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_target_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3TargetFailureNotificationRegistration")) {
            ds3_target_failure_notification_registration_response* ds3_target_failure_notification_registrations_response = NULL;
            error = _parse_ds3_target_failure_notification_registration_response(client, doc, child_node, &ds3_target_failure_notification_registrations_response);
            response->ds3_target_failure_notification_registrations = (ds3_target_failure_notification_registration_response**)ds3_target_failure_notification_registrations_array->pdata;
            g_ptr_array_add(ds3_target_failure_notification_registrations_array, ds3_target_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_target_failure_notification_registrations = (ds3_target_failure_notification_registration_response**)ds3_target_failure_notification_registrations_array->pdata;
    response->num_ds3_target_failure_notification_registrations = ds3_target_failure_notification_registrations_array->len;
    g_ptr_array_free(ds3_target_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_completed_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_completed_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* job_completed_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_completed_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "JobCompletedNotificationRegistration")) {
            ds3_job_completed_notification_registration_response* job_completed_notification_registrations_response = NULL;
            error = _parse_ds3_job_completed_notification_registration_response(client, doc, child_node, &job_completed_notification_registrations_response);
            response->job_completed_notification_registrations = (ds3_job_completed_notification_registration_response**)job_completed_notification_registrations_array->pdata;
            g_ptr_array_add(job_completed_notification_registrations_array, job_completed_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_completed_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->job_completed_notification_registrations = (ds3_job_completed_notification_registration_response**)job_completed_notification_registrations_array->pdata;
    response->num_job_completed_notification_registrations = job_completed_notification_registrations_array->len;
    g_ptr_array_free(job_completed_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_completed_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_created_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_created_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* job_created_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_created_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "JobCreatedNotificationRegistration")) {
            ds3_job_created_notification_registration_response* job_created_notification_registrations_response = NULL;
            error = _parse_ds3_job_created_notification_registration_response(client, doc, child_node, &job_created_notification_registrations_response);
            response->job_created_notification_registrations = (ds3_job_created_notification_registration_response**)job_created_notification_registrations_array->pdata;
            g_ptr_array_add(job_created_notification_registrations_array, job_created_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_created_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->job_created_notification_registrations = (ds3_job_created_notification_registration_response**)job_created_notification_registrations_array->pdata;
    response->num_job_created_notification_registrations = job_created_notification_registrations_array->len;
    g_ptr_array_free(job_created_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_created_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_creation_failed_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_creation_failed_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* job_creation_failed_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_creation_failed_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "JobCreationFailedNotificationRegistration")) {
            ds3_job_creation_failed_notification_registration_response* job_creation_failed_notification_registrations_response = NULL;
            error = _parse_ds3_job_creation_failed_notification_registration_response(client, doc, child_node, &job_creation_failed_notification_registrations_response);
            response->job_creation_failed_notification_registrations = (ds3_job_creation_failed_notification_registration_response**)job_creation_failed_notification_registrations_array->pdata;
            g_ptr_array_add(job_creation_failed_notification_registrations_array, job_creation_failed_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_creation_failed_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->job_creation_failed_notification_registrations = (ds3_job_creation_failed_notification_registration_response**)job_creation_failed_notification_registrations_array->pdata;
    response->num_job_creation_failed_notification_registrations = job_creation_failed_notification_registrations_array->len;
    g_ptr_array_free(job_creation_failed_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_creation_failed_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_cached_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_cached_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_object_cached_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_cached_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3ObjectCachedNotificationRegistration")) {
            ds3_s3_object_cached_notification_registration_response* s3_object_cached_notification_registrations_response = NULL;
            error = _parse_ds3_s3_object_cached_notification_registration_response(client, doc, child_node, &s3_object_cached_notification_registrations_response);
            response->s3_object_cached_notification_registrations = (ds3_s3_object_cached_notification_registration_response**)s3_object_cached_notification_registrations_array->pdata;
            g_ptr_array_add(s3_object_cached_notification_registrations_array, s3_object_cached_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_cached_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_object_cached_notification_registrations = (ds3_s3_object_cached_notification_registration_response**)s3_object_cached_notification_registrations_array->pdata;
    response->num_s3_object_cached_notification_registrations = s3_object_cached_notification_registrations_array->len;
    g_ptr_array_free(s3_object_cached_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_cached_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_lost_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_lost_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_object_lost_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_lost_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3ObjectLostNotificationRegistration")) {
            ds3_s3_object_lost_notification_registration_response* s3_object_lost_notification_registrations_response = NULL;
            error = _parse_ds3_s3_object_lost_notification_registration_response(client, doc, child_node, &s3_object_lost_notification_registrations_response);
            response->s3_object_lost_notification_registrations = (ds3_s3_object_lost_notification_registration_response**)s3_object_lost_notification_registrations_array->pdata;
            g_ptr_array_add(s3_object_lost_notification_registrations_array, s3_object_lost_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_lost_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_object_lost_notification_registrations = (ds3_s3_object_lost_notification_registration_response**)s3_object_lost_notification_registrations_array->pdata;
    response->num_s3_object_lost_notification_registrations = s3_object_lost_notification_registrations_array->len;
    g_ptr_array_free(s3_object_lost_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_lost_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_persisted_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_persisted_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_object_persisted_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_persisted_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3ObjectPersistedNotificationRegistration")) {
            ds3_s3_object_persisted_notification_registration_response* s3_object_persisted_notification_registrations_response = NULL;
            error = _parse_ds3_s3_object_persisted_notification_registration_response(client, doc, child_node, &s3_object_persisted_notification_registrations_response);
            response->s3_object_persisted_notification_registrations = (ds3_s3_object_persisted_notification_registration_response**)s3_object_persisted_notification_registrations_array->pdata;
            g_ptr_array_add(s3_object_persisted_notification_registrations_array, s3_object_persisted_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_persisted_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_object_persisted_notification_registrations = (ds3_s3_object_persisted_notification_registration_response**)s3_object_persisted_notification_registrations_array->pdata;
    response->num_s3_object_persisted_notification_registrations = s3_object_persisted_notification_registrations_array->len;
    g_ptr_array_free(s3_object_persisted_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_persisted_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pool_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PoolFailureNotificationRegistration")) {
            ds3_pool_failure_notification_registration_response* pool_failure_notification_registrations_response = NULL;
            error = _parse_ds3_pool_failure_notification_registration_response(client, doc, child_node, &pool_failure_notification_registrations_response);
            response->pool_failure_notification_registrations = (ds3_pool_failure_notification_registration_response**)pool_failure_notification_registrations_array->pdata;
            g_ptr_array_add(pool_failure_notification_registrations_array, pool_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pool_failure_notification_registrations = (ds3_pool_failure_notification_registration_response**)pool_failure_notification_registrations_array->pdata;
    response->num_pool_failure_notification_registrations = pool_failure_notification_registrations_array->len;
    g_ptr_array_free(pool_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domain_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomainFailureNotificationRegistration")) {
            ds3_storage_domain_failure_notification_registration_response* storage_domain_failure_notification_registrations_response = NULL;
            error = _parse_ds3_storage_domain_failure_notification_registration_response(client, doc, child_node, &storage_domain_failure_notification_registrations_response);
            response->storage_domain_failure_notification_registrations = (ds3_storage_domain_failure_notification_registration_response**)storage_domain_failure_notification_registrations_array->pdata;
            g_ptr_array_add(storage_domain_failure_notification_registrations_array, storage_domain_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domain_failure_notification_registrations = (ds3_storage_domain_failure_notification_registration_response**)storage_domain_failure_notification_registrations_array->pdata;
    response->num_storage_domain_failure_notification_registrations = storage_domain_failure_notification_registrations_array->len;
    g_ptr_array_free(storage_domain_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* system_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SystemFailureNotificationRegistration")) {
            ds3_system_failure_notification_registration_response* system_failure_notification_registrations_response = NULL;
            error = _parse_ds3_system_failure_notification_registration_response(client, doc, child_node, &system_failure_notification_registrations_response);
            response->system_failure_notification_registrations = (ds3_system_failure_notification_registration_response**)system_failure_notification_registrations_array->pdata;
            g_ptr_array_add(system_failure_notification_registrations_array, system_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->system_failure_notification_registrations = (ds3_system_failure_notification_registration_response**)system_failure_notification_registrations_array->pdata;
    response->num_system_failure_notification_registrations = system_failure_notification_registrations_array->len;
    g_ptr_array_free(system_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeFailureNotificationRegistration")) {
            ds3_tape_failure_notification_registration_response* tape_failure_notification_registrations_response = NULL;
            error = _parse_ds3_tape_failure_notification_registration_response(client, doc, child_node, &tape_failure_notification_registrations_response);
            response->tape_failure_notification_registrations = (ds3_tape_failure_notification_registration_response**)tape_failure_notification_registrations_array->pdata;
            g_ptr_array_add(tape_failure_notification_registrations_array, tape_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_failure_notification_registrations = (ds3_tape_failure_notification_registration_response**)tape_failure_notification_registrations_array->pdata;
    response->num_tape_failure_notification_registrations = tape_failure_notification_registrations_array->len;
    g_ptr_array_free(tape_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_partition_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartitionFailureNotificationRegistration")) {
            ds3_tape_partition_failure_notification_registration_response* tape_partition_failure_notification_registrations_response = NULL;
            error = _parse_ds3_tape_partition_failure_notification_registration_response(client, doc, child_node, &tape_partition_failure_notification_registrations_response);
            response->tape_partition_failure_notification_registrations = (ds3_tape_partition_failure_notification_registration_response**)tape_partition_failure_notification_registrations_array->pdata;
            g_ptr_array_add(tape_partition_failure_notification_registrations_array, tape_partition_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_partition_failure_notification_registrations = (ds3_tape_partition_failure_notification_registration_response**)tape_partition_failure_notification_registrations_array->pdata;
    response->num_tape_partition_failure_notification_registrations = tape_partition_failure_notification_registrations_array->len;
    g_ptr_array_free(tape_partition_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3Object")) {
            ds3_s3_object_response* s3_objects_response = NULL;
            error = _parse_ds3_s3_object_response(client, doc, child_node, &s3_objects_response);
            response->s3_objects = (ds3_s3_object_response**)s3_objects_array->pdata;
            g_ptr_array_add(s3_objects_array, s3_objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_objects = (ds3_s3_object_response**)s3_objects_array->pdata;
    response->num_s3_objects = s3_objects_array->len;
    g_ptr_array_free(s3_objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pool_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PoolFailure")) {
            ds3_pool_failure_response* pool_failures_response = NULL;
            error = _parse_ds3_pool_failure_response(client, doc, child_node, &pool_failures_response);
            response->pool_failures = (ds3_pool_failure_response**)pool_failures_array->pdata;
            g_ptr_array_add(pool_failures_array, pool_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pool_failures = (ds3_pool_failure_response**)pool_failures_array->pdata;
    response->num_pool_failures = pool_failures_array->len;
    g_ptr_array_free(pool_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_partition_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_partition_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_partition_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pool_partitions_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_partition_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PoolPartition")) {
            ds3_pool_partition_response* pool_partitions_response = NULL;
            error = _parse_ds3_pool_partition_response(client, doc, child_node, &pool_partitions_response);
            response->pool_partitions = (ds3_pool_partition_response**)pool_partitions_array->pdata;
            g_ptr_array_add(pool_partitions_array, pool_partitions_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_partition_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pool_partitions = (ds3_pool_partition_response**)pool_partitions_array->pdata;
    response->num_pool_partitions = pool_partitions_array->len;
    g_ptr_array_free(pool_partitions_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_partition_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pools_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Pool")) {
            ds3_pool_response* pools_response = NULL;
            error = _parse_ds3_pool_response(client, doc, child_node, &pools_response);
            response->pools = (ds3_pool_response**)pools_array->pdata;
            g_ptr_array_add(pools_array, pools_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pools = (ds3_pool_response**)pools_array->pdata;
    response->num_pools = pools_array->len;
    g_ptr_array_free(pools_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domain_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomainFailure")) {
            ds3_storage_domain_failure_response* storage_domain_failures_response = NULL;
            error = _parse_ds3_storage_domain_failure_response(client, doc, child_node, &storage_domain_failures_response);
            response->storage_domain_failures = (ds3_storage_domain_failure_response**)storage_domain_failures_array->pdata;
            g_ptr_array_add(storage_domain_failures_array, storage_domain_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domain_failures = (ds3_storage_domain_failure_response**)storage_domain_failures_array->pdata;
    response->num_storage_domain_failures = storage_domain_failures_array->len;
    g_ptr_array_free(storage_domain_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_member_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_member_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domain_members_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_member_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomainMember")) {
            ds3_storage_domain_member_response* storage_domain_members_response = NULL;
            error = _parse_ds3_storage_domain_member_response(client, doc, child_node, &storage_domain_members_response);
            response->storage_domain_members = (ds3_storage_domain_member_response**)storage_domain_members_array->pdata;
            g_ptr_array_add(storage_domain_members_array, storage_domain_members_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_member_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domain_members = (ds3_storage_domain_member_response**)storage_domain_members_array->pdata;
    response->num_storage_domain_members = storage_domain_members_array->len;
    g_ptr_array_free(storage_domain_members_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_member_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domains_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomain")) {
            ds3_storage_domain_response* storage_domains_response = NULL;
            error = _parse_ds3_storage_domain_response(client, doc, child_node, &storage_domains_response);
            response->storage_domains = (ds3_storage_domain_response**)storage_domains_array->pdata;
            g_ptr_array_add(storage_domains_array, storage_domains_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domains = (ds3_storage_domain_response**)storage_domains_array->pdata;
    response->num_storage_domains = storage_domains_array->len;
    g_ptr_array_free(storage_domains_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_system_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* system_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SystemFailure")) {
            ds3_system_failure_response* system_failures_response = NULL;
            error = _parse_ds3_system_failure_response(client, doc, child_node, &system_failures_response);
            response->system_failures = (ds3_system_failure_response**)system_failures_array->pdata;
            g_ptr_array_add(system_failures_array, system_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->system_failures = (ds3_system_failure_response**)system_failures_array->pdata;
    response->num_system_failures = system_failures_array->len;
    g_ptr_array_free(system_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_density_directive_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_density_directive_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_density_directives_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_density_directive_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeDensityDirective")) {
            ds3_tape_density_directive_response* tape_density_directives_response = NULL;
            error = _parse_ds3_tape_density_directive_response(client, doc, child_node, &tape_density_directives_response);
            response->tape_density_directives = (ds3_tape_density_directive_response**)tape_density_directives_array->pdata;
            g_ptr_array_add(tape_density_directives_array, tape_density_directives_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_density_directive_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_density_directives = (ds3_tape_density_directive_response**)tape_density_directives_array->pdata;
    response->num_tape_density_directives = tape_density_directives_array->len;
    g_ptr_array_free(tape_density_directives_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_density_directive_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_drive_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_drive_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_drive_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_drives_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_drive_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeDrive")) {
            ds3_tape_drive_response* tape_drives_response = NULL;
            error = _parse_ds3_tape_drive_response(client, doc, child_node, &tape_drives_response);
            response->tape_drives = (ds3_tape_drive_response**)tape_drives_array->pdata;
            g_ptr_array_add(tape_drives_array, tape_drives_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_drive_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_drives = (ds3_tape_drive_response**)tape_drives_array->pdata;
    response->num_tape_drives = tape_drives_array->len;
    g_ptr_array_free(tape_drives_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_drive_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_detailed_tape_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_detailed_tape_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* detailed_tape_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_detailed_tape_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeFailure")) {
            ds3_detailed_tape_failure_response* detailed_tape_failures_response = NULL;
            error = _parse_ds3_detailed_tape_failure_response(client, doc, child_node, &detailed_tape_failures_response);
            response->detailed_tape_failures = (ds3_detailed_tape_failure_response**)detailed_tape_failures_array->pdata;
            g_ptr_array_add(detailed_tape_failures_array, detailed_tape_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_tape_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->detailed_tape_failures = (ds3_detailed_tape_failure_response**)detailed_tape_failures_array->pdata;
    response->num_detailed_tape_failures = detailed_tape_failures_array->len;
    g_ptr_array_free(detailed_tape_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_tape_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_library_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_library_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_library_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_libraries_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_library_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeLibrary")) {
            ds3_tape_library_response* tape_libraries_response = NULL;
            error = _parse_ds3_tape_library_response(client, doc, child_node, &tape_libraries_response);
            response->tape_libraries = (ds3_tape_library_response**)tape_libraries_array->pdata;
            g_ptr_array_add(tape_libraries_array, tape_libraries_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_library_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_libraries = (ds3_tape_library_response**)tape_libraries_array->pdata;
    response->num_tape_libraries = tape_libraries_array->len;
    g_ptr_array_free(tape_libraries_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_library_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_partition_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartitionFailure")) {
            ds3_tape_partition_failure_response* tape_partition_failures_response = NULL;
            error = _parse_ds3_tape_partition_failure_response(client, doc, child_node, &tape_partition_failures_response);
            response->tape_partition_failures = (ds3_tape_partition_failure_response**)tape_partition_failures_array->pdata;
            g_ptr_array_add(tape_partition_failures_array, tape_partition_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_partition_failures = (ds3_tape_partition_failure_response**)tape_partition_failures_array->pdata;
    response->num_tape_partition_failures = tape_partition_failures_array->len;
    g_ptr_array_free(tape_partition_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_partitions_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartition")) {
            ds3_tape_partition_response* tape_partitions_response = NULL;
            error = _parse_ds3_tape_partition_response(client, doc, child_node, &tape_partitions_response);
            response->tape_partitions = (ds3_tape_partition_response**)tape_partitions_array->pdata;
            g_ptr_array_add(tape_partitions_array, tape_partitions_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_partitions = (ds3_tape_partition_response**)tape_partitions_array->pdata;
    response->num_tape_partitions = tape_partitions_array->len;
    g_ptr_array_free(tape_partitions_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_named_detailed_tape_partition_list_response(const ds3_client* client, const ds3_request* request, ds3_named_detailed_tape_partition_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_named_detailed_tape_partition_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* named_detailed_tape_partitions_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_named_detailed_tape_partition_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartition")) {
            ds3_named_detailed_tape_partition_response* named_detailed_tape_partitions_response = NULL;
            error = _parse_ds3_named_detailed_tape_partition_response(client, doc, child_node, &named_detailed_tape_partitions_response);
            response->named_detailed_tape_partitions = (ds3_named_detailed_tape_partition_response**)named_detailed_tape_partitions_array->pdata;
            g_ptr_array_add(named_detailed_tape_partitions_array, named_detailed_tape_partitions_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_named_detailed_tape_partition_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->named_detailed_tape_partitions = (ds3_named_detailed_tape_partition_response**)named_detailed_tape_partitions_array->pdata;
    response->num_named_detailed_tape_partitions = named_detailed_tape_partitions_array->len;
    g_ptr_array_free(named_detailed_tape_partitions_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_named_detailed_tape_partition_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tapes_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Tape")) {
            ds3_tape_response* tapes_response = NULL;
            error = _parse_ds3_tape_response(client, doc, child_node, &tapes_response);
            response->tapes = (ds3_tape_response**)tapes_array->pdata;
            g_ptr_array_add(tapes_array, tapes_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tapes = (ds3_tape_response**)tapes_array->pdata;
    response->num_tapes = tapes_array->len;
    g_ptr_array_free(tapes_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_target_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_target_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3TargetFailure")) {
            ds3_target_failure_response* ds3_target_failures_response = NULL;
            error = _parse_ds3_target_failure_response(client, doc, child_node, &ds3_target_failures_response);
            response->ds3_target_failures = (ds3_target_failure_response**)ds3_target_failures_array->pdata;
            g_ptr_array_add(ds3_target_failures_array, ds3_target_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_target_failures = (ds3_target_failure_response**)ds3_target_failures_array->pdata;
    response->num_ds3_target_failures = ds3_target_failures_array->len;
    g_ptr_array_free(ds3_target_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_read_preference_list_response(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_read_preference_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_target_read_preferences_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_read_preference_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3TargetReadPreference")) {
            ds3_target_read_preference_response* ds3_target_read_preferences_response = NULL;
            error = _parse_ds3_target_read_preference_response(client, doc, child_node, &ds3_target_read_preferences_response);
            response->ds3_target_read_preferences = (ds3_target_read_preference_response**)ds3_target_read_preferences_array->pdata;
            g_ptr_array_add(ds3_target_read_preferences_array, ds3_target_read_preferences_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_read_preference_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_target_read_preferences = (ds3_target_read_preference_response**)ds3_target_read_preferences_array->pdata;
    response->num_ds3_target_read_preferences = ds3_target_read_preferences_array->len;
    g_ptr_array_free(ds3_target_read_preferences_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_read_preference_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_list_response(const ds3_client* client, const ds3_request* request, ds3_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3Target")) {
            ds3_target_response* ds3_targets_response = NULL;
            error = _parse_ds3_target_response(client, doc, child_node, &ds3_targets_response);
            response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
            g_ptr_array_add(ds3_targets_array, ds3_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
    response->num_ds3_targets = ds3_targets_array->len;
    g_ptr_array_free(ds3_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_spectra_user_list_response(const ds3_client* client, const ds3_request* request, ds3_spectra_user_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_spectra_user_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* spectra_users_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_spectra_user_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "User")) {
            ds3_spectra_user_response* spectra_users_response = NULL;
            error = _parse_ds3_spectra_user_response(client, doc, child_node, &spectra_users_response);
            response->spectra_users = (ds3_spectra_user_response**)spectra_users_array->pdata;
            g_ptr_array_add(spectra_users_array, spectra_users_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_spectra_user_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->spectra_users = (ds3_spectra_user_response**)spectra_users_array->pdata;
    response->num_spectra_users = spectra_users_array->len;
    g_ptr_array_free(spectra_users_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_spectra_user_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_physical_placement_response(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_physical_placement_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_physical_placement_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3Targets")) {
            GPtrArray* ds3_targets_array;
            error = _parse_ds3_target_response_array(client, doc, child_node, &ds3_targets_array);
            response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
            response->num_ds3_targets = ds3_targets_array->len;
            g_ptr_array_free(ds3_targets_array, FALSE);
        } else if (element_equal(child_node, "Pools")) {
            GPtrArray* pools_array;
            error = _parse_ds3_pool_response_array(client, doc, child_node, &pools_array);
            response->pools = (ds3_pool_response**)pools_array->pdata;
            response->num_pools = pools_array->len;
            g_ptr_array_free(pools_array, FALSE);
        } else if (element_equal(child_node, "Tapes")) {
            GPtrArray* tapes_array;
            error = _parse_ds3_tape_response_array(client, doc, child_node, &tapes_array);
            response->tapes = (ds3_tape_response**)tapes_array->pdata;
            response->num_tapes = tapes_array->len;
            g_ptr_array_free(tapes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_physical_placement_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_physical_placement_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_capacity_summary_container_response(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_capacity_summary_container_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_capacity_summary_container_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Pool")) {
            error = _parse_ds3_storage_domain_capacity_summary_response(client, doc, child_node, &response->pool);
        } else if (element_equal(child_node, "Tape")) {
            error = _parse_ds3_storage_domain_capacity_summary_response(client, doc, child_node, &response->tape);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_capacity_summary_container_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_capacity_summary_container_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bulk_object_list_response(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bulk_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bulk_object_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bulk_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bulk_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_all_my_buckets_result_response(const ds3_client* client, const ds3_request* request, ds3_list_all_my_buckets_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_all_my_buckets_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListAllMyBucketsResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_all_my_buckets_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Buckets")) {
            GPtrArray* buckets_array;
            error = _parse_ds3_bucket_details_response_array(client, doc, child_node, &buckets_array);
            response->buckets = (ds3_bucket_details_response**)buckets_array->pdata;
            response->num_buckets = buckets_array->len;
            g_ptr_array_free(buckets_array, FALSE);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_all_my_buckets_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_all_my_buckets_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_delete_result_response(const ds3_client* client, const ds3_request* request, ds3_delete_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_delete_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* deleted_objects_array = g_ptr_array_new();
    GPtrArray* errors_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "DeleteResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_delete_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Deleted")) {
            ds3_s3_object_to_delete_response* deleted_objects_response = NULL;
            error = _parse_ds3_s3_object_to_delete_response(client, doc, child_node, &deleted_objects_response);
            response->deleted_objects = (ds3_s3_object_to_delete_response**)deleted_objects_array->pdata;
            g_ptr_array_add(deleted_objects_array, deleted_objects_response);
        } else if (element_equal(child_node, "Error")) {
            ds3_delete_object_error_response* errors_response = NULL;
            error = _parse_ds3_delete_object_error_response(client, doc, child_node, &errors_response);
            response->errors = (ds3_delete_object_error_response**)errors_array->pdata;
            g_ptr_array_add(errors_array, errors_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_delete_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->deleted_objects = (ds3_s3_object_to_delete_response**)deleted_objects_array->pdata;
    response->num_deleted_objects = deleted_objects_array->len;
    g_ptr_array_free(deleted_objects_array, FALSE);
    response->errors = (ds3_delete_object_error_response**)errors_array->pdata;
    response->num_errors = errors_array->len;
    g_ptr_array_free(errors_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_delete_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_objects_response(const ds3_client* client, const ds3_request* request, ds3_objects_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    struct _xmlAttr* attribute;
    ds3_objects_response* response;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Objects");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_objects_response, 1);
    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "ChunkId") == true) {
            response->chunk_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkNumber") == true) {
            response->chunk_number = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "NodeId") == true) {
            response->node_id = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_objects_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_objects_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_objects_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_master_object_list_response(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    struct _xmlAttr* attribute;
    ds3_master_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "MasterObjectList");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_master_object_list_response, 1);
    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Aggregating") == true) {
            response->aggregating = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "BucketName") == true) {
            response->bucket_name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "CachedSizeInBytes") == true) {
            response->cached_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkClientProcessingOrderGuarantee") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "CompletedSizeInBytes") == true) {
            response->completed_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "EntirelyInCache") == true) {
            response->entirely_in_cache = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "JobId") == true) {
            response->job_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Naked") == true) {
            response->naked = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Name") == true) {
            response->name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "OriginalSizeInBytes") == true) {
            response->original_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Priority") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "RequestType") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "StartDate") == true) {
            response->start_date = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Status") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->status = _match_ds3_job_status(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "UserId") == true) {
            response->user_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "UserName") == true) {
            response->user_name = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_master_object_list_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Nodes")) {
            GPtrArray* nodes_array;
            error = _parse_ds3_job_node_response_array(client, doc, child_node, &nodes_array);
            response->nodes = (ds3_job_node_response**)nodes_array->pdata;
            response->num_nodes = nodes_array->len;
            g_ptr_array_free(nodes_array, FALSE);
        } else if (element_equal(child_node, "Objects")) {
            ds3_objects_response* objects_response = NULL;
            error = _parse_ds3_objects_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_objects_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_master_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_objects_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_master_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_list_response(const ds3_client* client, const ds3_request* request, ds3_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Job")) {
            ds3_job_response* jobs_response = NULL;
            error = _parse_ds3_job_response(client, doc, child_node, &jobs_response);
            response->jobs = (ds3_job_response**)jobs_array->pdata;
            g_ptr_array_add(jobs_array, jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->jobs = (ds3_job_response**)jobs_array->pdata;
    response->num_jobs = jobs_array->len;
    g_ptr_array_free(jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_parts_result_response(const ds3_client* client, const ds3_request* request, ds3_list_parts_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_parts_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* parts_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListPartsResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_parts_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxParts")) {
            response->max_parts = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "NextPartNumberMarker")) {
            response->next_part_number_marker = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else if (element_equal(child_node, "PartNumberMarker")) {
            response->part_number_marker = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Part")) {
            ds3_multi_part_upload_part_response* parts_response = NULL;
            error = _parse_ds3_multi_part_upload_part_response(client, doc, child_node, &parts_response);
            response->parts = (ds3_multi_part_upload_part_response**)parts_array->pdata;
            g_ptr_array_add(parts_array, parts_response);
        } else if (element_equal(child_node, "IsTruncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UploadId")) {
            response->upload_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_parts_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->parts = (ds3_multi_part_upload_part_response**)parts_array->pdata;
    response->num_parts = parts_array->len;
    g_ptr_array_free(parts_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_parts_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_detailed_s3_object_list_response(const ds3_client* client, const ds3_request* request, ds3_detailed_s3_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_detailed_s3_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* detailed_s3_objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_detailed_s3_object_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_detailed_s3_object_response* detailed_s3_objects_response = NULL;
            error = _parse_ds3_detailed_s3_object_response(client, doc, child_node, &detailed_s3_objects_response);
            response->detailed_s3_objects = (ds3_detailed_s3_object_response**)detailed_s3_objects_array->pdata;
            g_ptr_array_add(detailed_s3_objects_array, detailed_s3_objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_s3_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->detailed_s3_objects = (ds3_detailed_s3_object_response**)detailed_s3_objects_array->pdata;
    response->num_detailed_s3_objects = detailed_s3_objects_array->len;
    g_ptr_array_free(detailed_s3_objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_s3_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_bucket_result_response(const ds3_client* client, const ds3_request* request, ds3_list_bucket_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_bucket_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* common_prefixes_array = g_ptr_array_new();
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListBucketResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_bucket_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CommonPrefixes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* common_prefixes = xml_get_string(doc, loop_node);
                g_ptr_array_add(common_prefixes_array, common_prefixes);
            }
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Delimiter")) {
            response->delimiter = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Marker")) {
            response->marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxKeys")) {
            response->max_keys = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NextMarker")) {
            response->next_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Contents")) {
            ds3_contents_response* objects_response = NULL;
            error = _parse_ds3_contents_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_contents_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else if (element_equal(child_node, "Prefix")) {
            response->prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsTruncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_bucket_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->common_prefixes = (ds3_str**)common_prefixes_array->pdata;
    response->num_common_prefixes = common_prefixes_array->len;
    g_ptr_array_free(common_prefixes_array, FALSE);
    response->objects = (ds3_contents_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_bucket_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_multi_part_uploads_result_response(const ds3_client* client, const ds3_request* request, ds3_list_multi_part_uploads_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_multi_part_uploads_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* common_prefixes_array = g_ptr_array_new();
    GPtrArray* uploads_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListMultipartUploadsResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_multi_part_uploads_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CommonPrefixes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* common_prefixes = xml_get_string(doc, loop_node);
                g_ptr_array_add(common_prefixes_array, common_prefixes);
            }
        } else if (element_equal(child_node, "Delimiter")) {
            response->delimiter = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "KeyMarker")) {
            response->key_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxUploads")) {
            response->max_uploads = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "NextKeyMarker")) {
            response->next_key_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NextUploadIdMarker")) {
            response->next_upload_id_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Prefix")) {
            response->prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsTruncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UploadIdMarker")) {
            response->upload_id_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Upload")) {
            ds3_multi_part_upload_response* uploads_response = NULL;
            error = _parse_ds3_multi_part_upload_response(client, doc, child_node, &uploads_response);
            response->uploads = (ds3_multi_part_upload_response**)uploads_array->pdata;
            g_ptr_array_add(uploads_array, uploads_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_multi_part_uploads_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->common_prefixes = (ds3_str**)common_prefixes_array->pdata;
    response->num_common_prefixes = common_prefixes_array->len;
    g_ptr_array_free(common_prefixes_array, FALSE);
    response->uploads = (ds3_multi_part_upload_response**)uploads_array->pdata;
    response->num_uploads = uploads_array->len;
    g_ptr_array_free(uploads_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_multi_part_uploads_result_response_free(response);
    }

    return error;
}
static ds3_paging* _parse_paging_headers(ds3_string_multimap* response_headers) {
    ds3_paging* response_paging = NULL;

    ds3_str* page_truncated_key = ds3_str_init("Page-Truncated");
    ds3_str* total_result_count_key = ds3_str_init("Total-Result-Count");

    ds3_string_multimap_entry* page_truncated_entry = ds3_string_multimap_lookup(response_headers, page_truncated_key);
    ds3_string_multimap_entry* total_result_count_entry = ds3_string_multimap_lookup(response_headers, total_result_count_key);

    if ((page_truncated_entry != NULL) || (total_result_count_entry != NULL)) {
        ds3_str* page_truncated_ds3_str = ds3_string_multimap_entry_get_value_by_index(page_truncated_entry, 0);
        ds3_str* total_result_count_ds3_str = ds3_string_multimap_entry_get_value_by_index(total_result_count_entry, 0);

        response_paging = g_new0(ds3_paging, 1);
        if (page_truncated_ds3_str != NULL) {
            response_paging->page_truncated = g_ascii_strtoll(page_truncated_ds3_str->value, NULL, 10);
            ds3_str_free(page_truncated_ds3_str);
        } else {
            response_paging->page_truncated = 0;
        }
        if (total_result_count_ds3_str != NULL) {
            response_paging->total_result_count = g_ascii_strtoll(total_result_count_ds3_str->value, NULL, 10);
            ds3_str_free(total_result_count_ds3_str);
        } else {
            response_paging->total_result_count = 0;
        }
    }

    ds3_str_free(page_truncated_key);
    ds3_str_free(total_result_count_key);
    ds3_string_multimap_entry_free(page_truncated_entry);
    ds3_string_multimap_entry_free(total_result_count_entry);

    return response_paging;
}


ds3_error* ds3_abort_multi_part_upload_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_complete_multi_part_upload_request(const ds3_client* client, const ds3_request* request, ds3_complete_multipart_upload_result_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, COMPLETE_MPU);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_complete_multipart_upload_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bucket_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_put_multi_part_upload_part_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*)) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, user_data, callback, NULL);
}
ds3_error* ds3_put_object_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*)) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, user_data, callback, NULL);
}
ds3_error* ds3_delete_bucket_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_objects_request(const ds3_client* client, const ds3_request* request, ds3_delete_result_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, STRING_LIST);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_delete_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_bucket_request(const ds3_client* client, const ds3_request* request, ds3_list_bucket_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_bucket_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_service_request(const ds3_client* client, const ds3_request* request, ds3_list_all_my_buckets_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_all_my_buckets_result_response(client, request, response, xml_blob);
}

ds3_error* ds3_get_object_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*)) {
    ds3_error* error;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    error = _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL, NULL);

    return error;
}

ds3_error* ds3_get_object_with_metadata(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*), ds3_metadata** _metadata) {
    ds3_error* error;
    ds3_string_multimap* return_headers;
    ds3_metadata* metadata;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    error = _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL, &return_headers);
    if (error == NULL) {
        metadata = _init_metadata(return_headers);
        *_metadata = metadata;
        ds3_string_multimap_free(return_headers);
    }

    return error;
}
ds3_error* ds3_head_bucket_request(const ds3_client* client, const ds3_request* request) {
    if (client == NULL || request == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }
    if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_head_object_request(const ds3_client* client, const ds3_request* request, ds3_metadata** _metadata) {
    ds3_error* error;
    ds3_string_multimap* return_headers;
    ds3_metadata* metadata;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    error = _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, &return_headers);

    if (error == NULL) {
        metadata = _init_metadata(return_headers);
        *_metadata = metadata;
        ds3_string_multimap_free(return_headers);
    }

    return error;
}

ds3_error* ds3_initiate_multi_part_upload_request(const ds3_client* client, const ds3_request* request, ds3_initiate_multipart_upload_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_initiate_multipart_upload_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_list_multi_part_upload_parts_request(const ds3_client* client, const ds3_request* request, ds3_list_parts_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_parts_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_list_multi_part_uploads_request(const ds3_client* client, const ds3_request* request, ds3_list_multi_part_uploads_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_multi_part_uploads_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bucket_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bucket_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_policy_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_policy_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_bucket_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_bucket_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_data_policy_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_data_policy_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_bucket_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_data_policy_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_bucket_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_bucket_acls_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_acl_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_data_policy_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_policy_acls_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_policy_acl_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_put_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_response(client, request, response, xml_blob);
}
ds3_error* ds3_force_full_cache_reclaim_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_cache_filesystem_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_cache_filesystem_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_cache_filesystems_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_cache_filesystem_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_cache_state_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_information_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_cache_information_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_cache_filesystem_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_cache_filesystem_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_bucket_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_capacity_summary_container_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domain_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_capacity_summary_container_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_system_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_capacity_summary_container_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_path_backend_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_path_backend_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_planner_blob_store_tasks_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_blob_store_tasks_information_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_blob_store_tasks_information_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_data_path_backend_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_path_backend_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_persistence_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_persistence_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_persistence_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_persistence_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_data_policies_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_policy_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_persistence_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_clear_suspect_blob_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_suspect_blob_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_suspect_blob_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_degraded_blobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_degraded_blob_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_degraded_blob_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_data_persistence_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_persistence_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_pool_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_pool_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_tape_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_tape_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_physical_placement_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_suspect_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_mark_suspect_blob_pools_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_mark_suspect_blob_tapes_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_mark_suspect_blob_targets_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_put_group_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_user_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_group_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_group_members_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_group_member_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_groups_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_group_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_user_is_member_of_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_allocate_job_chunk_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_objects_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_objects_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_all_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_all_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_all_canceled_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_all_completed_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, BULK_GET);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, BULK_PUT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_active_job_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_active_job_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_active_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_active_job_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_canceled_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_canceled_job_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_canceled_job_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_canceled_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_canceled_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_canceled_job_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_completed_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_completed_job_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_completed_job_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_completed_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_completed_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_completed_job_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_chunk_dao_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_chunk_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_chunk_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_chunk_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_objects_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_objects_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_to_replicate_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_str** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_str* _response;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    _response = ds3_str_init_with_size((char*)xml_blob->data, xml_blob->len);
    g_byte_array_free(xml_blob, TRUE);

    *response = _response;
    return error;
}
ds3_error* ds3_get_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_replicate_put_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, STRING);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_truncate_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_truncate_all_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_truncate_all_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_truncate_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_verify_safe_to_create_put_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_node_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_node_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_nodes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_node_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_node_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_node_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_completed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_created_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_creation_failed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_cached_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_lost_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_persisted_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_system_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_completed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_completed_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_job_completed_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_created_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_created_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_job_created_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_creation_failed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_creation_failed_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_job_creation_failed_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_cached_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_object_cached_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_cached_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_lost_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_object_lost_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_lost_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_persisted_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_object_persisted_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_persisted_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pool_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domain_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_system_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_system_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_system_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partition_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_partition_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_delete_folder_recursively_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_blob_persistence_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_str** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;
    ds3_str* _response;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, STRING);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    _response = ds3_str_init_with_size((char*)xml_blob->data, xml_blob->len);
    g_byte_array_free(xml_blob, TRUE);

    *response = _response;
    return error;
}
ds3_error* ds3_get_object_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_objects_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_s3_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_detailed_s3_object_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_physical_placement_for_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_physical_placement_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_physical_placement_for_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_physical_placement_for_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_physical_placement_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_physical_placement_for_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_import_on_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_import_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_compact_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_compact_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_deallocate_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_permanently_lost_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_pool_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_force_pool_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request) {



    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_format_all_foreign_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_format_foreign_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_blobs_on_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pool_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pool_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_partition_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_import_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_import_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_verify_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_convert_storage_domain_to_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_put_pool_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_storage_domain_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_storage_domain_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domain_members_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_member_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domains_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_system_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_system_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_system_information_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_information_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_system_information_response(client, request, response, xml_blob);
}
ds3_error* ds3_reset_instance_identifier_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_path_backend_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_system_health_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_health_verification_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_health_verification_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_eject_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_eject_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_format_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_format_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_import_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_import_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_online_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_online_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_verify_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_verify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_clean_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_drive_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_density_directive_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_permanently_lost_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_partition_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_eject_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_eject_storage_domain_blobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    error = _internal_request_dispatcher(client, request, NULL, NULL, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    return error;
}
ds3_error* ds3_eject_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_eject_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_force_tape_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request) {



    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_format_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_format_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_blobs_on_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_density_directive_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_density_directives_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_density_directive_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_drive_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_drives_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_drive_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_detailed_tape_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_libraries_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_library_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_library_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_library_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_library_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_library_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partition_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_partition_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partition_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_detailed_tape_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_partition_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_partitions_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_named_detailed_tape_partition_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_named_detailed_tape_partition_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_import_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_import_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_inspect_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_inspect_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_all_tape_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_online_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_online_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_ds3_target_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_force_target_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_blobs_on_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_data_policies_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_read_preferences_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_read_preference_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_all_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_pair_back_registered_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_register_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_delegate_create_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
}
ds3_error* ds3_delegate_delete_user_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_users_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_spectra_user_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
}
ds3_error* ds3_regenerate_user_secret_key_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
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
void ds3_data_replication_rule_response_free(ds3_data_replication_rule_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->data_policy_id);
    ds3_str_free(response->ds3_target_data_policy);
    ds3_str_free(response->ds3_target_id);
    ds3_str_free(response->id);

    g_free(response);
}
void ds3_degraded_blob_response_free(ds3_degraded_blob_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->blob_id);
    ds3_str_free(response->bucket_id);
    ds3_str_free(response->id);
    ds3_str_free(response->persistence_rule_id);
    ds3_str_free(response->replication_rule_id);

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
    ds3_str_free(response->read_from_ds3_target_id);
    ds3_str_free(response->read_from_pool_id);
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
void ds3_suspect_blob_target_response_free(ds3_suspect_blob_target_response* response) {
    if (response == NULL) {
        return;
    }

    ds3_str_free(response->blob_id);
    ds3_str_free(response->ds3_target_id);
    ds3_str_free(response->id);

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
    ds3_str_free(response->ds3_target_id);
    ds3_str_free(response->name);
    ds3_str_free(response->pool_id);
    ds3_str_free(response->tape_id);

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
    for (index = 0; index < response->num_data_replication_rules; index++) {
        ds3_data_replication_rule_response_free(response->data_replication_rules[index]);
    }
    g_free(response->data_replication_rules);
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
void ds3_suspect_blob_target_list_response_free(ds3_suspect_blob_target_list_response* response) {
    if (response == NULL) {
        return;
    }

    size_t index;
    for (index = 0; index < response->num_suspect_blob_targets; index++) {
        ds3_suspect_blob_target_response_free(response->suspect_blob_targets[index]);
    }
    g_free(response->suspect_blob_targets);
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
    for (index = 0; index < response->num_ds3_targets; index++) {
        ds3_target_response_free(response->ds3_targets[index]);
    }
    g_free(response->ds3_targets);
    for (index = 0; index < response->num_pools; index++) {
        ds3_pool_response_free(response->pools[index]);
    }
    g_free(response->pools);
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

    result = stat(file_to_stat, &file_info);
    if (result != 0) {
        fprintf(stderr, "Failed to get file info for %s\n", file_name);
    }

    obj->name = ds3_str_init(file_name);

    if (S_ISDIR(file_info.st_mode)) {
        obj->length = 0;
    } else {
        obj->length = file_info.st_size;
    }

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

ds3_bulk_object_list_response* ds3_init_bulk_object_list() {
    return g_new0(ds3_bulk_object_list_response, 1);
}
