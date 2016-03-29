#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_all_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%txt%");
    ds3_error* error = ds3_get_objects(client, request, &response);

    handle_error(error);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 5);

    ds3_free_request(request);
    ds3_free_objects_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_no_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%frankenstein.txt%");
    ds3_error* error = ds3_get_objects(client, request, &response);

    handle_error(error);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 0);

    ds3_free_request(request);
    ds3_free_objects_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_two_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%ulysses%");
    ds3_error* error = ds3_get_objects(client, request, &response);

    handle_error(error);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 2);

    ds3_free_request(request);
    ds3_free_objects_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_one_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%ulysses.txt%");
    ds3_error* error = ds3_get_objects(client, request, &response);

    handle_error(error);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 1);

    ds3_free_request(request);
    ds3_free_objects_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_one_objects_with_plus) {
	uint64_t num_objs;
	ds3_client* client = get_client();
	ds3_get_objects_response* response;
	const char* bucket_name = "Get_objects_with_plus";
	const char* plus_object = "Plus+Object"

	ds3_request* put_request = ds3_init_put_object_for_job(bucket_name, plus_object, 0, 1024, NULL);
	ds3_error* error = ds3_put_object(client, put_request, NULL, NULL);
	ds3_free_request(put_request);
	handle_error(error);

	ds3_request* request = ds3_init_get_objects();
	ds3_request_set_bucket_name(request, bucket_name);
	ds3_request_set_name(request, plus_object);
	error = ds3_get_objects(client, request, &response);

	handle_error(error);
	num_objs = response->num_objects;
	BOOST_CHECK_EQUAL(num_objs, 1);

	ds3_free_request(request);
	ds3_free_objects_response(response);

	clear_bucket(client, bucket_name);
	free_client(client);
}

BOOST_AUTO_TEST_CASE( get_folder_and_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects();
    ds3_request_set_bucket_name(request, bucket_name);
    ds3_request_set_name(request, "%resources%");
    ds3_error* error = ds3_get_objects(client, request, &response);

    handle_error(error);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 5);

    ds3_free_request(request);
    ds3_free_objects_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_incorrect_bucket_name ) {
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects();
    ds3_request_set_bucket_name(request, "fake_bucket");
    ds3_request_set_name(request, "%resources%");
    ds3_error* error = ds3_get_objects(client, request, &response);

    BOOST_CHECK(error!=NULL);
    BOOST_CHECK(error->error->status_code == 404);

    ds3_free_request(request);
    ds3_free_error(error);

    clear_bucket(client, bucket_name);
    free_client(client);
}
