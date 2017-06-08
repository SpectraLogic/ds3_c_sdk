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

// The BOOST_TEST_MODULE should be set only in this file
// so that a main function is only generated once
#define BOOST_TEST_MODULE DS3_Tests
#include <stdio.h>
#include <stdlib.h>
#include "ds3.h"
#include "ds3_utils.h"
#include "test.h"
#include <boost/test/included/unit_test.hpp>
#include <glib.h>

#define BUFF_SIZE 64

TempStorageIds ids;

struct BoostTestFixture {
    BoostTestFixture() {
        configure_for_tests();
    }
    ~BoostTestFixture() {
        teardown_after_tests();

        ds3_str_free(ids.data_policy_id);
        ds3_str_free(ids.data_persistence_rule_id);
        ds3_str_free(ids.storage_domain_member_id);

        ds3_cleanup();
    }
};

BOOST_GLOBAL_FIXTURE( BoostTestFixture );

void test_log(const char* message, void* user_data) {
    if (user_data) {
        int client_num = *((int*)user_data);
        fprintf(stderr, "ClientNum[%d], Log Message: %s\n", client_num, message);
    } else {
        fprintf(stderr, "Log Message: %s\n", message);
    }
}

ds3_client* get_client_at_loglvl(ds3_log_lvl log_lvl) {
    ds3_client* client;

    ds3_error* error = ds3_create_client_from_env(&client);

    if (error != NULL) {
        fprintf(stderr, "Failed to construct ds3_client from environment variables: %s\n", error->message->value);
        ds3_error_free(error);
        BOOST_FAIL("Failed to setup client.");
    }

    ds3_client_register_logging(client, log_lvl, test_log, NULL);

    return client;
}

ds3_client* get_client() {
    return get_client_at_loglvl(DS3_INFO);
}

void free_client(ds3_client* client) {
    ds3_creds_free(client->creds);
    ds3_client_free(client);
}

void configure_for_tests() {
    ds3_client* client = get_client();

    ds3_data_policy_response* put_dp_response = NULL;
    ds3_storage_domain_response* put_sd_response = NULL;
    ds3_pool_partition_response* put_pp_response = NULL;
    ds3_storage_domain_member_response* put_sd_member_response = NULL;
    ds3_data_persistence_rule_response* put_data_persistence_rule_response = NULL;

    ds3_str* data_policy_id;
    ds3_str* storage_domain_id;
    ds3_str* pool_partition_id;
    ds3_str* storage_domain_member_id;
    ds3_str* data_persistence_rule_id;

    // Create DataPolicy
    ds3_request* put_dp_request = ds3_init_put_data_policy_spectra_s3_request(TEST_DP_NAME);
    ds3_error* error = ds3_put_data_policy_spectra_s3_request(client, put_dp_request, &put_dp_response);
    data_policy_id = ds3_str_init(put_dp_response->id->value);

    ds3_request_free(put_dp_request);
    ds3_data_policy_response_free(put_dp_response);
    ds3_error_free(error);

    // Create StorageDomain
    ds3_request* put_sd_request = ds3_init_put_storage_domain_spectra_s3_request(TEST_SD_NAME);
    error = ds3_put_storage_domain_spectra_s3_request(client, put_sd_request, &put_sd_response);
    storage_domain_id = ds3_str_init(put_sd_response->id->value);

    ds3_request_free(put_sd_request);
    ds3_storage_domain_response_free(put_sd_response);
    ds3_error_free(error);

    // Create pool partition
    ds3_request* put_pp_request = ds3_init_put_pool_partition_spectra_s3_request(TEST_PP_NAME, DS3_POOL_TYPE_ONLINE);
    error = ds3_put_pool_partition_spectra_s3_request(client, put_pp_request, &put_pp_response);
    pool_partition_id = ds3_str_init(put_pp_response->id->value);

    ds3_request_free(put_pp_request);
    ds3_pool_partition_response_free(put_pp_response);
    ds3_error_free(error);

    // Create storage domain member linking pool partition to storage domain
    ds3_request* put_pool_sd_member_request = ds3_init_put_pool_storage_domain_member_spectra_s3_request(pool_partition_id->value, storage_domain_id->value);
    error = ds3_put_pool_storage_domain_member_spectra_s3_request(client, put_pool_sd_member_request, &put_sd_member_response);
    storage_domain_member_id = ds3_str_init(put_sd_member_response->id->value);

    ds3_request_free(put_pool_sd_member_request);
    ds3_storage_domain_member_response_free(put_sd_member_response);
    ds3_str_free(pool_partition_id);
    ds3_error_free(error);

    // Create data persistence rule
    ds3_request* put_data_persistence_rule_request = ds3_init_put_data_persistence_rule_spectra_s3_request(data_policy_id->value,
        DS3_DATA_ISOLATION_LEVEL_STANDARD,
        storage_domain_id->value,
        DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT);
    error = ds3_put_data_persistence_rule_spectra_s3_request(client, put_data_persistence_rule_request, &put_data_persistence_rule_response);
    data_persistence_rule_id = ds3_str_init(put_data_persistence_rule_response->id->value);

    ds3_request_free(put_data_persistence_rule_request);
    ds3_data_persistence_rule_response_free(put_data_persistence_rule_response);
    ds3_str_free(storage_domain_id);
    ds3_error_free(error);

    free_client(client);

    ids.data_policy_id = data_policy_id;
    ids.data_persistence_rule_id = data_persistence_rule_id;
    ids.storage_domain_member_id = storage_domain_member_id;
}

