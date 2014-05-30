#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#include "ds3.h"

//---------- Define opaque struct ----------//  
struct _ds3_request{
    http_verb verb;
    char *path;
    size_t path_size;
    uint64_t length;
    GHashTable * headers;
    GHashTable * query_params;
};

//---------- Networking code ----------// 
static void _init_curl(void) {
    static ds3_bool initialized = False;

    if(!initialized) {
        if(curl_global_init(CURL_GLOBAL_ALL) != 0) {
          fprintf(stderr, "Encountered an error initializing libcurl\n");
        }
        initialized = True;
    }
}

static char * _net_get_verb(http_verb verb) {
    switch(verb) {
        case GET: return "GET";
        case PUT: return "PUT";
        case POST: return "POST";
        case DELETE : return "DELETE";
        case HEAD : return "HEAD";
    }

    return NULL;
}

static unsigned char * _generate_signature_str(http_verb verb, char * resource_name, char * date,
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
    verb_str = _net_get_verb(verb);

    return (unsigned char *) g_strconcat(verb_str, "\n", md5, "\n", content_type, "\n", date, "\n", amz_headers, resource_name, NULL);
}

static char * _generate_date_string(void) {
    GDateTime * time  = g_date_time_new_now_local();
    
    char * date_string = g_date_time_format(time, "%a, %d %b %Y %T %z");
    fprintf(stdout, "Date: %s\n", date_string);
    g_date_time_unref(time);

    return date_string;
}

static char * _net_compute_signature(const ds3_creds *creds, http_verb verb, char * resource_name,
                             char * date, char * content_type, char * md5, char * amz_headers) {
    unsigned char * signature_str = _generate_signature_str(verb, resource_name, date, content_type, md5, amz_headers); 
    fprintf(stdout, "Signature:\n%s\n", signature_str);
   
    gsize bufSize = 256;
    guint8 * buffer = (guint8 *) calloc(bufSize, sizeof(guint8)); 

    GHmac *hmac = g_hmac_new(G_CHECKSUM_SHA1,(unsigned char *) creds->secret_key, creds->secret_key_len);
    g_hmac_update(hmac, signature_str, -1);
    g_hmac_get_digest(hmac, buffer, &bufSize);
    
    gchar * signature = g_base64_encode(buffer, bufSize);
    
    g_free(signature_str);
    g_hmac_unref(hmac);
    free(buffer);

    return signature;
}

static void _net_process_request(const ds3_client * client, const ds3_request * _request, void * read_user_struct, size_t (*read_handler_func)(void*, size_t, size_t, void*), void * write_user_struct, size_t (*write_handler_func)(void*, size_t, size_t, void*)) {
    _init_curl();
    
    struct _ds3_request * request = (struct _ds3_request *) _request;
    CURL * handle = curl_easy_init();
    CURLcode res;

    if(handle) {
        char * url = g_strconcat(client->endpoint, request->path, NULL);
        curl_easy_setopt(handle, CURLOPT_URL, url);
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L); //tell curl to follow redirects
        curl_easy_setopt(handle, CURLOPT_MAXREDIRS, client->num_redirects);
        if(client->proxy != NULL) {
          curl_easy_setopt(handle, CURLOPT_PROXY, client->proxy);
        }

        //Register the read and write handlers if they are set
        if(read_user_struct != NULL && read_handler_func != NULL) {
           curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, read_handler_func);
           curl_easy_setopt(handle, CURLOPT_WRITEDATA, read_user_struct);
        }

        if(write_user_struct != NULL && write_handler_func != NULL) {
            curl_easy_setopt(handle, CURLOPT_READDATA, write_user_struct);
            curl_easy_setopt(handle, CURLOPT_READFUNCTION, write_handler_func);
        }

        switch(request->verb) {
            case POST: {
                if (write_user_struct == NULL || write_handler_func == NULL) {
                    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
                }
                else {
                    curl_easy_setopt(handle, CURLOPT_POST, 1L);
                    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(handle, CURLOPT_INFILESIZE, request->length);
                }
                break;
            }
            case PUT: {
                if (write_user_struct == NULL || write_handler_func == NULL) {
                    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "PUT");
                }
                else {
                    curl_easy_setopt(handle, CURLOPT_PUT, 1L);
                    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(handle, CURLOPT_INFILESIZE, request->length);
                }
                break;
            }
            case DELETE: {
                curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "DELETE");
                break;
            }
            case HEAD: {
                curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "HEAD");
                break;
            }
            case GET: {
                //Placeholder if we need to put anything here.
                break;
            }
        }

        char * date = _generate_date_string(); 
        char * date_header = g_strconcat("Date: ", date, NULL);
        char * signature = _net_compute_signature(client->creds, request->verb, request->path, date, "", "", "");
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

