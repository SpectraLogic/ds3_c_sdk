#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"

void print_error(const ds3_error* error) {
      printf("ds3_error_message: %s\n", error->message->value);
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
        exit(-1);
    }
}

int main(void) {
    const char* bucket_name = "put_sample";
    const char* books[5] ={"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt", "resources/ulysses_large.txt"};

    // Get a client instance which uses the environment variables to get the endpoint and credentials
    ds3_client* client;
    ds3_error* error = ds3_create_client_from_env(&client);
    ds3_request* request;
    ds3_bulk_object_list* obj_list;
    ds3_get_available_chunks_response* chunks_response;
    ds3_bulk_response* response;
    ds3_bulk_object_list* bulk_obj_list;
    ds3_bulk_object bulk_obj;
    uint64_t chunk_count, current_chunk_count = 0;
    uint64_t chunk_index, obj_index;
    FILE* obj_file;

    handle_error(error);

    // Create a bucket where our files will be stored
    request = ds3_init_put_bucket(bucket_name);

    error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    handle_error(error);

    // Create the bulk put request
    obj_list = ds3_convert_file_list(books, 5);
    request = ds3_init_put_bulk(bucket_name, obj_list);

    // Initialize the bulk put
    error = ds3_bulk(client, request, &response);
    ds3_free_request(request);
    handle_error(error);

    chunk_count = response->list_size;

    // We need to transfer all the chunks
    while (current_chunk_count < chunk_count) {
        request = ds3_init_get_available_chunks(response->job_id->value);

        error = ds3_get_available_chunks(client, request, &chunks_response);
        ds3_free_request(request);
        handle_error(error);

        if (chunks_response->object_list->list_size > 0) {
            for (chunk_index = 0; chunk_index < chunks_response->object_list->list_size; chunk_index++) {
                bulk_obj_list = chunks_response->object_list->list[chunk_index];
                if (bulk_obj_list->chunk_number < current_chunk_count) {
                    continue;  // do not process a chunk we've already processed
                }
                for (obj_index = 0; obj_index < bulk_obj_list->size; obj_index++) {
                    bulk_obj = bulk_obj_list->list[obj_index];
                    request = ds3_init_put_object_for_job(bucket_name, bulk_obj.name->value, bulk_obj.offset, bulk_obj.length, response->job_id->value);
                    obj_file = fopen(bulk_obj.name->value, "r");
                    error = ds3_put_object(client, request, obj_file, ds3_read_from_file);
                    ds3_free_request(request);
                    fclose(obj_file);
                    handle_error(error);
                }
                current_chunk_count++;
            }
        }
        else {
            // When no chunks are returned we need to sleep for a while to free up cache space
            sleep(chunks_response->retry_after);
        }
    }

    // Cleanup the client and sdk
    ds3_free_creds(client->creds);
    ds3_free_client(client);
    ds3_cleanup();
    return 0;
}
