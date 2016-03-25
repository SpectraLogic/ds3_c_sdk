// The BOOST_TEST_MODULE should be set only in this file
// so that a main function is only generated once
#define BOOST_TEST_MODULE DS3_Tests
#include <stdio.h>
#include <stdlib.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

struct TestCleanup {
    TestCleanup() {}
    ~TestCleanup() {
        ds3_cleanup();
    }
};

BOOST_GLOBAL_FIXTURE( TestCleanup );

void test_log(const char* message, void* user_data) {
    fprintf(stderr, "Log Message: %s\n", message);
}

ds3_client* get_client_at_loglvl(ds3_log_lvl log_lvl) {
    ds3_client* client;

    ds3_error* error = ds3_create_client_from_env(&client);

    if (error != NULL) {
        fprintf(stderr, "Failed to construct ds3_client from enviornment variables: %s\n", error->message->value);
        ds3_free_error(error);
        BOOST_FAIL("Failed to setup client.");
    }

    ds3_client_register_logging(client, log_lvl, test_log, NULL);

    return client;
}

ds3_client* get_client() {
    return get_client_at_loglvl(DS3_INFO);
}

void print_error(const ds3_error* error) {
      if (NULL != error->message) {
        printf("ds3_error_message: %s\n", error->message->value);
      }
      if (error->error != NULL) {
          printf("ds3_status_code: %lu\n", error->error->status_code);
          printf("ds3_status_message: %s\n", error->error->status_message->value);
          printf("ds3_error_body: %s\n", error->error->error_body->value);
      }
}

void handle_error(ds3_error* error) {
    if (error != NULL) {
        print_error(error);
        ds3_free_error(error);
        BOOST_FAIL("Test failed with a ds3_error");
    }
}

bool handle_error_and_return_is_null(ds3_error* error){
    bool result = (error == NULL);
    handle_error(error);
    return result;
}

void clear_bucket(const ds3_client* client, const char* bucket_name) {
    uint64_t i;
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* bucket_response;

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &bucket_response);
    ds3_free_request(request);

    handle_error(error);

    for (i = 0; i < bucket_response->num_objects; i++) {
        if (bucket_response->objects[i].name->value[bucket_response->objects[i].name->size-1] == '/') {
            request = ds3_init_delete_folder(bucket_name, bucket_response->objects[i].name->value);
            error = ds3_delete_folder(client, request);
            ds3_free_request(request);
        } else {
            request = ds3_init_delete_object(bucket_name, bucket_response->objects[i].name->value);
            error = ds3_delete_object(client, request);
            ds3_free_request(request);
        }

        if (error != NULL) {
            fprintf(stderr, "Failed to delete object %s\n", bucket_response->objects[i].name->value);
            ds3_free_error(error);
        }
    }

    request = ds3_init_delete_bucket(bucket_name);
    error = ds3_delete_bucket(client, request);
    ds3_free_request(request);
    ds3_free_bucket_response(bucket_response);
    handle_error(error);
}

void populate_with_objects(const ds3_client* client, const char* bucket_name) {
    ds3_str* job_id = populate_with_objects_return_job(client, bucket_name);
    ds3_str_free(job_id);
}

ds3_bulk_object_list* default_object_list() {
    const char* books[5] = {"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt", "resources/ulysses_large.txt"};
    return ds3_convert_file_list(books, 5);
}

ds3_request* populate_bulk_return_request(const ds3_client* client, const char* bucket_name, ds3_bulk_object_list* obj_list) {
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_error* error = ds3_put_bucket(client, request);
    ds3_free_request(request);

    handle_error(error);

    request = ds3_init_put_bulk(bucket_name, obj_list);
    return request;
}

ds3_bulk_response* populate_bulk_return_response(const ds3_client* client, ds3_request* request) {
    ds3_bulk_response* response;
    ds3_error* error = ds3_bulk(client, request, &response);
    ds3_free_request(request);
    handle_error(error);
    return response;
}

ds3_str* populate_with_empty_objects(const ds3_client* client, const char* bucket_name) {
    ds3_bulk_object_list* obj_list = default_object_list();
    ds3_request* request = populate_bulk_return_request(client, bucket_name, obj_list);
    ds3_bulk_response* response = populate_bulk_return_response(client, request);
    ds3_free_bulk_object_list(obj_list);
    
    ds3_str* job_id = ds3_str_dup(response->job_id);
    ds3_free_bulk_response(response);
    return job_id;
}


ds3_get_available_chunks_response* ensure_available_chunks(const ds3_client* client, ds3_str* job_id) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    bool retry_get;
    ds3_get_available_chunks_response* chunk_response;
    do {
        retry_get = false;
        request = ds3_init_get_available_chunks(job_id->value);
        error = ds3_get_available_chunks(client, request, &chunk_response);
        ds3_free_request(request);

        BOOST_REQUIRE(handle_error_and_return_is_null(error));
        BOOST_REQUIRE(chunk_response != NULL);

        if (chunk_response->object_list->list_size == 0) {
            // if this happens we need to try the request
            BOOST_TEST_MESSAGE( "Hit retry, sleeping for: " << chunk_response->retry_after);
            retry_get = true;
            sleep(chunk_response->retry_after);
            ds3_free_available_chunks_response(chunk_response);
        }
    } while(retry_get);
    return chunk_response;
}

void populate_with_objects_from_bulk(const ds3_client* client, const char* bucket_name, ds3_bulk_response* response) {
    uint64_t i, n;
    ds3_error* error;
    ds3_request* request;

    ds3_get_available_chunks_response* chunk_response = ensure_available_chunks(client, response->job_id);
    
    for (i = 0; i < chunk_response->object_list->list_size; i++) {
        ds3_bulk_object_list* chunk_object_list = chunk_response->object_list->list[i];
        for (n = 0; n < chunk_object_list->size; n++) {
            ds3_bulk_object current_obj = chunk_object_list->list[n];
            FILE* file = fopen(current_obj.name->value, "r");

            request = ds3_init_put_object_for_job(bucket_name, current_obj.name->value, current_obj.offset,  current_obj.length, response->job_id->value);
            if (current_obj.offset > 0) {
                fseek(file, current_obj.offset, SEEK_SET);
            }
            error = ds3_put_object(client, request, file, ds3_read_from_file);
            ds3_free_request(request);
            
            fclose(file);
            handle_error(error);
        }
    }
    ds3_free_available_chunks_response(chunk_response);
}

ds3_str* populate_with_objects_return_job(const ds3_client* client, const char* bucket_name) {
    ds3_bulk_object_list* obj_list = default_object_list();
    ds3_request* request = populate_bulk_return_request(client, bucket_name, obj_list);
    ds3_bulk_response* response = populate_bulk_return_response(client, request);
    ds3_free_bulk_object_list(obj_list);
    
    ds3_str* job_id = ds3_str_dup(response->job_id);

    populate_with_objects_from_bulk(client, bucket_name, response);
    
    ds3_free_bulk_response(response);
    return job_id;
}

bool contains_object(const ds3_object* objects, uint64_t num_objects, const char* obj) {
    uint64_t i;
    for (i = 0; i < num_objects; i++) {
        if (strcmp(objects[i].name->value, obj) == 0) {
            return true;
        }
    }
    return false;
}

void free_client(ds3_client* client) {
    ds3_free_client(client);
}

