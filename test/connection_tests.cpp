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

#include <stdio.h>
#include "ds3.h"
#include "ds3_net.h"
#include "ds3_utils.h"
#include "test.h"
#include <boost/test/unit_test.hpp>
#include <glib.h>

BOOST_AUTO_TEST_CASE( ds3_client_create_free ) {
    printf("-----Testing ds3_client create and free-------\n");

    ds3_client* client = get_client();
    BOOST_CHECK(client->connection_pool != NULL);
    ds3_creds_free(client->creds);
    ds3_client_free(client);
}

BOOST_AUTO_TEST_CASE( ds3_connection_pool_copy ) {
    printf("-----Testing ds3_copy_client-------\n");

    ds3_client* client = get_client();

    ds3_client* client_copy = ds3_copy_client(client);
    BOOST_CHECK_EQUAL(client->endpoint->value, client_copy->endpoint->value);
    if (client->proxy) {
        BOOST_CHECK_EQUAL(client->proxy->value, client_copy->proxy->value);
    }
    BOOST_CHECK_EQUAL(client->num_redirects, client_copy->num_redirects);
    if (client->creds) {
        BOOST_CHECK_EQUAL(client->creds->access_id->value, client_copy->creds->access_id->value);
        BOOST_CHECK_EQUAL(client->creds->secret_key->value, client_copy->creds->secret_key->value);
    }
    BOOST_CHECK_EQUAL(client->net_callback, client_copy->net_callback);
    BOOST_CHECK_EQUAL(client->connection_pool, client_copy->connection_pool);

    ds3_creds_free(client->creds);
    ds3_client_free(client);

    BOOST_CHECK(client_copy->connection_pool != NULL);
    ds3_creds_free(client_copy->creds);
    ds3_client_free(client_copy);
}

