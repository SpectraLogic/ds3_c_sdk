#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ds3.h>

int main (int args, char * argv[]) {
    ds3_get_service_response *response; 
    uint64_t i,n;

    ds3_creds * creds = ds3_create_creds("<access_key>","<secret_key>");
    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);
    
    ds3_client_proxy(client, "192.168.56.1:8888");
    
    ds3_request* request = ds3_init_get_service();
    ds3_error* error = ds3_get_service(client, request, &response);
    ds3_free_request(request);
   
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

    if (response == NULL) {
        printf("Response was null\n");
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    if(response->num_buckets == NULL) {
        printf("Num buckets is null\n");
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    for (i = 0; i < response->num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        printf("Bucket: (%s) created on %s\n", bucket.name, bucket.creation_date);
    }
    
    ds3_free_service_response(response);

    /*
    
    ds3_bulk_response *response;
    char * bucket = "books6";

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

    ds3_request * create_bucket_request = ds3_init_put_bucket(bucket);

    ds3_put_bucket(client, create_bucket_request);
    ds3_free_request(create_bucket_request);
   
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

    clean_up:
    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
