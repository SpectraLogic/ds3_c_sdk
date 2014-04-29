#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "./ds3.h"

int main (int args, char * argv[]) {
    puts("Starting playing with sdk code\n");
    
    ds3_creds * creds = ds3_create_creds("cnlhbg==","mF728aI4");

    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);

    ds3_request * request = ds3_init_get_service();

    ds3_print_request(request);

    ds3_get_service_response * response = ds3_get_service(client, request);

    ds3_free_service_response(response);
    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_free_request(request);
    ds3_cleanup();

    return 0;
}
