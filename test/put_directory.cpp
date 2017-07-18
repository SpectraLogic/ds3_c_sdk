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

struct check_ds3_test_directory_given {
    return getenv(DS3_TEST_DIRECTORY) != NULL;
};

BOOST_AUTO_TEST_CASE( put_directory,
                      * boost::unit_test::precondition(check_ds3_test_directory_given)) {
    printf("-----Testing PUT all objects in a directory-------\n");

    const char* dir_path = getenv("DS3_TEST_DIRECTORY");
    BOOST_CHECK(dir_path != NULL);

    const char* bucket_name = "test_bulk_put_directory";

    ds3_request* request = NULL;
    ds3_master_object_list_response* bulk_response = NULL;

    ds3_client* client = get_client();
    int client_thread=1;
    ds3_client_register_logging(client, DS3_DEBUG, test_log, (void*)&client_thread); // Use DEBUG level logging

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    char** objects_list;
    uint64_t num_objs = 0;
    GDir* dir_info = g_dir_open(dir_path, 0, NULL);
    for (char* current_obj = (char*)g_dir_read_name(dir_info); current_obj != NULL; current_obj = (char*)g_dir_read_name(dir_info)) {
        objects_list[num_objs++] = current_obj;
    }

    ds3_bulk_object_list_response* bulk_object_list = ds3_convert_object_list_from_strings((const char**)objects_list, num_objs);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, bulk_object_list);
    ds3_master_object_list_response* mol;
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &mol);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(bulk_object_list);
    handle_error(error);

    ds3_master_object_list_response* chunks_list = ensure_available_chunks(client, mol->job_id);

    // Use helper functions from test.cpp
    GPtrArray* put_dir_args = new_put_chunks_threads_args(client, NULL, bucket_name, mol, chunks_list, 1, True); // Last param indicates verbose logging in the spawned thread

    // capture test start time
    struct timespec start_time_t, end_time_t;
    double elapsed_t;
    clock_gettime(CLOCK_MONOTONIC, &start_time_t);

    GThread* put_dir_xfer_thread = g_thread_new("put_dir_xfer_thread", (GThreadFunc)put_chunks_from_file, g_ptr_array_index(put_dir_args, 0));

    // Block and cleanup GThread(s)
    g_thread_join(put_dir_xfer_thread);

    // find elapsed CPU and real time
    clock_gettime(CLOCK_MONOTONIC, &end_time_t);
    elapsed_t = timespec_to_seconds(&end_time_t) - timespec_to_seconds(&start_time_t);
    ds3_log_message(client1->log, DS3_INFO, "  Elapsed time[%f]", elapsed_t);

    ds3_master_object_list_response_free(chunks_list);
    ds3_master_object_list_response_free(mol);
    put_chunks_threads_args_free(put_dir_args);
    clear_bucket(client, bucket_name);
    free_client(client);
}