static void net_cleanup(void) {
    curl_global_cleanup();
}

//---------- Ds3 code ----------// 
static GHashTable * _create_hash_table(void) {
    GHashTable * hash =  g_hash_table_new(g_str_hash, g_str_equal);
    return hash;
}

ds3_creds * ds3_create_creds(const char * access_id, const char * secret_key) {
    ds3_creds * creds;
    if(access_id == NULL || secret_key == NULL) {
        fprintf(stderr, "Arguments cannot be NULL\n");
        return NULL;
    }
    
    creds = g_new0(ds3_creds, 1);

    creds->access_id = g_strdup(access_id);
    creds->access_id_len = strlen(creds->access_id);

    creds->secret_key = g_strdup(secret_key); 
    creds->secret_key_len = strlen(creds->secret_key);

    return creds;
}

ds3_client * ds3_create_client(const char * endpoint, ds3_creds * creds) {
    ds3_client * client;
    if(endpoint == NULL) {
        fprintf(stderr, "Null endpoint\n");
        return NULL;
    }

    client = g_new0(ds3_client, 1);
    
    client->endpoint = g_strdup(endpoint);
    client->endpoint_len = strlen(endpoint);
    
    client->creds = creds;

    client->num_redirects = 5L; //default to 5 redirects before failing
    return client;
}

void ds3_client_proxy(ds3_client * client, const char * proxy) {
    client->proxy = g_strdup(proxy);
    client->proxy_len = strlen(proxy);
}

static struct _ds3_request * _common_request_init(void){
    struct _ds3_request * request = g_new0(struct _ds3_request, 1);
    request->headers = _create_hash_table();
    request->query_params = _create_hash_table();
    return request;
}

ds3_request * ds3_init_get_service(void) {
    struct _ds3_request * request = _common_request_init(); 
    request->verb = GET;
    request->path =  g_new0(char, 2);
    request->path [0] = '/';
    return (ds3_request *) request;
}

