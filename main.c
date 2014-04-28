#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "./ds3.h"
#include "./net.h"

int main (int args, char * argv[]) {
    puts("Starting playing with sdk code\n");

/*
    ds3_request * request = ds3_init_get_service();
    //ds3_client * client = ds3_init_client("http://www.google.com", NULL);


    ds3_client * client = ds3_create_client("http://192.168.56.102:8080", creds);

    //net_process_request(client, NULL);

    ds3_print_request(request);

    ds3_free_client(client);
    ds3_free_request(request);
    ds3_cleanup();

*/
    ds3_creds * creds = ds3_create_creds("cnlhbg==","wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY");
    char * signature = net_compute_signature(creds, GET, "/johnsmith/photos/puppy.jpg", "Tue, 27 Mar 2007 19:36:42 +0000", "", "", ""); 
    printf("%s\n", signature);

    g_free(signature);
    ds3_free_creds(creds);
    return 0;
}
