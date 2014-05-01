#include <glib.h>
#include <libxml/parser.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "ds3.h"
#include "net.h"

GHashTable * _create_hash_table(void) {
    GHashTable * hash =  g_hash_table_new(g_str_hash, g_str_equal);
    return hash;
}

ds3_creds * ds3_create_creds(const char * access_id, const char * secret_key) {
    ds3_creds * creds;
    if(access_id == NULL || secret_key == NULL) {
        fprintf(stderr, "Arguments cannot be NULL\n");
        return NULL;
    }
    
    creds = (ds3_creds *) calloc(1, sizeof(ds3_creds));

    creds->access_id = g_strdup(access_id);
    creds->access_id_len = strlen(creds->access_id);

    creds->secret_key = g_strdup(secret_key); 
    creds->secret_key_len = strlen(creds->secret_key);

    return creds;
}

ds3_client * ds3_create_client(const char * endpoint, ds3_creds * creds) {
    ds3_client * client;
    if(endpoint == NULL) {
        fprintf(stderr, "Null endpoint\n");
        return NULL;
    }

    client = (ds3_client *) calloc(1, sizeof(ds3_client));
    
    client->endpoint = g_strdup(endpoint);
    client->endpoint_len = strlen(endpoint);
    
    client->creds = creds;
    return client;
}

void ds3_client_proxy(ds3_client * client, const char * proxy) {
    client->proxy = g_strdup(proxy);
    client->proxy_len = strlen(proxy);
}

ds3_request * _common_request_init(void){
    ds3_request * request = (ds3_request *) calloc(1, sizeof(ds3_request));
    request->headers = _create_hash_table();
    request->query_params = _create_hash_table();
    return request;
}

ds3_request * ds3_init_get_service(void) {
    ds3_request * request = _common_request_init(); 
    request->verb = GET;
    request->path = (char *) calloc(2, sizeof(char));
    request->path [0] = '/';
    return request;
}

ds3_request * ds3_init_get_bucket(const char * bucket_name) {
    ds3_request * request = _common_request_init(); 
    request->verb = GET;
    request->path = g_strconcat("/", bucket_name, NULL);
    return request;
}

void _internal_request_dispatcher(const ds3_client * client, const ds3_request * request) {
    if(client == NULL || request == NULL) {
        fprintf(stderr, "All arguments must be filled in\n");
    }
    net_process_request(client, request);
}

ds3_get_service_response * ds3_get_service(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request); 
    return NULL;    
}

void ds3_get_bucket(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request);
}

void ds3_print_request(const ds3_request * request) {
    if(request == NULL) {
      fprintf(stderr, "Request object was null\n");
      return;
    }
    printf("Verb: %s\n", net_get_verb(request->verb));
    printf("Path: %s\n", request->path);
}

void ds3_free_service_response(ds3_get_service_response * response){
    size_t buckets;
    int i;

    if(response == NULL) {
        return;
    }

    buckets = response->num_buckets;

    for(i = 0; i<buckets; i++) {
        free(response->buckets[i]);
    }
    free(response->buckets);
    free(response);
}

void ds3_free_creds(ds3_creds * creds) {
    if(creds == NULL) {
        return;
    }

    if(creds->access_id != NULL) {
        g_free(creds->access_id);
    }

    if(creds->secret_key != NULL) {
        g_free(creds->secret_key);
    }
    free(creds);
}

void ds3_free_client(ds3_client * client) {
    if(client == NULL) {
      return;
    }
    if(client->endpoint != NULL) {
        g_free(client->endpoint);
    }
    if(client->proxy != NULL) {
        g_free(client->proxy);
    }
    free(client);
}

void ds3_free_request(ds3_request * request) {
    if(request == NULL) {
        return;
    }
    if(request->path != NULL) {
        g_free(request->path);
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