void teardown_after_tests() {
    ds3_client* client = get_client();
    ds3_error* error;

    // Delete DataPersistenceRule
    ds3_request* delete_data_persistence_rule_request = ds3_init_delete_data_persistence_rule_spectra_s3_request(ids.data_persistence_rule_id->value);
    error = ds3_delete_data_persistence_rule_spectra_s3_request(client, delete_data_persistence_rule_request);
    ds3_request_free(delete_data_persistence_rule_request);
    ds3_error_free(error);

    // Delete DataPolicy
    ds3_request* delete_data_policy_request = ds3_init_delete_data_policy_spectra_s3_request(TEST_DP_NAME);
    error = ds3_delete_data_policy_spectra_s3_request(client, delete_data_policy_request);
    ds3_request_free(delete_data_policy_request);
    ds3_error_free(error);

    // Delete StorageDomainMember
    ds3_request* delete_storage_domain_member_request = ds3_init_delete_storage_domain_member_spectra_s3_request(ids.storage_domain_member_id->value);
    error = ds3_delete_storage_domain_member_spectra_s3_request(client, delete_storage_domain_member_request);
    ds3_request_free(delete_storage_domain_member_request);
    ds3_error_free(error);

    // Delete StorageDomain
    ds3_request* delete_storage_domain_request = ds3_init_delete_storage_domain_spectra_s3_request(TEST_SD_NAME);
    error = ds3_delete_storage_domain_spectra_s3_request(client, delete_storage_domain_request);
    ds3_request_free(delete_storage_domain_request);
    ds3_error_free(error);

    // Delete PoolPartition
    ds3_request* delete_pool_partition_request = ds3_init_delete_pool_partition_spectra_s3_request(TEST_PP_NAME);
    error = ds3_delete_pool_partition_spectra_s3_request(client, delete_pool_partition_request);
    ds3_request_free(delete_pool_partition_request);
    ds3_error_free(error);

    free_client(client);
}

void print_error(const ds3_error* error) {
      if (NULL != error->message) {
        printf("ds3_error_message: %s\n", error->message->value);
      }
      if (error->error != NULL) {
          printf("ds3_status_code: %lu\n", (unsigned long)error->error->http_error_code);
          printf("ds3_status_message: %s\n", error->error->message->value);
          printf("ds3_error_body: %s\n", error->error->code->value);
      }
}

