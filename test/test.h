#include "ds3.h"

#ifndef __DS3_TEST__
#define __DS3_TEST__

ds3_client * get_client();
void clear_bucket(const ds3_client* client, const char* bucket_name);
void populate_with_objects(const ds3_client* client, const char* bucket_name);
bool contains_object(const ds3_object* objects, uint64_t num_objects, const char* obj);

#endif
