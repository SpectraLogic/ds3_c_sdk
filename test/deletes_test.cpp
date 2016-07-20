#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( delete_objects ) {
    ds3_client* client = get_client();
    const char* bucket_name = "delete_objects_test";
    populate_with_objects(client, bucket_name);

    ds3_delete_objects_response* obj_list = g_new0(ds3_delete_objects_response, 1);
    GPtrArray* ds3_str_list = g_ptr_array_new();
    g_ptr_array_add(ds3_str_list, ds3_str_init("resources/beowulf.txt"));
    g_ptr_array_add(ds3_str_list, ds3_str_init("resources/sherlock_holmes.txt"));
    g_ptr_array_add(ds3_str_list, ds3_str_init("resources/tale_of_two_cities.txt"));
    g_ptr_array_add(ds3_str_list, ds3_str_init("resources/ulysses.txt"));
    g_ptr_array_add(ds3_str_list, ds3_str_init("resources/ulysses_large.txt"));
    obj_list->strings_list = (ds3_str**)ds3_str_list->pdata;
    obj_list->num_strings = ds3_str_list->len;
    g_ptr_array_free(ds3_str_list, FALSE);

    ds3_request* request = ds3_init_delete_objects_request(bucket_name, obj_list);
    ds3_delete_result_response* response;
    ds3_error* error = ds3_delete_objects_request(client, request, &response);
    ds3_delete_objects_response_free(obj_list);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK(response->num_deleted_objects == 5);
    ds3_delete_result_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delete_non_existant_object ) {
    ds3_client* client = get_client();
    const char* bucket_name = "negative_delete_object_test";
    populate_with_objects(client, bucket_name);

    ds3_delete_objects_response* obj_list = g_new0(ds3_delete_objects_response, 1);
    ds3_str* fake_book = ds3_str_init("resources/frankenstein.txt");
    GPtrArray* ds3_str_list = g_ptr_array_new();
    g_ptr_array_add(ds3_str_list, fake_book);
    obj_list->strings_list = (ds3_str**)ds3_str_list->pdata;
    obj_list->num_strings = ds3_str_list->len;
    g_ptr_array_free(ds3_str_list, FALSE);

    ds3_request* request = ds3_init_delete_objects_request(bucket_name, obj_list);
    ds3_delete_result_response* response;
    ds3_error* error = ds3_delete_objects_request(client, request, &response);
    ds3_delete_objects_response_free(obj_list);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK(response->num_deleted_objects == 0);
    ds3_delete_result_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delete_folder ) {
    ds3_client* client = get_client();
    const char* bucket_name = "delete_folder_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_delete_folder_recursively_spectra_s3_request("resources", bucket_name);
    ds3_error* error = ds3_delete_folder_recursively_spectra_s3_request(client, request);
    ds3_request_free(request);
    handle_error(error);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delete_non_existant_folder ) {
    ds3_client* client = get_client();
    const char* bucket_name = "negative_delete_folder_test";
    const char* fake_folder = "fake_folder";
    populate_with_objects(client, bucket_name);
    ds3_request* request = ds3_init_delete_folder_recursively_spectra_s3_request(fake_folder, bucket_name);

    ds3_error* error = ds3_delete_folder_recursively_spectra_s3_request(client, request);
    ds3_request_free(request);
    BOOST_CHECK(error != NULL);
    BOOST_CHECK(error->error != NULL);
    BOOST_CHECK(error->error->http_error_code == 404);
    ds3_error_free(error);

    clear_bucket(client, bucket_name);
    free_client(client);
}