void handle_error(ds3_error* error) {
    BOOST_CHECK(error == NULL);
    if (error != NULL) {
        print_error(error);
        ds3_error_free(error);
    }
}

void clear_bucket(const ds3_client* client, const char* bucket_name) {
    ds3_request* request;
    ds3_error* error;

    request = ds3_init_delete_bucket_spectra_s3_request(bucket_name);
    ds3_request_set_force(request, True);

    error = ds3_delete_bucket_spectra_s3_request(client, request);
    ds3_request_free(request);
    ds3_error_free(error);
}

void populate_with_objects(const ds3_client* client, const char* bucket_name) {
    ds3_str* job_id = populate_with_objects_return_job(client, bucket_name);
    ds3_str_free(job_id);
}

void populate_with_multi_dir_objects(const ds3_client* client, const char* bucket_name) {
    ds3_str* job_id = populate_with_multi_dir_objects_return_job(client, bucket_name);
    ds3_str_free(job_id);
}

ds3_bulk_object_list_response* default_object_list() {
    const char* books[5] = {"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt", "resources/ulysses_large.txt"};
    return ds3_convert_file_list(books, 5);
}

ds3_bulk_object_list_response* multi_dirs_object_list() {
    const char* books[6] = {
      "resources/beowulf.txt",
      "resources/sherlock_holmes.txt",
      "resources/tale_of_two_cities.txt",
      "resources/ulysses.txt",
      "resources_2/foo.txt",
      "resources_2/bar.txt"};
    return ds3_convert_file_list(books, 6);
}

ds3_request* populate_bulk_return_request(const ds3_client* client, const char* bucket_name, ds3_bulk_object_list_response* obj_list) {
    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    ds3_error_free(error);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    return request;
}

ds3_master_object_list_response* populate_bulk_return_response(const ds3_client* client, ds3_request* request) {
    ds3_master_object_list_response* response;
    ds3_error* error = ds3_put_bulk_job_spectra_s3_request(client, request, &response);

    ds3_request_free(request);
    handle_error(error);
    return response;
}

ds3_str* populate_with_empty_objects(const ds3_client* client, const char* bucket_name) {
    ds3_bulk_object_list_response* obj_list = default_object_list();
    ds3_request* request = populate_bulk_return_request(client, bucket_name, obj_list);
    ds3_master_object_list_response* response = populate_bulk_return_response(client, request);

    ds3_str* job_id = ds3_str_dup(response->job_id);
    ds3_master_object_list_response_free(response);
    ds3_bulk_object_list_response_free(obj_list);
    return job_id;
}

ds3_master_object_list_response* ensure_available_chunks(const ds3_client* client, ds3_str* job_id) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    bool retry_get;
    ds3_master_object_list_response* chunk_response;
    do {
        retry_get = false;
        request =  ds3_init_get_job_chunks_ready_for_client_processing_spectra_s3_request(job_id->value);
        error = ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request(client, request, &chunk_response);
        ds3_request_free(request);
        handle_error(error);
        BOOST_CHECK(chunk_response != NULL);

        if (chunk_response->num_objects == 0) {
            // if this happens we need to try the request
            BOOST_TEST_MESSAGE( "Hit retry, sleeping for 30 seconds..."); //<< chunk_response->retry_after);
            retry_get = true;

            //TODO parse metadata retry_after
            //sleep(chunk_response->retry_after);
            g_usleep(30);
            ds3_master_object_list_response_free(chunk_response);
        }
    } while(retry_get);
    return chunk_response;
}

