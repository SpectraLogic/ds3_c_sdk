#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"
#include "samples.h"

int main (void) {
    // Get Service
    ds3_client* client;
    ds3_request* request;
    ds3_error* error;
    ds3_get_service_response *response;
    uint64_t i;

    // Create a client from environment variables
    error = ds3_create_client_from_env(&client);
    handle_error(error);


    // Create the get service request.  All requests to a DS3 appliance start this way.
    // All ds3_init_* functions return a ds3_request struct
    request = ds3_init_get_service();

    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    error = ds3_get_service(client, request, &response);
    ds3_free_request(request);

    handle_error(error);

    if(response->num_buckets == 0) {
        printf("No buckets returned\n");
        ds3_free_service_response(response);
        ds3_free_creds(client->creds);
        ds3_free_client(client);
        return 0;
    }

    for (i = 0; i < response->num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        printf("Bucket: (%s) created on %s\n", bucket.name->value, bucket.creation_date->value);
    }

    ds3_free_service_response(response);
    ds3_free_creds(client->creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
