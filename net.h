#ifndef __DS3__NET__HEADER__
#define __DS3__NET__HEADER__
#include "ds3.h"

char * net_get_verb(http_verb verb);

char * net_compute_signature(const ds3_creds *, http_verb verb, char * resource_name,
    char * date, char * content_type, char * md5, char * amz_headers);

void net_process_request(const ds3_client * client, const ds3_request * request, void * user_struct, size_t (*write_data)(void*, size_t, size_t, void*));

void net_cleanup();

#endif