ds3_request * ds3_init_get_bucket(const char * bucket_name) {
    struct _ds3_request * request = _common_request_init(); 
    request->verb = GET;
    request->path = g_strconcat("/", bucket_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_get_object(const char *bucket_name, const char *object_name) {
    struct _ds3_request * request = _common_request_init();
    request->verb = GET;
    request->path = g_strconcat("/", bucket_name, "/", object_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_delete_object(const char *bucket_name, const char *object_name) {
    struct _ds3_request * request = _common_request_init();
    request->verb = DELETE;
    request->path = g_strconcat("/", bucket_name, "/", object_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_put_object(const char *bucket_name, const char *object_name, uint64_t length) {
    struct _ds3_request * request = _common_request_init();
    request->verb = PUT;
    request->path = g_strconcat("/", bucket_name, "/", object_name, NULL);
    request->length = length;
    return (ds3_request *) request;
}

ds3_request * ds3_init_put_bucket(const char * bucket_name) {
    struct _ds3_request * request = _common_request_init();
    request->verb = PUT;
    request->path = g_strconcat("/", bucket_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_delete_bucket(const char * bucket_name) {
    struct _ds3_request * request = _common_request_init();
    request->verb = DELETE;
    request->path = g_strconcat("/", bucket_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_get_bulk(const char * bucket_name, const ds3_bulk_object * object_list) {
    struct _ds3_request * request = _common_request_init();
    request->verb = GET;
    request->path = g_strconcat("/_rest_/buckets/", bucket_name, NULL);
    g_hash_table_insert(request->query_params, "operation", "start_bulk_get");
    return (ds3_request *) request;
}

ds3_request * ds3_init_put_bulk(const char * bucket_name, const ds3_bulk_object * object_list) {
    struct _ds3_request * request = _common_request_init();
    request->verb = GET;
    request->path = g_strconcat("/_rest_/buckets/", bucket_name, NULL);
    g_hash_table_insert(request->query_params, "operation", "start_bulk_put");
    return (ds3_request *) request;
}

static void _internal_request_dispatcher(const ds3_client * client, const ds3_request * request, void * read_user_struct, size_t (*read_handler_func)(void*, size_t, size_t, void*), void * write_user_struct, size_t (*write_handler_func)(void*, size_t, size_t, void*)) {
    if(client == NULL || request == NULL) {
        fprintf(stderr, "All arguments must be filled in\n");
        return;
    }
    _net_process_request(client, request, read_user_struct, read_handler_func, write_user_struct, write_handler_func);
}

static size_t load_xml_buff(void* contents, size_t size, size_t nmemb, void *user_data) {
    size_t realsize = size * nmemb;
    GByteArray* blob = (GByteArray*) user_data;
    
    g_byte_array_append(blob, (const guint8 *) contents, realsize);
    return realsize;
}

static void _parse_buckets(xmlDocPtr doc, xmlNodePtr buckets_node, ds3_get_service_response * response) {
    xmlChar * text;
    xmlNodePtr data_ptr; 
    xmlNodePtr curr;
    GArray * array = g_array_new(FALSE, TRUE, sizeof(ds3_bucket));

    for(curr = buckets_node->xmlChildrenNode; curr != NULL; curr = curr->next) {
        ds3_bucket bucket; 
        memset(&bucket, 0, sizeof(ds3_bucket));
        for(data_ptr = curr->xmlChildrenNode; data_ptr != NULL; data_ptr = data_ptr->next) {
            if(xmlStrcmp(data_ptr->name, (const xmlChar *) "CreationDate") == 0) {
                text = xmlNodeListGetString(doc, data_ptr->xmlChildrenNode, 1);
                bucket.creation_date = g_strdup((char *) text);
                bucket.creation_date_size = strlen((char *) text);
                xmlFree(text);
            }else if(xmlStrcmp(data_ptr->name, (const xmlChar *) "Name") == 0) {
                text = xmlNodeListGetString(doc, data_ptr->xmlChildrenNode, 1);
                bucket.name = g_strdup((char *) text);
                bucket.name_size = strlen((char *) text);
                xmlFree(text);
            }
            else {
                fprintf(stderr, "Unknown element: (%s)\n", data_ptr->name);
            }
        }
        g_array_append_val(array, bucket);
    }

    response->num_buckets = array->len;
    response->buckets = (ds3_bucket *)array->data;
    g_array_free(array, FALSE);
}

static ds3_owner * _parse_owner(xmlDocPtr doc, xmlNodePtr owner_node) {
    xmlNodePtr child_node;
    xmlChar * text;
    ds3_owner * owner = g_new0(ds3_owner, 1);

    for(child_node = owner_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(xmlStrcmp(child_node->name, (const xmlChar *) "DisplayName") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            owner->name = g_strdup((char *) text);
            owner->name_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "ID") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            owner->id = g_strdup((char *) text);
            owner->id_size = strlen((char *) text);
            xmlFree(text);
        }
        else {
            fprintf(stderr, "Unknown xml element: (%s)\n", child_node->name);
        }
    }

    return owner;
}

ds3_get_service_response * ds3_get_service(const ds3_client * client, const ds3_request * request) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    GByteArray* xml_blob = g_byte_array_new();
    ds3_get_service_response * response;
    
    _internal_request_dispatcher(client, request, xml_blob, load_xml_buff, NULL, NULL);
   
    doc = xmlParseMemory((const char *) xml_blob->data, xml_blob->len);

    if(doc == NULL) {
        fprintf(stderr, "Failed to parse document.");
        fprintf(stdout, "Result: %s\n", xml_blob->data);
        g_byte_array_free(xml_blob, TRUE);
        return NULL;
    }

    root = xmlDocGetRootElement(doc);
    
    if(xmlStrcmp(root->name, (const xmlChar*) "ListAllMyBucketsResult") != 0) {
        fprintf(stderr, "wrong document, expected root node to be ListAllMyBucketsResult");
        fprintf(stdout, "Result: %s\n", xml_blob->data);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        return NULL;
    }

    response = g_new0(ds3_get_service_response, 1);
    
    for(child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(xmlStrcmp(child_node->name, (const xmlChar*) "Buckets") == 0) {
            //process buckets here
            _parse_buckets(doc, child_node, response);

        }
        else if(xmlStrcmp(child_node->name, (const xmlChar*) "Owner") == 0) {
            //process owner here
            ds3_owner * owner = _parse_owner(doc, child_node);
            response->owner = owner;
        }
        else {
            fprintf(stderr, "Unknown xml element: (%s)\b", child_node->name);
        }
    }

    xmlFreeDoc(doc);
    g_byte_array_free(xml_blob, TRUE);
    return response;    
}

static ds3_object _parse_object(xmlDocPtr doc, xmlNodePtr contents_node) {
    xmlNodePtr child_node;
    xmlChar *text;
    ds3_object object;
    memset(&object, 0, sizeof(ds3_object));

    for(child_node = contents_node->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(xmlStrcmp(child_node->name, (const xmlChar *) "Key") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            object.name = g_strdup((char *) text);
            object.name_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "ETag") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.etag= g_strdup((char *) text);
            object.etag_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "LastModified") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.last_modified = g_strdup((char *) text);
            object.last_modified_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "StorageClass") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.storage_class = g_strdup((char *) text);
            object.storage_class_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Size") == 0) { 
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            uint64_t size = strtoul((const char *)text, NULL, 10);
            object.size = size;
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Owner") == 0) { 
            ds3_owner * owner = _parse_owner(doc, child_node);
            object.owner = owner;
        }        
        else {
            fprintf(stderr, "Unknown xml element: (%s)\n", child_node->name);
        }
    }

    return object;
}

ds3_get_bucket_response * ds3_get_bucket(const ds3_client * client, const ds3_request * request) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    xmlChar * text;
    ds3_get_bucket_response * response;
    GArray * object_array = g_array_new(FALSE, TRUE, sizeof(ds3_object));
    GByteArray* xml_blob = g_byte_array_new();
    _internal_request_dispatcher(client, request, xml_blob, load_xml_buff, NULL, NULL);
    
    doc = xmlParseMemory((const char *) xml_blob->data, xml_blob->len);
    if(doc == NULL) {
        fprintf(stderr, "Failed to parse document");
        fprintf(stdout, "Result: %s\n", xml_blob->data);
        g_byte_array_free(xml_blob, TRUE); 
        return NULL;
    }

    root = xmlDocGetRootElement(doc);

    if(xmlStrcmp(root->name, (const xmlChar *) "ListBucketResult") != 0) {
        fprintf(stderr, "wrong document, expected root node to be ListBucketResult");
        fprintf(stdout, "Result: %s\n", xml_blob->data);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        return NULL;
    }

    response = g_new0(ds3_get_bucket_response, 1);

    for(child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if(xmlStrcmp(child_node->name, (const xmlChar *) "Contents") == 0) {
            ds3_object object = _parse_object(doc, child_node);
            g_array_append_val(object_array, object);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "CreationDate") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->creation_date = g_strdup((char *) text);
            response->creation_date_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "IsTruncated") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(strncmp((char *) text, "true", 4) == 0) {
                response->is_truncated = True; 
            }
            else {
                response->is_truncated = False; 
            }
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Marker") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->marker = g_strdup((char *) text);
            response->marker_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "MaxKeys") == 0) {
            uint64_t max_keys; 
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            max_keys = strtoul((const char *)text, NULL, 10); 
            response->max_keys = max_keys;
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Name") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            response->name = g_strdup((char *) text);
            response->name_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Delimiter") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->delimiter= g_strdup((char *) text);
            response->delimiter_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "NextMarker") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->next_marker= g_strdup((char *) text);
            response->next_marker_size = strlen((char *) text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Prefix") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->prefix = g_strdup((char *) text);
            response->prefix_size = strlen((char *) text);
            xmlFree(text);
        }
        else {
            fprintf(stderr, "Unknown element: (%s)\n", child_node->name);
        }
    }

    response->objects = (ds3_object *) object_array->data;
    response->num_objects = object_array->len;
    xmlFreeDoc(doc);
    g_array_free(object_array, FALSE);
    g_byte_array_free(xml_blob, TRUE);

    return response;
}

void ds3_get_object(const ds3_client * client, const ds3_request * request, void *user_data, size_t(*callback)(void*,size_t, size_t, void*)) {
    _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL);
}

