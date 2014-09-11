#include "stdio.h"
#include "stdlib.h"
#include "ds3.h"

static ds3_client * get_client() {
    char * endpoint = getenv("DS3_ENDPOINT");
    char * access_key = getenv("DS3_ACCESS_KEY");
    char * secret_key = getenv("DS3_SECRET_KEY");

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

    ds3_creds * creds = ds3_create_creds(access_key, secret_key);
 
    return ds3_create_client(endpoint, creds);
}

int main(int args, char * argv[]) {
    printf("Creating client:\n");
    ds3_client * client = get_client();
    printf("Cleaning up client\n");
    ds3_free_client(client);
    printf("Finished test run\n");
    return 0;
}
