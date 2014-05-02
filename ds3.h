#ifndef __DS3_HEADER__
#define __DS3_HEADER__

#include <glib.h>

typedef enum {
    false, true
}ds3_bool;

typedef enum {
  GET, PUT, POST, DELETE
}http_verb;

typedef struct {
    char * access_id;
    size_t access_id_len;
    char * secret_key;
    size_t secret_key_len;
}ds3_creds;

typedef struct {
    char * endpoint;
    size_t endpoint_len;
    char * proxy;
    size_t  proxy_len;
    ds3_creds * creds; 
}ds3_client;

typedef struct {
    http_verb verb; 
    char * path;
    GHashTable * headers;
    GHashTable * query_params;
}ds3_request;

typedef struct {
    char * creation_date;
    size_t creation_date_size;
    char * name;
    size_t name_size;
}ds3_bucket;

typedef struct {
    char * name;
    size_t name_size;
    char * id;
    size_t id_size;
}ds3_owner;

typedef struct {
    ds3_bucket ** buckets ;
    size_t num_buckets;
    ds3_owner *owner;
}ds3_get_service_response;


ds3_request * ds3_init_get_service(void);
ds3_request * ds3_init_get_bucket(const char * bucket_name);

ds3_creds * ds3_create_creds(const char * access_id, const char * secret_key);

ds3_client * ds3_create_client(const char * endpoint, ds3_creds * creds);
void ds3_client_proxy(ds3_client * client, const char * proxy);

ds3_get_service_response * ds3_get_service(const ds3_client * client, const ds3_request * request);
//TODO need to create a return type here
void ds3_get_bucket(const ds3_client * client, const ds3_request * request);


void ds3_print_request(const ds3_request * request);
void ds3_free_bucket(ds3_bucket * bucket);
void ds3_free_owner(ds3_owner * owber);
void ds3_free_creds(ds3_creds * client);
void ds3_free_client(ds3_client * client);
void ds3_free_request(ds3_request * request);
void ds3_cleanup(void);
#endif
