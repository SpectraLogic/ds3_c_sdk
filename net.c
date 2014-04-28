#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <glib.h>
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

char * _generate_signature_str(http_verb verb, char * resource_name, char * date,
                                     char * content_type, char * md5, char * amz_headers) {
    if(resource_name == NULL) {
        fprintf(stderr, "resource_name is required\n");
        return NULL;
    }
    if(date == NULL) {
        fprintf(stderr, "date is required");
        return NULL;
    }
    char * verb_str = net_get_verb(verb);
    /*
    size_t total_len = 0;
    total_len += strlen(verb_str) + 1; // +1 for the newline
    total_len += strlen(md5) +1;
    total_len += strlen(content_type) + 1;
    total_len += strlen(date) + 1;
    total_len += strlen(amz_headers);
    total_len += strlen(resource_name) +1; // +1 for NULL character 
    */

    return g_strconcat(verb_str, "\n", md5, "\n", content_type, "\n", date, "\n", amz_headers, resource_name, NULL);
}

char * net_compute_signature(const ds3_creds *creds, http_verb verb, char * resource_name,
                             char * date, char * content_type, char * md5, char * amz_headers) {
    char * signature_str = _generate_signature_str(verb, resource_name, date, content_type, md5, amz_headers); 
    fprintf(stdout, "Signature:\n%s\n", signature_str);
    //char * checksum = g_compute_hmac_for_string(G_CHECKSUM_SHA1, creds->secret_key, creds->secret_key_len, signature_str, -1);
   
    gsize bufSize = 256;
    guint8 * buffer = (guint8 *) calloc(bufSize, sizeof(guint8)); 

    GHmac *hmac = g_hmac_new(G_CHECKSUM_SHA1, creds->secret_key, creds->secret_key_len);
    g_hmac_update(hmac, signature_str, -1);
    g_hmac_get_digest(hmac, buffer, &bufSize);
    
    gchar * signature = g_base64_encode(buffer, bufSize);
    
    g_free(signature_str);
    g_hmac_unref(hmac);
    free(buffer);

    return signature;
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
