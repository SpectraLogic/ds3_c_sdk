#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>



BOOST_AUTO_TEST_CASE(get_job){
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_bulk_response* response_get = NULL;
    ds3_client* client = get_client();
    ds3_bulk_object_list* object_list = NULL;
    const char* bucket_name = "bucket_test_get_job";
    
    populate_with_objects(client, bucket_name);
    
    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &response);

    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);

    object_list = ds3_convert_object_list(response->objects, response->num_objects);

    ds3_free_bucket_response(response);

    request = ds3_init_get_bulk(bucket_name, object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);
  
	
	    
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &response_get);
    ds3_free_bulk_response(bulk_response);
	handle_error(error);
	
	
    BOOST_CHECK(response_get != NULL);
    BOOST_CHECK(response_get->status == IN_PROGRESS);
	ds3_free_request(request);
	ds3_free_bulk_response(response_get);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(delete_job){
	ds3_request* request;
	ds3_error* error;
	ds3_bulk_response* bulk_response = NULL;
	ds3_client* client = get_client();
	
	const char* bucket_name = "bucket_test_delete_job";
	
	ds3_str* job_id = populate_with_empty_objects(client, bucket_name);
	request = ds3_init_delete_job(job_id->value);
	error = ds3_delete_job(client,request);
	handle_error(error);
    ds3_free_request(request);
	 
	request = ds3_init_get_job(job_id->value);
    error = ds3_get_job(client, request, &bulk_response);
	handle_error(error);
	
	BOOST_CHECK(bulk_response->status==CANCELED);
	ds3_free_request(request);
	ds3_free_bulk_response(bulk_response);

    clear_bucket(client, bucket_name);
    ds3_str_free(job_id);
    free_client(client);
}



