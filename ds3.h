#include <glib.h>

#ifndef __DS3_HEADER__
#define __DS3_HEADER__

typedef enum {
    false, true
}ds3_bool;

typedef enum {
  GET, PUT, POST, DELETE
}http_verb;

typedef struct {
    char * access_id;
    char * secret_key;
}ds3_creds;

typedef struct {
    char * endpoint;
    ds3_creds * creds; 
}ds3_client;

typedef struct {
    http_verb verb; 
    char * uri;
    GHashTable * headers;
    GHashTable * query_params;
}ds3_request;

typedef struct {
    char ** buckets;
}ds3_get_service_response;


ds3_request * ds3_init_get_service(void);

ds3_client * ds3_init_client(const char * endpoint, const ds3_creds * creds);

ds3_get_service_response * ds3_get_service(const ds3_client * client, const ds3_request * request);

void ds3_print_request(const ds3_request * request);

void ds3_free_client(ds3_client * client);

void ds3_free_request(ds3_request * request);

void ds3_cleanup(void);

#endif
