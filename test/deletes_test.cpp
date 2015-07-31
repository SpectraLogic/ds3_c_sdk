#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( delete_objects ) {
    ds3_client* client = get_client();
    const char* bucket_name = "delete_objects_test";
    populate_with_objects(client, bucket_name);
    ds3_request* request = ds3_init_delete_objects(bucket_name);
    ds3_bulk_object_list* bulkList = ds3_init_bulk_object_list(5);
    ds3_bulk_object* bulkObj;

    bulkObj = &bulkList->list[0];
    bulkObj->name = ds3_str_init("resources/beowulf.txt");
    bulkObj = &bulkList->list[1];
    bulkObj->name = ds3_str_init("resources/sherlock_holmes.txt");
    bulkObj = &bulkList->list[2];
    bulkObj->name = ds3_str_init("resources/tale_of_two_cities.txt");
    bulkObj = &bulkList->list[3];
    bulkObj->name = ds3_str_init("resources/ulysses.txt");
    bulkObj = &bulkList->list[4];
    bulkObj->name = ds3_str_init("resources/ulysses_large.txt");

    ds3_error* error = ds3_delete_objects(client, request, bulkList);
    clear_bucket(client, bucket_name);

    handle_error(error);

    ds3_free_bulk_object_list(bulkList);
    ds3_free_request(request);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delete_non_existant_object ) {
    ds3_client* client = get_client();
    const char* bucket_name = "delete_objects_test";
    populate_with_objects(client, bucket_name);
    ds3_request* request = ds3_init_delete_objects(bucket_name);
    ds3_bulk_object_list* bulkList = ds3_init_bulk_object_list(1);
    ds3_bulk_object* bulkObj;

    bulkObj = &bulkList->list[0];
    bulkObj->name = ds3_str_init("resources/frankenstein.txt");

    ds3_error* error = ds3_delete_objects(client, request, bulkList);
    clear_bucket(client, bucket_name);

    handle_error(error);

    ds3_free_bulk_object_list(bulkList);
    ds3_free_request(request);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delete_folder ) {
    ds3_client* client = get_client();
    const char* bucket_name = "delete_folder_test";
    populate_with_objects(client, bucket_name);
    ds3_request* request = ds3_init_delete_folder(bucket_name, "resources");

    ds3_error* error = ds3_delete_folder(client, request);
    clear_bucket(client, bucket_name);

    handle_error(error);

    ds3_free_request(request);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delete_non_existant_folder ) {
    ds3_client* client = get_client();
    const char* bucket_name = "delete_folder_test";
    populate_with_objects(client, bucket_name);
    ds3_request* request = ds3_init_delete_folder(bucket_name, "fakeFolder");
    
    ds3_error* error = ds3_delete_folder(client, request);
    clear_bucket(client, bucket_name);

    ds3_free_request(request);
    free_client(client);

    BOOST_CHECK(error!=NULL);
    BOOST_CHECK(error->error->status_code == 404);
    ds3_free_error(error);
}
