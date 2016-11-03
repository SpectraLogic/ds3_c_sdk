#include <glib.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

//Testing a Duplicate Bucket Put
BOOST_AUTO_TEST_CASE(put_duplicate_bucket) {
    printf("-----Negative Testing Duplicate Bucket Creation-------\n");
    ds3_client* client = get_client();
    size_t bucket_index;
    bool found = false;
    const char* bucket_name = "negative_test_duplicate_bucket_name_bucket";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_get_service_request();
    ds3_list_all_my_buckets_result_response* service_response = NULL;
    error = ds3_get_service_request(client, request, &service_response);
    ds3_request_free(request);
    handle_error(error);

    for (bucket_index = 0; bucket_index < service_response->num_buckets; bucket_index++) {
        ds3_bucket_details_response* currentBucket = service_response->buckets[bucket_index];
        if (strcmp(bucket_name, currentBucket->name->value) == 0) {
            found = true;
            break;
        }
    }

    ds3_list_all_my_buckets_result_response_free(service_response);

    BOOST_CHECK(found);

    //Putting Duplicate Bucket
    error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    BOOST_CHECK(error!=NULL);
    BOOST_CHECK(error->error->http_error_code == 409);
    BOOST_CHECK(strcmp(error->error->code->value ,"Conflict")==0);
    ds3_error_free(error);

    //Deleting Created Bucket
    request = ds3_init_delete_bucket_request(bucket_name);
    error = ds3_delete_bucket_request(client, request);
    ds3_request_free(request);

    free_client(client);
    handle_error(error);
}


//testing Deletion of non existing bucket
BOOST_AUTO_TEST_CASE(delete_non_existing_bucket){
    printf("-----Negative Testing Non Existing Bucket Deletion-------\n");
    ds3_client* client = get_client();
    ds3_request* request;
    ds3_error* error;
    const char* bucket_name = "delete_non_existing_bucket";
    request = ds3_init_delete_bucket_request(bucket_name);
    error = ds3_delete_bucket_request(client, request);
    ds3_request_free(request);
    BOOST_CHECK(error != NULL);
    BOOST_CHECK(error->error->http_error_code == 404);
    BOOST_CHECK(strcmp(error->error->code->value ,"Not Found")==0);
    ds3_error_free(error);
    free_client(client);
}

