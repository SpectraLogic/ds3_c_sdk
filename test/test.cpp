// The BOOST_TEST_MODULE should be set only in this file
// so that a main function is only generated once
#define BOOST_TEST_MODULE DS3_Tests
#include <stdio.h>
#include <stdlib.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

ds3_client * get_client() {

    ds3_client* client;

    ds3_error* error = ds3_create_client_from_env(&client);

    if (error != NULL) {
        fprintf(stderr, "Failed to construct ds3_client from enviornment variables: %s\n", error->message->value);
        exit(1);
    }

    return client;
}

void clear_bucket(const ds3_client* client, const char* bucket_name) {
    uint64_t i;
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* bucket_response;

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &bucket_response);
    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);

    for (i = 0; i < bucket_response->num_objects; i++) {
        request = ds3_init_delete_object(bucket_name, bucket_response->objects[i].name->value);
        error = ds3_delete_object(client, request);
        ds3_free_request(request);

        if (error != NULL) {
            fprintf(stderr, "Failed to delete object %s\n", bucket_response->objects[i].name->value);
            ds3_free_error(error);
        }
    }

    request = ds3_init_delete_bucket(bucket_name);
    error = ds3_delete_bucket(client, request);
    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);
}

void populate_with_objects(const ds3_client* client, const char* bucket_name) {
    uint64_t i;
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    const char* books[4] ={"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt"};
    ds3_error* error = ds3_put_bucket(client, request);
    ds3_bulk_object_list* obj_list;
    ds3_bulk_response* response;
    ds3_allocate_chunk_response* chunk_response;

    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);

    obj_list = ds3_convert_file_list(books, 4);
    request = ds3_init_put_bulk(bucket_name, obj_list);
    error = ds3_bulk(client, request, &response);

    BOOST_REQUIRE(error == NULL);

    ds3_free_request(request);

    request = ds3_init_allocate_chunk(response->job_id->value);

    error = ds3_allocate_chunk(client, request, &chunk_response);

    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);

    BOOST_REQUIRE(chunk_response->retry_after == 0);
    BOOST_REQUIRE(chunk_response->objects != NULL);

    for (i = 0; i < chunk_response->objects->size; i++) {
        ds3_bulk_object bulk_object = chunk_response->objects->list[i];
        FILE* file = fopen(bulk_object.name->value, "r");

        request = ds3_init_put_object(bucket_name, bulk_object.name->value, bulk_object.length);
        error = ds3_put_object(client, request, file, ds3_read_from_file);
        ds3_free_request(request);

        BOOST_CHECK(error == NULL);
    }

    ds3_free_allocate_chunk_response(chunk_response);
    ds3_free_bulk_response(response);
    ds3_free_bulk_object_list(obj_list);
}

bool contains_object(const ds3_object* objects, uint64_t num_objects, const char* obj) {
    uint64_t i;
    for (i = 0; i < num_objects; i++) {
        fprintf(stderr, "obj name: %s\n", objects[i].name->value);
        if(strcmp(objects[i].name->value, obj) == 0) {
            return true;
        }
    }
    return false;
}
