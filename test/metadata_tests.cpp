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
#include <glib.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( put_metadata ) {
    printf("-----Testing put_metadata-------\n");

    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";
    FILE* file;

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_put_object_request(bucket_name, "empty-folder/", 0);
    error = ds3_put_object_request(client, request, NULL, NULL);
    ds3_request_free(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);

    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_job(request, bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "name", "value");

    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_CHECK(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 1);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_CHECK(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "name") == 0);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_emtpy_metadata ) {
    printf("-----Testing put_emtpy_metadata-------\n");

    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";
    FILE* file;

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_put_object_request(bucket_name, "empty-folder/", 0);
    error = ds3_put_object_request(client, request, NULL, NULL);
    ds3_request_free(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);

    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_job(request, bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "name", "");

    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 0);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_CHECK(metadata_entry == NULL);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_null_metadata ) {
    printf("-----Testing put_null_metadata-------\n");

    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";
    FILE* file;

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_put_object_request(bucket_name, "empty-folder/", 0);
    error = ds3_put_object_request(client, request, NULL, NULL);
    ds3_request_free(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);

    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_job(request, bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "name", NULL);

    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 0);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_CHECK(metadata_entry == NULL);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}
BOOST_AUTO_TEST_CASE( head_bucket ) {
    printf("-----Testing head_bucket-------\n");

    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_head_bucket_request(bucket_name);

    error = ds3_head_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( head_folder ) {
    printf("-----Testing head_folder-------\n");

    ds3_metadata* metadata_result;
    ds3_client* client = get_client();
    const char* bucket_name = "head_folder_test";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    const char* test_folder = "test_folder/";
    ds3_request* request = ds3_init_put_object_request(bucket_name, test_folder, 0);
    error   = ds3_put_object_request(client, request, NULL, NULL);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_head_object_request(bucket_name, test_folder);

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_CHECK(metadata_result != NULL);
    ds3_metadata_free(metadata_result);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_multiple_metadata_items ) {
    printf("-----Testing put_multiple_metadata_items-------\n");

    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    //ds3_client* client = get_client_at_loglvl(DS3_DEBUG);
    ds3_client* client = get_client();
    const char* bucket_name = "multi_metadata_test";
    FILE* file;

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_job(request, bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "key", "value2");
    ds3_request_set_metadata(request, "name", "value");
    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");
    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_CHECK(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 2);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_CHECK(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "name") == 0);
    BOOST_CHECK(g_strcmp0(metadata_entry->values[0]->value, "value") == 0);
    ds3_metadata_entry_free(metadata_entry);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "key");
    BOOST_CHECK(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "key") == 0);
    BOOST_CHECK(g_strcmp0(metadata_entry->values[0]->value, "value2") == 0);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}

static bool contains_key(const ds3_metadata_keys_result* metadata_keys, const char* key) {
    uint64_t i;
    for (i = 0; i < metadata_keys->num_keys; i++) {
        if (g_strcmp0(key, metadata_keys->keys[i]->value) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOST_AUTO_TEST_CASE( metadata_keys ) {
    printf("-----Testing metadata_keys-------\n");

    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result = NULL;
    ds3_metadata_keys_result* metadata_keys = NULL;

    const char* file_name[1] = {"resources/beowulf.txt"};
    //ds3_client* client = get_client_at_loglvl(DS3_DEBUG);
    ds3_client* client = get_client();
    const char* bucket_name = "key_metadata_test";
    FILE* file;

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_job(request, bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "key", "value2");
    ds3_request_set_metadata(request, "name", "value");

    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_CHECK(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 2);

    metadata_keys = ds3_metadata_keys(metadata_result);
    BOOST_CHECK(metadata_keys != NULL);

    BOOST_CHECK(metadata_keys->num_keys == 2);
    BOOST_CHECK(contains_key(metadata_keys, "key"));
    BOOST_CHECK(contains_key(metadata_keys, "name"));

    ds3_metadata_keys_free(metadata_keys);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_metadata_using_get_object_retrieval ) {
    printf("-----Testing put_metadata_using_get_object_retrieval-------\n");

    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client();
    const char* bucket_name = "put_metadata_using_get_object_retrieval";
    FILE* file;

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_job(request, bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");

    ds3_request_set_metadata(request, "name", "value");
    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, obj_list);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(obj_list);
    handle_error(error);

    request = ds3_init_get_object_request(bucket_name, "resources/beowulf.txt", 0);
    ds3_request_set_job(request, bulk_response->job_id->value);
    file = fopen("/dev/null", "w");
    error = ds3_get_object_with_metadata(client, request, file, ds3_write_to_file, &metadata_result);
    ds3_request_free(request);
    ds3_master_object_list_response_free(bulk_response);
    fclose(file);
    handle_error(error);
    BOOST_CHECK(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 1);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_CHECK(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "name") == 0);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}