void populate_with_objects_from_bulk(const ds3_client* client, const char* bucket_name, ds3_master_object_list_response* response) {
    uint64_t chunk_index, object_index;
    ds3_error* error;
    ds3_request* request;

    ds3_master_object_list_response* chunk_response = ensure_available_chunks(client, response->job_id);

    for (chunk_index = 0; chunk_index < chunk_response->num_objects; chunk_index++) {
        ds3_objects_response* chunk_object_list = chunk_response->objects[chunk_index];
        for (object_index = 0; object_index < chunk_object_list->num_objects; object_index++) {
            ds3_bulk_object_response* current_obj = chunk_object_list->objects[object_index];
            FILE* file = fopen(current_obj->name->value, "r");

            request = ds3_init_put_object_request(bucket_name, current_obj->name->value, current_obj->length);
            ds3_request_set_job(request, response->job_id->value);
            ds3_request_set_offset(request, current_obj->offset);
            if (current_obj->offset > 0) {
                fseek(file, current_obj->offset, SEEK_SET);
            }
            error = ds3_put_object_request(client, request, file, ds3_read_from_file);
            ds3_request_free(request);

            fclose(file);
            handle_error(error);
        }
    }
    ds3_master_object_list_response_free(chunk_response);
}

ds3_str* populate_with_object_list_return_job( const ds3_client* client,
                                               const char* bucket_name,
                                               ds3_bulk_object_list_response* object_list) {
    ds3_request* request = populate_bulk_return_request(client, bucket_name, object_list);
    ds3_master_object_list_response* master_object_list_response = populate_bulk_return_response(client, request);

    ds3_str* job_id = ds3_str_dup(master_object_list_response->job_id);
    populate_with_objects_from_bulk(client, bucket_name, master_object_list_response);

    ds3_master_object_list_response_free(master_object_list_response);
    return job_id;
}

ds3_str* populate_with_objects_return_job(const ds3_client* client, const char* bucket_name) {
    ds3_bulk_object_list_response* obj_list = default_object_list();
    ds3_str* job_id = populate_with_object_list_return_job(client, bucket_name, obj_list);
    ds3_bulk_object_list_response_free(obj_list);
    return job_id;
}

ds3_str* populate_with_multi_dir_objects_return_job(const ds3_client* client, const char* bucket_name) {
    ds3_bulk_object_list_response* obj_list = multi_dirs_object_list();
    ds3_str* job_id = populate_with_object_list_return_job(client, bucket_name, obj_list);
    ds3_bulk_object_list_response_free(obj_list);
    return job_id;
}

bool contains_object(ds3_list_bucket_result_response* bucket_list, const char* key) {
    uint64_t object_index;
    for (object_index = 0; object_index < bucket_list->num_objects; object_index++) {
        if (strcmp(bucket_list->objects[object_index]->key->value, key) == 0) {
            return true;
        }
    }
    return false;
}

ds3_error* create_bucket_with_data_policy(const ds3_client* client, const char* bucket_id, const char* data_policy_id) {
    ds3_request* request = ds3_init_put_bucket_spectra_s3_request(bucket_id);
    ds3_request_set_data_policy_id(request, data_policy_id);

    ds3_bucket_response* bucket_response = NULL;
    ds3_error* error = ds3_put_bucket_spectra_s3_request(client, request, &bucket_response);
    ds3_bucket_response_free(bucket_response);
    ds3_request_free(request);
    return error;
}

ds3_error* get_bucket_data_policy_id(const ds3_client* client, const char* bucket_name, ds3_str* data_policy_id) {
    ds3_request* request;
    ds3_error* error;
    ds3_bucket_response* bucket_response;

    request = ds3_init_get_bucket_spectra_s3_request(bucket_name);
    error = ds3_get_bucket_spectra_s3_request(client, request, &bucket_response);
    ds3_request_free(request);
    if (error != NULL) {
        ds3_bucket_response_free(bucket_response);
        return error;
    }

    data_policy_id = bucket_response->data_policy_id;
    ds3_bucket_response_free(bucket_response);

    return NULL;
}