BOOST_AUTO_TEST_CASE( create_bucket_with_copied_client ) {
    printf("-----Testing create bucket with copied client-------\n");

    ds3_client* client = get_client();
    ds3_connection_pool* cp = client->connection_pool;
    BOOST_CHECK(cp != NULL);

    ds3_client* client_copy = ds3_copy_client(client);
    BOOST_CHECK_EQUAL(cp, client_copy->connection_pool);

    const char* client_bucket_name = "create_bucket_from_original_client";
    ds3_error* error = create_bucket_with_data_policy(client, client_bucket_name, ids.data_policy_id->value);
    handle_error(error);
    clear_bucket(client, client_bucket_name);
    ds3_creds_free(client->creds);
    ds3_client_free(client);
    BOOST_CHECK(client_copy->connection_pool != NULL);

    const char* copied_client_bucket_name = "create_bucket_from_copied_client";
    error = create_bucket_with_data_policy(client_copy, copied_client_bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_get_service_request();
    ds3_list_all_my_buckets_result_response* bucket_list = NULL;
    error = ds3_get_service_request(client_copy, request, &bucket_list);
    ds3_request_free(request);
    handle_error(error);
    BOOST_CHECK(bucket_list != NULL);
    BOOST_CHECK_EQUAL(bucket_list->num_buckets, 1);
    BOOST_CHECK_EQUAL(bucket_list->buckets[0]->name->value, copied_client_bucket_name);
    ds3_list_all_my_buckets_result_response_free(bucket_list);

    request = ds3_init_get_bucket_request(copied_client_bucket_name);
    ds3_list_bucket_result_response* bucket_info = NULL;
    error = ds3_get_bucket_request(client_copy, request, &bucket_info);
    ds3_request_free(request);
    handle_error(error);
    BOOST_CHECK(bucket_info != NULL);
    BOOST_CHECK_EQUAL(bucket_info->num_objects, 0);
    BOOST_CHECK_EQUAL(bucket_info->name->value, copied_client_bucket_name);
    ds3_list_bucket_result_response_free(bucket_info);
    clear_bucket(client_copy, copied_client_bucket_name);

    ds3_creds_free(client_copy->creds);
    ds3_client_free(client_copy);
}

BOOST_AUTO_TEST_CASE( bulk_put_10k_very_small_files ) {
    printf("-----Testing Bulk PUT of 10k very small files-------\n");
    ds3_request* request = NULL;
    const char* bucket_name = "test_bulk_put_10k_very_small_files_bucket";
    const char* object_name = "resources/very_small_file.txt";
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_bulk_object_list_response* object_list = create_bulk_object_list_single_file(object_name, 10000);
    ds3_client* client = get_client_at_loglvl(DS3_INFO);
    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, object_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);
    handle_error(error);

    put_chunks_args* put_chunks_args_single_thread = g_new0(put_chunks_args, 1);
    put_chunks_args_single_thread->client = client;
    put_chunks_args_single_thread->num_threads = 1;
    put_chunks_args_single_thread->thread_num = 0;
    put_chunks_args_single_thread->job_id = bulk_response->job_id->value;
    put_chunks_args_single_thread->src_object_name = (char*)object_name;
    put_chunks_args_single_thread->bucket_name = (char*)bucket_name;
    put_chunks_args_single_thread->chunks_list = ensure_available_chunks(client, bulk_response->job_id);
    put_chunks_args_single_thread->verbose = False;

    put_chunks_from_file(put_chunks_args_single_thread);

    ds3_master_object_list_response_free(put_chunks_args_single_thread->chunks_list);
    ds3_master_object_list_response_free(bulk_response);
    g_free(put_chunks_args_single_thread);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( bulk_put_200_very_small_files_multithreaded ) {
    printf("-----Testing Bulk PUT of 200 very small files multithreaded-------\n");
    const char* bucket_name = "test_bulk_put_200_very_small_files_multithreaded";
    const uint8_t num_threads = 2;
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

    GPtrArray* put_objs_args_array = new_put_chunks_threads_args(client, object_name, NULL, bucket_name, bulk_response, chunk_response, num_threads, False);

    GThread* chunks_thread_0 = g_thread_new("objects_0", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_objs_args_array, 0));
    GThread* chunks_thread_1 = g_thread_new("objects_1", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_objs_args_array, 1));

    // Block and cleanup GThreads
    g_thread_join(chunks_thread_0);
    g_thread_join(chunks_thread_1);

    ds3_master_object_list_response_free(chunk_response);
    ds3_master_object_list_response_free(bulk_response);
    put_chunks_threads_args_free(put_objs_args_array);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( sequential_vs_parallel_xfer ) {
    printf("-----Testing BULK_PUT of objects in parallel (4 threads) vs sequentially (1 thread)-------\n");

    const char* sequential_bucket_name = "test_bulk_put_sequential";
    const char* obj_name = "resources/ulysses_46mb.txt";

    struct timespec start_time_t, end_time_t;
    double elapsed_sequential_t, elapsed_parallel_t;

    ds3_bulk_object_list_response* obj_list = create_bulk_object_list_single_file(obj_name, 100);
    ds3_client* client = get_client();
    ds3_master_object_list_response* mol = NULL;
    ds3_request* request = NULL;


    // *** Start Sequential test config ***
    ds3_error* error = create_bucket_with_data_policy(client, sequential_bucket_name, ids.data_policy_id->value);

    request = ds3_init_put_bulk_job_spectra_s3_request(sequential_bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &mol);
    ds3_request_free(request);
    handle_error(error);

    ds3_master_object_list_response* sequential_chunks = ensure_available_chunks(client, mol->job_id);

    GPtrArray* put_sequential_objs_threads_array = new_put_chunks_threads_args(client, obj_name, NULL, sequential_bucket_name, mol, sequential_chunks, 1, False);

    // capture sequential test start time
    clock_gettime(CLOCK_MONOTONIC, &start_time_t);

    GThread* xfer_sequential_thread = g_thread_new("sequential_objs_xfer", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_sequential_objs_threads_array, 0));

    // Block and cleanup GThreads
    g_thread_join(xfer_sequential_thread);

    // find elapsed CPU and real time
    clock_gettime(CLOCK_MONOTONIC, &end_time_t);
    elapsed_sequential_t = timespec_to_seconds(&end_time_t) - timespec_to_seconds(&start_time_t);
    printf("  Sequential elapsed time[%f]\n", elapsed_sequential_t);

    ds3_master_object_list_response_free(sequential_chunks);
    ds3_master_object_list_response_free(mol);
    put_chunks_threads_args_free(put_sequential_objs_threads_array);

    clear_bucket(client, sequential_bucket_name);


    // *** Start Parallel test config ***
    const char* parallel_bucket_name = "test_bulk_put_parallel";

    error = create_bucket_with_data_policy(client, parallel_bucket_name, ids.data_policy_id->value);

    request = ds3_init_put_bulk_job_spectra_s3_request(parallel_bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &mol);
    ds3_request_free(request);
    handle_error(error);

    ds3_master_object_list_response* parallel_chunks = ensure_available_chunks(client, mol->job_id);

    GPtrArray* put_parallel_objs_threads_array = new_put_chunks_threads_args(client, obj_name, NULL, parallel_bucket_name, mol, parallel_chunks, 4, False);

    // capture sequential test start time
    clock_gettime(CLOCK_MONOTONIC, &start_time_t);

    GThread* xfer_parallel_thread_0 = g_thread_new("parallel_objs_xfer", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_parallel_objs_threads_array, 0));
    GThread* xfer_parallel_thread_1 = g_thread_new("parallel_objs_xfer", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_parallel_objs_threads_array, 1));
    GThread* xfer_parallel_thread_2 = g_thread_new("parallel_objs_xfer", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_parallel_objs_threads_array, 2));
    GThread* xfer_parallel_thread_3 = g_thread_new("parallel_objs_xfer", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_parallel_objs_threads_array, 3));

    // Block and cleanup GThreads
    g_thread_join(xfer_parallel_thread_0);
    g_thread_join(xfer_parallel_thread_1);
    g_thread_join(xfer_parallel_thread_2);
    g_thread_join(xfer_parallel_thread_3);

    // find elapsed CPU and real time
    clock_gettime(CLOCK_MONOTONIC, &end_time_t);
    elapsed_parallel_t = timespec_to_seconds(&end_time_t) - timespec_to_seconds(&start_time_t);
    printf("  Parallel elapsed time[%f]\n", elapsed_parallel_t);

    ds3_master_object_list_response_free(parallel_chunks);
    ds3_master_object_list_response_free(mol);
    put_chunks_threads_args_free(put_parallel_objs_threads_array);

    clear_bucket(client, parallel_bucket_name);

    ds3_bulk_object_list_response_free(obj_list);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( multiple_client_xfer ) {
    printf("-----Testing BULK_PUT of objects from 2 clients in parallel-------\n");

    const char* client1_bucket_name = "test_bulk_put_client1";
    const char* client2_bucket_name = "test_bulk_put_client2";
    const char* obj_name = "resources/ulysses_46mb.txt";

    struct timespec start_time_t, end_time_t;
    double elapsed_t;

    ds3_bulk_object_list_response* obj_list = create_bulk_object_list_single_file(obj_name, 100);
    ds3_client* client1 = get_client();
    ds3_client* client2 = ds3_copy_client(client1); // share the connection pool

    int client1_thread=1, client2_thread=2;
    ds3_client_register_logging(client1, DS3_INFO, test_log, (void*)&client1_thread);
    ds3_client_register_logging(client2, DS3_INFO, test_log, (void*)&client2_thread);

    ds3_master_object_list_response* mol1 = NULL;
    ds3_master_object_list_response* mol2 = NULL;
    ds3_request* request = NULL;

    ds3_log_message(client1->log, DS3_INFO, "Create bucket1");
    ds3_error* error = create_bucket_with_data_policy(client1, client1_bucket_name, ids.data_policy_id->value);
    handle_error(error);
    ds3_log_message(client2->log, DS3_INFO, "Create bucket2");
    error = create_bucket_with_data_policy(client2, client2_bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_log_message(client1->log, DS3_INFO, "init put_bulk bucket1");
    request = ds3_init_put_bulk_job_spectra_s3_request(client1_bucket_name, obj_list);
    ds3_log_message(client1->log, DS3_INFO, "put_bulk bucket1");
    error = ds3_put_bulk_job_spectra_s3_request(client1, request, &mol1);
    ds3_request_free(request);
    handle_error(error);

    ds3_log_message(client2->log, DS3_INFO, "init put_bulk bucket2");
    request = ds3_init_put_bulk_job_spectra_s3_request(client2_bucket_name, obj_list);
    ds3_log_message(client2->log, DS3_INFO, "put_bulk bucket2");
    error = ds3_put_bulk_job_spectra_s3_request(client2, request, &mol2);
    ds3_request_free(request);
    handle_error(error);

    ds3_master_object_list_response* client1_chunks = ensure_available_chunks(client1, mol1->job_id);
    ds3_master_object_list_response* client2_chunks = ensure_available_chunks(client2, mol2->job_id);

    GPtrArray* client1_put_objs_args = new_put_chunks_threads_args(client1, obj_name, NULL, client1_bucket_name, mol1, client1_chunks, 1, True);
    GPtrArray* client2_put_objs_args = new_put_chunks_threads_args(client2, obj_name, NULL, client2_bucket_name, mol2, client2_chunks, 1, True);

    // capture sequential test start time
    clock_gettime(CLOCK_MONOTONIC, &start_time_t);

    GThread* client1_xfer_thread = g_thread_new("client1_objs_xfer", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(client1_put_objs_args, 0));
    GThread* client2_xfer_thread = g_thread_new("client2_objs_xfer", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(client2_put_objs_args, 0));

    // Block and cleanup GThreads
    g_thread_join(client1_xfer_thread);
    g_thread_join(client2_xfer_thread);

    // find elapsed CPU and real time
    clock_gettime(CLOCK_MONOTONIC, &end_time_t);
    elapsed_t = timespec_to_seconds(&end_time_t) - timespec_to_seconds(&start_time_t);
    ds3_log_message(client1->log, DS3_INFO, "  Elapsed time[%f]", elapsed_t);

    ds3_master_object_list_response_free(client1_chunks);
    ds3_master_object_list_response_free(mol1);
    put_chunks_threads_args_free(client1_put_objs_args);
    clear_bucket(client1, client1_bucket_name);
    free_client(client1);

    ds3_master_object_list_response_free(client2_chunks);
    ds3_master_object_list_response_free(mol2);
    put_chunks_threads_args_free(client2_put_objs_args);
    clear_bucket(client2, client2_bucket_name);
    free_client(client2);

    ds3_bulk_object_list_response_free(obj_list);
}


/*
 * Transfer from a memory buffer rather than a file on disk to eliminate any Disk Read bottleneck.
 */
BOOST_AUTO_TEST_CASE( performance_bulk_put ) {
    printf("-----Testing BULK_PUT performance-------\n");

    const char* bucket_name1 = "bulk_put_performance_bucket1";
    const char* bucket_name2 = "bulk_put_performance_bucket2";
    const char* bucket_name3 = "bulk_put_performance_bucket3";
    ds3_request* request = NULL;
    ds3_master_object_list_response* bulk_response1 = NULL;
    ds3_master_object_list_response* bulk_response2 = NULL;
    ds3_master_object_list_response* bulk_response3 = NULL;

    ds3_client* client1 = get_client();
    ds3_client* client2 = ds3_copy_client(client1); // share the connection pool
    ds3_client* client3 = ds3_copy_client(client1); // share the connection pool
    // Log per thread
    int client1_thread=1, client2_thread=2, client3_thread=3;
    ds3_client_register_logging(client1, DS3_INFO, test_log, (void*)&client1_thread);
    ds3_client_register_logging(client2, DS3_INFO, test_log, (void*)&client2_thread);
    ds3_client_register_logging(client3, DS3_INFO, test_log, (void*)&client3_thread);


    ds3_error* error = create_bucket_with_data_policy(client1, bucket_name1, ids.data_policy_id->value);
    handle_error(error);
    error = create_bucket_with_data_policy(client1, bucket_name2, ids.data_policy_id->value);
    handle_error(error);
    error = create_bucket_with_data_policy(client1, bucket_name3, ids.data_policy_id->value);
    handle_error(error);

    // Create the list of fake files to transfer
    size_t obj_size = 512 * 1024 * 1024; // 512MB
    const char* obj_prefix = "perf_obj";
    size_t num_files = 10;
    ds3_bulk_object_list_response* obj_list = create_bulk_object_list_from_prefix_with_size(obj_prefix, num_files, obj_size);

    // Create the BULK_PUT jobs
    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name1, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client1, request, &bulk_response1);
    handle_error(error);
    ds3_request_free(request);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name2, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client1, request, &bulk_response2);
    handle_error(error);
    ds3_request_free(request);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name3, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client1, request, &bulk_response3);
    handle_error(error);
    ds3_request_free(request);

    // Ensure cache space for the jobs
    ds3_master_object_list_response* chunks_response1 = ensure_available_chunks(client1, bulk_response1->job_id);
    ds3_master_object_list_response* chunks_response2 = ensure_available_chunks(client2, bulk_response2->job_id);
    ds3_master_object_list_response* chunks_response3 = ensure_available_chunks(client3, bulk_response3->job_id);

    GPtrArray* put_perf_objs_threads_array1 = new_put_chunks_threads_args(client1, obj_prefix, NULL, bucket_name1, bulk_response1, chunks_response1, 1, True);
    GPtrArray* put_perf_objs_threads_array2 = new_put_chunks_threads_args(client2, obj_prefix, NULL, bucket_name2, bulk_response2, chunks_response2, 1, True);
    GPtrArray* put_perf_objs_threads_array3 = new_put_chunks_threads_args(client3, obj_prefix, NULL, bucket_name3, bulk_response3, chunks_response3, 1, True);

    // capture sequential test start time
    struct timespec start_time_t, end_time_t;
    double elapsed_t;
    clock_gettime(CLOCK_MONOTONIC, &start_time_t);

    // Spawn threads
    GThread* xfer_thread_1 = g_thread_new("performance_objs_xfer_1", (GThreadFunc)put_chunks_from_mem, g_ptr_array_index(put_perf_objs_threads_array1, 0));
    GThread* xfer_thread_2 = g_thread_new("performance_objs_xfer_2", (GThreadFunc)put_chunks_from_mem, g_ptr_array_index(put_perf_objs_threads_array2, 0));
    GThread* xfer_thread_3 = g_thread_new("performance_objs_xfer_3", (GThreadFunc)put_chunks_from_mem, g_ptr_array_index(put_perf_objs_threads_array3, 0));

    // Block and cleanup GThreads
    g_thread_join(xfer_thread_1);
    g_thread_join(xfer_thread_2);
    g_thread_join(xfer_thread_3);

    // find elapsed CPU and real time
    clock_gettime(CLOCK_MONOTONIC, &end_time_t);
    elapsed_t = timespec_to_seconds(&end_time_t) - timespec_to_seconds(&start_time_t);
    printf("  Elapsed time[%f]\n", elapsed_t);

    ds3_master_object_list_response_free(bulk_response1);
    ds3_master_object_list_response_free(chunks_response1);
    put_chunks_threads_args_free(put_perf_objs_threads_array1);
    clear_bucket(client1, bucket_name1);
    free_client(client1);

    ds3_master_object_list_response_free(bulk_response2);
    ds3_master_object_list_response_free(chunks_response2);
    put_chunks_threads_args_free(put_perf_objs_threads_array2);
    clear_bucket(client2, bucket_name2);
    free_client(client2);

    ds3_master_object_list_response_free(bulk_response3);
    ds3_master_object_list_response_free(chunks_response3);
    put_chunks_threads_args_free(put_perf_objs_threads_array3);
    clear_bucket(client3, bucket_name3);
    free_client(client3);

    ds3_bulk_object_list_response_free(obj_list);
}

