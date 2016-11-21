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

#include "ds3.h"

#ifndef __DS3_TEST__
#define __DS3_TEST__

static const char TEST_DP_NAME[] = "test_data_policy";
static const char TEST_SD_NAME[] = "test_storage_domain";
static const char TEST_PP_NAME[] = "test_pool_partition";

struct TempStorageIds {
    ds3_str* data_policy_id;
    ds3_str* data_persistence_rule_id;
    ds3_str* storage_domain_member_id;
};

// global variable to track ids for use in tests
extern TempStorageIds ids;

ds3_client* get_client();
ds3_client* get_client_at_loglvl(ds3_log_lvl lvl);

void configure_for_tests();
void teardown_after_tests();

void clear_bucket(const ds3_client* client, const char* bucket_name);

void populate_with_objects(const ds3_client* client, const char* bucket_name);
void populate_with_multi_dir_objects(const ds3_client* client, const char* bucket_name);

ds3_str* populate_with_object_list_return_job( const ds3_client* client,
                                               const char* bucket_name,
                                               ds3_bulk_object_list_response* object_list);

ds3_str* populate_with_objects_return_job(const ds3_client* client, const char* bucket_name);
ds3_str* populate_with_multi_dir_objects_return_job(const ds3_client* client, const char* bucket_name);
ds3_str* populate_with_empty_objects(const ds3_client* client, const char* bucket_name);

ds3_request* populate_bulk_return_request(const ds3_client* client, const char* bucket_name, ds3_bulk_object_list_response* obj_list);
bool contains_object(ds3_list_bucket_result_response* bucket_list, const char* key);

ds3_master_object_list_response* populate_bulk_return_response(const ds3_client* client, ds3_request* request);
void populate_with_objects_from_bulk(const ds3_client* client, const char* bucket_name, ds3_master_object_list_response* response);

ds3_master_object_list_response* ensure_available_chunks(const ds3_client* client, ds3_str* job_id);

void handle_error(ds3_error* error);

void free_client(ds3_client* client);

ds3_bulk_object_list_response* default_object_list();
ds3_bulk_object_list_response* multi_dirs_object_list();

ds3_error* create_bucket_with_data_policy(const ds3_client* client, const char* bucket_id, const char* data_policy_id);

// caller must free data_policy_id
ds3_error* get_bucket_data_policy_id(const ds3_client* client, const char* bucket_name, ds3_str* data_policy_id);
ds3_error* get_bucket_data_policy_checksum_type(const ds3_client* client, const char* bucket_name, ds3_checksum_type* checksum_type);
#endif
