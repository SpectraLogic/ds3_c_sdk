#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include "net.h"
#include "ds3.h"

void _init_curl(void) {
    static ds3_bool initialized = false;

    if(!initialized) {
        if(curl_global_init(CURL_GLOBAL_ALL) != 0) {
          fprintf(stderr, "Encountered an error initializing libcurl\n");
        }
        initialized = true;
    }
    else {
        fprintf(stderr, "libcurl was already initialized\n");
    }
}

char * net_get_verb(http_verb verb) {
    switch(verb) {
        case GET: return "GET";
        case PUT: return "PUT";
        case POST: return "POST";
        case DELETE : return "DELETE";
    }
}

void net_process_request(const ds3_client * client, const ds3_request * request) {
    _init_curl();
    
    CURL * handle = curl_easy_init();
    CURLcode res;

    if(handle) {
      curl_easy_setopt(handle, CURLOPT_URL, client->endpoint);

      res = curl_easy_perform(handle);
      if(res != CURLE_OK) {
          fprintf(stderr, "curl_easy_perform() failed %s\n", curl_easy_strerror(res));
      }

      curl_easy_cleanup(handle);
    }
    else {
        printf("Failed to create curl handle\n");
    }
}

void net_cleanup(void) {
    curl_global_cleanup();
}
