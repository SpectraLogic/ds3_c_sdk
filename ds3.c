#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#include "ds3.h"
#include "util.h"
#include "net.h"

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

static _ds3_request * _common_request_init(void){
    _ds3_request * request = g_new0(_ds3_request, 1);
    request->headers = _create_hash_table();
    request->query_params = _create_hash_table();
    return request;
}

ds3_request * ds3_init_get_service(void) {
    _ds3_request * request = _common_request_init(); 
    request->verb = GET;
    request->path =  g_new0(char, 2);
    request->path [0] = '/';
    return (ds3_request *) request;
}

ds3_request * ds3_init_get_bucket(const char * bucket_name) {
    _ds3_request * request = _common_request_init(); 
    request->verb = GET;
    request->path = g_strconcat("/", bucket_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_get_object(const char *bucket_name, const char *object_name) {
    _ds3_request * request = _common_request_init();
    request->verb = GET;
    request->path = g_strconcat("/", bucket_name, "/", object_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_delete_object(const char *bucket_name, const char *object_name) {
    _ds3_request * request = _common_request_init();
    request->verb = DELETE;
    request->path = g_strconcat("/", bucket_name, "/", object_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_put_object(const char *bucket_name, const char *object_name, uint64_t length) {
    _ds3_request * request = _common_request_init();
    request->verb = PUT;
    request->path = g_strconcat("/", bucket_name, "/", object_name, NULL);
    request->length = length;
    return (ds3_request *) request;
}

ds3_request * ds3_init_put_bucket(const char * bucket_name) {
    _ds3_request * request = _common_request_init();
    request->verb = PUT;
    request->path = g_strconcat("/", bucket_name, NULL);
    return (ds3_request *) request;
}

ds3_request * ds3_init_delete_bucket(const char * bucket_name) {
    _ds3_request * request = _common_request_init();
    request->verb = DELETE;
    request->path = g_strconcat("/", bucket_name, NULL);
    return (ds3_request *) request;
}

static void _internal_request_dispatcher(const ds3_client * client, const ds3_request * request,void * user_struct, size_t (*write_data)(void*, size_t, size_t, void*)) {
    if(client == NULL || request == NULL) {
        fprintf(stderr, "All arguments must be filled in\n");
        return;
    }
    net_process_request(client, request, user_struct, write_data);
}

static size_t load_xml_buff(void* contents, size_t size, size_t nmemb, void *user_data) {
    size_t realsize = size * nmemb;
    GByteArray* blob = (GByteArray*) user_data;
    
    g_byte_array_append(blob, contents, realsize);
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
                bucket.creation_date = g_strdup(text);
                bucket.creation_date_size = strlen(text);
                xmlFree(text);
            }else if(xmlStrcmp(data_ptr->name, (const xmlChar *) "Name") == 0) {
                text = xmlNodeListGetString(doc, data_ptr->xmlChildrenNode, 1);
                bucket.name= g_strdup(text);
                bucket.name_size= strlen(text);
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
            owner->name = g_strdup(text);
            owner->name_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "ID") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            owner->id = g_strdup(text);
            owner->id_size = strlen(text);
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
    
    _internal_request_dispatcher(client, request, xml_blob, load_xml_buff);
   
    doc = xmlParseMemory(xml_blob->data, xml_blob->len);

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
            object.name = g_strdup(text);
            object.name_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "ETag") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.etag= g_strdup(text);
            object.etag_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "LastModified") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.last_modified = g_strdup(text);
            object.last_modified_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "StorageClass") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            object.storage_class = g_strdup(text);
            object.storage_class_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Size") == 0) { 
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            uint64_t size = strtoul(text, NULL, 10);
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
    _internal_request_dispatcher(client, request, xml_blob, load_xml_buff);
    
    doc = xmlParseMemory(xml_blob->data, xml_blob->len);
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
            response->creation_date = g_strdup(text);
            response->creation_date_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "IsTruncated") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(strncmp(text, "true", 4) == 0) {
                response->is_truncated = true; 
            }
            else {
                response->is_truncated = false; 
            }
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Marker") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->marker= g_strdup(text);
            response->marker_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "MaxKeys") == 0) {
            uint64_t max_keys; 
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            max_keys = strtoul(text, NULL, 10); 
            response->max_keys = max_keys;
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Name") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            response->name = g_strdup(text);
            response->name_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Delimiter") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->delimiter= g_strdup(text);
            response->delimiter_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "NextMarker") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->next_marker= g_strdup(text);
            response->next_marker_size = strlen(text);
            xmlFree(text);
        }
        else if(xmlStrcmp(child_node->name, (const xmlChar *) "Prefix") == 0) {
            text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
            if(text == NULL) {
                continue;
            }
            response->prefix = g_strdup(text);
            response->prefix_size = strlen(text);
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
    _internal_request_dispatcher(client, request, user_data, callback);
}

void ds3_put_object(const ds3_client * client, const ds3_request * request, void * user_data, size_t (* callback)(void *, size_t, size_t, void *)) {
    _internal_request_dispatcher(client, request, user_data, callback);
}

void ds3_delete_object(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request, NULL, NULL);
}

void ds3_put_bucket(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request, NULL, NULL);
}

void ds3_delete_bucket(const ds3_client * client, const ds3_request * request) {
    _internal_request_dispatcher(client, request, NULL, NULL);
}

void ds3_print_request(const ds3_request * _request) {
    const _ds3_request * request; 
    if(_request == NULL) {
      fprintf(stderr, "Request object was null\n");
      return;
    }
    request = (_ds3_request *)_request;
    printf("Verb: %s\n", net_get_verb(request->verb));
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
    _ds3_request * request; 
    if(_request == NULL) {
        return;
    }
    request = (_ds3_request *) _request;
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