ds3_error* get_bucket_data_policy_checksum_type(ds3_client* client, const char* bucket_name, ds3_checksum_type* checksum_type) {
    ds3_request* request;
    ds3_error* error;
    ds3_str* data_policy_id = NULL;
    ds3_data_policy_response* data_policy_response;

    // Get bucket data policy ID
    error = get_bucket_data_policy_id(client, bucket_name, data_policy_id);
    if (error != NULL) {
        return error;
    }

    // Get DataPolicy checksum_type
    request = ds3_init_get_data_policy_spectra_s3_request(data_policy_id->value);
    error = ds3_get_data_policy_spectra_s3_request(client, request, &data_policy_response);
    ds3_request_free(request);
    ds3_str_free(data_policy_id);
    if (error != NULL) {
        ds3_data_policy_response_free(data_policy_response);
        return error;
    }

    *checksum_type = data_policy_response->checksum_type;

    // cleanup
    ds3_data_policy_response_free(data_policy_response);

    return NULL;
}


double timespec_to_seconds(struct timespec* ts) {
    return (double)ts->tv_sec + (double)ts->tv_nsec / 1000000000.0;
}


/**
 * Find the size of a local file then create a ds3_bulk_object_list_response with the same name many times, append a number
 */
ds3_bulk_object_list_response* create_bulk_object_list_single_file(const char* file_name, size_t num_files) {
    struct stat file_info;
    memset(&file_info, 0, sizeof(struct stat));
    stat(file_name, &file_info);

    return create_bulk_object_list_from_prefix_with_size(file_name, num_files, file_info.st_size);

}

/**
 * Create a ds3_bulk_object_list_response with the same name many times, append a number.
 */
ds3_bulk_object_list_response* create_bulk_object_list_from_prefix_with_size(const char* put_name_prefix, size_t num_files, size_t size) {
    char put_filename[BUFF_SIZE];

    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();

    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();
    for (size_t index = 0; index < num_files; index++) {
        g_snprintf(put_filename, BUFF_SIZE, "%s_%05lu", put_name_prefix, index);

        ds3_bulk_object_response* obj = g_new0(ds3_bulk_object_response, 1);
        obj->name = ds3_str_init(put_filename);
        obj->length = size;
        g_ptr_array_add(ds3_bulk_object_response_array, obj);
    }

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    return obj_list;
}

GPtrArray* new_put_chunks_threads_args_performance(ds3_client* client,
                                       const char* src_obj_name,
                                       const char* dest_bucket_name,
                                       const ds3_master_object_list_response* bulk_response,
                                       ds3_master_object_list_response* available_chunks,
                                       const uint8_t num_threads,
                                       const ds3_bool verbose,
                                       const ds3_bool performance) {
    GPtrArray* put_chunks_args_array = g_ptr_array_new();

    for (uint8_t thread_index = 0; thread_index < num_threads; thread_index++) {
        put_chunks_args* put_objects_args = g_new0(put_chunks_args, 1);
        put_objects_args->client = client;
        put_objects_args->job_id = bulk_response->job_id->value;
        put_objects_args->src_object_name = (char*)src_obj_name;
        put_objects_args->bucket_name = (char*)dest_bucket_name;
        put_objects_args->chunks_list = available_chunks;
        put_objects_args->thread_num = thread_index;
        put_objects_args->num_threads = num_threads;
        put_objects_args->verbose = verbose;
        put_objects_args->performance = performance;
        g_ptr_array_add(put_chunks_args_array, put_objects_args);
    }

    return put_chunks_args_array;
}

/*
 * Returned put_chunks_threads_args* must be freed with put_chunks_threads_args_free();
 */
GPtrArray* new_put_chunks_threads_args(ds3_client* client,
                                       const char* src_obj_name,
                                       const char* dest_bucket_name,
                                       const ds3_master_object_list_response* bulk_response,
                                       ds3_master_object_list_response* available_chunks,
                                       const uint8_t num_threads,
                                       const ds3_bool verbose) {
    return new_put_chunks_threads_args_performance(client, src_obj_name, dest_bucket_name, bulk_response, available_chunks, num_threads, verbose, False);
}

void put_chunks_threads_args_free(GPtrArray* array) {
    for (size_t index = 0; index < array->len; index++) {
        g_free(g_ptr_array_index(array, index));
    }

    g_ptr_array_free(array, TRUE);
}

