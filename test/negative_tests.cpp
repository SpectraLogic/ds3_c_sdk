/*
 * The Following Program Consists of a bunch of Negative Tests
 * 1. Dupilcate Bucket Put Test
 * 2. Deleting Non Existent Bucket
 * 3. Deleting Non Exiting  Object
 * 4. Creation of Bucket with Invalid Name
 * 5. Duplicate Object List Creation
 * 6. Duplicate Object Creation
 * 7. Bulk Put With Empty Object List
 * 8. Duplicate Delete Job
 * 9. Get Non Existing Get Job
 * 10. Put Bad Checksum
*/

#include <glib.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

//Testing a Duplicate Bucket Put
BOOST_AUTO_TEST_CASE(put_duplicate_bucket) {
    printf("-----Negative Testing Duplicate Bucket Creation-------\n");
    ds3_client* client = get_client();
    uint64_t i;
    bool found = false;
    const char* bucket_name = "duplicatename_test_bucket";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_get_service_response* response;

    ds3_error* error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    handle_error(error);

    request = ds3_init_get_service();
    error = ds3_get_service(client, request, &response);
    ds3_free_request(request);
    handle_error(error);

    for (i = 0; i < response->num_buckets; i++) {
        if (strcmp(bucket_name, response->buckets[i].name->value) == 0) {
            found = true;
            break;
        }
    }

    ds3_free_service_response(response);

    BOOST_CHECK(found);

    //Putting Duplicate Bucket
    request = ds3_init_put_bucket(bucket_name);
    error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    BOOST_CHECK(error!=NULL);
    BOOST_CHECK(error->error->status_code == 409);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Conflict")==0);
    ds3_free_error(error);

    //Deleting Created Bucket
    request = ds3_init_delete_bucket(bucket_name);
    error = ds3_delete_bucket(client, request);
    ds3_free_request(request);

    free_client(client);
    handle_error(error);
}

//testing Deletion of non existing bucket
BOOST_AUTO_TEST_CASE(delete_non_existing_bucket){
    printf("-----Negative Testing Non Existing Bucket Deletion-------\n");
    ds3_client* client = get_client();
    ds3_request* request ;
    ds3_error* error ;
    const char* bucket_name = "delete_non_existing_bucket";
    request = ds3_init_delete_bucket(bucket_name);
    error = ds3_delete_bucket(client, request);
    ds3_free_request(request);
    BOOST_CHECK(error != NULL);
    BOOST_CHECK(error->error->status_code == 404);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Not Found")==0);
    ds3_free_error(error);
    free_client(client);
}

