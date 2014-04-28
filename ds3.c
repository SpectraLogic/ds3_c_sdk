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

ds3_creds * ds3_create_creds(const char * access_id, const char * secret_key) {
    if(access_id == NULL || secret_key == NULL) {
        fprintf(stderr, "Arguments cannot be NULL\n");
        return NULL;
    }
    
    ds3_creds * creds = (ds3_creds *) calloc(1, sizeof(ds3_creds));

    creds->access_id = g_strdup(access_id);
    creds->access_id_len = strlen(creds->access_id);

    creds->secret_key = g_strdup(secret_key); 
    creds->secret_key_len = strlen(creds->secret_key);

    return creds;
}

ds3_client * ds3_create_client(const char * endpoint, const ds3_creds * creds) {
    if(endpoint == NULL) {
        fprintf(stderr, "Null endpoint\n");
        return NULL;
    }

    ds3_client * client = (ds3_client *) calloc(1, sizeof(ds3_client));
    
    client->endpoint = g_strdup(endpoint);
    client->endpoint_len = strlen(client->endpoint);

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

void ds3_free_creds(ds3_creds * creds) {
    if(creds == NULL) {
        return;
    }

    if(creds->access_id != NULL) {
        free(creds->access_id);
    }

    if(creds->secret_key != NULL) {
        free(creds->secret_key);
    }
    free(creds);
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
