#include "ds3.h"

#ifndef __DS3_TEST__
#define __DS3_TEST__

ds3_client* get_client();
ds3_client* get_client_at_loglvl(ds3_log_lvl lvl);
void clear_bucket(const ds3_client* client, const char* bucket_name);
void populate_with_objects(const ds3_client* client, const char* bucket_name, uint32_t max_upload_size = 0);
ds3_str* populate_with_objects_return_job(const ds3_client* client, const char* bucket_name, uint32_t max_upload_size = 0);
ds3_str* populate_with_empty_objects(const ds3_client* client, const char* bucket_name);
bool contains_object(const ds3_object* objects, uint64_t num_objects, const char* obj);
void handle_error(ds3_error* error);
void free_client(ds3_client* client);
#endif
