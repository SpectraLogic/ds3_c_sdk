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

void put_test_file(ds3_client* client, const char* file_name, const char* object_name, const char* bucket_name) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    FILE* file = fopen(file_name, "r");

    // Get file size
    uint64_t size = get_file_size(file_name);

    // Perform naked put object
    request = ds3_init_put_object_request(bucket_name, object_name, size);
    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
}

size_t test_write_buff_counter(void* buffer, size_t size, size_t nmemb, void* user_byte_count) {
    int cur_byte_count = size * nmemb;

    int* total_byte_count = (int*)user_byte_count;
    *total_byte_count += cur_byte_count;
    return cur_byte_count;
}

BOOST_AUTO_TEST_CASE( get_objects_with_versioning ) {
    printf("-----Testing GET object with versioning-------\n");
    const char* dp_name = "c_test_dp_with_versioning";
    const char* bucket_name = "test_get_object_bucket";
    const char* file_name1 = "resources/sherlock_holmes.txt";
    const char* file_name2 = "resources/beowulf.txt";
    const char* object_name = "sherlock_holmes_versioned.txt";
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_data_policy_response* put_dp_response = NULL;
    ds3_list_bucket_result_response* get_bucket_response = NULL;
    ds3_bulk_object_list_response* bulk_objects_response = NULL;
    ds3_master_object_list_response* bulk_get_response = NULL;

    ds3_client* client = get_client();

    // Create data policy with versioning turned on
    request = ds3_init_put_data_policy_spectra_s3_request(dp_name);
    ds3_request_set_versioning_ds3_versioning_level(request, DS3_VERSIONING_LEVEL_KEEP_MULTIPLE_VERSIONS);
    error = ds3_put_data_policy_spectra_s3_request(client, request, &put_dp_response);
    handle_error(error);

    BOOST_CHECK(put_dp_response != NULL);
    ds3_str* data_policy_id = ds3_str_init(put_dp_response->id->value);

    ds3_request_free(request);
    ds3_data_policy_response_free(put_dp_response);

    // Create data persistence rule
    ds3_data_persistence_rule_response* put_data_persistence_rule_response = NULL;
    request = ds3_init_put_data_persistence_rule_spectra_s3_request(
            data_policy_id->value,
            DS3_DATA_ISOLATION_LEVEL_STANDARD,
            TEST_SD_NAME,
            DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT);
    error = ds3_put_data_persistence_rule_spectra_s3_request(client, request, &put_data_persistence_rule_response);
    handle_error(error);
    ds3_str* data_persistence_rule_id = ds3_str_init(put_data_persistence_rule_response->id->value);

    ds3_request_free(request);
    ds3_data_persistence_rule_response_free(put_data_persistence_rule_response);

    // Create bucket with data policy
    error = create_bucket_with_data_policy(client, bucket_name, data_policy_id->value);
    handle_error(error);

    // Put object twice
    put_test_file(client, file_name1, object_name, bucket_name);
    put_test_file(client, file_name2, object_name, bucket_name);

    // Get version of an object TODO
    request = ds3_init_get_bucket_request(bucket_name);
    ds3_request_set_versions(request, True);
    error = ds3_get_bucket_request(client, request, &get_bucket_response);
    handle_error(error);

    BOOST_CHECK_EQUAL(get_bucket_response->num_objects, 0);
    BOOST_CHECK_EQUAL(get_bucket_response->num_versioned_objects, 2);

    ds3_request_free(request);


    // Perform bulk get with both versioned objects
    //TODO start
    bulk_objects_response = ds3_convert_object_list((const ds3_contents_response**)get_bucket_response->versioned_objects, get_bucket_response->num_versioned_objects);
    ds3_list_bucket_result_response_free(get_bucket_response);

    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, bulk_objects_response);

    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_get_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(bulk_objects_response);
    handle_error(error);

    /* TODO uncomment once related bug is fixed
    ds3_master_object_list_response* chunk_response = NULL;
    chunk_response = ensure_available_chunks(client, bulk_get_response->job_id);


    for (uint64_t chunk_index = 0; chunk_index < chunk_response->num_objects; chunk_index++) {
        ds3_objects_response* chunk_object_list = chunk_response->objects[chunk_index];
        for (uint64_t chunk_object_index = 0; chunk_object_index < chunk_object_list->num_objects; chunk_object_index++) {
            ds3_bulk_object_response* current_obj = chunk_object_list->objects[chunk_object_index];

            const uint64_t length = current_obj->length;
            const uint64_t offset = current_obj->offset;

            request = ds3_init_get_object_request(chunk_response->bucket_name->value, current_obj->name->value, length);
            ds3_request_set_job(request, chunk_response->job_id->value);
            ds3_request_set_offset(request, offset);
            ds3_request_set_version_id(request, current_obj->id->value);

            int byte_count = 0;
            error = ds3_get_object_request(client, request, &byte_count, test_write_buff_counter);
            ds3_request_free(request);
            handle_error(error);

            BOOST_CHECK(byte_count > 0); //todo update to check for expected length for this version
        }
    }

    ds3_master_object_list_response_free(chunk_response);
    */
    ds3_master_object_list_response_free(bulk_get_response);

    // Delete bucket with force i.e. with objects
    request = ds3_init_delete_bucket_spectra_s3_request(bucket_name);
    ds3_request_set_force(request, True);
    error = ds3_delete_bucket_spectra_s3_request(client, request);
    handle_error(error);
    ds3_request_free(request);

    // Delete data persistence rule
    ds3_request* delete_data_persistence_rule_request = ds3_init_delete_data_persistence_rule_spectra_s3_request(data_persistence_rule_id->value);
    error = ds3_delete_data_persistence_rule_spectra_s3_request(client, delete_data_persistence_rule_request);
    handle_error(error);
    ds3_request_free(delete_data_persistence_rule_request);
    ds3_str_free(data_persistence_rule_id);

    // Delete DataPolicy
    ds3_request* delete_data_policy_request = ds3_init_delete_data_policy_spectra_s3_request(dp_name);
    error = ds3_delete_data_policy_spectra_s3_request(client, delete_data_policy_request);
    handle_error(error);
    ds3_request_free(delete_data_policy_request);
    ds3_str_free(data_policy_id);

    free_client(client);
}
