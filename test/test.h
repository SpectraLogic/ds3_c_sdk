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


#ifndef __DS3_TEST__
#define __DS3_TEST__

#include <glib.h>
#include "ds3.h"

static const char TEST_DP_NAME[] = "c_sdk_test_data_policy";
static const char TEST_SD_NAME[] = "c_sdk_test_storage_domain";
static const char TEST_PP_NAME[] = "c_sdk_test_pool_partition";

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

/**
 * Find the size of a local file then create a ds3_bulk_object_list_response with the same name many times, append a number
 */
ds3_bulk_object_list_response* create_bulk_object_list_single_file(const char* file_name, size_t num_files);
/**
 * Create a ds3_bulk_object_list_response with the same name and size many times, append a number.
 */
ds3_bulk_object_list_response* create_bulk_object_list_from_prefix_with_size(const char* put_name_prefix, size_t num_files, size_t size);

/**
 * g_thread_new only takes a single parameter to pass to the spawned thread, so its necessary
 * to wrap multiple parameters in a struct to be passed along.
 */
typedef struct {
    uint8_t                              num_threads;
    uint8_t                              thread_num;
    ds3_client*                          client;
    char*                                job_id;
    char*                                src_object_name;
    char*                                bucket_name;
    ds3_master_object_list_response*     chunks_list;
    ds3_bool                             verbose;
    ds3_bulk_object_response*            current_object;
} put_chunks_args;

double timespec_to_seconds(struct timespec* ts);
void test_log(const char* message, void* user_data);

/*
 * Returned put_chunks_threads_args* must be freed with put_chunks_threads_args_free();
 */
GPtrArray* new_put_chunks_threads_args(ds3_client* client,
                                       const char* src_obj_name,
                                       const char* dest_bucket_name,
                                       const ds3_master_object_list_response* bulk_response,
                                       ds3_master_object_list_response* available_chunks,
                                       const uint8_t num_threads,
                                       const ds3_bool verbose);
void put_chunks_threads_args_free(GPtrArray* array);

/**
 * To be passed as GThreadFunc arg to g_thread_new() along with a put_chunks_args struct
 */
void put_chunks_from_file(void* args);
/**
 * To be passed as GThreadFunc arg to g_thread_new() along with a put_chunks_args struct
 * Reads input from a memory buffer rather than a File*
 */
void put_chunks_from_mem(void* args);

/*
 * Helper functions for a performance test to read data from memory rather than a File
 */
#define XFER_BUFFER_SIZE 16 * 1024 // 16kb

struct xfer_info {
    char   data[XFER_BUFFER_SIZE];
    size_t size;
    size_t total_read;
    put_chunks_args* args;
};

void init_xfer_info(const xfer_info* xfer_info_to_init, uint16_t size_in_mb);
size_t ds3_test_read_from_mem(void* buffer, size_t size, size_t nmemb, void* user_data);

#endif