// Fill a 1MB buffer with a pattern of characters
static const char TEST_BUFFER_FILLER[] = "0123456789ABCDEF"; // 16 chars
void init_xfer_info(xfer_info* xfer_info_to_init) {
    for(size_t offset = 0; offset < XFER_BUFFER_SIZE; offset += 32) {
        memcpy(xfer_info_to_init->data + offset, TEST_BUFFER_FILLER, 16);
    }
    xfer_info_to_init->size = XFER_BUFFER_SIZE;
}

size_t ds3_test_read_from_mem(void* buffer, size_t size, size_t nmemb, void* user_data) {
    xfer_info* my_xfer_info = (struct xfer_info*)user_data;
    my_xfer_info->size = nmemb * size;
    size_t xfer_chunk_size;

    if (my_xfer_info->size <= XFER_BUFFER_SIZE) {
        memcpy(buffer, my_xfer_info->data, my_xfer_info->size);
        my_xfer_info->offset = my_xfer_info->size;
    } else {
        my_xfer_info->offset = 0;

        while (my_xfer_info->size > 0) {
            xfer_chunk_size = (my_xfer_info->size < XFER_BUFFER_SIZE) ? my_xfer_info->size : XFER_BUFFER_SIZE;

            my_xfer_info->size -= xfer_chunk_size;
            my_xfer_info->offset += xfer_chunk_size;
        }
    }

    return my_xfer_info->offset;
}


/**
 * To be passed as GThreadFunc arg to g_thread_new() along with a put_chunks_args struct
 */
void put_chunks(void* args) {
    put_chunks_args* _args = (put_chunks_args*)args;
    ds3_objects_response* chunk_object_list = NULL;
    ds3_error* error = NULL;

    FILE* file;
    struct xfer_info my_xfer_info;
    if (_args->performance) {
        init_xfer_info(&my_xfer_info);
    }

    for (size_t chunk_index = 0; chunk_index < _args->chunks_list->num_objects; chunk_index++) {
        chunk_object_list = _args->chunks_list->objects[chunk_index];
        for (size_t object_index = 0; object_index < chunk_object_list->num_objects; object_index++) {

            // Work distribution
            if (object_index % _args->num_threads == _args->thread_num) {
                ds3_bulk_object_response* object = chunk_object_list->objects[object_index];
                ds3_request* request = ds3_init_put_object_request(_args->bucket_name, object->name->value, object->length);
                ds3_request_set_job(request, _args->job_id);

                if (_args->verbose) {
                    ds3_log_message(_args->client->log, DS3_INFO, "  GlibThread[%d] BEGIN xfer File[%s] Chunk[%lu]", _args->thread_num, object->name->value, _args->chunks_list->num_objects);
                }
                if (_args->performance) {
                    // Transfer each object for reading from the memory buffer
                    error = ds3_put_object_request(_args->client, request, &my_xfer_info, ds3_test_read_from_mem);
                } else {
                    if (_args->verbose) {
                        ds3_log_message(_args->client->log, DS3_INFO, "  GlibThread[%d] open File*[%s]", _args->thread_num, _args->src_object_name);
                    }
                    file = fopen(_args->src_object_name, "r");
                    if (object->offset != 0) {
                        fseek(file, object->offset, SEEK_SET);
                    }
                    error = ds3_put_object_request(_args->client, request, file, ds3_read_from_file);
                    fclose(file);
                    if (_args->verbose) {
                        ds3_log_message(_args->client->log, DS3_INFO, "  GLibThread[%d] File*[%s] closed.", _args->thread_num, _args->src_object_name);
                    }
                }
                if (_args->verbose) {
                    ds3_log_message(_args->client->log, DS3_INFO, "  GlibThread[%d] END xfer File[%s] Chunk[%lu]", _args->thread_num, object->name->value, _args->chunks_list->num_objects);
                }

                ds3_request_free(request);
                handle_error(error);
            }
        }
    }

}

