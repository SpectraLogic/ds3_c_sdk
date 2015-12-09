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

    ds3_get_service_response *response;
    uint64_t i;

    // Setup client credentials and then the actual client itself.
    ds3_creds * creds = ds3_create_creds(DS3_ACCESS_KEY, DS3_SECRET_KEY);
    ds3_client * client = ds3_create_client(DS3_ENDPOINT, creds);

    // You can optionally set a proxy server that a request should be sent through
    ds3_client_proxy(client, HTTP_PROXY);

    // Create the get service request.  All requests to a DS3 appliance start this way.
    // All ds3_init_* functions return a ds3_request struct

    ds3_request* request = ds3_init_get_service();

    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    ds3_error* error = ds3_get_service(client, request, &response);
    ds3_free_request(request);

    // Check that the request completed successfully
    if(error != NULL) {
        if(error->error != NULL) {
            printf("Got an error (%lu): %s\n", (long unsigned int)error->error->status_code, error->message->value);
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

    if(response->num_buckets == 0) {
        printf("No buckets returned\n");
        ds3_free_service_response(response);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 0;
    }

    for (i = 0; i < response->num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        printf("Bucket: (%s) created on %s\n", bucket.name->value, bucket.creation_date->value);
    }

    ds3_free_service_response(response);

    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();

    /*
    //Create Bucket

    ds3_creds * creds = ds3_create_creds("cnlhbg==","ZIjGDQAs");
    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);

    //ds3_client_proxy(client, "192.168.56.1:8888");
    char * bucket = "books";
    ds3_request * create_bucket_request = ds3_init_put_bucket(bucket);
    ds3_error* error = ds3_put_bucket(client, create_bucket_request);
    ds3_free_request(create_bucket_request);

    if(error != NULL) {
        if(error->error != NULL) {
            printf("Got an error (%lu): %s\n", error->error->status_code, error->message);
            printf("Message Body: %s\n", error->error->error_body);
        }
        else {
            printf("Got a runtime error: %s\n", error->message);
        }
        ds3_free_error(error);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    printf("Successfully created bucket: %s\n", bucket);
    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();
    */


    /*
    ds3_bulk_response *response;
    char * bucket = "books";

    const char * files[] = {"huckfinn.txt", "ulysses.txt"};

    ds3_bulk_object_list * list = ds3_convert_file_list(files, 2);

    ds3_request* request = ds3_init_put_bulk(bucket, list);

    ds3_error* error = ds3_bulk(client, request, &response);
    ds3_free_request(request);

    printf("JobId: %s, Total Object Lists: %lu\n", response->job_id, response->list_size);

    ds3_free_error(error);
    ds3_free_bulk_response(response);

    ds3_free_bulk_object_list(list);

    request = ds3_init_get_bucket(bucket);

    ds3_print_request(request);

    ds3_get_bucket_response * bucket_response = ds3_get_bucket(client, request);
    ds3_free_request(request);
    for(n = 0; n < bucket_response->num_objects; n++) {
        ds3_object object = bucket_response->objects[n];
        printf("Object: (%s) with size %lu\n", object.name, object.size);
        request = ds3_init_get_object(bucket, object.name);
        FILE * out = fopen(object.name, "w");
        ds3_get_object(client, request, out, ds3_write_to_file);
        ds3_free_request(request);
        fclose(out);
    }

    ds3_free_bucket_response(bucket_response);


    char * object = "huckfinn.txt";

    struct stat st;
    memset(&st, 0, sizeof(struct stat));

    if(stat(object, &st)!= 0) {
        fprintf(stderr, "Failed to stat file\n");
        goto clean_up;
    }

    ds3_request * put_object_request = ds3_init_put_object(bucket, object, st.st_size);

    FILE * in = fopen(object, "r");

    ds3_put_object(client, put_object_request, in, ds3_read_from_file);

    ds3_free_request(put_object_request);

    ds3_request * create_bucket_request = ds3_init_put_bucket(bucket);

    ds3_put_bucket(client, create_bucket_request);
    ds3_free_request(create_bucket_request);


    request = ds3_init_get_service();
    ds3_get_service_response * response = ds3_get_service(client, request);
    ds3_free_request(request);

    for(i = 0; i < response->num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        printf("Bucket: (%s) created on %s\n", bucket.name, bucket.creation_date);

        request = ds3_init_get_bucket(bucket.name);

        ds3_print_request(request);

        ds3_get_bucket_response * bucket_response = ds3_get_bucket(client, request);
        ds3_free_request(request);
        for(n = 0; n < bucket_response->num_objects; n++) {
            ds3_object object = bucket_response->objects[n];
            printf("Object: (%s) with size %lu\n", object.name, object.size);
            request = ds3_init_get_object(bucket.name, object.name);
            FILE * out = fopen(object.name, "w");
            ds3_get_object(client, request, out, ds3_write_to_file);
            ds3_free_request(request);
            fclose(out);
        }

        ds3_free_bucket_response(bucket_response);
    }

    ds3_free_service_response(response);
    */

    return 0;
}