void ds3_put_object(const ds3_client * client, const ds3_request * request, void * user_data, size_t (* callback)(void *, size_t, size_t, void *)) {
    _internal_request_dispatcher(client, request, NULL, NULL, user_data, callback);
}

void ds3_delete_object(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

void ds3_put_bucket(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

void ds3_delete_bucket(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL);
}

ds3_bulk_response * ds3_bulk(const ds3_client * client, const ds3_request * request) {

    xmlNodePtr root_node, objects_node, object_node;
    xmlDocPtr doc;
    xmlChar *xml_buff;
    int buff_size;
    
    doc = xmlNewDoc((xmlChar *)"1.0");
    root_node = xmlNewNode(NULL, (xmlChar *) "MasterObjectList");
    
    objects_node = xmlNewNode(NULL, (xmlChar *) "Objects");

    xmlAddChild(root_node, objects_node);

    object_node = xmlNewNode(NULL, (xmlChar *) "Object");
    xmlAddChild(objects_node, object_node);

    xmlSetProp(object_node, (xmlChar *) "Name", (xmlChar *) "file1.txt");
    xmlSetProp(object_node, (xmlChar *) "Size", (xmlChar *) "1245");

    xmlDocSetRootElement(doc, root_node);

    xmlDocDumpFormatMemory(doc, &xml_buff, &buff_size, 1);
    printf("%s\n", (char *) xml_buff);

    xmlFreeDoc(doc);
    //GByteArray* xml_blob = g_byte_array_new();
    //_internal_request_dispatcher(client, request, xml_blob, load_xml_buff, NULL, NULL);

    xmlFree(xml_buff);


    return NULL;
}


void ds3_print_request(const ds3_request * _request) {
    const struct _ds3_request * request; 
    if(_request == NULL) {
      fprintf(stderr, "Request object was null\n");
      return;
    }
    request = (struct _ds3_request *)_request;
    printf("Verb: %s\n", _net_get_verb(request->verb));
    printf("Path: %s\n", request->path);
}

void ds3_free_bucket_response(ds3_get_bucket_response * response){
    size_t num_objects;
    int i;
    if(response == NULL) {
        return;
    }

    num_objects = response->num_objects;

    for(i = 0; i < num_objects; i++) {
        ds3_object object = response->objects[i];
        g_free(object.name);
        g_free(object.etag);
        g_free(object.storage_class);
        g_free(object.last_modified);
        ds3_free_owner(object.owner);
    }

    g_free(response->objects);
    g_free(response->creation_date);
    g_free(response->marker);
    g_free(response->delimiter);
    g_free(response->name);
    g_free(response->next_marker);
    g_free(response->prefix);

    g_free(response);
}

void ds3_free_service_response(ds3_get_service_response * response){
    size_t num_buckets;
    int i;

    if(response == NULL) {
        return;
    }

    num_buckets = response->num_buckets;

    for(i = 0; i<num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        g_free(bucket.name);
        g_free(bucket.creation_date);
    }
    ds3_free_owner(response->owner);
    g_free(response->buckets);
    g_free(response);
}

void ds3_free_bulk_response(ds3_bulk_response * response) {
    if(response == NULL) {
        fprintf(stderr, "Bulk response was NULL\n");
        return;
    }

    if(response->job_id != NULL) {
        g_free(response->job_id);
    }

    g_free(response);
}

void ds3_free_bucket(ds3_bucket * bucket) {
    if(bucket == NULL) {
        fprintf(stderr, "Bucket was NULL\n");
        return;
    }
    if(bucket->name != NULL) {
        g_free(bucket->name);
    }
    if(bucket->creation_date != NULL) {
        g_free(bucket->creation_date);
    }
    g_free(bucket);
}

void ds3_free_owner(ds3_owner * owner) {
    if(owner == NULL) {
        fprintf(stderr, "Owner was NULL\n");
        return;
    }
    if(owner->name != NULL) {
        g_free(owner->name);
    }
    if(owner->id != NULL) {
        g_free(owner->id);
    }
    g_free(owner);
}

void ds3_free_creds(ds3_creds * creds) {
    if(creds == NULL) {
        return;
    }

    if(creds->access_id != NULL) {
        g_free(creds->access_id);
    }

    if(creds->secret_key != NULL) {
        g_free(creds->secret_key);
    }
    g_free(creds);
}

void ds3_free_client(ds3_client * client) {
    if(client == NULL) {
      return;
    }
    if(client->endpoint != NULL) {
        g_free(client->endpoint);
    }
    if(client->proxy != NULL) {
        g_free(client->proxy);
    }
    g_free(client);
}

void ds3_free_request(ds3_request * _request) {
    struct _ds3_request * request; 
    if(_request == NULL) {
        return;
    }
    request = (struct _ds3_request *) _request;
    if(request->path != NULL) {
        g_free(request->path);
    }
    if(request->headers != NULL) {
        g_hash_table_destroy(request->headers);
    }
    if(request->query_params != NULL) {
        g_hash_table_destroy(request->query_params);
    }
    g_free(request);
}

void ds3_cleanup(void) {
    net_cleanup();
}

size_t ds3_write_to_file(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return fwrite(buffer, size, nmemb, (FILE *) user_data);
}

size_t ds3_read_from_file(void* buffer, size_t size, size_t nmemb, void* user_data) {
    return fread(buffer, size, nmemb, (FILE *) user_data);
}

static ds3_bulk_object _ds3_bulk_object_from_file(const char * file_name) {

    struct stat file_info;
    memset(&file_info, 0, sizeof(struct stat));

    int result = stat(file_name, &file_info);
    if (result != 0) {
        fprintf(stderr, "Failed to get file info for %s\n", file_name);
    }

    ds3_bulk_object obj;
    memset(&obj, 0, sizeof(ds3_bulk_object));

    obj.name = g_strdup(file_name);
    obj.name_size = strlen(file_name);
    obj.size = file_info.st_size;

    return obj;
}

ds3_bulk_object_list * ds3_convert_file_list(const char** file_list, uint64_t num_files) {
    uint64_t i;
    ds3_bulk_object_list* obj_list = g_new0(ds3_bulk_object_list, 1);
    obj_list->size = num_files;
    obj_list->list = g_new0(ds3_bulk_object, num_files);
    
    for(i = 0; i < num_files; i++) {
        obj_list->list[i] = _ds3_bulk_object_from_file(file_list[i]);
    }

    return obj_list;
}

void ds3_free_bulk_object_list(ds3_bulk_object_list* object_list) {
    uint64_t i, count;
    if(object_list == NULL) {
        return;
    }
    count = object_list->size;
    for(i = 0; i < count; i++) {
        char * file_name = object_list->list[i].name;
        if (file_name == NULL) {
            continue;
        }
        g_free(file_name);
    }

    g_free(object_list->list);
    g_free(object_list);
}
