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

    printf("-----Testing Get Job-------\n");

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

    printf("-----Testing Delete Job-------\n");

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

BOOST_AUTO_TEST_CASE(get_jobs){
    ds3_client* client = get_client();
    ds3_error* error = NULL;
    ds3_request* request = NULL;

    ds3_get_bucket_response* get_bucket_response = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_get_jobs_response* get_jobs_response = NULL;

    const char* bucket1_name = "bucket_test_get_job1";
    ds3_bulk_object_list* bucket1_object_list = NULL;
    ds3_str* bucket1_job_id = NULL;
    ds3_bool found_bucket1_job = False;
    const char* bucket2_name = "bucket_test_get_job2";
    ds3_bulk_object_list* bucket2_object_list = NULL;
    ds3_str* bucket2_job_id = NULL;
    ds3_bool found_bucket2_job = False;

    printf("-----Testing Get Jobs-------\n");

    /* create bucket1 with objects */
    populate_with_objects(client, bucket1_name);
    request = ds3_init_get_bucket(bucket1_name);
    error = ds3_get_bucket(client, request, &get_bucket_response);
    ds3_free_request(request);
    handle_error(error);

    // retain object_list for ds3_init_get_bulk
    bucket1_object_list = ds3_convert_object_list(get_bucket_response->objects, get_bucket_response->num_objects);
    ds3_free_bucket_response(get_bucket_response);

    /* create bucket2 with objects */
    populate_with_objects(client, bucket2_name);
    request = ds3_init_get_bucket(bucket2_name);
    error = ds3_get_bucket(client, request, &get_bucket_response);
    ds3_free_request(request);
    handle_error(error);

    // retain object_list for ds3_init_get_bulk
    bucket2_object_list = ds3_convert_object_list(get_bucket_response->objects, get_bucket_response->num_objects);
    ds3_free_bucket_response(get_bucket_response);

    /* init bulk get bucket1 */
    request = ds3_init_get_bulk(bucket1_name, bucket1_object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);
    bucket1_job_id = ds3_str_dup(bulk_response->job_id);
    ds3_free_bulk_object_list(bucket1_object_list);
    ds3_free_bulk_response(bulk_response);
    ds3_free_request(request);
    handle_error(error);

    /* init bulk get bucket2 */
    request = ds3_init_get_bulk(bucket2_name, bucket2_object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);
    bucket2_job_id = ds3_str_dup(bulk_response->job_id);
    ds3_free_bulk_object_list(bucket2_object_list);
    ds3_free_bulk_response(bulk_response);
    ds3_free_request(request);
    handle_error(error);

    /* GET jobs */
    request = ds3_init_get_jobs();
    error = ds3_get_jobs(client, request, &get_jobs_response);
    ds3_free_request(request);
    handle_error(error);

    /* Assert valid response contents */
    BOOST_CHECK(get_jobs_response != NULL);
    uint8_t job_index;
    for( job_index = 0; job_index < get_jobs_response->jobs_size; job_index++ ) {
        bulk_response = get_jobs_response->jobs[job_index];
        BOOST_CHECK( bulk_response->bucket_name->value != NULL);
        BOOST_CHECK( bulk_response->job_id->value != NULL);
        BOOST_CHECK( bulk_response->start_date->value != NULL);
        BOOST_CHECK( bulk_response->user_id->value != NULL);
        BOOST_CHECK( bulk_response->user_name->value != NULL);
        if(0 == strcmp(bulk_response->job_id->value, bucket1_job_id->value)) {
            found_bucket1_job = True;
        } else if(0 == strcmp(bulk_response->job_id->value, bucket2_job_id->value)) {
            found_bucket2_job = True;
        }
    }

    /* teardown */
    clear_bucket(client, bucket1_name);
    clear_bucket(client, bucket2_name);
    free_client(client);
    ds3_str_free(bucket1_job_id);
    ds3_str_free(bucket2_job_id);
    ds3_free_get_jobs_response( get_jobs_response );
    BOOST_REQUIRE(True == found_bucket1_job);
    BOOST_REQUIRE(True == found_bucket2_job);
}

