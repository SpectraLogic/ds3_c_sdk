#include "ds3.h"

#ifndef __DS3_TEST__
#define __DS3_TEST__

ds3_client* get_client();
ds3_client* get_client_at_loglvl(ds3_log_lvl lvl);
void clear_bucket(const ds3_client* client, const char* bucket_name);
void populate_with_objects(const ds3_client* client, const char* bucket_name);
ds3_str* populate_with_objects_return_job(const ds3_client* client, const char* bucket_name);
ds3_str* populate_with_empty_objects(const ds3_client* client, const char* bucket_name);

ds3_request* populate_bulk_return_request(const ds3_client* client, const char* bucket_name, ds3_bulk_object_list* obj_list);
ds3_bulk_response* populate_bulk_return_response(const ds3_client* client, ds3_request* request);
void populate_with_objects_from_bulk(const ds3_client* client, const char* bucket_name, ds3_bulk_response* response);

bool contains_object(const ds3_object* objects, uint64_t num_objects, const char* obj);
void handle_error(ds3_error* error);
void free_client(ds3_client* client);

// remove?
ds3_bulk_object_list* default_object_list();
#endif
