#include <stdlib.h>
#include <stdio.h>
#include "./ds3.h"
#include "./net.h"

int main (int args, char * argv[]) {
    printf("Starting playing with sdk code\n");

    ds3_request * request = ds3_init_get_service();
    ds3_client * client = ds3_init_client("http://www.google.com", NULL);

    //net_process_request(client, NULL);

    ds3_print_request(request);

    ds3_free_client(client);
    ds3_free_request(request);
    ds3_cleanup();

    return 0;
}
