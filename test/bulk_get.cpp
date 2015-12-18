#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ds3.h"
#include "ds3_net.h"
#include "test.h"
#include <boost/test/unit_test.hpp>
#include "checksum.h"

#define FILE_TEMPLATE "bulk-XXXXXX"

BOOST_AUTO_TEST_CASE( bulk_get ) {
    uint64_t i, n;
    uint64_t file_index = 0;
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_bulk_response* completed_job = NULL;
    ds3_get_bucket_response* response = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_bulk_object_list* object_list = NULL;
    ds3_get_available_chunks_response* chunk_response = NULL;
    bool retry_get;

    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    char* orignal_file_path [5];

    char** tmp_files;

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &response);

    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);

    tmp_files = (char**) calloc(response->num_objects, sizeof(char*));

    object_list = ds3_convert_object_list(response->objects, response->num_objects);

    ds3_free_bucket_response(response);

    request = ds3_init_get_bulk(bucket_name, object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);

    BOOST_REQUIRE(error == NULL);

    do {
        retry_get = false;
        request = ds3_init_get_available_chunks(bulk_response->job_id->value);

        error = ds3_get_available_chunks(client, request, &chunk_response);

        ds3_free_request(request);

        BOOST_REQUIRE(error == NULL);

        BOOST_REQUIRE(chunk_response != NULL);

        if (chunk_response->object_list->list_size == 0) {
            // if this happens we need to try the request
            retry_get = true;
            BOOST_TEST_MESSAGE( "Hit retry, sleeping for: " << chunk_response->retry_after) ;
            sleep(chunk_response->retry_after);
        }

    } while(retry_get);

    BOOST_REQUIRE(error == NULL);

    bool checksum_passed = true;

    for (i = 0; i < chunk_response->object_list->list_size; i++) {
        ds3_bulk_object_list* chunk_object_list = chunk_response->object_list->list[i];
        for(n = 0; n < chunk_object_list->size; n++, file_index++) {
            FILE* w_file;
            ds3_bulk_object current_obj = chunk_object_list->list[n];
            request = ds3_init_get_object_for_job(bucket_name, current_obj.name->value, current_obj.offset, bulk_response->job_id->value);
            orignal_file_path[file_index] = current_obj.name->value;
            tmp_files[file_index] = (char*) calloc(12, sizeof(char));
            memcpy(tmp_files[file_index], FILE_TEMPLATE, 11);
            w_file = fopen(tmp_files[file_index], "w+");
            error = ds3_get_object(client, request, w_file, ds3_write_to_file);
            ds3_free_request(request);
            fclose(w_file);
            handle_error(error);
            printf("------Performing Data Integrity Test-------\n");
            checksum_passed = checksum_passed && compare_hash(orignal_file_path[file_index],tmp_files[file_index]);
            printf("\n");
        }
    }


    for (i = 0; i < file_index; i++) {
        unlink(tmp_files[i]);
        free(tmp_files[i]);
    }

    free(tmp_files);

    BOOST_CHECK(checksum_passed == true);

    // check to make sure that the 'job' has completed
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_available_chunks_response(chunk_response);
    ds3_free_bulk_response(completed_job);
    ds3_free_bulk_response(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
    handle_error(error);
}

