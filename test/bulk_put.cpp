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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <sys/stat.h>
#include <boost/test/unit_test.hpp>
#include <inttypes.h>
#include "ds3.h"
#include "ds3_net.h"
#include "test.h"

#define BUFF_SIZE 16

/**
 * Create a ds3_bulk_object_list_response with the same name many times, append a number
 */
ds3_bulk_object_list_response* create_bulk_object_list_single_file(const char* file_name, size_t num_files) {
    char put_filename[BUFF_SIZE];

    struct stat file_info;
    memset(&file_info, 0, sizeof(struct stat));
    stat(file_name, &file_info);

    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();

    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();
    for (size_t index = 0; index < num_files; index++) {
        g_snprintf(put_filename, BUFF_SIZE, "file_%05lu", index);

        ds3_bulk_object_response* obj = g_new0(ds3_bulk_object_response, 1);
        obj->name = ds3_str_init(put_filename);
        obj->length = file_info.st_size;
        g_ptr_array_add(ds3_bulk_object_response_array, obj);
    }

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    return obj_list;
}

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
} test_put_chunks_args;

void put_chunks(void* args) {
    test_put_chunks_args* put_chunks_args = (test_put_chunks_args*)args;
    ds3_objects_response* chunk_object_list = NULL;

    for (size_t chunk_index = 0; chunk_index < put_chunks_args->chunks_list->num_objects; chunk_index++) {
        chunk_object_list = put_chunks_args->chunks_list->objects[chunk_index];
        for (size_t object_index = 0; object_index < chunk_object_list->num_objects; object_index++) {

            // Work distribution
            if (object_index % put_chunks_args->num_threads == put_chunks_args->thread_num) {
                ds3_bulk_object_response* object = chunk_object_list->objects[object_index];
                // Send the same file every time, give it a different destination name
                FILE* file = fopen(put_chunks_args->src_object_name, "r");
                if (file == NULL) {
                    printf("Unable to open %s for read (FILE NULL), skipping put to bucket %s!\n", put_chunks_args->src_object_name, put_chunks_args->bucket_name);
                    return;
                }

                ds3_request* request = ds3_init_put_object_request(put_chunks_args->bucket_name, object->name->value, object->length);
                ds3_request_set_job(request, put_chunks_args->job_id);
                if (object->offset > 0) {
                    fseek(file, object->offset, SEEK_SET);
                }
                ds3_error* error = ds3_put_object_request(put_chunks_args->client, request, file, ds3_read_from_file);
                ds3_request_free(request);

                fclose(file);
                handle_error(error);
            }
        }
    }
}

BOOST_AUTO_TEST_CASE( bulk_put_10k_very_small_files ) {
    printf("-----Testing Bulk PUT of 10k very small files-------\n");
    ds3_request* request = NULL;
    const char* bucket_name = "test_bulk_put_10k_very_small_files_bucket";
    const char* object_name = "resources/very_small_file.txt";
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_bulk_object_list_response* object_list = create_bulk_object_list_single_file(object_name, 10000);
    ds3_client* client = get_client();

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, object_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);
    handle_error(error);

    test_put_chunks_args* put_chunks_args = g_new0(test_put_chunks_args, 1);
    put_chunks_args->client = client;
    put_chunks_args->num_threads = 1;
    put_chunks_args->thread_num = 0;
    put_chunks_args->job_id = bulk_response->job_id->value;
    put_chunks_args->src_object_name = (char*)object_name;
    put_chunks_args->bucket_name = (char*)bucket_name;
    put_chunks_args->chunks_list = ensure_available_chunks(client, bulk_response->job_id);

    put_chunks(put_chunks_args);

    ds3_master_object_list_response_free(put_chunks_args->chunks_list);
    ds3_master_object_list_response_free(bulk_response);
    g_free(put_chunks_args);

    clear_bucket(client, bucket_name);
    free_client(client);
}


