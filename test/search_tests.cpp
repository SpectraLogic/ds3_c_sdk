#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_all_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_s3_object_list_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%txt%");
    ds3_error* error = ds3_get_objects_spectra_s3_request(client, request, &response);

    handle_error(error);
    num_objs = response->num_s3_objects;
    BOOST_CHECK_EQUAL(num_objs, 5);

    ds3_request_free(request);
    ds3_s3_object_list_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_no_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_s3_object_list_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%frankenstein.txt%");
    ds3_error* error = ds3_get_objects_spectra_s3_request(client, request, &response);
    handle_error(error);
    num_objs = response->num_s3_objects;
    BOOST_CHECK_EQUAL(num_objs, 0);

    ds3_request_free(request);
    ds3_s3_object_list_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_two_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_s3_object_list_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%ulysses%");
    ds3_error* error = ds3_get_objects_spectra_s3_request(client, request, &response);
    handle_error(error);
    num_objs = response->num_s3_objects;
    BOOST_CHECK_EQUAL(num_objs, 2);

    ds3_request_free(request);
    ds3_s3_object_list_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_one_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_s3_object_list_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%ulysses.txt%");
    ds3_error* error = ds3_get_objects_spectra_s3_request(client, request, &response);
    handle_error(error);
    num_objs = response->num_s3_objects;
    BOOST_CHECK_EQUAL(num_objs, 1);

    ds3_request_free(request);
    ds3_s3_object_list_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_folder_and_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_s3_object_list_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%resources%");
    ds3_error* error = ds3_get_objects_spectra_s3_request(client, request, &response);
    handle_error(error);
    num_objs = response->num_s3_objects;
    BOOST_CHECK_EQUAL(num_objs, 5);

    ds3_request_free(request);
    ds3_s3_object_list_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_incorrect_bucket_name ) {
    ds3_client* client = get_client();
    ds3_s3_object_list_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_name(request, "fake_bucket");
    ds3_request_set_name(request, "%resources%");
    ds3_error* error = ds3_get_objects_spectra_s3_request(client, request, &response);
    BOOST_CHECK(error!=NULL);
    BOOST_CHECK(error->error->http_error_code == 404);

    ds3_request_free(request);
    ds3_error_free(error);

    clear_bucket(client, bucket_name);
    free_client(client);
}