//testing get_bucket with empty parameter for bucket_name
BOOST_AUTO_TEST_CASE(get_bucket_with_empty_bucket_name){
    printf("-----Negative Testing get_bucket with empty bucket_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "";
    ds3_request* request = ds3_init_get_bucket(bucket_name);
    ds3_error* error;
    ds3_get_bucket_response* response;
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(g_str_has_prefix(error->message->value, "The bucket name parameter is required") == (boolean)TRUE);
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    ds3_free_error(error);
}

//testing get_bucket with null parameter for bucket_name
BOOST_AUTO_TEST_CASE(get_bucket_with_null_bucket_name){
    printf("-----Negative Testing get_bucket with null bucket_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = NULL;
    ds3_request* request = ds3_init_get_bucket(bucket_name);
    ds3_error* error;
    ds3_get_bucket_response* response;
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK((boolean)TRUE == g_str_has_prefix(error->message->value, "The bucket name parameter is required"));
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    ds3_free_error(error);
}

//testing head_object with empty parameter for object_name
BOOST_AUTO_TEST_CASE(head_object_with_empty_object_name){
    printf("-----Negative Testing head_object with empty object_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "test_head_object_with_empty_object_name";

    ds3_request* put_request = ds3_init_put_bucket(bucket_name);
    ds3_error* error = ds3_put_bucket(client, put_request);
    ds3_free_request(put_request);
    handle_error(error);

    ds3_request* request = ds3_init_head_object(bucket_name,"");
    ds3_metadata* response = NULL;
    error = ds3_head_object(client, request, &response);
    ds3_free_request(request);
    clear_bucket(client, bucket_name);
    ds3_free_metadata(response);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    BOOST_CHECK(g_str_has_prefix(error->message->value, "The object name parameter is required") == (boolean)TRUE);
    ds3_free_error(error);
}

//testing head_object with null parameter for object_name
BOOST_AUTO_TEST_CASE(head_object_with_null_object_name){
    printf("-----Negative Testing head_object with null object_name parameter-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "test_head_object_with_null_object_name";

    ds3_request* put_request = ds3_init_put_bucket(bucket_name);
    ds3_error* error = ds3_put_bucket(client, put_request);
    ds3_free_request(put_request);
    handle_error(error);

    ds3_request* request = ds3_init_head_object(bucket_name, NULL);
    ds3_metadata* response = NULL;
    error = ds3_head_object(client, request, &response);
    ds3_free_request(request);
    clear_bucket(client, bucket_name);
    ds3_free_metadata(response);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->code == DS3_ERROR_MISSING_ARGS);
    BOOST_CHECK(g_str_has_prefix(error->message->value, "The object name parameter is required") == (boolean)TRUE);
    ds3_free_error(error);
}

//testing Head bucket of non existing bucket
BOOST_AUTO_TEST_CASE(head_bucket_non_existing_bucket){
    printf("-----Testing Non Existing Head Bucket-------\n");
    ds3_error* error;
    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";
    ds3_request* request;
    request = ds3_init_head_bucket(bucket_name);
    error = ds3_head_bucket(client, request);
    ds3_free_request(request);
    BOOST_CHECK(error != NULL);
    BOOST_CHECK(error->error->status_code == 404);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Not Found")==0);
    ds3_free_error(error);
    free_client(client);
}


//testing Deletion of non existing object
BOOST_AUTO_TEST_CASE(delete_non_existing_object) {
    printf("-----Negative Testing Non Existing Object Deletion-------\n");
    //First Creating a Bucket
    ds3_client* client = get_client();
    uint64_t i;
    bool found = false;
    const char* bucket_name = "test_bucket";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_get_service_response* response;

    ds3_error* error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    handle_error(error);

    request = ds3_init_get_service();
    error = ds3_get_service(client, request, &response);
    ds3_free_request(request);
    handle_error(error);

    for (i = 0; i < response->num_buckets; i++) {
        if (strcmp(bucket_name, response->buckets[i].name->value) == 0) {
            found = true;
            break;
        }
    }

    ds3_free_service_response(response);
    BOOST_CHECK(found);

    //Deleting Non Existing Object
    request = ds3_init_delete_object(bucket_name,"delete_non_existing_object");
    error = ds3_delete_object(client, request);
    ds3_free_request(request);

    //Deleting Created Bucket
    request = ds3_init_delete_bucket(bucket_name);
    handle_error(ds3_delete_bucket(client, request));
    ds3_free_request(request);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
	  BOOST_CHECK(error->error->status_code == 404);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Not Found") == 0);
    ds3_free_error(error);
}

//testing Bad Bucket Name Creation
BOOST_AUTO_TEST_CASE(bad_bucket_name) {
    printf("-----Negative Testing Bad Bucket Name creation-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "bad:bucket";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_error* error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->status_code == 400);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Bad Request") == 0);
    ds3_free_error(error);
}

//testing creation of object list with duplicate objects
BOOST_AUTO_TEST_CASE(put_duplicate_object_list){
    printf("-----Negative Testing Object List With Duplicate Objects Creation-------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "test_bucket_duplicate_object";

    //Adding Duplicate Object to the Bucket List
    const char* books[] ={"resources/beowulf.txt","resources/sherlock_holmes.txt","resources/beowulf.txt"};
    ds3_bulk_object_list* obj_list;
    ds3_bulk_response* response;
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_error* error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    handle_error(error);
    obj_list = ds3_convert_file_list(books, 3);
    request = ds3_init_put_bulk(bucket_name, obj_list);
    error = ds3_bulk(client, request, &response);
    ds3_free_request(request);
    ds3_free_bulk_object_list(obj_list);
    clear_bucket(client, bucket_name);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->status_code == 409);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Conflict") == 0);
    ds3_free_error(error);
}


//testing creation of duplicate object
BOOST_AUTO_TEST_CASE(put_duplicate_object){
    printf("-----Negative Testing Duplicate Object Creation -------\n");
    ds3_client* client = get_client();
    const char* bucket_name = "test_bucket_new";

    //Pre populating few objects
    populate_with_objects(client, bucket_name);

    //Testing creation of preexisting objects
    const char* books[] ={"resources/beowulf.txt","resources/sherlock_holmes.txt"};
    ds3_bulk_object_list* obj_list;
    ds3_bulk_response* response;
    ds3_request* request;
    ds3_error* error;

    obj_list = ds3_convert_file_list(books,2);
    request = ds3_init_put_bulk(bucket_name, obj_list);
    error = ds3_bulk(client, request, &response);
    ds3_free_request(request);
    ds3_free_bulk_object_list(obj_list);
    clear_bucket(client, bucket_name);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->status_code == 409);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Conflict") == 0);
    ds3_free_error(error);
}


//testing Bulk Put with empty object list
BOOST_AUTO_TEST_CASE(put_empty_object_list) {
    printf("-----Negative Testing Put Empty Object List-------\n");
    ds3_client* client = get_client();
    ds3_bulk_object_list* obj_list = NULL;
    obj_list = ds3_init_bulk_object_list(0);
    ds3_bulk_response* response;
    const char* bucket_name = "Bucket_with_empty_list";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_error* error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    handle_error(error);

    request = ds3_init_put_bulk(bucket_name, obj_list);
    error = ds3_bulk(client, request, &response);
    ds3_free_request(request);
    ds3_free_bulk_object_list(obj_list);

    //Deleting Created Bucket
    clear_bucket(client, bucket_name);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(strcmp(error->message->value ,"The bulk command requires a list of objects to process") == 0);
    ds3_free_error(error);
}

BOOST_AUTO_TEST_CASE(delete_multiple_job) {
    printf("-----Negative Testing Multiple Delete Jobs-------\n");
    ds3_request* request;
    ds3_error* error;
    ds3_client* client = get_client();
    const char* bucket_name = "bucket_test_get_job";

    ds3_str* job_id = populate_with_objects_return_job(client, bucket_name);
    request = ds3_init_delete_job(job_id->value);
    error = ds3_delete_job(client,request);
    ds3_str_free(job_id);
    handle_error(error);

    error = ds3_delete_job(client,request);
    ds3_free_request(request);
    clear_bucket(client, bucket_name);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->status_code == 404);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Not Found") == 0);
    ds3_free_error(error);
}