BOOST_AUTO_TEST_CASE( partial_get ) {
    uint64_t i, n;
    uint64_t file_index = 0;
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_bulk_response* completed_job = NULL;
    ds3_get_bucket_response* response = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_bulk_object_list* object_list = NULL;
    ds3_get_available_chunks_response* chunk_response = NULL;
    bool retry_get;

    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    char* orignal_file_path [5];

    char** tmp_files;

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &response);

    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);

    tmp_files = (char**) calloc(response->num_objects, sizeof(char*));

    object_list = ds3_convert_object_list(response->objects, response->num_objects);

    ds3_free_bucket_response(response);

    request = ds3_init_get_bulk(bucket_name, object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);

    BOOST_REQUIRE(error == NULL);

    do {
        retry_get = false;
        request = ds3_init_get_available_chunks(bulk_response->job_id->value);

        error = ds3_get_available_chunks(client, request, &chunk_response);

        ds3_free_request(request);

        BOOST_REQUIRE(error == NULL);

        BOOST_REQUIRE(chunk_response != NULL);

        if (chunk_response->object_list->list_size == 0) {
            // if this happens we need to try the request
            retry_get = true;
            BOOST_TEST_MESSAGE( "Hit retry, sleeping for: " << chunk_response->retry_after) ;
            sleep(chunk_response->retry_after);
        }

    } while(retry_get);

    BOOST_REQUIRE(error == NULL);

    bool checksum_passed = true;

    uint32_t segment_size = 3200;
    for (i = 0; i < chunk_response->object_list->list_size; i++) {
        ds3_bulk_object_list* chunk_object_list = chunk_response->object_list->list[i];
        for(n = 0; n < chunk_object_list->size; n++, file_index++) {
            FILE* w_file;
            ds3_bulk_object current_obj = chunk_object_list->list[n];
            request = ds3_init_get_object_for_job(bucket_name, current_obj.name->value, current_obj.offset, bulk_response->job_id->value);
            ds3_request_set_byte_range(request, segment_size, segment_size*2-1);
            ds3_request_set_byte_range(request, segment_size*3, segment_size*4-1);
            orignal_file_path[file_index] = current_obj.name->value;
            tmp_files[file_index] = (char*) calloc(12, sizeof(char));
            memcpy(tmp_files[file_index], FILE_TEMPLATE, 11);
            w_file = fopen(tmp_files[file_index], "w+");
            error = ds3_get_object(client, request, w_file, ds3_write_to_file);
            ds3_free_request(request);
            fclose(w_file);
            handle_error(error);
            printf("------Performing Data Integrity Test-------\n");
            checksum_passed = checksum_passed && compare_hash_extended(orignal_file_path[file_index], tmp_files[file_index], segment_size, segment_size, 0);
            checksum_passed = checksum_passed && compare_hash_extended(orignal_file_path[file_index], tmp_files[file_index], segment_size, segment_size*3, segment_size);
            printf("\n");
        }
    }


    for (i = 0; i < file_index; i++) {
        unlink(tmp_files[i]);
        free(tmp_files[i]);
    }

    free(tmp_files);

    BOOST_CHECK(checksum_passed == true);

    // NOTE: this currently returns an error from the server because of the partial get
    // check to make sure that the 'job' has completed
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);
    handle_error(error);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_available_chunks_response(chunk_response);
    ds3_free_bulk_response(completed_job);
    ds3_free_bulk_response(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( escape_urls ) {
    const char *delimiters[4]={"or", "/", "@", "="};
    const char *strings_to_test[5]={"some normal text", "/an/object/name", "bytes=0-255,300-400,550-800", "orqwerty/qwerty@qwerty=", "`1234567890-=~!@#$%^&*()_+[]\{}|;:,./<>?"}; 
    const char *object_name_results[5]={"some%20normal%20text", "/an/object/name", "bytes%3D0-255%2C300-400%2C550-800", "orqwerty/qwerty%40qwerty%3D",
                                        "%601234567890-%3D~%21%40%23%24%25%5E%26%2A%28%29_%2B%5B%5D%7B%7D%7C%3B%3A%2C./%3C%3E%3F"};
    const char *range_header_results[5]={"some%20normal%20text", "%2Fan%2Fobject%2Fname", "bytes=0-255,300-400,550-800", "orqwerty%2Fqwerty%40qwerty=",
                                         "%601234567890-=~%21%40%23%24%25%5E%26%2A%28%29_%2B%5B%5D%7B%7D%7C%3B%3A,.%2F%3C%3E%3F"};
    const char *general_delimiter_results[5]={"some%20normal%20text", "/an/object/name", "bytes=0-255%2C300-400%2C550-800", "orqwerty/qwerty@qwerty=",
                                              "%601234567890-=~%21@%23%24%25%5E%26%2A%28%29_%2B%5B%5D%7B%7D%7C%3B%3A%2C./%3C%3E%3F"};

    for(int i=0; i<5; i++) {
        BOOST_CHECK(strcmp(escape_url_object_name(strings_to_test[i]), object_name_results[i]) == 0);
    }
    for(int i=0; i<5; i++) {
        BOOST_CHECK(strcmp(escape_url_range_header(strings_to_test[i]), range_header_results[i]) == 0);
    }
    for(int i=0; i<5; i++) {
        BOOST_CHECK(strcmp(escape_url_extended(strings_to_test[i], delimiters, 4), general_delimiter_results[i]) == 0);
    }
}

BOOST_AUTO_TEST_CASE( convert_list_helper ) {
    const char* books[4] ={"beowulf.txt", "sherlock_holmes.txt", "tale_of_two_cities.txt", "ulysses.txt"};
    ds3_bulk_object_list* obj_list;

    obj_list = ds3_convert_file_list_with_basepath(books, 4, "resources/");

    BOOST_CHECK(strcmp(obj_list->list[0].name->value, "beowulf.txt") == 0);
    BOOST_CHECK(obj_list->list[0].length == 294059);

    ds3_free_bulk_object_list(obj_list);
}

BOOST_AUTO_TEST_CASE( directory_size ) {
    const char* books[1] ={"resources"};
    ds3_bulk_object_list* obj_list;

    obj_list = ds3_convert_file_list_with_basepath(books, 1, NULL);

    BOOST_CHECK(strcmp(obj_list->list[0].name->value, "resources") == 0);
    BOOST_CHECK(obj_list->list[0].length == 0);

    ds3_free_bulk_object_list(obj_list);
}
