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
#include <time.h>
#include "ds3.h"
#include "ds3_net.h"
#include "test.h"


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