BOOST_AUTO_TEST_CASE(get_non_existing_job) {
    printf("-----Negative Testing Non Existing Get Job-------\n");
    ds3_request* request;
    ds3_error* error;
    ds3_bulk_response* bulk_response = NULL;
    ds3_client* client = get_client();
    request = ds3_init_get_job("b44d7ddc-608a-4d46-9e9e-9433b0b62911");
    error = ds3_get_job(client,request,&bulk_response);
    ds3_free_request(request);
    ds3_free_bulk_response(bulk_response);
    free_client(client);

    BOOST_REQUIRE(error != NULL);
    BOOST_CHECK(error->error->status_code == 404);
    BOOST_CHECK(strcmp(error->error->status_message->value ,"Not Found") == 0);
    ds3_free_error(error);
}

BOOST_AUTO_TEST_CASE(bad_checksum) {
    uint64_t i, n, checksums;
    const char* bucket_name = "bucket_test_md5";
    ds3_request* request;
    const char* books[] ={"resources/beowulf.txt"};
    ds3_client* client = get_client();
    ds3_error* error;
    ds3_bulk_object_list* obj_list;
    ds3_bulk_response* response;
    ds3_allocate_chunk_response* chunk_response;

    printf("-----Testing Request With Bad Checksum-------\n");

    obj_list = ds3_convert_file_list(books, 1);

    for (checksums = 0; checksums < 5; checksums ++) {
        request = ds3_init_put_bucket(bucket_name);
        error = ds3_put_bucket(client, request);
        ds3_free_request(request);
        handle_error(error);


        request = ds3_init_put_bulk(bucket_name, obj_list);
        error = ds3_bulk(client, request, &response);

        ds3_free_request(request);
        handle_error(error);

        for (n = 0; n < response->list_size; n ++) {
            request = ds3_init_allocate_chunk(response->list[n]->chunk_id->value);
            error = ds3_allocate_chunk(client, request, &chunk_response);
            ds3_free_request(request);
            handle_error(error);
            BOOST_REQUIRE(chunk_response->retry_after == 0);
            BOOST_REQUIRE(chunk_response->objects != NULL);

            for (i = 0; i < chunk_response->objects->size; i++) {
                ds3_bulk_object bulk_object = chunk_response->objects->list[i];
                FILE* file = fopen(bulk_object.name->value, "r");

                request = ds3_init_put_object_for_job(bucket_name, bulk_object.name->value, bulk_object.offset,  bulk_object.length, response->job_id->value);

                switch (checksums) {
                    case 0:
                        ds3_request_set_md5(request,"ra3fg==");
                        break;
                    case 1:
                        ds3_request_set_sha256(request,"SbGH1ZtYIqOjO+E=");
                        break;
                    case 2:
                        ds3_request_set_sha512(request,"qNLwiDVNQ3YCBjY6YowRE8Hsqw+iwP9KOKM0Xvw==");
                        break;
                    case 3:
                        ds3_request_set_crc32(request,"b==");
                        break;
                    case 4:
                        ds3_request_set_crc32c(request, "+Z==");
                        break;
                }

                if (bulk_object.offset > 0) {
		    fseek(file, (long)bulk_object.offset, SEEK_SET);
                }
                error = ds3_put_object(client, request, file, ds3_read_from_file);
                ds3_free_request(request);
                fclose(file);
                BOOST_REQUIRE(error != NULL);
                BOOST_CHECK(error->error->status_code == 400);
                BOOST_CHECK(strcmp(error->error->status_message->value, "Bad Request")==0);
                ds3_free_error(error);
            }
            ds3_free_allocate_chunk_response(chunk_response);
        }

        ds3_free_bulk_response(response);
        clear_bucket(client, bucket_name);
    }

    ds3_free_bulk_object_list(obj_list);
    free_client(client);
}
