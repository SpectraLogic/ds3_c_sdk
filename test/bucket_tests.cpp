#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( bulk_put ) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    uint64_t num_objs;

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    num_objs = response->num_objects;

    BOOST_CHECK(error == NULL);
    BOOST_CHECK_EQUAL(num_objs, 5);

    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/beowulf.txt"));

    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( prefix ) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    uint64_t num_objs;

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    ds3_request_set_prefix(request, "resources/beo");
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    BOOST_CHECK(error == NULL);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 1);

    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/beowulf.txt"));

    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);

    free_client(client);
}

BOOST_AUTO_TEST_CASE( delimiter ) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    uint64_t num_objs;

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    ds3_request_set_delimiter(request, "/");
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    BOOST_CHECK(error == NULL);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 0);
	
    BOOST_CHECK_EQUAL(response->num_common_prefixes, 1);
    BOOST_CHECK_EQUAL(strcmp(response->common_prefixes[0]->value, "resources/"), 0);

    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(marker){
	ds3_request* request;
	ds3_error* error;
	ds3_get_bucket_response* response;
	ds3_client* client = get_client();
	const char* bucket_name = "bucket_test_marker";
	uint64_t num_objs;
	
	populate_with_objects(client,bucket_name);
	
	request = ds3_init_get_bucket(bucket_name);
	ds3_request_set_marker(request,"resources/sherlock_holmes.txt");
	error = ds3_get_bucket(client,request,&response);
	ds3_free_request(request);
	
	handle_error(error);
	num_objs = response->num_objects;
	
	BOOST_CHECK_EQUAL(num_objs, 3);
	BOOST_CHECK(contains_object(response->objects, num_objs, "resources/tale_of_two_cities.txt"));
	BOOST_CHECK(contains_object(response->objects, num_objs, "resources/ulysses.txt"));
	BOOST_CHECK(contains_object(response->objects, num_objs, "resources/ulysses_large.txt"));
	
	ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
	
	
}

BOOST_AUTO_TEST_CASE(max_keys)
{
	ds3_request* request;
	ds3_error* error;
	ds3_get_bucket_response* response;
	ds3_client* client = get_client();
	const char* bucket_name = "bucket_test_max_keys";
	uint64_t num_objs;
	
	populate_with_objects(client,bucket_name);
	
	request = ds3_init_get_bucket(bucket_name);
	ds3_request_set_max_keys(request,2);
	
	error = ds3_get_bucket(client,request,&response);
	ds3_free_request(request);
	
	handle_error(error);
	num_objs = response->num_objects;
	
	BOOST_CHECK_EQUAL(num_objs, 2);
	BOOST_CHECK(contains_object(response->objects, num_objs, "resources/beowulf.txt"));
	BOOST_CHECK(contains_object(response->objects, num_objs, "resources/sherlock_holmes.txt"));
	ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
	
	
}




