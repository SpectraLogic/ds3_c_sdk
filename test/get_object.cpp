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
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>
#include <glib.h>
#include <sys/stat.h>

ds3_contents_response* get_contents_by_name(const ds3_list_bucket_result_response* bucket_list, const char* object_name) {
    if (bucket_list == NULL) {
        return NULL;
    }

    ds3_contents_response* object = NULL;
    for (size_t object_index = 0; object_index < bucket_list->num_objects; object_index++) {
        object = bucket_list->objects[object_index];
        if (g_strcmp0(object_name, object->key->value) == 0) {
            return object;
        }
    }

    return NULL;
}

BOOST_AUTO_TEST_CASE( get_object ) {
    printf("-----Testing GET object-------\n");

    const char* bucket_name = "test_get_object_bucket";
    const char* object_name = "resources/sherlock_holmes.txt";
    const char* get_object_name = "sherlock_holmes_get.txt";
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_list_bucket_result_response* get_bucket_response;
    ds3_contents_response* sherlock = NULL;
    uint64_t sherlock_size = 0;
    FILE* fp;
    struct stat fstat;
    int status = 0;

    ds3_client* client = get_client();
    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    error = ds3_get_bucket_request(client, request, &get_bucket_response);
    ds3_request_free(request);
    handle_error(error);

    sherlock = get_contents_by_name(get_bucket_response, object_name);
    BOOST_CHECK(sherlock != NULL);

    sherlock_size = sherlock->size;
    ds3_list_bucket_result_response_free(get_bucket_response);

    request = ds3_init_get_object_request(bucket_name, object_name, sherlock_size);
    fp = fopen(get_object_name, "w+");

    error = ds3_get_object_request(client, request, fp, ds3_write_to_file);
    ds3_request_free(request);
    fclose(fp);
    handle_error(error);

    status = stat(get_object_name, &fstat);
    BOOST_CHECK(status != -1);
    BOOST_CHECK(fstat.st_size > 0);

    remove(get_object_name);
    clear_bucket(client, bucket_name);
    free_client(client);
}

