#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"

int main(void) {

    ///*
    // Get Bucket

    ds3_get_bucket_response *response;
    uint64_t i;

    // Setup client credentials and then the actual client itself.
    ds3_creds * creds = ds3_create_creds("cnlhbg==","ZIjGDQAs");
    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);

    // You can optionally set a proxy server that a request should be sent through
    //ds3_client_proxy(client, "192.168.56.1:8888");

    char * bucket = "books";

    // Create the get bucket request.
    ds3_request* request = ds3_init_get_bucket(bucket);

    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    ds3_error* error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    // Check that the request completed successfully
    if(error != NULL) {
        if(error->error != NULL) {
            printf("Got an error (%lu): %s\n", error->error->status_code, error->message->value);
            printf("Message Body: %s\n", error->error->error_body->value);
        }
        else {
            printf("Got a runtime error: %s\n", error->message->value);
        }
        ds3_free_error(error);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    if (response == NULL) {
        printf("Response was null\n");
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    if(response->num_objects == 0) {
        printf("No objects returned\n");
        ds3_free_bucket_response(response);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 0;
    }

    for (i = 0; i < response->num_objects; i++) {
        ds3_object object = response->objects[i];
        printf("Object: (%s) created on %s\n", object.name->value, object.last_modified->value);
    }

    ds3_free_bucket_response(response);

    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
