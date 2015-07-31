#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"

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
    ds3_bulk_response* response;

    handle_error(error);

    // Create a bucket where we will put our files
    request = ds3_init_put_bucket(bucket_name);

    error = ds3_put_bucket(client, request);
    ds3_free_request(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(books, 5);
    request = ds3_init_put_bulk(bucket_name, obj_list);

    error = ds3_bulk(client, request, &response);
    ds3_free_request(request);

    handle_error(error);

    // Cleanup the client and sdk
    ds3_free_creds(client->creds);
    ds3_free_client(client);
    ds3_cleanup();
    return 0;
}
