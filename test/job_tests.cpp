#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(get_job){
    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_master_object_list_response* response_get = NULL;
    ds3_client* client = get_client();
    ds3_bulk_object_list_response* object_list = NULL;
    const char* bucket_name = "bucket_test_get_job";

    printf("-----Testing Get Job-------\n");

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    object_list = ds3_convert_object_list((const ds3_contents_response**)response->objects, response->num_objects);
    ds3_list_bucket_result_response_free(response);

    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, object_list);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);

    request = ds3_init_get_job_spectra_s3_request(bulk_response->job_id->value);
    error = ds3_get_job_spectra_s3_request(client, request, &response_get);
    ds3_master_object_list_response_free(bulk_response);
    handle_error(error);

    BOOST_CHECK(response_get != NULL);
    BOOST_CHECK(response_get->status == DS3_JOB_STATUS_IN_PROGRESS);
    ds3_request_free(request);
    ds3_master_object_list_response_free(response_get);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(cancel_job){
    ds3_request* request;
    ds3_error* error;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_client* client = get_client();

    const char* bucket_name = "bucket_test_cancel_job";

    printf("-----Testing Cancel Job-------\n");

    ds3_str* job_id = populate_with_empty_objects(client, bucket_name);

    request = ds3_init_cancel_job_spectra_s3_request(job_id->value);
    ds3_request_set_force(request, True);
    error = ds3_cancel_job_spectra_s3_request(client, request);
    handle_error(error);
    ds3_request_free(request);

    request = ds3_init_get_job_spectra_s3_request(job_id->value);
    error = ds3_get_job_spectra_s3_request(client, request, &bulk_response);
    ds3_str_free(job_id);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK(bulk_response->status == DS3_JOB_STATUS_CANCELED);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_jobs){
    ds3_client* client = get_client();
    ds3_error* error = NULL;
    ds3_request* request = NULL;

    ds3_list_bucket_result_response* get_bucket_response = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_job_list_response* get_jobs_response = NULL;

    const char* bucket1_name = "bucket_test_get_job1";
    ds3_bulk_object_list_response* bucket1_object_list = NULL;
    ds3_str* bucket1_job_id = NULL;
    ds3_bool found_bucket1_job = False;
    const char* bucket2_name = "bucket_test_get_job2";
    ds3_bulk_object_list_response* bucket2_object_list = NULL;
    ds3_str* bucket2_job_id = NULL;
    ds3_bool found_bucket2_job = False;

    printf("-----Testing Get Jobs-------\n");

    /* create bucket1 with objects */
    populate_with_objects(client, bucket1_name);

    request = ds3_init_get_bucket_request(bucket1_name);
    error = ds3_get_bucket_request(client, request, &get_bucket_response);
    ds3_request_free(request);
    handle_error(error);

    // retain object_list for ds3_init_get_bulk
    bucket1_object_list = ds3_convert_object_list((const ds3_contents_response**)get_bucket_response->objects, get_bucket_response->num_objects);
    ds3_list_bucket_result_response_free(get_bucket_response);

    /* create bucket2 with objects */
    request = ds3_init_get_bucket_request(bucket2_name);
    populate_with_objects(client, bucket2_name);
    error = ds3_get_bucket_request(client, request, &get_bucket_response);
    ds3_request_free(request);
    handle_error(error);

    // retain object_list for ds3_init_get_bulk
    bucket2_object_list = ds3_convert_object_list((const ds3_contents_response**)get_bucket_response->objects, get_bucket_response->num_objects);
    ds3_list_bucket_result_response_free(get_bucket_response);

    /* init bulk get bucket1 */
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket1_name, bucket1_object_list);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    bucket1_job_id = ds3_str_dup(bulk_response->job_id);
    ds3_bulk_object_list_response_free(bucket1_object_list);
    ds3_master_object_list_response_free(bulk_response);
    ds3_request_free(request);
    handle_error(error);

    /* init bulk get bucket2 */
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket2_name, bucket2_object_list);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    bucket2_job_id = ds3_str_dup(bulk_response->job_id);
    ds3_bulk_object_list_response_free(bucket2_object_list);
    ds3_master_object_list_response_free(bulk_response);
    ds3_request_free(request);
    handle_error(error);

    /* GET jobs */
    request = ds3_init_get_jobs_spectra_s3_request();
    error = ds3_get_jobs_spectra_s3_request(client, request, &get_jobs_response);
    ds3_request_free(request);
    handle_error(error);

    /* Assert valid response contents */
    BOOST_CHECK(get_jobs_response != NULL);
    uint8_t job_index;
    for( job_index = 0; job_index < get_jobs_response->num_jobs; job_index++ ) {
        ds3_job_response* job_response = get_jobs_response->jobs[job_index];
        BOOST_CHECK(job_response->bucket_name->value != NULL);
        BOOST_CHECK(job_response->job_id->value != NULL);
        BOOST_CHECK(job_response->start_date->value != NULL);
        BOOST_CHECK(job_response->user_id->value != NULL);
        BOOST_CHECK(job_response->user_name->value != NULL);
        if(0 == strcmp(job_response->job_id->value, bucket1_job_id->value)) {
            found_bucket1_job = True;
        } else if(0 == strcmp(job_response->job_id->value, bucket2_job_id->value)) {
            found_bucket2_job = True;
        }
    }

    /* teardown */
    clear_bucket(client, bucket1_name);
    clear_bucket(client, bucket2_name);
    free_client(client);
    ds3_str_free(bucket1_job_id);
    ds3_str_free(bucket2_job_id);
    ds3_job_list_response_free(get_jobs_response);
    BOOST_CHECK(True == found_bucket1_job);
    BOOST_CHECK(True == found_bucket2_job);
}

BOOST_AUTO_TEST_CASE( GetJobToReplicateRequestHandler_response_type_not_parsed ) {
    printf("-----Testing GetJobToReplicateRequestHandler response type parsing-------\n");

    const char* bucket_name = "bucket_test_response_type_parsing";
    ds3_client* client = get_client();
    ds3_request* request;
    ds3_error* error;

    ds3_list_bucket_result_response* response = NULL;

    ds3_master_object_list_response* bulk_response = NULL;
    ds3_bulk_object_list_response* object_list = NULL;

    ds3_str* response_str = NULL;
    ds3_str* job_id = NULL;

    populate_with_objects(client, bucket_name);

    // Get bucket contents
    request = ds3_init_get_bucket_request(bucket_name);
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    object_list = ds3_convert_object_list((const ds3_contents_response**)response->objects, response->num_objects);
    ds3_list_bucket_result_response_free(response);

    // Create a bulk_get job
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, object_list);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    job_id = ds3_str_dup(bulk_response->job_id);
    ds3_request_free(request);
    ds3_error_free(error);
    ds3_bulk_object_list_response_free(object_list);

    // Replicate the job
    request = ds3_init_get_job_to_replicate_spectra_s3_request(job_id->value);
    error = ds3_get_job_to_replicate_spectra_s3_request(client, request, &response_str);

    BOOST_CHECK(error == NULL);
    BOOST_CHECK(response_str->size > 0);
    ds3_str_free(response_str);
    ds3_str_free(job_id);
    ds3_error_free(error);
    ds3_request_free(request);

    ds3_master_object_list_response_free(bulk_response);
    clear_bucket(client, bucket_name);
    free_client(client);
}
