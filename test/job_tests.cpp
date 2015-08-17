#include <stdio.h>
#include "../src/ds3.h"
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
    const char* bucket2_name = "bucket_test_get_job2";
    ds3_bulk_object_list* bucket2_object_list = NULL;

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
    ds3_free_request(request);
    ds3_free_bulk_object_list(bucket1_object_list);


    /* init bulk get bucket2 */
    request = ds3_init_get_bulk(bucket2_name, bucket2_object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);
    ds3_free_request(request);
    ds3_free_bulk_object_list(bucket2_object_list);

    /* GET jobs */
    request = ds3_init_get_jobs();
    error = ds3_get_jobs(client, request, &get_jobs_response);
    handle_error(error);
    ds3_free_request(request);
    BOOST_CHECK(get_jobs_response != NULL);
/*
typedef struct{
    char* value;
    size_t size;
}ds3_str;

typedef struct {
    ds3_str*                bucket_name;
    uint64_t                cached_size_in_bytes;
    ds3_chunk_ordering      chunk_order;
    uint64_t                completed_size_in_bytes;
    ds3_str*                job_id;
    uint64_t                original_size_in_bytes;
    ds3_job_priority        priority;
    ds3_job_request_type    request_type;
    ds3_str*                start_date;
    ds3_str*                user_id;
    ds3_str*                user_name;
    ds3_write_optimization  write_optimization;
    ds3_bulk_object_list**  list;
    size_t                  list_size;
    ds3_job_status          status;
}ds3_bulk_response;

typedef struct {
    ds3_bulk_response** jobs;
    size_t   jobs_size;
}ds3_get_jobs_response;
*/
    uint8_t job_index;
    printf("\n\n  get_jobs_response->jobs_size[%lu]\n", get_jobs_response->jobs_size );

    for( job_index = 0; job_index < get_jobs_response->jobs_size; job_index++ ) {
        bulk_response = get_jobs_response->jobs[job_index];
        printf( "  job_index[%u]\n", job_index );
        printf( "  bucket_name[%s]\n", bulk_response->bucket_name->value );
        printf( "  job_id[%s]\n", bulk_response->job_id->value );
        printf( "  start_date[%s]\n", bulk_response->start_date->value );
        printf( "  user_id[%s]\n", bulk_response->user_id->value );
        printf( "  user_name[%s]\n", bulk_response->user_name->value );
    }
//    BOOST_CHECK(get_jobs_response->jobs_size == 2);

    /* teardown */
    clear_bucket(client, bucket1_name);
    clear_bucket(client, bucket2_name);
    free_client(client);
    ds3_free_get_jobs_response( get_jobs_response );
}

