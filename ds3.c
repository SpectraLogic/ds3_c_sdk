#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "net.h"
#include "ds3.h"

GHashTable * _create_hash_table(void) {
    GHashTable * hash =  g_hash_table_new(g_str_hash, g_str_equal);
    return hash;
}

ds3_client * ds3_init_client(const char * endpoint, const ds3_creds * creds) {
    if(endpoint == NULL) {
        printf("Null endpoint\n");
        return NULL;
    }

    ds3_client * client = (ds3_client *) calloc(1, sizeof(ds3_client));
    size_t length = strlen(endpoint);
    client->endpoint = (char *) calloc(length + 1, sizeof(char));
    strncpy(client->endpoint, endpoint, length);
    return client;
}

ds3_request * ds3_init_get_service(void) {
    ds3_request * request = (ds3_request *) calloc(1, sizeof(ds3_request));
    request->verb = GET;
    request->uri = (char *) calloc(2, sizeof(char));
    request->uri[0] = '/';
    request->headers = _create_hash_table();
    request->query_params = _create_hash_table();
    return request;
}

ds3_get_service_response * ds3_get_service(const ds3_client * client, const ds3_request * request) {

}

void ds3_print_request(const ds3_request * request) {
    if(request == NULL) {
      printf("Request object was null\n");
      return;
    }
    printf("Verb: %s\n", net_get_verb(request->verb));
    printf("Path: %s\n", request->uri);
}

void ds3_free_client(ds3_client * client) {
    if(client == NULL) {
      return;
    }
    if(client->endpoint != NULL) {
        free(client->endpoint);
    }
    if(client->creds != NULL) {
        free(client->creds);
    }
    free(client);
}

void ds3_free_request(ds3_request * request) {
    if(request == NULL) {
        return;
    }
    if(request->uri != NULL) {
        free(request->uri);
    }
    if(request->headers != NULL) {
        g_hash_table_destroy(request->headers);
    }
    if(request->query_params != NULL) {
        g_hash_table_destroy(request->query_params);
    }
    free(request);
}

void ds3_cleanup(void) {
    net_cleanup();
}
