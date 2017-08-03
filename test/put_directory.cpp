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
#include "ds3_utils.h"
#include "test.h"

BOOST_AUTO_TEST_CASE( put_directory_4_threads) {
    printf("-----Testing PUT all objects in a directory with 4 threads-------\n");

    const char* dir_path = getenv("DS3_TEST_DIRECTORY");
    if (dir_path == NULL) {
        printf("ENV[DS3_TEST_DIRECTORY] unset - Skipping put_directory_4_threads test.\n");
        return;
    }

    const char* bucket_name = "test_bulk_put_directory";
    printf("  Putting all files in [%s] to bucket [%s]\n", dir_path, bucket_name);

    ds3_client* client = get_client_at_loglvl(DS3_DEBUG);
    int client_thread=1;
    ds3_client_register_logging(client, DS3_DEBUG, test_log, (void*)&client_thread); // Use DEBUG level logging

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    char* objects_list[100];
    uint64_t num_objs = 0;
    GDir* dir_info = g_dir_open(dir_path, 0, NULL);
    for (char* current_obj = (char*)g_dir_read_name(dir_info); current_obj != NULL; current_obj = (char*)g_dir_read_name(dir_info)) {
        objects_list[num_objs++] = current_obj;
        printf("  obj[%" PRIu64 "][%s]\n", num_objs, objects_list[num_objs-1]);
    }

    ds3_bulk_object_list_response* bulk_object_list = ds3_convert_file_list_with_basepath((const char**)objects_list, num_objs, dir_path);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, bulk_object_list);
    ds3_master_object_list_response* mol;
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &mol);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(bulk_object_list);
    handle_error(error);

    // Allocate cache
    ds3_master_object_list_response* chunks_list = ensure_available_chunks(client, mol->job_id);

    // Use helper functions from test.cpp
    const uint8_t num_threads = 4;
    GPtrArray* put_dir_args = new_put_chunks_threads_args(client, NULL, dir_path, bucket_name, mol, chunks_list, num_threads, True); // Last param indicates verbose logging in the spawned thread


    // capture test start time
    struct timespec start_time_t, end_time_t;
    double elapsed_t;
    clock_gettime(CLOCK_MONOTONIC, &start_time_t);

    GThread* put_dir_xfer_thread_0 = g_thread_new("put_dir_xfer_thread_0", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_dir_args, 0));
    GThread* put_dir_xfer_thread_1 = g_thread_new("put_dir_xfer_thread_1", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_dir_args, 1));
    GThread* put_dir_xfer_thread_2 = g_thread_new("put_dir_xfer_thread_2", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_dir_args, 2));
    GThread* put_dir_xfer_thread_3 = g_thread_new("put_dir_xfer_thread_3", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_dir_args, 3));

    // Block and cleanup GThread(s)
    g_thread_join(put_dir_xfer_thread_0);
    g_thread_join(put_dir_xfer_thread_1);
    g_thread_join(put_dir_xfer_thread_2);
    g_thread_join(put_dir_xfer_thread_3);

    // find elapsed CPU and real time
    clock_gettime(CLOCK_MONOTONIC, &end_time_t);
    elapsed_t = timespec_to_seconds(&end_time_t) - timespec_to_seconds(&start_time_t);
    ds3_log_message(client->log, DS3_INFO, "  Elapsed time[%f]", elapsed_t);

    g_dir_close(dir_info);
    ds3_master_object_list_response_free(chunks_list);
    ds3_master_object_list_response_free(mol);
    put_chunks_threads_args_free(put_dir_args);
    clear_bucket(client, bucket_name);
    free_client(client);
}
