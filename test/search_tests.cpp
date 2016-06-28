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
    ds3_request_set_bucket_id(request, bucket_name);
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
    ds3_request_set_bucket_id(request, bucket_name);
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
    ds3_request_set_bucket_id(request, bucket_name);
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
    ds3_request_set_bucket_id(request, bucket_name);
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

BOOST_AUTO_TEST_CASE(get_objects_with_plus_query_param) {
    printf("-----Testing Search Object with +-------\n");

    ds3_client* client = get_client();
    const char* bucket_name = "get_objects_with_plus_query_param";
    const char* plus_object = "Plus+Object";

    ds3_error* bucket_error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(bucket_error);

    ds3_request* put_request = ds3_init_put_object_request(bucket_name, plus_object, 1024);
    FILE* obj_file = fopen("resources/beowulf.txt", "r");

    ds3_error* error = ds3_put_object_request(client, put_request, obj_file, ds3_read_from_file);
    ds3_request_free(put_request);
    fclose(obj_file);
    handle_error(error);

    ds3_request* get_obj_request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_id(get_obj_request, bucket_name);
    ds3_request_set_name(get_obj_request, plus_object);
    ds3_s3_object_list_response* objects_response = NULL;
    error = ds3_get_objects_spectra_s3_request(client, get_obj_request, &objects_response);
    handle_error(error);

    BOOST_CHECK_EQUAL(objects_response->num_s3_objects, 1);
    ds3_s3_object_response* search_object_returned = objects_response->s3_objects[0];
    BOOST_CHECK_EQUAL(search_object_returned->name->value, plus_object);

    ds3_request_free(get_obj_request);
    ds3_s3_object_list_response_free(objects_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_objects_with_special_chars_query_param) {
    printf("-----Testing Search Object with special char-------\n");

    ds3_client* client = get_client();
    const char* bucket_name = "TestSpecialCharacterInObjectName";
    const char* special_char_object = "varsity1314/_projects/VARSITY 13-14/_versions/Varsity 13-14 (2015-10-05 1827)/_project/Trash/PCMAC HD.avb";

    ds3_error* bucket_error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    handle_error(bucket_error);

    ds3_request* put_request = ds3_init_put_object_request(bucket_name, special_char_object, 1024);
    FILE* obj_file = fopen("resources/beowulf.txt", "r");

    ds3_error* error = ds3_put_object_request(client, put_request, obj_file, ds3_read_from_file);
    ds3_request_free(put_request);
    fclose(obj_file);
    handle_error(error);

    ds3_request* get_obj_request = ds3_init_get_objects_spectra_s3_request();
    ds3_request_set_bucket_id(get_obj_request, bucket_name);
    ds3_request_set_name(get_obj_request, special_char_object);
    ds3_s3_object_list_response* objects_response = NULL;
    error = ds3_get_objects_spectra_s3_request(client, get_obj_request, &objects_response);
    ds3_request_free(get_obj_request);
    handle_error(error);

    BOOST_CHECK_EQUAL(objects_response->num_s3_objects, 1);
    ds3_s3_object_response* search_object_returned = objects_response->s3_objects[0];
    BOOST_CHECK_EQUAL(search_object_returned->name->value, special_char_object);

    ds3_s3_object_list_response_free(objects_response);

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
    ds3_request_set_bucket_id(request, bucket_name);
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
    ds3_request_set_bucket_id(request, "fake_bucket");
    ds3_request_set_name(request, "%resources%");
    ds3_error* error = ds3_get_objects_spectra_s3_request(client, request, &response);
    BOOST_CHECK(error!=NULL);
    BOOST_CHECK(error->error->http_error_code == 404);

    ds3_request_free(request);
    ds3_error_free(error);

    clear_bucket(client, bucket_name);
    free_client(client);
}
