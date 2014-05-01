#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <glib.h>
#include "net.h"
#include "ds3.h"
#include "util.h"

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
    char * verb_str; 
    if(resource_name == NULL) {
        fprintf(stderr, "resource_name is required\n");
        return NULL;
    }
    if(date == NULL) {
        fprintf(stderr, "date is required");
        return NULL;
    }
    verb_str = net_get_verb(verb);

    return g_strconcat(verb_str, "\n", md5, "\n", content_type, "\n", date, "\n", amz_headers, resource_name, NULL);
}

char * _generate_date_string(void) {
    GDateTime * time  = g_date_time_new_now_local();
    
    char * date_string = g_date_time_format(time, "%a, %d %b %Y %T %z");
    fprintf(stdout, "Date: %s\n", date_string);
    g_date_time_unref(time);

    return date_string;
}

char * net_compute_signature(const ds3_creds *creds, http_verb verb, char * resource_name,
                             char * date, char * content_type, char * md5, char * amz_headers) {
    char * signature_str = _generate_signature_str(verb, resource_name, date, content_type, md5, amz_headers); 
    fprintf(stdout, "Signature:\n%s\n", signature_str);
   
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

//TODO this should return some kind of response
//     need to think about how to return back data for a stream (large object)
//     and data that will be consummed by the xml parser
void net_process_request(const ds3_client * client, const ds3_request * request) {
    _init_curl();
    
    CURL * handle = curl_easy_init();
    CURLcode res;

    if(handle) {
        char * url = g_strconcat(client->endpoint, request->path, NULL);
        curl_easy_setopt(handle, CURLOPT_URL, url);

        if(client->proxy != NULL) {
          curl_easy_setopt(handle, CURLOPT_PROXY, client->proxy);
        }

        char * date = _generate_date_string(); 
        char * date_header = g_strconcat("Date: ", date, NULL);
        char * signature = net_compute_signature(client->creds, request->verb, request->path, date, "", "", "");
        struct curl_slist *headers = NULL;
        char * auth_header = g_strconcat("Authorization: AWS ", client->creds->access_id, ":", signature, NULL);

        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, date_header);

        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(handle);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed %s\n", curl_easy_strerror(res));
        }

        g_free(url);
        g_free(date);
        g_free(date_header);
        g_free(signature);
        g_free(auth_header);
        curl_slist_free_all(headers);
        curl_easy_cleanup(handle);
    }
    else {
        printf("Failed to create curl handle\n");
    }
}

void net_cleanup(void) {
    curl_global_cleanup();
}
