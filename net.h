#include "ds3.h"

#ifndef __DS3__NET__HEADER__
#define __DS3__NET__HEADER__

char * net_get_verb(http_verb verb);

void net_process_request(const ds3_client * client, const ds3_request * request); 

void net_cleanup();

#endif
