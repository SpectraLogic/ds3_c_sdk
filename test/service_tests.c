#include <stdbool.h>
#include <stdio.h> 
#include "ds3.h"
#include "test.h"
#include "service_tests.h"

char* test_get_service(const ds3_client* client){
    ds3_request* request = ds3_init_get_service();
    ds3_get_service_response* response;
    
    ds3_error* error = ds3_get_service(client, request, &response);

    if (error != NULL) {
        return error->message;
    }

    ds3_free_request(request);
    ds3_free_service_response(response);
    return NULL;
}

char* test_put_bucket(const ds3_client* client) {
    int i;
    bool found = false;
    char* bucket_name = "test_bucket";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_get_service_response* response;

    ds3_error* error = ds3_put_bucket(client, request);

    if (error != NULL) {
        return error->message;
    }
    
    ds3_free_request(request);
    request = ds3_init_get_service();

    error = ds3_get_service(client, request, &response);

    if (error != NULL) {
        return error->message;
    }

    for (i = 0; i < response->num_buckets; i++) {
        fprintf(stderr, "Expected Name (%s) actual (%s)\n", bucket_name, response->buckets[i].name);
        if (strcmp(bucket_name, response->buckets[i].name) == 0) {
            found = true;
            break;
        } 
    }

    ds3_free_request(request);
    ds3_free_service_response(response);

    if (found != true) {
        return "Could not find the bucket that should have been created.";
    }

    request = ds3_init_delete_bucket(bucket_name);
    error = ds3_delete_bucket(client, request);

    if (error != NULL) {
        return error->message;
    }

    ds3_free_request(request);
    return NULL;
}
