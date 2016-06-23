#include "ds3.h"

#ifndef __DS3_TEST__
#define __DS3_TEST__

ds3_client* get_client();
ds3_client* get_client_at_loglvl(ds3_log_lvl lvl);
void clear_bucket(const ds3_client* client, const char* bucket_name);
void populate_with_objects(const ds3_client* client, const char* bucket_name);
ds3_str* populate_with_objects_return_job(const ds3_client* client, const char* bucket_name);
ds3_str* populate_with_empty_objects(const ds3_client* client, const char* bucket_name);

ds3_request* populate_bulk_return_request(const ds3_client* client, const char* bucket_name, ds3_bulk_object_list_response* obj_list);
ds3_master_object_list_response* populate_bulk_return_response(const ds3_client* client, ds3_request* request);
void populate_with_objects_from_bulk(const ds3_client* client, const char* bucket_name, ds3_master_object_list_response* response);
ds3_master_object_list_response* ensure_available_chunks(const ds3_client* client, ds3_str* job_id);

bool contains_object(ds3_list_bucket_result_response* bucket_list, const char* key);
void handle_error(ds3_error* error);
bool handle_error_and_return_is_null(ds3_error* error);
void free_client(ds3_client* client);

ds3_bulk_object_list_response* default_object_list();

ds3_error* get_bucket_data_policy_id(const ds3_client* client, const char* bucket_name, ds3_str* data_policy_id);
ds3_error* get_bucket_data_policy_checksum(const ds3_client* client, const char* bucket_name, ds3_checksum_type* checksum_type);
#endif
