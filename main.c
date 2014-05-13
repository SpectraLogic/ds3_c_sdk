#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "./ds3.h"

int main (int args, char * argv[]) {
    ds3_request *request;
    int i,n;
    puts("Starting playing with sdk code\n");

    ds3_creds * creds = ds3_create_creds("cnlhbg==","6S2XeqEy");

    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);
    ds3_client_proxy(client, "192.168.56.1:8888");

    ds3_request * create_bucket_request = ds3_init_put_bucket("yay_bucket2");

    ds3_put_bucket(client, create_bucket_request);
    ds3_free_request(create_bucket_request);

    request = ds3_init_get_service();
    ds3_get_service_response * response = ds3_get_service(client, request);
    ds3_free_request(request);

    for(i = 0; i < response->num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        printf("Bucket: (%s) created on %s\n", bucket.name, bucket.creation_date);
    }

    /*

    for(i = 0; i < response->num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        printf("Bucket: (%s) created on %s\n", bucket.name, bucket.creation_date);
        
        request = ds3_init_get_bucket("bucket");

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

    */

    ds3_free_service_response(response);
    
    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