//testing get_bucket with empty parameter for bucket_name
BOOST_AUTO_TEST_CASE(get_bucket_with_empty_bucket_name){
    printf("-----Negative Testing get_bucket with empty bucket_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "";

    ds3_request* request = ds3_init_get_bucket_request(bucket_name);
    ds3_error* error;
    ds3_list_bucket_result_response* response = NULL;
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    ds3_list_bucket_result_response_free(response);
    free_client(client);

    //printf("error->error->code[%s]\n", error->error->code->value);
    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(g_str_has_prefix(error->message->value, "The bucket name parameter is required") == (gboolean)TRUE);
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    ds3_error_free(error);
}

//testing get_bucket with null parameter for bucket_name
BOOST_AUTO_TEST_CASE(get_bucket_with_null_bucket_name){
    printf("-----Negative Testing get_bucket with null bucket_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = NULL;
    ds3_request* request = ds3_init_get_bucket_request(bucket_name);
    ds3_error* error = NULL;
    ds3_list_bucket_result_response* response = NULL;
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    ds3_list_bucket_result_response_free(response);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(g_str_has_prefix(error->message->value, "The bucket name parameter is required") == (gboolean)TRUE);
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    ds3_error_free(error);
}

//testing head_object with empty parameter for object_name
BOOST_AUTO_TEST_CASE(head_object_with_empty_object_name){
    printf("-----Negative Testing head_object with empty object_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "test_head_object_with_empty_object_name";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_head_object_request(bucket_name, "");
    ds3_metadata* response = NULL;
    error = ds3_head_object_request(client, request, &response);
    ds3_request_free(request);
    clear_bucket(client, bucket_name);
    ds3_metadata_free(response);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    BOOST_CHECK(g_str_has_prefix(error->message->value, "The object name parameter is required") == (gboolean)TRUE);
    ds3_error_free(error);
}

//testing head_object with null parameter for object_name
BOOST_AUTO_TEST_CASE(head_object_with_null_object_name){
    printf("-----Negative Testing head_object with null object_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "test_head_object_with_null_object_name";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_head_object_request(bucket_name, NULL);
    ds3_metadata* response = NULL;
    error = ds3_head_object_request(client, request, &response);
    ds3_request_free(request);
    clear_bucket(client, bucket_name);
    ds3_metadata_free(response);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    BOOST_CHECK(g_str_has_prefix(error->message->value, "The object name parameter is required") == (gboolean)TRUE);
    ds3_error_free(error);
}

//testing Head bucket of non existing bucket
BOOST_AUTO_TEST_CASE(head_bucket_non_existing_bucket){
    printf("-----Negative Testing Non Existing Head Bucket-------\n");
    ds3_error* error;
    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";
    ds3_request* request = NULL;
    request = ds3_init_head_bucket_request(bucket_name);
    error = ds3_head_bucket_request(client, request);
    ds3_request_free(request);
    BOOST_CHECK(error != NULL);
    BOOST_CHECK(error->error->http_error_code == 404);
    BOOST_CHECK(strcmp(error->error->code->value ,"Not Found")==0);
    ds3_error_free(error);
    free_client(client);
}

//testing Deletion of non existing object
BOOST_AUTO_TEST_CASE(delete_non_existing_object) {
    printf("-----Negative Testing Non Existing Object Deletion-------\n");
    //First Creating a Bucket
    ds3_client* client = get_client();
    uint64_t bucket_index;
    bool found = false;
    const char* bucket_name = "test_bucket";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_get_service_request();
    ds3_list_all_my_buckets_result_response* service_response = NULL;
    error = ds3_get_service_request(client, request, &service_response);
    ds3_request_free(request);
    handle_error(error);

    for (bucket_index = 0; bucket_index < service_response->num_buckets; bucket_index++) {
        if (strcmp(bucket_name, service_response->buckets[bucket_index]->name->value) == 0) {
            found = true;
            break;
        }
    }

    ds3_list_all_my_buckets_result_response_free(service_response);
    BOOST_CHECK(found);

    //Deleting Non Existing Object
    request = ds3_init_delete_object_request(bucket_name,"delete_non_existing_object");
    error = ds3_delete_object_request(client, request);
    ds3_request_free(request);

    //Deleting Created Bucket
    request = ds3_init_delete_bucket_request(bucket_name);
    handle_error(ds3_delete_bucket_request(client, request));
    ds3_request_free(request);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
	  BOOST_CHECK(error->error->http_error_code == 404);
    BOOST_CHECK(strcmp(error->error->code->value ,"Not Found") == 0);
    ds3_error_free(error);
}

//testing Bad Bucket Name Creation
BOOST_AUTO_TEST_CASE(bad_bucket_name) {
    printf("-----Negative Testing Bad Bucket Name creation-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "bad:bucket";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->http_error_code == 400);
    BOOST_CHECK(strcmp(error->error->code->value ,"Bad Request") == 0);
    ds3_error_free(error);

    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_bucket_bucket_name_with_trailing_slash){
    printf("-----Negative Testing get_bucket with bucket_name/ with trailing slash-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "trailing_slash/";

    ds3_error* put_bucket_error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    BOOST_CHECK(put_bucket_error != NULL);
    BOOST_CHECK(put_bucket_error->code == DS3_ERROR_BAD_STATUS_CODE);
    BOOST_CHECK(put_bucket_error->error->http_error_code == 400);
    BOOST_CHECK_EQUAL( g_strcmp0(put_bucket_error->error->code->value, "Bad Request"), 0);

    ds3_error_free(put_bucket_error);
    clear_bucket(client, bucket_name);
    free_client(client);
}

//testing creation of object list with duplicate objects
BOOST_AUTO_TEST_CASE(put_duplicate_object_list){
    printf("-----Negative Testing Object List With Duplicate Objects Creation-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "test_bucket_duplicate_object";

    //Adding Duplicate Object to the Bucket List
    const char* books[] = {"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/beowulf.txt"};
    ds3_bulk_object_list_response* obj_list = NULL;
    ds3_master_object_list_response* response = NULL;

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    obj_list = ds3_convert_file_list(books, 3);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(obj_list);
    clear_bucket(client, bucket_name);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->http_error_code == 400);
    BOOST_CHECK(strcmp(error->error->code->value ,"Bad Request") == 0);
    ds3_error_free(error);
}

//testing Bulk Put with empty object list
BOOST_AUTO_TEST_CASE(put_empty_object_list) {
    printf("-----Negative Testing Put Empty Object List-------\n");
    ds3_client* client = get_client();
    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();
    ds3_master_object_list_response* response = NULL;
    const char* bucket_name = "Bucket_with_empty_list";

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(error);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(obj_list);

    //Deleting Created Bucket
    clear_bucket(client, bucket_name);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(strcmp(error->message->value ,"The bulk command requires a list of objects to process") == 0);
    ds3_error_free(error);
}

BOOST_AUTO_TEST_CASE(delete_multiple_job) {
    printf("-----Negative Testing Multiple Delete Jobs-------\n");
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_client* client = get_client();
    const char* bucket_name = "bucket_test_get_job";

    ds3_str* job_id = populate_with_objects_return_job(client, bucket_name);

    request = ds3_init_cancel_job_spectra_s3_request(job_id->value);
    ds3_request_set_force(request, True);
    error = ds3_cancel_job_spectra_s3_request(client,request);
    ds3_str_free(job_id);
    handle_error(error);

    error = ds3_cancel_job_spectra_s3_request(client,request);
    ds3_request_free(request);
    clear_bucket(client, bucket_name);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->http_error_code == 404);
    BOOST_CHECK(strcmp(error->error->code->value ,"Not Found") == 0);
    ds3_error_free(error);
}

BOOST_AUTO_TEST_CASE(get_non_existing_job) {
    printf("-----Negative Testing Non Existing Get Job-------\n");
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_client* client = get_client();

    request = ds3_init_get_job_spectra_s3_request("b44d7ddc-608a-4d46-9e9e-9433b0b62911");
    error = ds3_get_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_master_object_list_response_free(bulk_response);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->http_error_code == 404);
    BOOST_CHECK(strcmp(error->error->code->value, "Not Found") == 0);
    ds3_error_free(error);
}

