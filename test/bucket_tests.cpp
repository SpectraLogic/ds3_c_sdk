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

BOOST_AUTO_TEST_CASE( bulk_put ) {
    printf("-----Testing Bulk PUT-------\n");

    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "test_bulk_put_bucket";
    uint64_t num_objs;

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 5);
    BOOST_CHECK(contains_object(response, "resources/beowulf.txt"));

    ds3_list_bucket_result_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( empty_folder ) {
    printf("-----Testing put empty folder-------\n");

    ds3_client* client = get_client();
    const char* bucket_name = "test_put_empty_folder_bucket";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_put_object_request(bucket_name, "empty-folder/", 0);
    error   = ds3_put_object_request(client, request, NULL, NULL);
    ds3_request_free(request);
    handle_error(error);

    ds3_list_bucket_result_response* response;
    request = ds3_init_get_bucket_request(bucket_name);
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK_EQUAL(response->num_objects, 1);
    BOOST_CHECK(contains_object(response, "empty-folder/"));

    ds3_list_bucket_result_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( prefix ) {
    printf("-----Testing Prefix-------\n");

    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "test_prefix_bucket";

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    ds3_request_set_prefix(request, "resources/beo");
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK_EQUAL(response->num_objects, 1);
    BOOST_CHECK(contains_object(response, "resources/beowulf.txt"));

    ds3_list_bucket_result_response_free(response);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delimiter ) {
    printf("-----Testing Delimiter-------\n");

    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "test_delimiter_bucket";

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    ds3_request_set_delimiter(request, "/");
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK_EQUAL(response->num_objects, 0);
    BOOST_CHECK_EQUAL(response->num_common_prefixes, 1);
    BOOST_CHECK_EQUAL(strcmp(response->common_prefixes[0]->value, "resources/"), 0);

    ds3_list_bucket_result_response_free(response);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( common_prefixes ) {
    printf("-----Testing CommonPrefixes-------\n");

    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "test_common_prefixes_bucket";
    ds3_bool found_resources = False;
    ds3_bool found_resources_2 = False;
    size_t common_prefix_index;

    populate_with_multi_dir_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    ds3_request_set_delimiter(request, "/");
    ds3_request_set_prefix(request, "resources");
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK_EQUAL(response->num_objects, 0);
    BOOST_CHECK_EQUAL(response->num_common_prefixes, 2);
    for(common_prefix_index = 0; common_prefix_index < response->num_common_prefixes; common_prefix_index++) {
        if(strcmp(response->common_prefixes[common_prefix_index]->value, "resources/") == 0) {
            found_resources = True;
        } else if (strcmp(response->common_prefixes[common_prefix_index]->value, "resources_2/") == 0) {
            found_resources_2 = True;
        }
    }
    BOOST_CHECK_EQUAL(found_resources, True);
    BOOST_CHECK_EQUAL(found_resources_2, True);

    ds3_list_bucket_result_response_free(response);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(marker) {
    printf("-----Testing Marker-------\n");

    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "test_marker_bucket";

    populate_with_objects(client,bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    ds3_request_set_marker(request, "resources/sherlock_holmes.txt");
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK_EQUAL(response->num_objects, 3);
    BOOST_CHECK(contains_object(response, "resources/tale_of_two_cities.txt"));
    BOOST_CHECK(contains_object(response, "resources/ulysses.txt"));
    BOOST_CHECK(contains_object(response, "resources/ulysses_large.txt"));

    ds3_list_bucket_result_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(max_keys) {
    printf("-----Testing Max-Keys-------\n");

    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "test_max_keys_bucket";

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    ds3_request_set_max_keys(request, 2);
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK_EQUAL(response->num_objects, 2);
    BOOST_CHECK(contains_object(response, "resources/beowulf.txt"));
    BOOST_CHECK(contains_object(response, "resources/sherlock_holmes.txt"));
    ds3_list_bucket_result_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

// This test case is no longer valid for r3x since checksums types can only be 
// set on a per data-policy basis rather than per object.
/*
BOOST_AUTO_TEST_CASE(checksum) {
    uint64_t checksums, chunk_index, object_index;
    const char* bucket_name = "checksum_test_bucket";
    ds3_request* request;
    const char* books[] ={"resources/beowulf.txt"};
    ds3_client* client = get_client();
    ds3_error* error;
    ds3_bulk_object_list_response* obj_list;
    ds3_master_object_list_response* response;
    ds3_bool job_done = False;

    ds3_str* data_policy_id;
    ds3_bool original_checksum_type_known = False;
    ds3_checksum_type original_checksum_type;
    ds3_checksum_type test_checksum_type;
    ds3_data_policy_response* data_policy_response;

    printf("-----Testing Checksums-------\n");

    obj_list = ds3_convert_file_list(books, 1);

    for (checksums = 0; checksums < 5; checksums ++) {
        request = init_put_bucket_request(bucket_name);
        error = put_bucket_request(client, request);
        ds3_request_free(request);
        handle_error(error);

        if (original_checksum_type_known == False) {
            error = get_bucket_data_policy_id(client, bucket_name, data_policy_id);
            handle_error(error);

            error = get_bucket_data_policy_checksum(client, bucket_name, &original_checksum_type);
            handle_error(error);
        }

        request = init_put_bulk_job_spectra_s3_request(bucket_name, NULL, NULL, NULL, NULL, NULL, obj_list);
        error = put_bulk_job_spectra_s3_request(client, request, &response);
        ds3_request_free(request);
        handle_error(error);

        while (job_done == False) {
            request = init_get_job_chunks_ready_for_client_processing_spectra_s3_request(response->objects[object_index]->chunk_id->value, NULL);
            error = get_job_chunks_ready_for_client_processing_spectra_s3_request(client, request, &response);
            ds3_request_free(request);
            if (error->error->http_error_code == 410) {
                job_done = True;
                break;
            }
            handle_error(error);

            for (chunk_index = 0; chunk_index < response->num_objects; chunk_index++) {
                ds3_objects_response* job_chunk = response->objects[chunk_index];

                for (object_index = 0; object_index < job_chunk->num_objects; object_index++) {
                    ds3_bulk_object_response* bulk_object = job_chunk->objects[object_index];
                    const uint64_t* length = &bulk_object->length;
                    const uint64_t* offset = &bulk_object->offset;
                    FILE* file = fopen(bulk_object->name->value, "r");

                    request = init_put_object_request(bucket_name, bulk_object->name->value, length, response->job_id->value,  offset);

                    switch(checksums) {
                        case 0:
                            test_checksum_type = DS3_CHECKSUM_TYPE_MD5;
                            request = init_modify_data_policy_spectra_s3_request(data_policy_id->value, NULL, &test_checksum_type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                            error = modify_data_policy_spectra_s3_request(client, request, &data_policy_response);
                            handle_error(error);
                            ds3_request_set_md5(request,"rCu751L6xhB5zyL+soa3fg==");
                            break;
                        case 1:
                            test_checksum_type = DS3_CHECKSUM_TYPE_SHA_256;
                            request = init_modify_data_policy_spectra_s3_request(data_policy_id->value, NULL, &test_checksum_type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                            error = modify_data_policy_spectra_s3_request(client, request, &data_policy_response);
                            handle_error(error);
                            ds3_request_set_sha256(request,"SbLeE3Wb46VCj1atLhS3FRC/Li87wTGH1ZtYIqOjO+E=");
                            break;
                        case 2:
                            test_checksum_type = DS3_CHECKSUM_TYPE_SHA_512;
                            request = init_modify_data_policy_spectra_s3_request(data_policy_id->value, NULL, &test_checksum_type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                            error = modify_data_policy_spectra_s3_request(client, request, &data_policy_response);
                            handle_error(error);
                            ds3_request_set_sha512(request,"qNLwiDVNQ3YCYs9gjyB2LS5cYHMvKdnLaMveIazkWKROKr03F9i+sV5YEvTBjY6YowRE8Hsqw+iwP9KOKM0Xvw==");
                            break;
                        case 3:
                            test_checksum_type = DS3_CHECKSUM_TYPE_CRC_32;
                            request = init_modify_data_policy_spectra_s3_request(data_policy_id->value, NULL, &test_checksum_type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                            error = modify_data_policy_spectra_s3_request(client, request, &data_policy_response);
                            handle_error(error);
                            ds3_request_set_crc32(request,"bgHSXg==");
                            break;
                        case 4:
                            test_checksum_type = DS3_CHECKSUM_TYPE_CRC_32C;
                            request = init_modify_data_policy_spectra_s3_request(data_policy_id->value, NULL, &test_checksum_type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                            error = modify_data_policy_spectra_s3_request(client, request, &data_policy_response);
                            handle_error(error);
                            ds3_request_set_crc32c(request, "+ZBZbQ==");
                            break;
                    }

                    if (bulk_object.offset > 0) {
                        fseek(file, bulk_object.offset, SEEK_SET);
                    }
                    error = ds3_put_object(client, request, file, ds3_read_from_file);
                    ds3_free_request(request);
                    fclose(file);
                    handle_error(error);
                }
                ds3_free_allocate_chunk_response(chunk_response);
        }

        ds3_free_bulk_response(response);
        clear_bucket(client, bucket_name);
    }

    ds3_str_free(data_policy_id);
    ds3_free_bulk_object_list(obj_list);
    free_client(client);
}
*/