BOOST_AUTO_TEST_CASE( bulk_put_200_very_small_files_multithreaded ) {
    printf("-----Testing Bulk PUT of 200 very small files multithreaded-------\n");
    const char* bucket_name = "test_bulk_put_200_very_small_files_multithreaded";
    const char* object_name = "resources/very_small_file.txt";
    ds3_request* request = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_bulk_object_list_response* object_list = create_bulk_object_list_single_file(object_name, 200);
    ds3_client* client = get_client();

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, object_list);

    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);
    handle_error(error);

    ds3_master_object_list_response* chunk_response = ensure_available_chunks(client, bulk_response->job_id);

    // send to child thread 1
    test_put_chunks_args* put_odd_objects_args = g_new0(test_put_chunks_args, 1);
    put_odd_objects_args->client = client;
    put_odd_objects_args->job_id = bulk_response->job_id->value;
    put_odd_objects_args->src_object_name = (char*)object_name;
    put_odd_objects_args->bucket_name = (char*)bucket_name;
    put_odd_objects_args->chunks_list = chunk_response;
    put_odd_objects_args->thread_num = 0;
    put_odd_objects_args->num_threads = 2;

    // send to child thread 2
    test_put_chunks_args* put_even_objects_args = g_new0(test_put_chunks_args, 1);
    put_even_objects_args->client = client;
    put_even_objects_args->job_id = bulk_response->job_id->value;
    put_even_objects_args->src_object_name = (char*)object_name;
    put_even_objects_args->bucket_name = (char*)bucket_name;
    put_even_objects_args->chunks_list = chunk_response;
    put_even_objects_args->thread_num = 1;
    put_even_objects_args->num_threads = 2;

    GThread* even_chunks_thread = g_thread_new("even_objects", (GThreadFunc)put_chunks, put_even_objects_args);
    GThread* odd_chunks_thread = g_thread_new("odd_objects", (GThreadFunc)put_chunks, put_odd_objects_args);

    // Block and cleanup GThreads
    g_thread_join(even_chunks_thread);
    g_thread_join(odd_chunks_thread);

    ds3_master_object_list_response_free(chunk_response);
    ds3_master_object_list_response_free(bulk_response);
    g_free(put_odd_objects_args);
    g_free(put_even_objects_args);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_utf_object_name ) {
    printf("-----Testing PUT object with UTF Characters in name-------\n");

    const char* bucket_name = "utf-8_chars_bucket";
    const char* object_name_1 = "ÖÄ§Ö.txt";
    const char* object_name_2 = "file£name.txt";
    ds3_request* request = NULL;
    ds3_master_object_list_response* bulk_response = NULL;

    ds3_client* client = get_client();
    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    // Create obj_list with 1 folder with UTF-8 characters
    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();
    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();

    ds3_bulk_object_response* obj1 = g_new0(ds3_bulk_object_response, 1);
    obj1->name = ds3_str_init(object_name_1);
    obj1->length = 0;
    g_ptr_array_add(ds3_bulk_object_response_array, obj1);
    ds3_bulk_object_response* obj2 = g_new0(ds3_bulk_object_response, 1);
    obj2->name = ds3_str_init(object_name_2);
    obj2->length = 0;
    g_ptr_array_add(ds3_bulk_object_response_array, obj2);

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(obj_list);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_64bit_object_sizes ) {
    printf("-----Testing PUT objects with 64bit object sizes in name-------\n");

    uint64_t test_objects_size = 429496729600; // 400gb > 32bit
    const char* bucket_name = "test_64_bit_object_sizes_bucket";
    ds3_request* request = NULL;
    ds3_master_object_list_response* bulk_response = NULL;

    ds3_client* client = get_client();
    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    // Create obj_list with 1 folder with UTF-8 characters
    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();
    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();

    ds3_bulk_object_response* obj1 = g_new0(ds3_bulk_object_response, 1);
    obj1->name = ds3_str_init("max_size_obj_1");
    obj1->length = test_objects_size;
    g_ptr_array_add(ds3_bulk_object_response_array, obj1);
    ds3_bulk_object_response* obj2 = g_new0(ds3_bulk_object_response, 1);
    obj2->name = ds3_str_init("max_size_obj_2");
    obj2->length = test_objects_size;
    g_ptr_array_add(ds3_bulk_object_response_array, obj2);

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(obj_list);
    handle_error(error);

    BOOST_CHECK_EQUAL(bulk_response->num_objects, 14);

    BOOST_CHECK_EQUAL(bulk_response->objects[0]->objects[0]->length, 68719476736);
    BOOST_CHECK_EQUAL(bulk_response->objects[0]->objects[0]->offset, 0);

    BOOST_CHECK_EQUAL(bulk_response->objects[13]->objects[0]->length, 17179869184);
    BOOST_CHECK_EQUAL(bulk_response->objects[13]->objects[0]->offset, 412316860416);

    ds3_master_object_list_response_free(bulk_response);
    clear_bucket(client, bucket_name);
    free_client(client);
}

