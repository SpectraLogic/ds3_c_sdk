#include <stdio.h>
#include <stdlib.h>
#include "ds3.h"
#include "test.h"
#include "service_tests.h"
#include "bucket_tests.h"

static ds3_client * get_client() {
    char* endpoint = getenv("DS3_ENDPOINT");
    char* access_key = getenv("DS3_ACCESS_KEY");
    char* secret_key = getenv("DS3_SECRET_KEY");
    char* proxy = getenv("http_proxy");

    ds3_client* client;

    if (endpoint == NULL) {
        fprintf(stderr, "DS3_ENDPOINT must be set.\n");
        exit(1);
    }

    if (access_key == NULL) {
        fprintf(stderr, "DS3_ACCESS_KEY must be set.\n");
        exit(1);
    }

    if (secret_key == NULL) {
        fprintf(stderr, "DS3_SECRET_KEY must be set.\n");
        exit(1);
    }

    ds3_creds* creds = ds3_create_creds(access_key, secret_key);
    
    client = ds3_create_client(endpoint, creds);
    
    if (proxy != NULL) {
        fprintf(stderr, "Setting proxy: %s\n", proxy);
        ds3_client_proxy(client, proxy); 
    }
    
    return client; 
}

static void run_tests(const ds3_client* client, const test* tests){
    int i;
    char* response;
    test current_test;
    for(i = 0; tests[i] != NULL; i++){
        current_test = tests[i];
        response = current_test(client);
        if (response != NULL) {
            fprintf(stderr, "Test failed with response: %s\n", response);
            break;
        }
    }
    printf("Finished test run\n");
}

int main(int args, char * argv[]) {
    printf("Creating client\n");
    ds3_client * client = get_client();

    test tests[4] = {test_get_service, test_put_bucket, test_bulk_put, NULL};

    // Start test run.
    run_tests(client, tests);

    printf("Cleaning up client\n");
    ds3_free_client(client);
    printf("Finished test run\n");
    return 0;
}
