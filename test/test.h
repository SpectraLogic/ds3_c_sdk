#include "ds3.h"

#ifndef __DS3_TEST__
#define __DS3_TEST__

ds3_client * get_client();
void clear_bucket(const ds3_client* client, const char* bucket_name);

#endif
