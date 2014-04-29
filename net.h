#include "ds3.h"

#ifndef __DS3__NET__HEADER__
#define __DS3__NET__HEADER__

#define net_log(...) fprintf(stderr, __VA_ARGS__)

char * net_get_verb(http_verb verb);

char * net_compute_signature(const ds3_creds *, http_verb verb, char * resource_name,
    char * date, char * content_type, char * md5, char * amz_headers);

void net_process_request(const ds3_client * client, const ds3_request * request); 

void net_cleanup();

#endif
