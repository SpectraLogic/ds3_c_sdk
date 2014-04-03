#include <glib.h>
#include <stdlib.h>
#include <malloc.h>
#include "./ds3.h"

GHashTable * create_hash_table() {
    GHashTable * hash =  g_hash_table_new(g_str_hash, g_str_equal);
    return hash;
}

const char * get_verb(http_verb verb) {
    switch(verb) {
        case GET: return "GET";
        case PUT: return "PUT";
        case POST: return "POST";
        case DELETE : return "DELETE";
    }
}

ds3_request * ds3_init_get_service() {
    ds3_request * request = (ds3_request *) calloc(1, sizeof(ds3_request));
    request->verb = GET;
    request->uri = (char *) calloc(2, sizeof(char));
    request->uri[0] = '/';
    request->headers = create_hash_table();
    request->query_params = create_hash_table();
    return request;
}

void ds3_print_request(const ds3_request * request) {
    printf("Verb: %s\n", get_verb(request->verb));
    printf("Path: %s\n", request->uri);
}

void ds3_free_request(ds3_request * request) {
    free(request->uri);
    g_hash_table_destroy(request->headers);
    g_hash_table_destroy(request->query_params);
    free(request);
}
