/*
 * ******************************************************************************
 *   Copyright 2014-2017 Spectra Logic Corporation. All Rights Reserved.
 *   Licensed under the Apache License, Version 2.0 (the "License"). You may not use
 *   this file except in compliance with the License. A copy of the License is located at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   or in the "license" file accompanying this file.
 *   This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 *   CONDITIONS OF ANY KIND, either express or implied. See the License for the
 *   specific language governing permissions and limitations under the License.
 * ****************************************************************************
 */

/* This Code is Auto-Generated; DO NOT MODIFY! */


#include <glib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libxml/parser.h>

#include "ds3.h"
#include "ds3_net.h"
#include "ds3_request.h"
#include "ds3_response_header_utils.h"
#include "ds3_string_multimap_impl.h"
#include "ds3_uint64_string_map.h"
#include "ds3_utils.h"

#ifdef _WIN32
  #include <io.h>
  #ifndef PRIu64
    #define PRIu64 "I64u"
  #endif
#else
  #include <inttypes.h>
#endif


//The max size of an uint32_t is 10 characters + NULL
//The max size of an uint64_t is 20 characters + NULL
#define STRING_BUFFER_SIZE 32


struct _ds3_metadata {
    GHashTable* metadata;
};

static void _ds3_metadata_entry_free(gpointer pointer) {
    ds3_metadata_entry* entry;
    if (pointer == NULL) {
        return; // do nothing
    }

    entry = (ds3_metadata_entry*) pointer;

    ds3_metadata_entry_free(entry);
}

/*
 * This copies all the header values in the ds3_string_multimap_entry struct so that they may be safely returned to the user
 * without having to worry about if the data is freed internally.
 */
static const char* METADATA_PREFIX = "x-amz-meta-";
static ds3_metadata_entry* ds3_metadata_entry_init(ds3_string_multimap_entry* header_entry) {
    guint i;
    ds3_str* header_value;
    GPtrArray* values = g_ptr_array_new();
    ds3_str* key_name;
    ds3_str* full_key;
    ds3_metadata_entry* response = g_new0(ds3_metadata_entry, 1);
    int metadata_prefix_length = strlen(METADATA_PREFIX);

    unsigned int num_values = ds3_string_multimap_entry_get_num_values(header_entry);
    for (i = 0; i < num_values; i++) {
        header_value = ds3_string_multimap_entry_get_value_by_index(header_entry, i);
        g_ptr_array_add(values, header_value);
    }

    full_key = ds3_string_multimap_entry_get_key(header_entry);
    key_name = ds3_str_init(full_key->value + metadata_prefix_length);
    ds3_str_free(full_key);

    response->num_values = num_values;
    response->name = key_name;
    response->values = (ds3_str**) g_ptr_array_free(values, FALSE);
    return response;
}

/* The headers hash table contains all the response headers which have the following types:
 * Key - char*
 * Value - ds3_response_header
 *
 * All values should be copied from the struct to avoid memory issues
 */
static ds3_metadata* _init_metadata(ds3_string_multimap* response_headers) {
    struct _ds3_metadata* metadata = g_new0(struct _ds3_metadata, 1);
    GHashTableIter iter;
    gpointer _key, _value;
    ds3_str* key;
    ds3_metadata_entry* entry;
    metadata->metadata = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, _ds3_metadata_entry_free);

    if (response_headers == NULL) {
        fprintf(stderr, "response headers was null\n");
    }

    g_hash_table_iter_init(&iter, ds3_string_multimap_get_hashtable(response_headers));
    while(g_hash_table_iter_next(&iter, &_key, &_value)) {
        key = (ds3_str*) _key;
        if (g_str_has_prefix(key->value, "x-amz-meta-")) {
            ds3_string_multimap_entry* mm_entry = ds3_string_multimap_lookup(response_headers, key);
            entry = ds3_metadata_entry_init(mm_entry);
            g_hash_table_insert(metadata->metadata, g_strdup(entry->name->value), entry);
            ds3_string_multimap_entry_free(mm_entry);
        }
    }

    return (ds3_metadata*) metadata;
}

ds3_metadata_entry* ds3_metadata_get_entry(const ds3_metadata* _metadata, const char* name) {
    ds3_metadata_entry* copy;
    ds3_metadata_entry* orig;
    ds3_str** metadata_copy;
    uint64_t i;
    struct _ds3_metadata* metadata = (struct _ds3_metadata*) _metadata;

    if (_metadata == NULL) {
        return NULL;
    }

    orig = (ds3_metadata_entry*) g_hash_table_lookup(metadata->metadata, name);
    if (orig == NULL) {
        return NULL;
    }
    copy = g_new0(ds3_metadata_entry, 1);
    metadata_copy = g_new0(ds3_str*, (gsize)orig->num_values);

    for (i = 0; i < orig->num_values; i++) {
        metadata_copy[i] = ds3_str_dup(orig->values[i]);
    }

    copy->num_values = orig->num_values;
    copy->name = ds3_str_dup(orig->name);
    copy->values = metadata_copy;

    return copy;
}

unsigned int ds3_metadata_size(const ds3_metadata* _metadata) {
    struct _ds3_metadata* metadata = (struct _ds3_metadata*) _metadata;
    if (metadata == NULL) {
        return 0;
    }
    return g_hash_table_size(metadata->metadata);
}

ds3_metadata_keys_result* ds3_metadata_keys(const ds3_metadata* _metadata) {
    GPtrArray* return_keys;
    ds3_metadata_keys_result* result;
    struct _ds3_metadata* metadata;
    GList* keys;
    GList* tmp_key;

    if (_metadata == NULL) {
        return NULL;
    }

    return_keys = g_ptr_array_new();
    result = g_new0(ds3_metadata_keys_result, 1);
    metadata = (struct _ds3_metadata*) _metadata;
    keys = g_hash_table_get_keys(metadata->metadata);
    tmp_key = keys;

    while(tmp_key != NULL) {
        g_ptr_array_add(return_keys, ds3_str_init((char*)tmp_key->data));
        tmp_key = tmp_key->next;
    }

    g_list_free(keys);
    result->num_keys = return_keys->len;
    result->keys = (ds3_str**) g_ptr_array_free(return_keys, FALSE);
    return result;
}

void ds3_metadata_free(ds3_metadata* _metadata) {
    struct _ds3_metadata* metadata;
    if (_metadata == NULL) return;

    metadata = (struct _ds3_metadata*) _metadata;

    if (metadata->metadata == NULL) return;

    g_hash_table_destroy(metadata->metadata);

    g_free(metadata);
}

void ds3_metadata_entry_free(ds3_metadata_entry* entry) {
    if (entry == NULL) {
        return;
    }

    int value_index;
    ds3_str* value;
    if (entry->name != NULL) {
        ds3_str_free(entry->name);
    }

    if (entry->values != NULL) {
        for (value_index = 0; value_index < entry->num_values; value_index++) {
            value = entry->values[value_index];
            ds3_str_free(value);
        }
        g_free(entry->values);
    }
    g_free(entry);
}

void ds3_metadata_keys_free(ds3_metadata_keys_result* metadata_keys) {
    uint64_t key_index;
    if (metadata_keys == NULL) {
        return;
    }

    if (metadata_keys->keys != NULL) {
        for (key_index = 0; key_index < metadata_keys->num_keys; key_index++) {
            ds3_str_free(metadata_keys->keys[key_index]);
        }
        g_free(metadata_keys->keys);
    }
    g_free(metadata_keys);
}
static bool attribute_equal(const struct _xmlAttr* attribute, const char* attribute_name) {
    return xmlStrcmp(attribute->name, (const xmlChar*) attribute_name) == 0;
}

static bool element_equal(const xmlNodePtr xml_node, const char* element_name) {
    return xmlStrcmp(xml_node->name, (const xmlChar*) element_name) == 0;
}

static uint16_t xml_get_uint16(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    uint16_t size;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    if (text == NULL) {
        return 0;
    }
    size = atoi((char*)text);
    xmlFree(text);
    return size;
}

static uint64_t xml_get_uint16_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_uint16(doc, (xmlNodePtr) attribute);
}

static uint64_t xml_get_uint64(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    uint64_t size;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    if (text == NULL) {
        return 0;
    }
    size = g_ascii_strtoull((const char*)text, NULL, 10);
    xmlFree(text);
    return size;
}

static uint64_t xml_get_uint64_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_uint64(doc, (xmlNodePtr) attribute);
}

static ds3_str* xml_get_string(xmlDocPtr doc, xmlNodePtr child_node) {
    xmlChar* text;
    ds3_str* result;
    text = xmlNodeListGetString(doc, child_node->xmlChildrenNode, 1);
    if (text == NULL) {
        // Element is found, but is empty: <name />
        return NULL;
    }
    result = ds3_str_init((const char*) text);
    xmlFree(text);
    return result;
}

static ds3_str* xml_get_string_from_attribute(xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_string(doc, (xmlNodePtr) attribute);
}

static ds3_bool xml_get_bool(const ds3_log* log, xmlDocPtr doc, const xmlNodePtr xml_node) {
    xmlChar* text;
    ds3_bool result;
    text = xmlNodeListGetString(doc, xml_node->xmlChildrenNode, 1);
    if (xmlStrcmp(text, (xmlChar*)"true") == 0) {
        result = True;
    } else if (xmlStrcmp(text, (xmlChar*)"false") == 0) {
        result = False;
    } else {
        ds3_log_message(log, DS3_ERROR, "Unknown boolean value");
        result = False;
    }
    xmlFree(text);
    return result;
}

static ds3_bool xml_get_bool_from_attribute(const ds3_log* log, xmlDocPtr doc, struct _xmlAttr* attribute) {
    return xml_get_bool(log, doc, (xmlNodePtr) attribute);
}

typedef struct {
    char* buff;
    size_t size;
    size_t total_read;
}ds3_xml_send_buff;


static size_t _ds3_send_xml_buff(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t to_read;
    size_t remaining;
    ds3_xml_send_buff* xml_buff;

    xml_buff = (ds3_xml_send_buff*) user_data;
    to_read = size * nmemb;
    remaining = xml_buff->size - xml_buff->total_read;

    if (remaining < to_read) {
        to_read = remaining;
    }

    strncpy((char*)buffer, xml_buff->buff + xml_buff->total_read, to_read);
    xml_buff->total_read += to_read;
    return to_read;
}

typedef enum {
    BULK_PUT,
    BULK_GET,
    BULK_DELETE,
    GET_PHYSICAL_PLACEMENT,
    COMPLETE_MPU,
    STRING,
    STRING_LIST,
    DATA
}object_list_type;

static ds3_error* _internal_request_dispatcher(
        const ds3_client* client,
        const ds3_request* request,
        void* read_user_struct,
        size_t (*read_handler_func)(void*, size_t, size_t, void*),
        void* write_user_struct,
        size_t (*write_handler_func)(void*, size_t, size_t, void*),
        ds3_string_multimap** return_headers) {
    if (client == NULL || request == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }
    return net_process_request(client, request, read_user_struct, read_handler_func, write_user_struct, write_handler_func, return_headers);
}

static int num_chars_in_ds3_str(const ds3_str *const str, char ch) {
    int num_matches = 0;
    size_t index;

    for (index = 0; index < str->size; index++) {
        if (str->value[index] == '/') {
            num_matches++;
        }
    }

    return num_matches;
}

static ds3_error* _get_request_xml_nodes(
        GByteArray* xml_blob,
        xmlDocPtr* _doc,
        xmlNodePtr* _root,
        char* root_element_name) {
    xmlNodePtr root;

    xmlDocPtr doc = xmlParseMemory((const char*) xml_blob->data, xml_blob->len);
    if (doc == NULL) {
        char* message = g_strconcat("Failed to parse response document.  The actual response is: ", xml_blob->data, NULL);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    root = xmlDocGetRootElement(doc);
    if (element_equal(root, root_element_name) == false) {
        char* message = g_strconcat("Expected the root element to be '", root_element_name, "'.  The actual response is: ", xml_blob->data, NULL);
        xmlFreeDoc(doc);
        g_byte_array_free(xml_blob, TRUE);
        ds3_error* error = ds3_create_error(DS3_ERROR_INVALID_XML, message);
        g_free(message);
        return error;
    }

    *_doc = doc;
    *_root = root;

    g_byte_array_free(xml_blob, TRUE);
    return NULL;
}

static xmlDocPtr _generate_xml_bulk_objects_list(const ds3_bulk_object_list_response* obj_list, object_list_type list_type, ds3_job_chunk_client_processing_order_guarantee order) {
    char size_buff[STRING_BUFFER_SIZE];
    xmlDocPtr doc;
    ds3_bulk_object_response* obj;
    xmlNodePtr objects_node, object_node;
    size_t obj_index;

    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    objects_node = xmlNewNode(NULL, (xmlChar*) "Objects");

    if (list_type == BULK_GET) {
        if (order == DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE) {
            xmlSetProp(objects_node, (xmlChar*) "ChunkClientProcessingOrderGuarantee", (const xmlChar *) "NONE");
        } else if (order == DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_IN_ORDER) {
            xmlSetProp(objects_node, (xmlChar*) "ChunkClientProcessingOrderGuarantee", (const xmlChar *) "IN_ORDER");
        } else {
            return NULL;
        }
    }


    for (obj_index = 0; obj_index < obj_list->num_objects; obj_index++) {
        obj = obj_list->objects[obj_index];
        memset(size_buff, 0, sizeof(size_buff));
        g_snprintf(size_buff, STRING_BUFFER_SIZE, "%" PRIu64, obj->length);

        object_node = xmlNewNode(NULL, (xmlChar*) "Object");
        xmlAddChild(objects_node, object_node);

        xmlSetProp(object_node, (xmlChar*) "Name", (xmlChar*) obj->name->value);
        if (list_type == BULK_PUT) {
            xmlSetProp(object_node, (xmlChar*) "Size", (xmlChar*) size_buff);
        }
    }

    xmlDocSetRootElement(doc, objects_node);

    return doc;
}

static xmlDocPtr _generate_xml_complete_mpu(const ds3_complete_multipart_upload_response* mpu_list) {
    char size_buff[STRING_BUFFER_SIZE];
    xmlDocPtr doc;
    ds3_multipart_upload_part_response* part;
    xmlNodePtr parts_node, part_node;
    int part_num;

    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    parts_node = xmlNewNode(NULL, (xmlChar*) "CompleteMultipartUpload");

    for (part_num = 0; part_num < mpu_list->num_parts; part_num++) {
        part = mpu_list->parts[part_num];

        part_node = xmlNewNode(NULL, (xmlChar*) "Part");
        xmlAddChild(parts_node, part_node);

        g_snprintf(size_buff, STRING_BUFFER_SIZE, "%d", part->part_number);
        xmlNewTextChild(part_node, NULL, (xmlChar*) "PartNumber", (xmlChar*) size_buff);

        xmlNewTextChild(part_node, NULL, (xmlChar*) "ETag", (xmlChar*) part->etag->value);
    }

    xmlDocSetRootElement(doc, parts_node);
    return doc;
}

static xmlDocPtr _generate_xml_delete_objects(ds3_delete_objects_response* keys_list) {
    xmlDocPtr doc;
    ds3_str* key;
    xmlNodePtr del_node, obj_node;
    int key_num;

    // Start creating the xml body to send to the server.
    doc = xmlNewDoc((xmlChar*)"1.0");
    del_node = xmlNewNode(NULL, (xmlChar*) "Delete");

    for (key_num = 0; key_num < keys_list->num_strings; key_num++) {
        key = keys_list->strings_list[key_num];

        obj_node = xmlNewNode(NULL, (xmlChar*) "Object");
        xmlAddChild(del_node, obj_node);

        xmlNewTextChild(obj_node, NULL, (xmlChar*) "Key", (xmlChar*) key->value);
    }

    xmlDocSetRootElement(doc, del_node);
    return doc;
}

static ds3_error* _init_request_payload(const ds3_request* _request,
                                        ds3_xml_send_buff* send_buff,
                                        const object_list_type operation_type) {
    xmlDocPtr doc;

    struct _ds3_request* request = (struct _ds3_request*) _request;

    // Clear send_buff
    memset(send_buff, 0, sizeof(ds3_xml_send_buff));

    switch(operation_type) {
        case BULK_PUT:
        case BULK_GET:
        case GET_PHYSICAL_PLACEMENT:
            if (request->object_list == NULL || request->object_list->num_objects == 0) {
                return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bulk command requires a list of objects to process");
            }
            doc = _generate_xml_bulk_objects_list(request->object_list, operation_type, request->chunk_ordering);
            break;

        case COMPLETE_MPU:
            if (request->mpu_list == NULL || request->mpu_list->num_parts == 0) {
                return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The complete multipart upload command requires a list of objects to process");
            }
            doc = _generate_xml_complete_mpu(request->mpu_list);
            break;

        case BULK_DELETE:
        case STRING_LIST:
            if (request->delete_objects == NULL || request->delete_objects->num_strings == 0) {
                return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The delete objects command requires a list of objects to process");
            }
            doc = _generate_xml_delete_objects(request->delete_objects);
            break;

        case STRING: // *** not XML - do not interpret
            send_buff->buff = request->delete_objects->strings_list[0]->value;
            send_buff->size = request->delete_objects->strings_list[0]->size;
            request->length = send_buff->size;
            return NULL;
            break;

        default:
            return ds3_create_error(DS3_ERROR_INVALID_XML, "Unknown request payload type");
    }

    xmlDocDumpFormatMemory(doc, (xmlChar**) &send_buff->buff, (int*) &send_buff->size, 1);
    request->length = send_buff->size; // make sure to set the size of the request.

    xmlFreeDoc(doc);

    return NULL;
}


static ds3_auto_inspect_mode _match_ds3_auto_inspect_mode(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NEVER") == 0) {
        return DS3_AUTO_INSPECT_MODE_NEVER;
    } else if (xmlStrcmp(text, (const xmlChar*) "MINIMAL") == 0) {
        return DS3_AUTO_INSPECT_MODE_MINIMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "FULL") == 0) {
        return DS3_AUTO_INSPECT_MODE_FULL;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_AUTO_INSPECT_MODE_NEVER for safety.", text);
        return DS3_AUTO_INSPECT_MODE_NEVER;
    }
}
static ds3_priority _match_ds3_priority(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CRITICAL") == 0) {
        return DS3_PRIORITY_CRITICAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "URGENT") == 0) {
        return DS3_PRIORITY_URGENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "HIGH") == 0) {
        return DS3_PRIORITY_HIGH;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_PRIORITY_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOW") == 0) {
        return DS3_PRIORITY_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "BACKGROUND") == 0) {
        return DS3_PRIORITY_BACKGROUND;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_PRIORITY_CRITICAL for safety.", text);
        return DS3_PRIORITY_CRITICAL;
    }
}
static ds3_bucket_acl_permission _match_ds3_bucket_acl_permission(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "LIST") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_LIST;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_READ;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_WRITE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DELETE") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_DELETE;
    } else if (xmlStrcmp(text, (const xmlChar*) "JOB") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_JOB;
    } else if (xmlStrcmp(text, (const xmlChar*) "OWNER") == 0) {
        return DS3_BUCKET_ACL_PERMISSION_OWNER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_BUCKET_ACL_PERMISSION_LIST for safety.", text);
        return DS3_BUCKET_ACL_PERMISSION_LIST;
    }
}
static ds3_data_isolation_level _match_ds3_data_isolation_level(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "STANDARD") == 0) {
        return DS3_DATA_ISOLATION_LEVEL_STANDARD;
    } else if (xmlStrcmp(text, (const xmlChar*) "BUCKET_ISOLATED") == 0) {
        return DS3_DATA_ISOLATION_LEVEL_BUCKET_ISOLATED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_ISOLATION_LEVEL_STANDARD for safety.", text);
        return DS3_DATA_ISOLATION_LEVEL_STANDARD;
    }
}
static ds3_data_persistence_rule_type _match_ds3_data_persistence_rule_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PERMANENT") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "TEMPORARY") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_TYPE_TEMPORARY;
    } else if (xmlStrcmp(text, (const xmlChar*) "RETIRED") == 0) {
        return DS3_DATA_PERSISTENCE_RULE_TYPE_RETIRED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT for safety.", text);
        return DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT;
    }
}
static ds3_data_placement_rule_state _match_ds3_data_placement_rule_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_DATA_PLACEMENT_RULE_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "INCLUSION_IN_PROGRESS") == 0) {
        return DS3_DATA_PLACEMENT_RULE_STATE_INCLUSION_IN_PROGRESS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_PLACEMENT_RULE_STATE_NORMAL for safety.", text);
        return DS3_DATA_PLACEMENT_RULE_STATE_NORMAL;
    }
}
static ds3_data_replication_rule_type _match_ds3_data_replication_rule_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PERMANENT") == 0) {
        return DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "RETIRED") == 0) {
        return DS3_DATA_REPLICATION_RULE_TYPE_RETIRED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT for safety.", text);
        return DS3_DATA_REPLICATION_RULE_TYPE_PERMANENT;
    }
}
static ds3_feature_key_type _match_ds3_feature_key_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "AWS_S3_CLOUD_OUT") == 0) {
        return DS3_FEATURE_KEY_TYPE_AWS_S3_CLOUD_OUT;
    } else if (xmlStrcmp(text, (const xmlChar*) "MICROSOFT_AZURE_CLOUD_OUT") == 0) {
        return DS3_FEATURE_KEY_TYPE_MICROSOFT_AZURE_CLOUD_OUT;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_FEATURE_KEY_TYPE_AWS_S3_CLOUD_OUT for safety.", text);
        return DS3_FEATURE_KEY_TYPE_AWS_S3_CLOUD_OUT;
    }
}
static ds3_job_chunk_blob_store_state _match_ds3_job_chunk_blob_store_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PENDING") == 0) {
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_PROGRESS") == 0) {
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "COMPLETED") == 0) {
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_COMPLETED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_CHUNK_BLOB_STORE_STATE_PENDING for safety.", text);
        return DS3_JOB_CHUNK_BLOB_STORE_STATE_PENDING;
    }
}
static ds3_job_chunk_client_processing_order_guarantee _match_ds3_job_chunk_client_processing_order_guarantee(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_ORDER") == 0) {
        return DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_IN_ORDER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE for safety.", text);
        return DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    }
}
static ds3_job_request_type _match_ds3_job_request_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "PUT") == 0) {
        return DS3_JOB_REQUEST_TYPE_PUT;
    } else if (xmlStrcmp(text, (const xmlChar*) "GET") == 0) {
        return DS3_JOB_REQUEST_TYPE_GET;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY") == 0) {
        return DS3_JOB_REQUEST_TYPE_VERIFY;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_REQUEST_TYPE_PUT for safety.", text);
        return DS3_JOB_REQUEST_TYPE_PUT;
    }
}
static ds3_ltfs_file_naming_mode _match_ds3_ltfs_file_naming_mode(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "OBJECT_NAME") == 0) {
        return DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME;
    } else if (xmlStrcmp(text, (const xmlChar*) "OBJECT_ID") == 0) {
        return DS3_LTFS_FILE_NAMING_MODE_OBJECT_ID;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME for safety.", text);
        return DS3_LTFS_FILE_NAMING_MODE_OBJECT_NAME;
    }
}
static ds3_s3_initial_data_placement_policy _match_ds3_s3_initial_data_placement_policy(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "STANDARD") == 0) {
        return DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_STANDARD;
    } else if (xmlStrcmp(text, (const xmlChar*) "REDUCED_REDUNDANCY") == 0) {
        return DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_REDUCED_REDUNDANCY;
    } else if (xmlStrcmp(text, (const xmlChar*) "STANDARD_IA") == 0) {
        return DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_STANDARD_IA;
    } else if (xmlStrcmp(text, (const xmlChar*) "GLACIER") == 0) {
        return DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_GLACIER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_STANDARD for safety.", text);
        return DS3_S3_INITIAL_DATA_PLACEMENT_POLICY_STANDARD;
    }
}
static ds3_s3_object_type _match_ds3_s3_object_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "DATA") == 0) {
        return DS3_S3_OBJECT_TYPE_DATA;
    } else if (xmlStrcmp(text, (const xmlChar*) "FOLDER") == 0) {
        return DS3_S3_OBJECT_TYPE_FOLDER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_S3_OBJECT_TYPE_DATA for safety.", text);
        return DS3_S3_OBJECT_TYPE_DATA;
    }
}
static ds3_s3_region _match_ds3_s3_region(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "GOV_CLOUD") == 0) {
        return DS3_S3_REGION_GOV_CLOUD;
    } else if (xmlStrcmp(text, (const xmlChar*) "US_EAST_1") == 0) {
        return DS3_S3_REGION_US_EAST_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "US_WEST_1") == 0) {
        return DS3_S3_REGION_US_WEST_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "US_WEST_2") == 0) {
        return DS3_S3_REGION_US_WEST_2;
    } else if (xmlStrcmp(text, (const xmlChar*) "EU_WEST_1") == 0) {
        return DS3_S3_REGION_EU_WEST_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "EU_CENTRAL_1") == 0) {
        return DS3_S3_REGION_EU_CENTRAL_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "AP_SOUTH_1") == 0) {
        return DS3_S3_REGION_AP_SOUTH_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "AP_SOUTHEAST_1") == 0) {
        return DS3_S3_REGION_AP_SOUTHEAST_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "AP_SOUTHEAST_2") == 0) {
        return DS3_S3_REGION_AP_SOUTHEAST_2;
    } else if (xmlStrcmp(text, (const xmlChar*) "AP_NORTHEAST_1") == 0) {
        return DS3_S3_REGION_AP_NORTHEAST_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "AP_NORTHEAST_2") == 0) {
        return DS3_S3_REGION_AP_NORTHEAST_2;
    } else if (xmlStrcmp(text, (const xmlChar*) "SA_EAST_1") == 0) {
        return DS3_S3_REGION_SA_EAST_1;
    } else if (xmlStrcmp(text, (const xmlChar*) "CN_NORTH_1") == 0) {
        return DS3_S3_REGION_CN_NORTH_1;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_S3_REGION_GOV_CLOUD for safety.", text);
        return DS3_S3_REGION_GOV_CLOUD;
    }
}
static ds3_storage_domain_failure_type _match_ds3_storage_domain_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ILLEGAL_EJECTION_OCCURRED") == 0) {
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "MEMBER_BECAME_READ_ONLY") == 0) {
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_MEMBER_BECAME_READ_ONLY;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITES_STALLED_DUE_TO_NO_FREE_MEDIA_REMAINING") == 0) {
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_WRITES_STALLED_DUE_TO_NO_FREE_MEDIA_REMAINING;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED for safety.", text);
        return DS3_STORAGE_DOMAIN_FAILURE_TYPE_ILLEGAL_EJECTION_OCCURRED;
    }
}
static ds3_storage_domain_member_state _match_ds3_storage_domain_member_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "EXCLUSION_IN_PROGRESS") == 0) {
        return DS3_STORAGE_DOMAIN_MEMBER_STATE_EXCLUSION_IN_PROGRESS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL for safety.", text);
        return DS3_STORAGE_DOMAIN_MEMBER_STATE_NORMAL;
    }
}
static ds3_system_failure_type _match_ds3_system_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "RECONCILE_TAPE_ENVIRONMENT_FAILED") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "RECONCILE_POOL_ENVIRONMENT_FAILED") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_RECONCILE_POOL_ENVIRONMENT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_CRITICAL_DATA_VERIFICATION_ERROR_REQUIRES_USER_CONFIRMATION;
    } else if (xmlStrcmp(text, (const xmlChar*) "MICROSOFT_AZURE_WRITES_REQUIRE_FEATURE_LICENSE") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_MICROSOFT_AZURE_WRITES_REQUIRE_FEATURE_LICENSE;
    } else if (xmlStrcmp(text, (const xmlChar*) "AWS_S3_WRITES_REQUIRE_FEATURE_LICENSE") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_AWS_S3_WRITES_REQUIRE_FEATURE_LICENSE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATABASE_RUNNING_OUT_OF_SPACE") == 0) {
        return DS3_SYSTEM_FAILURE_TYPE_DATABASE_RUNNING_OUT_OF_SPACE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED for safety.", text);
        return DS3_SYSTEM_FAILURE_TYPE_RECONCILE_TAPE_ENVIRONMENT_FAILED;
    }
}
static ds3_unavailable_media_usage_policy _match_ds3_unavailable_media_usage_policy(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ALLOW") == 0) {
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "DISCOURAGED") == 0) {
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISCOURAGED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DISALLOW") == 0) {
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_DISALLOW;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW for safety.", text);
        return DS3_UNAVAILABLE_MEDIA_USAGE_POLICY_ALLOW;
    }
}
static ds3_versioning_level _match_ds3_versioning_level(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return DS3_VERSIONING_LEVEL_NONE;
    } else if (xmlStrcmp(text, (const xmlChar*) "KEEP_LATEST") == 0) {
        return DS3_VERSIONING_LEVEL_KEEP_LATEST;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_VERSIONING_LEVEL_NONE for safety.", text);
        return DS3_VERSIONING_LEVEL_NONE;
    }
}
static ds3_write_optimization _match_ds3_write_optimization(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CAPACITY") == 0) {
        return DS3_WRITE_OPTIMIZATION_CAPACITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "PERFORMANCE") == 0) {
        return DS3_WRITE_OPTIMIZATION_PERFORMANCE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_WRITE_OPTIMIZATION_CAPACITY for safety.", text);
        return DS3_WRITE_OPTIMIZATION_CAPACITY;
    }
}
static ds3_write_preference_level _match_ds3_write_preference_level(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "HIGH") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_HIGH;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOW") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "NEVER_SELECT") == 0) {
        return DS3_WRITE_PREFERENCE_LEVEL_NEVER_SELECT;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_WRITE_PREFERENCE_LEVEL_HIGH for safety.", text);
        return DS3_WRITE_PREFERENCE_LEVEL_HIGH;
    }
}
static ds3_pool_failure_type _match_ds3_pool_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "BLOB_READ_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE") == 0) {
        return DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_MISSING") == 0) {
        return DS3_POOL_FAILURE_TYPE_DATA_CHECKPOINT_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_FORMAT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_IMPORT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_INCOMPLETE") == 0) {
        return DS3_POOL_FAILURE_TYPE_IMPORT_INCOMPLETE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE") == 0) {
        return DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_DATA_INTEGRITY") == 0) {
        return DS3_POOL_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "INSPECT_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_INSPECT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "QUIESCED") == 0) {
        return DS3_POOL_FAILURE_TYPE_QUIESCED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_VERIFY_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_FAILED") == 0) {
        return DS3_POOL_FAILURE_TYPE_WRITE_FAILED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED for safety.", text);
        return DS3_POOL_FAILURE_TYPE_BLOB_READ_FAILED;
    }
}
static ds3_pool_health _match_ds3_pool_health(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "OK") == 0) {
        return DS3_POOL_HEALTH_OK;
    } else if (xmlStrcmp(text, (const xmlChar*) "DEGRADED") == 0) {
        return DS3_POOL_HEALTH_DEGRADED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_HEALTH_OK for safety.", text);
        return DS3_POOL_HEALTH_OK;
    }
}
static ds3_pool_state _match_ds3_pool_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "BLANK") == 0) {
        return DS3_POOL_STATE_BLANK;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_POOL_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOST") == 0) {
        return DS3_POOL_STATE_LOST;
    } else if (xmlStrcmp(text, (const xmlChar*) "FOREIGN") == 0) {
        return DS3_POOL_STATE_FOREIGN;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_PENDING") == 0) {
        return DS3_POOL_STATE_IMPORT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_IN_PROGRESS") == 0) {
        return DS3_POOL_STATE_IMPORT_IN_PROGRESS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_STATE_BLANK for safety.", text);
        return DS3_POOL_STATE_BLANK;
    }
}
static ds3_pool_type _match_ds3_pool_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NEARLINE") == 0) {
        return DS3_POOL_TYPE_NEARLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE") == 0) {
        return DS3_POOL_TYPE_ONLINE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_POOL_TYPE_NEARLINE for safety.", text);
        return DS3_POOL_TYPE_NEARLINE;
    }
}
static ds3_import_conflict_resolution_mode _match_ds3_import_conflict_resolution_mode(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CANCEL") == 0) {
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL;
    } else if (xmlStrcmp(text, (const xmlChar*) "ACCEPT_MOST_RECENT") == 0) {
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_MOST_RECENT;
    } else if (xmlStrcmp(text, (const xmlChar*) "ACCEPT_EXISTING") == 0) {
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_ACCEPT_EXISTING;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL for safety.", text);
        return DS3_IMPORT_CONFLICT_RESOLUTION_MODE_CANCEL;
    }
}
static ds3_quiesced _match_ds3_quiesced(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NO") == 0) {
        return DS3_QUIESCED_NO;
    } else if (xmlStrcmp(text, (const xmlChar*) "PENDING") == 0) {
        return DS3_QUIESCED_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "YES") == 0) {
        return DS3_QUIESCED_YES;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_QUIESCED_NO for safety.", text);
        return DS3_QUIESCED_NO;
    }
}
static ds3_reserved_task_type _match_ds3_reserved_task_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ANY") == 0) {
        return DS3_RESERVED_TASK_TYPE_ANY;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ") == 0) {
        return DS3_RESERVED_TASK_TYPE_READ;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE") == 0) {
        return DS3_RESERVED_TASK_TYPE_WRITE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_RESERVED_TASK_TYPE_ANY for safety.", text);
        return DS3_RESERVED_TASK_TYPE_ANY;
    }
}
static ds3_import_export_configuration _match_ds3_import_export_configuration(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "SUPPORTED") == 0) {
        return DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED;
    } else if (xmlStrcmp(text, (const xmlChar*) "NOT_SUPPORTED") == 0) {
        return DS3_IMPORT_EXPORT_CONFIGURATION_NOT_SUPPORTED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED for safety.", text);
        return DS3_IMPORT_EXPORT_CONFIGURATION_SUPPORTED;
    }
}
static ds3_tape_drive_state _match_ds3_tape_drive_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TAPE_DRIVE_STATE_OFFLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_TAPE_DRIVE_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "ERROR") == 0) {
        return DS3_TAPE_DRIVE_STATE_ERROR;
    } else if (xmlStrcmp(text, (const xmlChar*) "NOT_COMPATIBLE_IN_PARTITION_DUE_TO_NEWER_TAPE_DRIVES") == 0) {
        return DS3_TAPE_DRIVE_STATE_NOT_COMPATIBLE_IN_PARTITION_DUE_TO_NEWER_TAPE_DRIVES;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_DRIVE_STATE_OFFLINE for safety.", text);
        return DS3_TAPE_DRIVE_STATE_OFFLINE;
    }
}
static ds3_tape_drive_type _match_ds3_tape_drive_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return DS3_TAPE_DRIVE_TYPE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO5") == 0) {
        return DS3_TAPE_DRIVE_TYPE_LTO5;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO6") == 0) {
        return DS3_TAPE_DRIVE_TYPE_LTO6;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO7") == 0) {
        return DS3_TAPE_DRIVE_TYPE_LTO7;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO8") == 0) {
        return DS3_TAPE_DRIVE_TYPE_LTO8;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS1140") == 0) {
        return DS3_TAPE_DRIVE_TYPE_TS1140;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS1150") == 0) {
        return DS3_TAPE_DRIVE_TYPE_TS1150;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS1155") == 0) {
        return DS3_TAPE_DRIVE_TYPE_TS1155;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_DRIVE_TYPE_UNKNOWN for safety.", text);
        return DS3_TAPE_DRIVE_TYPE_UNKNOWN;
    }
}
static ds3_tape_failure_type _match_ds3_tape_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "BAR_CODE_CHANGED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAR_CODE_DUPLICATE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_BAR_CODE_DUPLICATE;
    } else if (xmlStrcmp(text, (const xmlChar*) "BLOB_READ_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_BLOB_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_MISSING") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DATA_CHECKPOINT_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "DELAYED_OWNERSHIP_FAILURE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DELAYED_OWNERSHIP_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DRIVE_CLEAN_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DRIVE_CLEAN_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DRIVE_CLEANED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_DRIVE_CLEANED;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_FORMAT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "GET_TAPE_INFORMATION_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_GET_TAPE_INFORMATION_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_INCOMPLETE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_IMPORT_INCOMPLETE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE") == 0) {
        return DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_TAKE_OWNERSHIP_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED_DUE_TO_DATA_INTEGRITY") == 0) {
        return DS3_TAPE_FAILURE_TYPE_IMPORT_FAILED_DUE_TO_DATA_INTEGRITY;
    } else if (xmlStrcmp(text, (const xmlChar*) "INSPECT_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_INSPECT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "REIMPORT_REQUIRED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_REIMPORT_REQUIRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "SERIAL_NUMBER_MISMATCH") == 0) {
        return DS3_TAPE_FAILURE_TYPE_SERIAL_NUMBER_MISMATCH;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_VERIFY_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_FAILED") == 0) {
        return DS3_TAPE_FAILURE_TYPE_WRITE_FAILED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED for safety.", text);
        return DS3_TAPE_FAILURE_TYPE_BAR_CODE_CHANGED;
    }
}
static ds3_tape_partition_failure_type _match_ds3_tape_partition_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CLEANING_TAPE_REQUIRED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "DUPLICATE_TAPE_BAR_CODES_DETECTED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_DUPLICATE_TAPE_BAR_CODES_DETECTED;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_STALLED_DUE_TO_OFFLINE_TAPES") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_EJECT_STALLED_DUE_TO_OFFLINE_TAPES;
    } else if (xmlStrcmp(text, (const xmlChar*) "MINIMUM_DRIVE_COUNT_NOT_MET") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_MINIMUM_DRIVE_COUNT_NOT_MET;
    } else if (xmlStrcmp(text, (const xmlChar*) "MOVE_FAILED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "MOVE_FAILED_DUE_TO_PREPARE_TAPE_FOR_REMOVAL_FAILURE") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_MOVE_FAILED_DUE_TO_PREPARE_TAPE_FOR_REMOVAL_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "NO_USABLE_DRIVES") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_NO_USABLE_DRIVES;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_STALLED_DUE_TO_NO_STORAGE_SLOTS") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_ONLINE_STALLED_DUE_TO_NO_STORAGE_SLOTS;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_DRIVE_IN_ERROR") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_IN_ERROR;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_DRIVE_MISSING") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_DRIVE_QUIESCED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_QUIESCED;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_DRIVE_TYPE_MISMATCH") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_DRIVE_TYPE_MISMATCH;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_EJECTION_BY_OPERATOR_REQUIRED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_EJECTION_BY_OPERATOR_REQUIRED;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_MEDIA_TYPE_INCOMPATIBLE") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_MEDIA_TYPE_INCOMPATIBLE;
    } else if (xmlStrcmp(text, (const xmlChar*) "TAPE_REMOVAL_UNEXPECTED") == 0) {
        return DS3_TAPE_PARTITION_FAILURE_TYPE_TAPE_REMOVAL_UNEXPECTED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED for safety.", text);
        return DS3_TAPE_PARTITION_FAILURE_TYPE_CLEANING_TAPE_REQUIRED;
    }
}
static ds3_tape_partition_state _match_ds3_tape_partition_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ONLINE") == 0) {
        return DS3_TAPE_PARTITION_STATE_ONLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TAPE_PARTITION_STATE_OFFLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "ERROR") == 0) {
        return DS3_TAPE_PARTITION_STATE_ERROR;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_PARTITION_STATE_ONLINE for safety.", text);
        return DS3_TAPE_PARTITION_STATE_ONLINE;
    }
}
static ds3_tape_state _match_ds3_tape_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_TAPE_STATE_NORMAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TAPE_STATE_OFFLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_PENDING") == 0) {
        return DS3_TAPE_STATE_ONLINE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "ONLINE_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_ONLINE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "PENDING_INSPECTION") == 0) {
        return DS3_TAPE_STATE_PENDING_INSPECTION;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return DS3_TAPE_STATE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE") == 0) {
        return DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY") == 0) {
        return DS3_TAPE_STATE_DATA_CHECKPOINT_FAILURE_DUE_TO_READ_ONLY;
    } else if (xmlStrcmp(text, (const xmlChar*) "DATA_CHECKPOINT_MISSING") == 0) {
        return DS3_TAPE_STATE_DATA_CHECKPOINT_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTFS_WITH_FOREIGN_DATA") == 0) {
        return DS3_TAPE_STATE_LTFS_WITH_FOREIGN_DATA;
    } else if (xmlStrcmp(text, (const xmlChar*) "RAW_IMPORT_PENDING") == 0) {
        return DS3_TAPE_STATE_RAW_IMPORT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "RAW_IMPORT_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_RAW_IMPORT_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "FOREIGN") == 0) {
        return DS3_TAPE_STATE_FOREIGN;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_PENDING") == 0) {
        return DS3_TAPE_STATE_IMPORT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_IMPORT_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "INCOMPATIBLE") == 0) {
        return DS3_TAPE_STATE_INCOMPATIBLE;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOST") == 0) {
        return DS3_TAPE_STATE_LOST;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAD") == 0) {
        return DS3_TAPE_STATE_BAD;
    } else if (xmlStrcmp(text, (const xmlChar*) "CANNOT_FORMAT_DUE_TO_WRITE_PROTECTION") == 0) {
        return DS3_TAPE_STATE_CANNOT_FORMAT_DUE_TO_WRITE_PROTECTION;
    } else if (xmlStrcmp(text, (const xmlChar*) "SERIAL_NUMBER_MISMATCH") == 0) {
        return DS3_TAPE_STATE_SERIAL_NUMBER_MISMATCH;
    } else if (xmlStrcmp(text, (const xmlChar*) "BAR_CODE_MISSING") == 0) {
        return DS3_TAPE_STATE_BAR_CODE_MISSING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_PENDING") == 0) {
        return DS3_TAPE_STATE_FORMAT_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORMAT_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_FORMAT_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_TO_EE_IN_PROGRESS") == 0) {
        return DS3_TAPE_STATE_EJECT_TO_EE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECT_FROM_EE_PENDING") == 0) {
        return DS3_TAPE_STATE_EJECT_FROM_EE_PENDING;
    } else if (xmlStrcmp(text, (const xmlChar*) "EJECTED") == 0) {
        return DS3_TAPE_STATE_EJECTED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_STATE_NORMAL for safety.", text);
        return DS3_TAPE_STATE_NORMAL;
    }
}
static ds3_tape_type _match_ds3_tape_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "LTO5") == 0) {
        return DS3_TAPE_TYPE_LTO5;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO6") == 0) {
        return DS3_TAPE_TYPE_LTO6;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO7") == 0) {
        return DS3_TAPE_TYPE_LTO7;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTOM8") == 0) {
        return DS3_TAPE_TYPE_LTOM8;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO8") == 0) {
        return DS3_TAPE_TYPE_LTO8;
    } else if (xmlStrcmp(text, (const xmlChar*) "LTO_CLEANING_TAPE") == 0) {
        return DS3_TAPE_TYPE_LTO_CLEANING_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JC") == 0) {
        return DS3_TAPE_TYPE_TS_JC;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JY") == 0) {
        return DS3_TAPE_TYPE_TS_JY;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JK") == 0) {
        return DS3_TAPE_TYPE_TS_JK;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JD") == 0) {
        return DS3_TAPE_TYPE_TS_JD;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JZ") == 0) {
        return DS3_TAPE_TYPE_TS_JZ;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_JL") == 0) {
        return DS3_TAPE_TYPE_TS_JL;
    } else if (xmlStrcmp(text, (const xmlChar*) "TS_CLEANING_TAPE") == 0) {
        return DS3_TAPE_TYPE_TS_CLEANING_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNKNOWN") == 0) {
        return DS3_TAPE_TYPE_UNKNOWN;
    } else if (xmlStrcmp(text, (const xmlChar*) "FORBIDDEN") == 0) {
        return DS3_TAPE_TYPE_FORBIDDEN;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TAPE_TYPE_LTO5 for safety.", text);
        return DS3_TAPE_TYPE_LTO5;
    }
}
static ds3_target_access_control_replication _match_ds3_target_access_control_replication(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NONE") == 0) {
        return DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE;
    } else if (xmlStrcmp(text, (const xmlChar*) "USERS") == 0) {
        return DS3_TARGET_ACCESS_CONTROL_REPLICATION_USERS;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE for safety.", text);
        return DS3_TARGET_ACCESS_CONTROL_REPLICATION_NONE;
    }
}
static ds3_target_failure_type _match_ds3_target_failure_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "IMPORT_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_IMPORT_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IMPORT_INCOMPLETE") == 0) {
        return DS3_TARGET_FAILURE_TYPE_IMPORT_INCOMPLETE;
    } else if (xmlStrcmp(text, (const xmlChar*) "NOT_ONLINE") == 0) {
        return DS3_TARGET_FAILURE_TYPE_NOT_ONLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_WRITE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "WRITE_INITIATE_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_WRITE_INITIATE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_READ_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "READ_INITIATE_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_READ_INITIATE_FAILED;
    } else if (xmlStrcmp(text, (const xmlChar*) "VERIFY_FAILED") == 0) {
        return DS3_TARGET_FAILURE_TYPE_VERIFY_FAILED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_FAILURE_TYPE_IMPORT_FAILED for safety.", text);
        return DS3_TARGET_FAILURE_TYPE_IMPORT_FAILED;
    }
}
static ds3_target_read_preference_type _match_ds3_target_read_preference_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "MINIMUM_LATENCY") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY;
    } else if (xmlStrcmp(text, (const xmlChar*) "AFTER_ONLINE_POOL") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_ONLINE_POOL;
    } else if (xmlStrcmp(text, (const xmlChar*) "AFTER_NEARLINE_POOL") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NEARLINE_POOL;
    } else if (xmlStrcmp(text, (const xmlChar*) "AFTER_NON_EJECTABLE_TAPE") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_AFTER_NON_EJECTABLE_TAPE;
    } else if (xmlStrcmp(text, (const xmlChar*) "LAST_RESORT") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_LAST_RESORT;
    } else if (xmlStrcmp(text, (const xmlChar*) "NEVER") == 0) {
        return DS3_TARGET_READ_PREFERENCE_TYPE_NEVER;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY for safety.", text);
        return DS3_TARGET_READ_PREFERENCE_TYPE_MINIMUM_LATENCY;
    }
}
static ds3_target_state _match_ds3_target_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ONLINE") == 0) {
        return DS3_TARGET_STATE_ONLINE;
    } else if (xmlStrcmp(text, (const xmlChar*) "OFFLINE") == 0) {
        return DS3_TARGET_STATE_OFFLINE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_TARGET_STATE_ONLINE for safety.", text);
        return DS3_TARGET_STATE_ONLINE;
    }
}
static ds3_blob_store_task_state _match_ds3_blob_store_task_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "NOT_READY") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_NOT_READY;
    } else if (xmlStrcmp(text, (const xmlChar*) "READY") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_READY;
    } else if (xmlStrcmp(text, (const xmlChar*) "PENDING_EXECUTION") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_PENDING_EXECUTION;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_PROGRESS") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "COMPLETED") == 0) {
        return DS3_BLOB_STORE_TASK_STATE_COMPLETED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_BLOB_STORE_TASK_STATE_NOT_READY for safety.", text);
        return DS3_BLOB_STORE_TASK_STATE_NOT_READY;
    }
}
static ds3_cache_entry_state _match_ds3_cache_entry_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "ALLOCATED") == 0) {
        return DS3_CACHE_ENTRY_STATE_ALLOCATED;
    } else if (xmlStrcmp(text, (const xmlChar*) "IN_CACHE") == 0) {
        return DS3_CACHE_ENTRY_STATE_IN_CACHE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_CACHE_ENTRY_STATE_ALLOCATED for safety.", text);
        return DS3_CACHE_ENTRY_STATE_ALLOCATED;
    }
}
static ds3_job_status _match_ds3_job_status(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "IN_PROGRESS") == 0) {
        return DS3_JOB_STATUS_IN_PROGRESS;
    } else if (xmlStrcmp(text, (const xmlChar*) "COMPLETED") == 0) {
        return DS3_JOB_STATUS_COMPLETED;
    } else if (xmlStrcmp(text, (const xmlChar*) "CANCELED") == 0) {
        return DS3_JOB_STATUS_CANCELED;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_JOB_STATUS_IN_PROGRESS for safety.", text);
        return DS3_JOB_STATUS_IN_PROGRESS;
    }
}
static ds3_database_physical_space_state _match_ds3_database_physical_space_state(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CRITICAL") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_CRITICAL;
    } else if (xmlStrcmp(text, (const xmlChar*) "LOW") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "NEAR_LOW") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_NEAR_LOW;
    } else if (xmlStrcmp(text, (const xmlChar*) "NORMAL") == 0) {
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_NORMAL;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_DATABASE_PHYSICAL_SPACE_STATE_CRITICAL for safety.", text);
        return DS3_DATABASE_PHYSICAL_SPACE_STATE_CRITICAL;
    }
}
static ds3_http_response_format_type _match_ds3_http_response_format_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "DEFAULT") == 0) {
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT;
    } else if (xmlStrcmp(text, (const xmlChar*) "JSON") == 0) {
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_JSON;
    } else if (xmlStrcmp(text, (const xmlChar*) "XML") == 0) {
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_XML;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT for safety.", text);
        return DS3_HTTP_RESPONSE_FORMAT_TYPE_DEFAULT;
    }
}
static ds3_request_type _match_ds3_request_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "DELETE") == 0) {
        return DS3_REQUEST_TYPE_DELETE;
    } else if (xmlStrcmp(text, (const xmlChar*) "GET") == 0) {
        return DS3_REQUEST_TYPE_GET;
    } else if (xmlStrcmp(text, (const xmlChar*) "HEAD") == 0) {
        return DS3_REQUEST_TYPE_HEAD;
    } else if (xmlStrcmp(text, (const xmlChar*) "POST") == 0) {
        return DS3_REQUEST_TYPE_POST;
    } else if (xmlStrcmp(text, (const xmlChar*) "PUT") == 0) {
        return DS3_REQUEST_TYPE_PUT;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_REQUEST_TYPE_DELETE for safety.", text);
        return DS3_REQUEST_TYPE_DELETE;
    }
}
static ds3_naming_convention_type _match_ds3_naming_convention_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CONCAT_LOWERCASE") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE;
    } else if (xmlStrcmp(text, (const xmlChar*) "CONSTANT") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CONSTANT;
    } else if (xmlStrcmp(text, (const xmlChar*) "UNDERSCORED") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_UNDERSCORED;
    } else if (xmlStrcmp(text, (const xmlChar*) "CAMEL_CASE_WITH_FIRST_LETTER_UPPERCASE") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_UPPERCASE;
    } else if (xmlStrcmp(text, (const xmlChar*) "CAMEL_CASE_WITH_FIRST_LETTER_LOWERCASE") == 0) {
        return DS3_NAMING_CONVENTION_TYPE_CAMEL_CASE_WITH_FIRST_LETTER_LOWERCASE;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE for safety.", text);
        return DS3_NAMING_CONVENTION_TYPE_CONCAT_LOWERCASE;
    }
}
static ds3_checksum_type _match_ds3_checksum_type(const ds3_log* log, const xmlChar* text) {
    if (xmlStrcmp(text, (const xmlChar*) "CRC_32") == 0) {
        return DS3_CHECKSUM_TYPE_CRC_32;
    } else if (xmlStrcmp(text, (const xmlChar*) "CRC_32C") == 0) {
        return DS3_CHECKSUM_TYPE_CRC_32C;
    } else if (xmlStrcmp(text, (const xmlChar*) "MD5") == 0) {
        return DS3_CHECKSUM_TYPE_MD5;
    } else if (xmlStrcmp(text, (const xmlChar*) "SHA_256") == 0) {
        return DS3_CHECKSUM_TYPE_SHA_256;
    } else if (xmlStrcmp(text, (const xmlChar*) "SHA_512") == 0) {
        return DS3_CHECKSUM_TYPE_SHA_512;
    } else {
        ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s'.  Returning DS3_CHECKSUM_TYPE_CRC_32 for safety.", text);
        return DS3_CHECKSUM_TYPE_CRC_32;
    }
}

//************ STRUCT PARSERS **************
static ds3_error* _parse_ds3_azure_data_replication_rule_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_azure_data_replication_rule_response** _response) {
    ds3_azure_data_replication_rule_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_azure_data_replication_rule_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxBlobPartSizeInBytes")) {
            response->max_blob_part_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ReplicateDeletes")) {
            response->replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_blob_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_blob_response** _response) {
    ds3_blob_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_blob_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ByteOffset")) {
            response->byte_offset = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Checksum")) {
            response->checksum = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ChecksumType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->checksum_type = _match_ds3_checksum_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Length")) {
            response->length = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ObjectId")) {
            response->object_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_blob_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_bucket_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bucket_response** _response) {
    ds3_bucket_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bucket_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Empty")) {
            response->empty = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastPreferredChunkSizeInBytes")) {
            response->last_preferred_chunk_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "LogicalUsedCapacity")) {
            response->logical_used_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_bucket_acl_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bucket_acl_response** _response) {
    ds3_bucket_acl_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bucket_acl_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Permission")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->permission = _match_ds3_bucket_acl_permission(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_canceled_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_canceled_job_response** _response) {
    ds3_canceled_job_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_canceled_job_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CanceledDueToTimeout")) {
            response->canceled_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCanceled")) {
            response->date_canceled = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_canceled_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_completed_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_completed_job_response** _response) {
    ds3_completed_job_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_completed_job_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCompleted")) {
            response->date_completed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_completed_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_persistence_rule_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_persistence_rule_response** _response) {
    ds3_data_persistence_rule_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_persistence_rule_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsolationLevel")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->isolation_level = _match_ds3_data_isolation_level(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MinimumDaysToRetain")) {
            response->minimum_days_to_retain = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_persistence_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_persistence_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_policy_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_policy_response** _response) {
    ds3_data_policy_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_policy_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AlwaysForcePutJobCreation")) {
            response->always_force_put_job_creation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysMinimizeSpanningAcrossMedia")) {
            response->always_minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysReplicateDeletes")) {
            response->always_replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BlobbingEnabled")) {
            response->blobbing_enabled = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChecksumType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->checksum_type = _match_ds3_checksum_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultBlobSize")) {
            response->default_blob_size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "DefaultGetJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_get_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultPutJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_put_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyAfterWrite")) {
            response->default_verify_after_write = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DefaultVerifyJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_verify_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "EndToEndCrcRequired")) {
            response->end_to_end_crc_required = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsObjectNamingAllowed")) {
            response->ltfs_object_naming_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RebuildPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->rebuild_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Versioning")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->versioning = _match_ds3_versioning_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_policy_acl_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_policy_acl_response** _response) {
    ds3_data_policy_acl_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_policy_acl_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_degraded_blob_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_degraded_blob_response** _response) {
    ds3_degraded_blob_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_degraded_blob_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureReplicationRuleId")) {
            response->azure_replication_rule_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Ds3ReplicationRuleId")) {
            response->ds3_replication_rule_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PersistenceRuleId")) {
            response->persistence_rule_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "S3ReplicationRuleId")) {
            response->s3_replication_rule_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_degraded_blob_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_data_replication_rule_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_data_replication_rule_response** _response) {
    ds3_data_replication_rule_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_data_replication_rule_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReplicateDeletes")) {
            response->replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetDataPolicy")) {
            response->target_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_feature_key_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_feature_key_response** _response) {
    ds3_feature_key_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_feature_key_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CurrentValue")) {
            response->current_value = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ExpirationDate")) {
            response->expiration_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->key = _match_ds3_feature_key_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LimitValue")) {
            response->limit_value = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_feature_key_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_group_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_group_response** _response) {
    ds3_group_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_group_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BuiltIn")) {
            response->built_in = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_group_member_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_group_member_response** _response) {
    ds3_group_member_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_group_member_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberGroupId")) {
            response->member_group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberUserId")) {
            response->member_user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_active_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_active_job_response** _response) {
    ds3_active_job_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_active_job_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Aggregating")) {
            response->aggregating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DeadJobCleanupAllowed")) {
            response->dead_job_cleanup_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImplicitJobIdResolution")) {
            response->implicit_job_id_resolution = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "MinimizeSpanningAcrossMedia")) {
            response->minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Replicating")) {
            response->replicating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TruncatedDueToTimeout")) {
            response->truncated_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "VerifyAfterWrite")) {
            response->verify_after_write = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_active_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_node_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_node_response** _response) {
    ds3_node_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_node_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPathHttpPort")) {
            response->data_path_http_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttpsPort")) {
            response->data_path_https_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathIpAddress")) {
            response->data_path_ip_address = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DnsName")) {
            response->dns_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHeartbeat")) {
            response->last_heartbeat = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_node_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_data_replication_rule_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_data_replication_rule_response** _response) {
    ds3_s3_data_replication_rule_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_data_replication_rule_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "InitialDataPlacement")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->initial_data_placement = _match_ds3_s3_initial_data_placement_policy(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MaxBlobPartSizeInBytes")) {
            response->max_blob_part_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ReplicateDeletes")) {
            response->replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_response** _response) {
    ds3_s3_object_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Latest")) {
            response->latest = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_s3_object_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_response** _response) {
    ds3_storage_domain_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoEjectMediaFullThreshold")) {
            response->auto_eject_media_full_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponCron")) {
            response->auto_eject_upon_cron = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCancellation")) {
            response->auto_eject_upon_job_cancellation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCompletion")) {
            response->auto_eject_upon_job_completion = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponMediaFull")) {
            response->auto_eject_upon_media_full = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsFileNaming")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->ltfs_file_naming = _match_ds3_ltfs_file_naming_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MaxTapeFragmentationPercent")) {
            response->max_tape_fragmentation_percent = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MaximumAutoVerificationFrequencyInDays")) {
            response->maximum_auto_verification_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MediaEjectionAllowed")) {
            response->media_ejection_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecureMediaAllocation")) {
            response->secure_media_allocation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "VerifyPriorToAutoEject")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_prior_to_auto_eject = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteOptimization")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_optimization = _match_ds3_write_optimization(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_capacity_summary_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_capacity_summary_response** _response) {
    ds3_storage_domain_capacity_summary_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_capacity_summary_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PhysicalAllocated")) {
            response->physical_allocated = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "PhysicalFree")) {
            response->physical_free = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "PhysicalUsed")) {
            response->physical_used = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_capacity_summary_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_failure_response** _response) {
    ds3_storage_domain_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_storage_domain_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_member_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_member_response** _response) {
    ds3_storage_domain_member_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_member_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolPartitionId")) {
            response->pool_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_storage_domain_member_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapePartitionId")) {
            response->tape_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            response->tape_type = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "WritePreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_preference = _match_ds3_write_preference_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_system_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_system_failure_response** _response) {
    ds3_system_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_system_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_system_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_spectra_user_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_spectra_user_response** _response) {
    ds3_spectra_user_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_spectra_user_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AuthId")) {
            response->auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultDataPolicyId")) {
            response->default_data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecretKey")) {
            response->secret_key = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_spectra_user_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_azure_target_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_azure_target_failure_notification_registration_response** _response) {
    ds3_azure_target_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_azure_target_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_failure_notification_registration_response** _response) {
    ds3_target_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_completed_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_completed_notification_registration_response** _response) {
    ds3_job_completed_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_completed_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_completed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_created_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_created_notification_registration_response** _response) {
    ds3_job_created_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_created_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_created_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_creation_failed_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_creation_failed_notification_registration_response** _response) {
    ds3_job_creation_failed_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_creation_failed_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_creation_failed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_failure_notification_registration_response** _response) {
    ds3_pool_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_cached_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_cached_notification_registration_response** _response) {
    ds3_s3_object_cached_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_cached_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_cached_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_lost_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_lost_notification_registration_response** _response) {
    ds3_s3_object_lost_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_lost_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_lost_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_object_persisted_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_persisted_notification_registration_response** _response) {
    ds3_s3_object_persisted_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_persisted_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_persisted_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_target_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_target_failure_notification_registration_response** _response) {
    ds3_s3_target_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_target_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_storage_domain_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_storage_domain_failure_notification_registration_response** _response) {
    ds3_storage_domain_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_storage_domain_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_system_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_system_failure_notification_registration_response** _response) {
    ds3_system_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_system_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_failure_notification_registration_response** _response) {
    ds3_tape_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_partition_failure_notification_registration_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_partition_failure_notification_registration_response** _response) {
    ds3_tape_partition_failure_notification_registration_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_partition_failure_notification_registration_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_cache_filesystem_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_cache_filesystem_response** _response) {
    ds3_cache_filesystem_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_cache_filesystem_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoReclaimInitiateThreshold")) {
            response->auto_reclaim_initiate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoReclaimTerminateThreshold")) {
            response->auto_reclaim_terminate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BurstThreshold")) {
            response->burst_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxCapacityInBytes")) {
            response->max_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "MaxPercentUtilizationOfFilesystem")) {
            response->max_percent_utilization_of_filesystem = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "NodeId")) {
            response->node_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Path")) {
            response->path = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_response** _response) {
    ds3_pool_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableCapacity")) {
            response->available_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Guid")) {
            response->guid = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Health")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->health = _match_ds3_pool_health(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Mountpoint")) {
            response->mountpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoweredOn")) {
            response->powered_on = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReservedCapacity")) {
            response->reserved_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_pool_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TotalCapacity")) {
            response->total_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UsedCapacity")) {
            response->used_capacity = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_pool_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_pool_response* response = NULL;
        error = _parse_ds3_pool_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_pool_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_pool_response_array;

    return error;
}
static ds3_error* _parse_ds3_pool_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_failure_response** _response) {
    ds3_pool_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolId")) {
            response->pool_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_pool_partition_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_pool_partition_response** _response) {
    ds3_pool_partition_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_pool_partition_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_pool_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_pool_response** _response) {
    ds3_suspect_blob_pool_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_pool_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateWritten")) {
            response->date_written = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolId")) {
            response->pool_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_pool_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_tape_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_tape_response** _response) {
    ds3_suspect_blob_tape_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_tape_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OrderIndex")) {
            response->order_index = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_tape_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_response** _response) {
    ds3_tape_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableRawCapacity")) {
            response->available_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BarCode")) {
            response->bar_code = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DescriptionForIdentification")) {
            response->description_for_identification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectDate")) {
            response->eject_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLabel")) {
            response->eject_label = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLocation")) {
            response->eject_location = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectPending")) {
            response->eject_pending = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "FullOfData")) {
            response->full_of_data = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCheckpoint")) {
            response->last_checkpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartiallyVerifiedEndOfTape")) {
            response->partially_verified_end_of_tape = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PreviousState")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->previous_state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TakeOwnershipPending")) {
            response->take_ownership_pending = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TotalRawCapacity")) {
            response->total_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            response->type = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "VerifyPending")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_pending = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteProtected")) {
            response->write_protected = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_tape_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_tape_response* response = NULL;
        error = _parse_ds3_tape_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_tape_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_tape_response_array;

    return error;
}
static ds3_error* _parse_ds3_tape_density_directive_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_density_directive_response** _response) {
    ds3_tape_density_directive_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_density_directive_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Density")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->density = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            response->tape_type = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_density_directive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_drive_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_drive_response** _response) {
    ds3_tape_drive_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_drive_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CleaningRequired")) {
            response->cleaning_required = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ForceTapeRemoval")) {
            response->force_tape_removal = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCleaned")) {
            response->last_cleaned = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MfgSerialNumber")) {
            response->mfg_serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReservedTaskType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->reserved_task_type = _match_ds3_reserved_task_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_drive_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_drive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_detailed_tape_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_detailed_tape_failure_response** _response) {
    ds3_detailed_tape_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_detailed_tape_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeDriveId")) {
            response->tape_drive_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_tape_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_library_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_library_response** _response) {
    ds3_tape_library_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_library_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ManagementUrl")) {
            response->management_url = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_library_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_partition_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_partition_response** _response) {
    ds3_tape_partition_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_partition_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MinimumReadReservedDrives")) {
            response->minimum_read_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MinimumWriteReservedDrives")) {
            response->minimum_write_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialId")) {
            response->serial_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tape_partition_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_partition_failure_response** _response) {
    ds3_tape_partition_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_partition_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_partition_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_azure_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_azure_target_response** _response) {
    ds3_azure_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_azure_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccountKey")) {
            response->account_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AccountName")) {
            response->account_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoVerifyFrequencyInDays")) {
            response->auto_verify_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketPrefix")) {
            response->cloud_bucket_prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketSuffix")) {
            response->cloud_bucket_suffix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Https")) {
            response->https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFullyVerified")) {
            response->last_fully_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_azure_target_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_azure_target_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_azure_target_response* response = NULL;
        error = _parse_ds3_azure_target_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_azure_target_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_azure_target_response_array;

    return error;
}
static ds3_error* _parse_ds3_azure_target_bucket_name_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_azure_target_bucket_name_response** _response) {
    ds3_azure_target_bucket_name_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_azure_target_bucket_name_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_bucket_name_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_azure_target_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_azure_target_failure_response** _response) {
    ds3_azure_target_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_azure_target_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_target_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_azure_target_read_preference_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_azure_target_read_preference_response** _response) {
    ds3_azure_target_read_preference_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_azure_target_read_preference_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_response** _response) {
    ds3_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccessControlReplication")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->access_control_replication = _match_ds3_target_access_control_replication(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "AdminAuthId")) {
            response->admin_auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AdminSecretKey")) {
            response->admin_secret_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathEndPoint")) {
            response->data_path_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttps")) {
            response->data_path_https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DataPathPort")) {
            response->data_path_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathProxy")) {
            response->data_path_proxy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathVerifyCertificate")) {
            response->data_path_verify_certificate = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReplicatedUserDefaultDataPolicy")) {
            response->replicated_user_default_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_target_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_target_response* response = NULL;
        error = _parse_ds3_target_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_target_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_target_response_array;

    return error;
}
static ds3_error* _parse_ds3_target_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_failure_response** _response) {
    ds3_target_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_target_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_target_read_preference_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_target_read_preference_response** _response) {
    ds3_target_read_preference_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_target_read_preference_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_target_response** _response) {
    ds3_s3_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccessKey")) {
            response->access_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoVerifyFrequencyInDays")) {
            response->auto_verify_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketPrefix")) {
            response->cloud_bucket_prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketSuffix")) {
            response->cloud_bucket_suffix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathEndPoint")) {
            response->data_path_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Https")) {
            response->https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFullyVerified")) {
            response->last_fully_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OfflineDataStagingWindowInTb")) {
            response->offline_data_staging_window_in_tb = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ProxyDomain")) {
            response->proxy_domain = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ProxyHost")) {
            response->proxy_host = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ProxyPassword")) {
            response->proxy_password = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ProxyPort")) {
            response->proxy_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "ProxyUsername")) {
            response->proxy_username = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Region")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->region = _match_ds3_s3_region(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SecretKey")) {
            response->secret_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "StagedDataExpirationInDays")) {
            response->staged_data_expiration_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_target_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_s3_target_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_s3_target_response* response = NULL;
        error = _parse_ds3_s3_target_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_s3_target_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_s3_target_response_array;

    return error;
}
static ds3_error* _parse_ds3_s3_target_bucket_name_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_target_bucket_name_response** _response) {
    ds3_s3_target_bucket_name_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_target_bucket_name_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_bucket_name_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_target_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_target_failure_response** _response) {
    ds3_s3_target_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_target_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Date")) {
            response->date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_target_failure_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_s3_target_read_preference_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_target_read_preference_response** _response) {
    ds3_s3_target_read_preference_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_target_read_preference_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_azure_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_azure_target_response** _response) {
    ds3_suspect_blob_azure_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_azure_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_azure_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_ds3_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_ds3_target_response** _response) {
    ds3_suspect_blob_ds3_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_ds3_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_ds3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_suspect_blob_s3_target_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_suspect_blob_s3_target_response** _response) {
    ds3_suspect_blob_s3_target_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_suspect_blob_s3_target_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobId")) {
            response->blob_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_s3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_build_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_build_information_response** _response) {
    ds3_build_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_build_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Branch")) {
            response->branch = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Revision")) {
            response->revision = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_build_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_set_of_tape_bar_codes_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_set_of_tape_bar_codes_response** _response) {
    ds3_set_of_tape_bar_codes_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* tape_bar_codes_array = g_ptr_array_new();

    response = g_new0(ds3_set_of_tape_bar_codes_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeBarCodes")) {
            xmlNodePtr loop_node;
            GPtrArray* tape_bar_codes_array = g_ptr_array_new();
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* tape_bar_codes = xml_get_string(doc, loop_node);
                g_ptr_array_add(tape_bar_codes_array, tape_bar_codes);
            }
            response->tape_bar_codes = (ds3_str**)tape_bar_codes_array->pdata;
            response->num_tape_bar_codes = tape_bar_codes_array->len;
            g_ptr_array_free(tape_bar_codes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_set_of_tape_bar_codes_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_bar_codes = (ds3_str**)tape_bar_codes_array->pdata;
    response->num_tape_bar_codes = tape_bar_codes_array->len;
    g_ptr_array_free(tape_bar_codes_array, FALSE);

    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_tapes_must_be_onlined_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tapes_must_be_onlined_response** _response) {
    ds3_tapes_must_be_onlined_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* tapes_to_online_array = g_ptr_array_new();

    response = g_new0(ds3_tapes_must_be_onlined_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapesToOnline")) {
            ds3_set_of_tape_bar_codes_response* tapes_to_online_response = NULL;
            error = _parse_ds3_set_of_tape_bar_codes_response(client, doc, child_node, &tapes_to_online_response);
            response->tapes_to_online = (ds3_set_of_tape_bar_codes_response**)tapes_to_online_array->pdata;
            g_ptr_array_add(tapes_to_online_array, tapes_to_online_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tapes_must_be_onlined_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tapes_to_online = (ds3_set_of_tape_bar_codes_response**)tapes_to_online_array->pdata;
    response->num_tapes_to_online = tapes_to_online_array->len;
    g_ptr_array_free(tapes_to_online_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tapes_must_be_onlined_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_blob_store_task_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_blob_store_task_information_response** _response) {
    ds3_blob_store_task_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_blob_store_task_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DateScheduled")) {
            response->date_scheduled = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateStarted")) {
            response->date_started = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Description")) {
            response->description = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DriveId")) {
            response->drive_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolId")) {
            response->pool_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_blob_store_task_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetType")) {
            response->target_type = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_blob_store_task_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_cache_entry_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_cache_entry_information_response** _response) {
    ds3_cache_entry_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_cache_entry_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Blob")) {
            error = _parse_ds3_blob_response(client, doc, child_node, &response->blob);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_cache_entry_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_entry_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_entry_information_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_cache_filesystem_information_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_cache_filesystem_information_response** _response) {
    ds3_cache_filesystem_information_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* entries_array = g_ptr_array_new();

    response = g_new0(ds3_cache_filesystem_information_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AvailableCapacityInBytes")) {
            response->available_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CacheFilesystem")) {
            error = _parse_ds3_cache_filesystem_response(client, doc, child_node, &response->cache_filesystem);
        } else if (element_equal(child_node, "Entries")) {
            ds3_cache_entry_information_response* entries_response = NULL;
            error = _parse_ds3_cache_entry_information_response(client, doc, child_node, &entries_response);
            response->entries = (ds3_cache_entry_information_response**)entries_array->pdata;
            g_ptr_array_add(entries_array, entries_response);
        } else if (element_equal(child_node, "Summary")) {
            response->summary = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TotalCapacityInBytes")) {
            response->total_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "UnavailableCapacityInBytes")) {
            response->unavailable_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "UsedCapacityInBytes")) {
            response->used_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->entries = (ds3_cache_entry_information_response**)entries_array->pdata;
    response->num_entries = entries_array->len;
    g_ptr_array_free(entries_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_filesystem_information_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_bucket_details_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bucket_details_response** _response) {
    ds3_bucket_details_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bucket_details_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_details_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_bucket_details_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_bucket_details_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_bucket_details_response* response = NULL;
        error = _parse_ds3_bucket_details_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_bucket_details_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_bucket_details_response_array;

    return error;
}
static ds3_error* _parse_ds3_delete_object_error_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_delete_object_error_response** _response) {
    ds3_delete_object_error_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_delete_object_error_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Code")) {
            response->code = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Message")) {
            response->message = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_delete_object_error_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_multi_part_upload_part_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_multi_part_upload_part_response** _response) {
    ds3_multi_part_upload_part_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_multi_part_upload_part_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartNumber")) {
            response->part_number = xml_get_uint16(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_multi_part_upload_part_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_node_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_node_response** _response) {
    ds3_job_node_response* response;
    struct _xmlAttr* attribute;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_node_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "EndPoint") == true) {
            response->end_point = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "HttpPort") == true) {
            response->http_port = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "HttpsPort") == true) {
            response->https_port = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Id") == true) {
            response->id = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_job_node_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }



    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_job_node_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_job_node_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_job_node_response* response = NULL;
        error = _parse_ds3_job_node_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_job_node_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_job_node_response_array;

    return error;
}
static ds3_error* _parse_ds3_s3_object_to_delete_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_s3_object_to_delete_response** _response) {
    ds3_s3_object_to_delete_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_s3_object_to_delete_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_to_delete_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_user_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_user_response** _response) {
    ds3_user_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_user_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DisplayName")) {
            response->display_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ID")) {
            response->id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_user_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_named_detailed_tape_partition_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_named_detailed_tape_partition_response** _response) {
    ds3_named_detailed_tape_partition_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* tape_types_array = g_ptr_array_new();

    response = g_new0(ds3_named_detailed_tape_partition_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DriveTypes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            GByteArray* enum_array = g_byte_array_new();
            ds3_tape_drive_type drive_types;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                xmlChar* text = xmlNodeListGetString(doc, loop_node, 1);
                if (text == NULL) {
                    continue;
                }
                drive_types = _match_ds3_tape_drive_type(client->log, text);
                g_byte_array_append(enum_array, (const guint8*) &drive_types, sizeof(ds3_tape_drive_type));
            }
            response->drive_types = (ds3_tape_drive_type*)enum_array->data;
            response->num_drive_types = enum_array->len;
            g_byte_array_free(enum_array, FALSE);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MinimumReadReservedDrives")) {
            response->minimum_read_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MinimumWriteReservedDrives")) {
            response->minimum_write_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialId")) {
            response->serial_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeTypes")) {
            xmlNodePtr loop_node;
            GPtrArray* tape_types_array = g_ptr_array_new();
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* tape_types = xml_get_string(doc, loop_node);
                g_ptr_array_add(tape_types_array, tape_types);
            }
            response->tape_types = (ds3_str**)tape_types_array->pdata;
            response->num_tape_types = tape_types_array->len;
            g_ptr_array_free(tape_types_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_named_detailed_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_types = (ds3_str**)tape_types_array->pdata;
    response->num_tape_types = tape_types_array->len;
    g_ptr_array_free(tape_types_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_named_detailed_tape_partition_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_tape_failure_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_tape_failure_response** _response) {
    ds3_tape_failure_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_tape_failure_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Cause")) {
            response->cause = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Tape")) {
            error = _parse_ds3_tape_response(client, doc, child_node, &response->tape);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_type_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_type_response** _response) {
    ds3_type_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_type_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BeansRetrieverName")) {
            response->beans_retriever_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DomainName")) {
            response->domain_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NumberOfType")) {
            response->number_of_type = xml_get_uint16(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_type_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    *_response = response;

    return error;
}

static ds3_error* _parse_ds3_physical_placement_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_physical_placement_response** _response) {
    ds3_physical_placement_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_physical_placement_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureTargets")) {
            GPtrArray* azure_targets_array;
            error = _parse_ds3_azure_target_response_array(client, doc, child_node, &azure_targets_array);
            response->azure_targets = (ds3_azure_target_response**)azure_targets_array->pdata;
            response->num_azure_targets = azure_targets_array->len;
            g_ptr_array_free(azure_targets_array, FALSE);
        } else if (element_equal(child_node, "Ds3Targets")) {
            GPtrArray* ds3_targets_array;
            error = _parse_ds3_target_response_array(client, doc, child_node, &ds3_targets_array);
            response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
            response->num_ds3_targets = ds3_targets_array->len;
            g_ptr_array_free(ds3_targets_array, FALSE);
        } else if (element_equal(child_node, "Pools")) {
            GPtrArray* pools_array;
            error = _parse_ds3_pool_response_array(client, doc, child_node, &pools_array);
            response->pools = (ds3_pool_response**)pools_array->pdata;
            response->num_pools = pools_array->len;
            g_ptr_array_free(pools_array, FALSE);
        } else if (element_equal(child_node, "S3Targets")) {
            GPtrArray* s3_targets_array;
            error = _parse_ds3_s3_target_response_array(client, doc, child_node, &s3_targets_array);
            response->s3_targets = (ds3_s3_target_response**)s3_targets_array->pdata;
            response->num_s3_targets = s3_targets_array->len;
            g_ptr_array_free(s3_targets_array, FALSE);
        } else if (element_equal(child_node, "Tapes")) {
            GPtrArray* tapes_array;
            error = _parse_ds3_tape_response_array(client, doc, child_node, &tapes_array);
            response->tapes = (ds3_tape_response**)tapes_array->pdata;
            response->num_tapes = tapes_array->len;
            g_ptr_array_free(tapes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_physical_placement_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_physical_placement_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_bulk_object_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bulk_object_response** _response) {
    ds3_bulk_object_response* response;
    struct _xmlAttr* attribute;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_bulk_object_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Bucket") == true) {
            response->bucket = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Id") == true) {
            response->id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "InCache") == true) {
            response->in_cache = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Latest") == true) {
            response->latest = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Length") == true) {
            response->length = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Name") == true) {
            response->name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Offset") == true) {
            response->offset = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Version") == true) {
            response->version = xml_get_uint64_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_bulk_object_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PhysicalPlacement")) {
            error = _parse_ds3_physical_placement_response(client, doc, child_node, &response->physical_placement);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bulk_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bulk_object_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_bulk_object_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_bulk_object_response* response = NULL;
        error = _parse_ds3_bulk_object_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_bulk_object_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_bulk_object_response_array;

    return error;
}
static ds3_error* _parse_ds3_bulk_object_list_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_bulk_object_list_response** _response) {
    ds3_bulk_object_list_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    response = g_new0(ds3_bulk_object_list_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bulk_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bulk_object_list_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_job_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_job_response** _response) {
    ds3_job_response* response;
    struct _xmlAttr* attribute;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_job_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Aggregating") == true) {
            response->aggregating = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "BucketName") == true) {
            response->bucket_name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "CachedSizeInBytes") == true) {
            response->cached_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkClientProcessingOrderGuarantee") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "CompletedSizeInBytes") == true) {
            response->completed_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "EntirelyInCache") == true) {
            response->entirely_in_cache = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "JobId") == true) {
            response->job_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Naked") == true) {
            response->naked = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Name") == true) {
            response->name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "OriginalSizeInBytes") == true) {
            response->original_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Priority") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "RequestType") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "StartDate") == true) {
            response->start_date = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Status") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->status = _match_ds3_job_status(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "UserId") == true) {
            response->user_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "UserName") == true) {
            response->user_name = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_job_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Nodes")) {
            GPtrArray* nodes_array;
            error = _parse_ds3_job_node_response_array(client, doc, child_node, &nodes_array);
            response->nodes = (ds3_job_node_response**)nodes_array->pdata;
            response->num_nodes = nodes_array->len;
            g_ptr_array_free(nodes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_job_response_array(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, GPtrArray** _response) {
    ds3_error* error = NULL;
    xmlNodePtr child_node;
    GPtrArray* ds3_job_response_array = g_ptr_array_new();

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        ds3_job_response* response = NULL;
        error = _parse_ds3_job_response(client, doc, child_node, &response);
        g_ptr_array_add(ds3_job_response_array, response);

        if (error != NULL) {
            break;
        }
    }

    *_response = ds3_job_response_array;

    return error;
}
static ds3_error* _parse_ds3_objects_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_objects_response** _response) {
    ds3_objects_response* response;
    struct _xmlAttr* attribute;
    xmlNodePtr child_node;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    response = g_new0(ds3_objects_response, 1);

    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "ChunkId") == true) {
            response->chunk_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkNumber") == true) {
            response->chunk_number = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "NodeId") == true) {
            response->node_id = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_objects_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_objects_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_objects_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_multi_part_upload_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_multi_part_upload_response** _response) {
    ds3_multi_part_upload_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_multi_part_upload_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Initiated")) {
            response->initiated = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else if (element_equal(child_node, "UploadId")) {
            response->upload_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_multi_part_upload_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_multi_part_upload_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_contents_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_contents_response** _response) {
    ds3_contents_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_contents_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else if (element_equal(child_node, "Size")) {
            response->size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "StorageClass")) {
            response->storage_class = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_contents_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_contents_response_free(response);
    }

    return error;
}

static ds3_error* _parse_ds3_detailed_s3_object_response(const ds3_client* client, const xmlDocPtr doc, const xmlNodePtr root, ds3_detailed_s3_object_response** _response) {
    ds3_detailed_s3_object_response* response;
    xmlNodePtr child_node;
    ds3_error* error = NULL;

    response = g_new0(ds3_detailed_s3_object_response, 1);


    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Blobs")) {
            error = _parse_ds3_bulk_object_list_response(client, doc, child_node, &response->blobs);
        } else if (element_equal(child_node, "BlobsBeingPersisted")) {
            response->blobs_being_persisted = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BlobsDegraded")) {
            response->blobs_degraded = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BlobsInCache")) {
            response->blobs_in_cache = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BlobsTotal")) {
            response->blobs_total = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Latest")) {
            response->latest = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            response->owner = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Size")) {
            response->size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_s3_object_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_s3_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_s3_object_response_free(response);
    }

    return error;
}


//************ TOP LEVEL STRUCT PARSERS **************
static ds3_error* _parse_top_level_ds3_azure_data_replication_rule_response(const ds3_client* client, const ds3_request* request, ds3_azure_data_replication_rule_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_data_replication_rule_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_data_replication_rule_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxBlobPartSizeInBytes")) {
            response->max_blob_part_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ReplicateDeletes")) {
            response->replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_data_replication_rule_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bucket_response(const ds3_client* client, const ds3_request* request, ds3_bucket_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Empty")) {
            response->empty = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastPreferredChunkSizeInBytes")) {
            response->last_preferred_chunk_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "LogicalUsedCapacity")) {
            response->logical_used_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bucket_acl_response(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_acl_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_acl_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Permission")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->permission = _match_ds3_bucket_acl_permission(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_acl_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_canceled_job_response(const ds3_client* client, const ds3_request* request, ds3_canceled_job_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_canceled_job_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_canceled_job_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CanceledDueToTimeout")) {
            response->canceled_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCanceled")) {
            response->date_canceled = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_canceled_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_canceled_job_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_completed_job_response(const ds3_client* client, const ds3_request* request, ds3_completed_job_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_completed_job_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_completed_job_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DateCompleted")) {
            response->date_completed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_completed_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_completed_job_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_path_backend_response(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_path_backend_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_path_backend_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Activated")) {
            response->activated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AllowNewJobRequests")) {
            response->allow_new_job_requests = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoActivateTimeoutInMins")) {
            response->auto_activate_timeout_in_mins = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "AutoInspect")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->auto_inspect = _match_ds3_auto_inspect_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CacheAvailableRetryAfterInSeconds")) {
            response->cache_available_retry_after_in_seconds = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DefaultImportConflictResolutionMode")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_import_conflict_resolution_mode = _match_ds3_import_conflict_resolution_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyDataAfterImport")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_verify_data_after_import = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyDataPriorToImport")) {
            response->default_verify_data_prior_to_import = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "InstanceId")) {
            response->instance_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHeartbeat")) {
            response->last_heartbeat = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartiallyVerifyLastPercentOfTapes")) {
            response->partially_verify_last_percent_of_tapes = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UnavailableMediaPolicy")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->unavailable_media_policy = _match_ds3_unavailable_media_usage_policy(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UnavailablePoolMaxJobRetryInMins")) {
            response->unavailable_pool_max_job_retry_in_mins = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UnavailableTapePartitionMaxJobRetryInMins")) {
            response->unavailable_tape_partition_max_job_retry_in_mins = xml_get_uint16(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_path_backend_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_path_backend_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_persistence_rule_response(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_persistence_rule_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_persistence_rule_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsolationLevel")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->isolation_level = _match_ds3_data_isolation_level(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MinimumDaysToRetain")) {
            response->minimum_days_to_retain = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_persistence_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_persistence_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_persistence_rule_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AlwaysForcePutJobCreation")) {
            response->always_force_put_job_creation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysMinimizeSpanningAcrossMedia")) {
            response->always_minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AlwaysReplicateDeletes")) {
            response->always_replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BlobbingEnabled")) {
            response->blobbing_enabled = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ChecksumType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->checksum_type = _match_ds3_checksum_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultBlobSize")) {
            response->default_blob_size = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "DefaultGetJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_get_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultPutJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_put_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DefaultVerifyAfterWrite")) {
            response->default_verify_after_write = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DefaultVerifyJobPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_verify_job_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "EndToEndCrcRequired")) {
            response->end_to_end_crc_required = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsObjectNamingAllowed")) {
            response->ltfs_object_naming_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "RebuildPriority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->rebuild_priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Versioning")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->versioning = _match_ds3_versioning_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_acl_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_acl_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_acl_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_acl_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_acl_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_replication_rule_response(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_replication_rule_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_replication_rule_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReplicateDeletes")) {
            response->replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetDataPolicy")) {
            response->target_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_replication_rule_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_response(const ds3_client* client, const ds3_request* request, ds3_group_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BuiltIn")) {
            response->built_in = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_member_response(const ds3_client* client, const ds3_request* request, ds3_group_member_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_member_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_member_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "GroupId")) {
            response->group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberGroupId")) {
            response->member_group_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MemberUserId")) {
            response->member_user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_member_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_active_job_response(const ds3_client* client, const ds3_request* request, ds3_active_job_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_active_job_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_active_job_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Aggregating")) {
            response->aggregating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CachedSizeInBytes")) {
            response->cached_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ChunkClientProcessingOrderGuarantee")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "CompletedSizeInBytes")) {
            response->completed_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "CreatedAt")) {
            response->created_at = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DeadJobCleanupAllowed")) {
            response->dead_job_cleanup_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImplicitJobIdResolution")) {
            response->implicit_job_id_resolution = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "MinimizeSpanningAcrossMedia")) {
            response->minimize_spanning_across_media = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Naked")) {
            response->naked = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OriginalSizeInBytes")) {
            response->original_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Priority")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Rechunked")) {
            response->rechunked = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Replicating")) {
            response->replicating = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "RequestType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Truncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TruncatedDueToTimeout")) {
            response->truncated_due_to_timeout = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "VerifyAfterWrite")) {
            response->verify_after_write = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_active_job_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_active_job_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_chunk_response(const ds3_client* client, const ds3_request* request, ds3_job_chunk_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_chunk_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_chunk_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BlobStoreState")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->blob_store_state = _match_ds3_job_chunk_blob_store_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ChunkNumber")) {
            response->chunk_number = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobCreationDate")) {
            response->job_creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NodeId")) {
            response->node_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PendingTargetCommit")) {
            response->pending_target_commit = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ReadFromAzureTargetId")) {
            response->read_from_azure_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadFromDs3TargetId")) {
            response->read_from_ds3_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadFromPoolId")) {
            response->read_from_pool_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadFromS3TargetId")) {
            response->read_from_s3_target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadFromTapeId")) {
            response->read_from_tape_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_chunk_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_chunk_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_node_response(const ds3_client* client, const ds3_request* request, ds3_node_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_node_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_node_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPathHttpPort")) {
            response->data_path_http_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttpsPort")) {
            response->data_path_https_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathIpAddress")) {
            response->data_path_ip_address = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DnsName")) {
            response->dns_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHeartbeat")) {
            response->last_heartbeat = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_node_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_node_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_data_replication_rule_response(const ds3_client* client, const ds3_request* request, ds3_s3_data_replication_rule_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_data_replication_rule_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_data_replication_rule_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyId")) {
            response->data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "InitialDataPlacement")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->initial_data_placement = _match_ds3_s3_initial_data_placement_policy(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MaxBlobPartSizeInBytes")) {
            response->max_blob_part_size_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "ReplicateDeletes")) {
            response->replicate_deletes = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_data_placement_rule_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_data_replication_rule_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_data_replication_rule_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_data_replication_rule_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Latest")) {
            response->latest = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_s3_object_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Version")) {
            response->version = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoEjectMediaFullThreshold")) {
            response->auto_eject_media_full_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponCron")) {
            response->auto_eject_upon_cron = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCancellation")) {
            response->auto_eject_upon_job_cancellation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponJobCompletion")) {
            response->auto_eject_upon_job_completion = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AutoEjectUponMediaFull")) {
            response->auto_eject_upon_media_full = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LtfsFileNaming")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->ltfs_file_naming = _match_ds3_ltfs_file_naming_mode(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MaxTapeFragmentationPercent")) {
            response->max_tape_fragmentation_percent = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MaximumAutoVerificationFrequencyInDays")) {
            response->maximum_auto_verification_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MediaEjectionAllowed")) {
            response->media_ejection_allowed = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecureMediaAllocation")) {
            response->secure_media_allocation = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "VerifyPriorToAutoEject")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_prior_to_auto_eject = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteOptimization")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_optimization = _match_ds3_write_optimization(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_member_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_member_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_member_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoolPartitionId")) {
            response->pool_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_storage_domain_member_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapePartitionId")) {
            response->tape_partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            response->tape_type = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "WritePreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->write_preference = _match_ds3_write_preference_level(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_member_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_member_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_spectra_user_response(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_spectra_user_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_spectra_user_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AuthId")) {
            response->auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultDataPolicyId")) {
            response->default_data_policy_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SecretKey")) {
            response->secret_key = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_spectra_user_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_spectra_user_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_completed_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_completed_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_completed_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_completed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_completed_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_created_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_created_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_created_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_created_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_created_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_creation_failed_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_creation_failed_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_creation_failed_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_creation_failed_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_creation_failed_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_cached_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_cached_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_cached_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_cached_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_cached_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_lost_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_lost_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_lost_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_lost_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_lost_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_persisted_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_persisted_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_persisted_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "JobId")) {
            response->job_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_persisted_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_persisted_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_failure_notification_registration_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_failure_notification_registration_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_failure_notification_registration_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Format")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->format = _match_ds3_http_response_format_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFailure")) {
            response->last_failure = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastHttpResponseCode")) {
            response->last_http_response_code = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "LastNotification")) {
            response->last_notification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NamingConvention")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->naming_convention = _match_ds3_naming_convention_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NotificationEndPoint")) {
            response->notification_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NotificationHttpMethod")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->notification_http_method = _match_ds3_request_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "NumberOfFailuresSinceLastSuccess")) {
            response->number_of_failures_since_last_success = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "UserId")) {
            response->user_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_notification_registration_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_failure_notification_registration_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_cache_filesystem_response(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_cache_filesystem_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_cache_filesystem_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AutoReclaimInitiateThreshold")) {
            response->auto_reclaim_initiate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "AutoReclaimTerminateThreshold")) {
            response->auto_reclaim_terminate_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BurstThreshold")) {
            response->burst_threshold = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxCapacityInBytes")) {
            response->max_capacity_in_bytes = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "MaxPercentUtilizationOfFilesystem")) {
            response->max_percent_utilization_of_filesystem = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "NodeId")) {
            response->node_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Path")) {
            response->path = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_filesystem_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_response(const ds3_client* client, const ds3_request* request, ds3_pool_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableCapacity")) {
            response->available_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Guid")) {
            response->guid = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Health")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->health = _match_ds3_pool_health(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Mountpoint")) {
            response->mountpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PoweredOn")) {
            response->powered_on = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReservedCapacity")) {
            response->reserved_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_pool_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TotalCapacity")) {
            response->total_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "UsedCapacity")) {
            response->used_capacity = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_partition_response(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_partition_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_partition_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_pool_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_partition_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_response(const ds3_client* client, const ds3_request* request, ds3_tape_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AssignedToStorageDomain")) {
            response->assigned_to_storage_domain = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "AvailableRawCapacity")) {
            response->available_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "BarCode")) {
            response->bar_code = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DescriptionForIdentification")) {
            response->description_for_identification = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectDate")) {
            response->eject_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLabel")) {
            response->eject_label = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectLocation")) {
            response->eject_location = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "EjectPending")) {
            response->eject_pending = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "FullOfData")) {
            response->full_of_data = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastAccessed")) {
            response->last_accessed = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCheckpoint")) {
            response->last_checkpoint = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastModified")) {
            response->last_modified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastVerified")) {
            response->last_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartiallyVerifiedEndOfTape")) {
            response->partially_verified_end_of_tape = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PreviousState")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->previous_state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "StorageDomainId")) {
            response->storage_domain_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TakeOwnershipPending")) {
            response->take_ownership_pending = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "TotalRawCapacity")) {
            response->total_raw_capacity = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            response->type = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "VerifyPending")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->verify_pending = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "WriteProtected")) {
            response->write_protected = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_density_directive_response(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_density_directive_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_density_directive_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Density")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->density = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TapeType")) {
            response->tape_type = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_density_directive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_density_directive_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_drive_response(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_drive_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_drive_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CleaningRequired")) {
            response->cleaning_required = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ForceTapeRemoval")) {
            response->force_tape_removal = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastCleaned")) {
            response->last_cleaned = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MfgSerialNumber")) {
            response->mfg_serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PartitionId")) {
            response->partition_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReservedTaskType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->reserved_task_type = _match_ds3_reserved_task_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_drive_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeId")) {
            response->tape_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Type")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_drive_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_drive_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_library_response(const ds3_client* client, const ds3_request* request, ds3_tape_library_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_library_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_library_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ManagementUrl")) {
            response->management_url = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_library_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_library_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MinimumReadReservedDrives")) {
            response->minimum_read_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MinimumWriteReservedDrives")) {
            response->minimum_write_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialId")) {
            response->serial_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccountKey")) {
            response->account_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AccountName")) {
            response->account_name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoVerifyFrequencyInDays")) {
            response->auto_verify_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketPrefix")) {
            response->cloud_bucket_prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketSuffix")) {
            response->cloud_bucket_suffix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Https")) {
            response->https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFullyVerified")) {
            response->last_fully_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_bucket_name_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_bucket_name_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_bucket_name_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_bucket_name_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_bucket_name_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_bucket_name_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_read_preference_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_read_preference_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_read_preference_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_read_preference_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_read_preference_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_response(const ds3_client* client, const ds3_request* request, ds3_target_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccessControlReplication")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->access_control_replication = _match_ds3_target_access_control_replication(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "AdminAuthId")) {
            response->admin_auth_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AdminSecretKey")) {
            response->admin_secret_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathEndPoint")) {
            response->data_path_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathHttps")) {
            response->data_path_https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DataPathPort")) {
            response->data_path_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "DataPathProxy")) {
            response->data_path_proxy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathVerifyCertificate")) {
            response->data_path_verify_certificate = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "ReplicatedUserDefaultDataPolicy")) {
            response->replicated_user_default_data_policy = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_read_preference_response(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_read_preference_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_read_preference_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_read_preference_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AccessKey")) {
            response->access_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "AutoVerifyFrequencyInDays")) {
            response->auto_verify_frequency_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketPrefix")) {
            response->cloud_bucket_prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CloudBucketSuffix")) {
            response->cloud_bucket_suffix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DataPathEndPoint")) {
            response->data_path_end_point = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "DefaultReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->default_read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Https")) {
            response->https = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "LastFullyVerified")) {
            response->last_fully_verified = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "OfflineDataStagingWindowInTb")) {
            response->offline_data_staging_window_in_tb = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "PermitGoingOutOfSync")) {
            response->permit_going_out_of_sync = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "ProxyDomain")) {
            response->proxy_domain = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ProxyHost")) {
            response->proxy_host = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ProxyPassword")) {
            response->proxy_password = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ProxyPort")) {
            response->proxy_port = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "ProxyUsername")) {
            response->proxy_username = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "Region")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->region = _match_ds3_s3_region(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SecretKey")) {
            response->secret_key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "StagedDataExpirationInDays")) {
            response->staged_data_expiration_in_days = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_target_state(client->log, text);
            xmlFree(text);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_bucket_name_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_bucket_name_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_bucket_name_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_bucket_name_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_bucket_name_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_bucket_name_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_read_preference_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_read_preference_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_read_preference_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_read_preference_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketId")) {
            response->bucket_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ReadPreference")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->read_preference = _match_ds3_target_read_preference_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TargetId")) {
            response->target_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_read_preference_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_read_preference_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_blob_store_tasks_information_response(const ds3_client* client, const ds3_request* request, ds3_blob_store_tasks_information_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_blob_store_tasks_information_response* response;
    ds3_error* error = NULL;
    GPtrArray* tasks_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_blob_store_tasks_information_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Tasks")) {
            ds3_blob_store_task_information_response* tasks_response = NULL;
            error = _parse_ds3_blob_store_task_information_response(client, doc, child_node, &tasks_response);
            response->tasks = (ds3_blob_store_task_information_response**)tasks_array->pdata;
            g_ptr_array_add(tasks_array, tasks_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_blob_store_tasks_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tasks = (ds3_blob_store_task_information_response**)tasks_array->pdata;
    response->num_tasks = tasks_array->len;
    g_ptr_array_free(tasks_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_blob_store_tasks_information_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_cache_information_response(const ds3_client* client, const ds3_request* request, ds3_cache_information_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_cache_information_response* response;
    ds3_error* error = NULL;
    GPtrArray* filesystems_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_cache_information_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Filesystems")) {
            ds3_cache_filesystem_information_response* filesystems_response = NULL;
            error = _parse_ds3_cache_filesystem_information_response(client, doc, child_node, &filesystems_response);
            response->filesystems = (ds3_cache_filesystem_information_response**)filesystems_array->pdata;
            g_ptr_array_add(filesystems_array, filesystems_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->filesystems = (ds3_cache_filesystem_information_response**)filesystems_array->pdata;
    response->num_filesystems = filesystems_array->len;
    g_ptr_array_free(filesystems_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_information_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_complete_multipart_upload_result_response(const ds3_client* client, const ds3_request* request, ds3_complete_multipart_upload_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_complete_multipart_upload_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "CompleteMultipartUploadResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_complete_multipart_upload_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ETag")) {
            response->e_tag = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Location")) {
            response->location = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_complete_multipart_upload_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_complete_multipart_upload_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_detailed_tape_partition_response(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_partition_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_detailed_tape_partition_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_types_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_detailed_tape_partition_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DriveType")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->drive_type = _match_ds3_tape_drive_type(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "DriveTypes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            GByteArray* enum_array = g_byte_array_new();
            ds3_tape_drive_type drive_types;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                xmlChar* text = xmlNodeListGetString(doc, loop_node, 1);
                if (text == NULL) {
                    continue;
                }
                drive_types = _match_ds3_tape_drive_type(client->log, text);
                g_byte_array_append(enum_array, (const guint8*) &drive_types, sizeof(ds3_tape_drive_type));
            }
            response->drive_types = (ds3_tape_drive_type*)enum_array->data;
            response->num_drive_types = enum_array->len;
            g_byte_array_free(enum_array, FALSE);
        } else if (element_equal(child_node, "ErrorMessage")) {
            response->error_message = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Id")) {
            response->id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "ImportExportConfiguration")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->import_export_configuration = _match_ds3_import_export_configuration(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "LibraryId")) {
            response->library_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MinimumReadReservedDrives")) {
            response->minimum_read_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "MinimumWriteReservedDrives")) {
            response->minimum_write_reserved_drives = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Quiesced")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->quiesced = _match_ds3_quiesced(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "SerialId")) {
            response->serial_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "State")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->state = _match_ds3_tape_partition_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "TapeTypes")) {
            xmlNodePtr loop_node;
            GPtrArray* tape_types_array = g_ptr_array_new();
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* tape_types = xml_get_string(doc, loop_node);
                g_ptr_array_add(tape_types_array, tape_types);
            }
            response->tape_types = (ds3_str**)tape_types_array->pdata;
            response->num_tape_types = tape_types_array->len;
            g_ptr_array_free(tape_types_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_tape_partition_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_types = (ds3_str**)tape_types_array->pdata;
    response->num_tape_types = tape_types_array->len;
    g_ptr_array_free(tape_types_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_tape_partition_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_initiate_multipart_upload_result_response(const ds3_client* client, const ds3_request* request, ds3_initiate_multipart_upload_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_initiate_multipart_upload_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "InitiateMultipartUploadResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_initiate_multipart_upload_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "UploadId")) {
            response->upload_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_initiate_multipart_upload_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_initiate_multipart_upload_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_information_response(const ds3_client* client, const ds3_request* request, ds3_system_information_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_information_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_information_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "ApiVersion")) {
            response->api_version = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "BackendActivated")) {
            response->backend_activated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "BuildInformation")) {
            error = _parse_ds3_build_information_response(client, doc, child_node, &response->build_information);
        } else if (element_equal(child_node, "InstanceId")) {
            response->instance_id = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Now")) {
            response->now = xml_get_uint64(doc, child_node);
        } else if (element_equal(child_node, "SerialNumber")) {
            response->serial_number = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_information_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_information_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_health_verification_result_response(const ds3_client* client, const ds3_request* request, ds3_health_verification_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_health_verification_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_health_verification_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DatabaseFilesystemFreeSpace")) {
            xmlChar* text = xmlNodeListGetString(doc, child_node, 1);
            if (text == NULL) {
                continue;
            }
            response->database_filesystem_free_space = _match_ds3_database_physical_space_state(client->log, text);
            xmlFree(text);
        } else if (element_equal(child_node, "MsRequiredToVerifyDataPlannerHealth")) {
            response->ms_required_to_verify_data_planner_health = xml_get_uint64(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_health_verification_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_health_verification_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Failure")) {
            ds3_tape_failure_response* failures_response = NULL;
            error = _parse_ds3_tape_failure_response(client, doc, child_node, &failures_response);
            response->failures = (ds3_tape_failure_response**)failures_array->pdata;
            g_ptr_array_add(failures_array, failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->failures = (ds3_tape_failure_response**)failures_array->pdata;
    response->num_failures = failures_array->len;
    g_ptr_array_free(failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bucket_acl_list_response(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_acl_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* bucket_acls_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_acl_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "BucketAcl")) {
            ds3_bucket_acl_response* bucket_acls_response = NULL;
            error = _parse_ds3_bucket_acl_response(client, doc, child_node, &bucket_acls_response);
            response->bucket_acls = (ds3_bucket_acl_response**)bucket_acls_array->pdata;
            g_ptr_array_add(bucket_acls_array, bucket_acls_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_acl_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->bucket_acls = (ds3_bucket_acl_response**)bucket_acls_array->pdata;
    response->num_bucket_acls = bucket_acls_array->len;
    g_ptr_array_free(bucket_acls_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_acl_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_acl_list_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_acl_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* data_policy_acls_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_acl_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicyAcl")) {
            ds3_data_policy_acl_response* data_policy_acls_response = NULL;
            error = _parse_ds3_data_policy_acl_response(client, doc, child_node, &data_policy_acls_response);
            response->data_policy_acls = (ds3_data_policy_acl_response**)data_policy_acls_array->pdata;
            g_ptr_array_add(data_policy_acls_array, data_policy_acls_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_acl_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->data_policy_acls = (ds3_data_policy_acl_response**)data_policy_acls_array->pdata;
    response->num_data_policy_acls = data_policy_acls_array->len;
    g_ptr_array_free(data_policy_acls_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_acl_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bucket_list_response(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bucket_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* buckets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bucket_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            ds3_bucket_response* buckets_response = NULL;
            error = _parse_ds3_bucket_response(client, doc, child_node, &buckets_response);
            response->buckets = (ds3_bucket_response**)buckets_array->pdata;
            g_ptr_array_add(buckets_array, buckets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bucket_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->buckets = (ds3_bucket_response**)buckets_array->pdata;
    response->num_buckets = buckets_array->len;
    g_ptr_array_free(buckets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bucket_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_cache_filesystem_list_response(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_cache_filesystem_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* cache_filesystems_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_cache_filesystem_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CacheFilesystem")) {
            ds3_cache_filesystem_response* cache_filesystems_response = NULL;
            error = _parse_ds3_cache_filesystem_response(client, doc, child_node, &cache_filesystems_response);
            response->cache_filesystems = (ds3_cache_filesystem_response**)cache_filesystems_array->pdata;
            g_ptr_array_add(cache_filesystems_array, cache_filesystems_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_cache_filesystem_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->cache_filesystems = (ds3_cache_filesystem_response**)cache_filesystems_array->pdata;
    response->num_cache_filesystems = cache_filesystems_array->len;
    g_ptr_array_free(cache_filesystems_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_cache_filesystem_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_data_replication_rule_list_response(const ds3_client* client, const ds3_request* request, ds3_azure_data_replication_rule_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_data_replication_rule_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* azure_data_replication_rules_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_data_replication_rule_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureDataReplicationRule")) {
            ds3_azure_data_replication_rule_response* azure_data_replication_rules_response = NULL;
            error = _parse_ds3_azure_data_replication_rule_response(client, doc, child_node, &azure_data_replication_rules_response);
            response->azure_data_replication_rules = (ds3_azure_data_replication_rule_response**)azure_data_replication_rules_array->pdata;
            g_ptr_array_add(azure_data_replication_rules_array, azure_data_replication_rules_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_data_replication_rule_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->azure_data_replication_rules = (ds3_azure_data_replication_rule_response**)azure_data_replication_rules_array->pdata;
    response->num_azure_data_replication_rules = azure_data_replication_rules_array->len;
    g_ptr_array_free(azure_data_replication_rules_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_data_replication_rule_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_persistence_rule_list_response(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_persistence_rule_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* data_persistence_rules_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_persistence_rule_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPersistenceRule")) {
            ds3_data_persistence_rule_response* data_persistence_rules_response = NULL;
            error = _parse_ds3_data_persistence_rule_response(client, doc, child_node, &data_persistence_rules_response);
            response->data_persistence_rules = (ds3_data_persistence_rule_response**)data_persistence_rules_array->pdata;
            g_ptr_array_add(data_persistence_rules_array, data_persistence_rules_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_persistence_rule_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->data_persistence_rules = (ds3_data_persistence_rule_response**)data_persistence_rules_array->pdata;
    response->num_data_persistence_rules = data_persistence_rules_array->len;
    g_ptr_array_free(data_persistence_rules_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_persistence_rule_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_policy_list_response(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_policy_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* data_policies_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_policy_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DataPolicy")) {
            ds3_data_policy_response* data_policies_response = NULL;
            error = _parse_ds3_data_policy_response(client, doc, child_node, &data_policies_response);
            response->data_policies = (ds3_data_policy_response**)data_policies_array->pdata;
            g_ptr_array_add(data_policies_array, data_policies_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_policy_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->data_policies = (ds3_data_policy_response**)data_policies_array->pdata;
    response->num_data_policies = data_policies_array->len;
    g_ptr_array_free(data_policies_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_policy_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_data_replication_rule_list_response(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_data_replication_rule_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_data_replication_rules_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_data_replication_rule_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3DataReplicationRule")) {
            ds3_data_replication_rule_response* ds3_data_replication_rules_response = NULL;
            error = _parse_ds3_data_replication_rule_response(client, doc, child_node, &ds3_data_replication_rules_response);
            response->ds3_data_replication_rules = (ds3_data_replication_rule_response**)ds3_data_replication_rules_array->pdata;
            g_ptr_array_add(ds3_data_replication_rules_array, ds3_data_replication_rules_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_data_replication_rule_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_data_replication_rules = (ds3_data_replication_rule_response**)ds3_data_replication_rules_array->pdata;
    response->num_ds3_data_replication_rules = ds3_data_replication_rules_array->len;
    g_ptr_array_free(ds3_data_replication_rules_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_data_replication_rule_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_data_replication_rule_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_data_replication_rule_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_data_replication_rule_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_data_replication_rules_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_data_replication_rule_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3DataReplicationRule")) {
            ds3_s3_data_replication_rule_response* s3_data_replication_rules_response = NULL;
            error = _parse_ds3_s3_data_replication_rule_response(client, doc, child_node, &s3_data_replication_rules_response);
            response->s3_data_replication_rules = (ds3_s3_data_replication_rule_response**)s3_data_replication_rules_array->pdata;
            g_ptr_array_add(s3_data_replication_rules_array, s3_data_replication_rules_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_data_replication_rule_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_data_replication_rules = (ds3_s3_data_replication_rule_response**)s3_data_replication_rules_array->pdata;
    response->num_s3_data_replication_rules = s3_data_replication_rules_array->len;
    g_ptr_array_free(s3_data_replication_rules_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_data_replication_rule_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_degraded_blob_list_response(const ds3_client* client, const ds3_request* request, ds3_degraded_blob_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_degraded_blob_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* degraded_blobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_degraded_blob_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "DegradedBlob")) {
            ds3_degraded_blob_response* degraded_blobs_response = NULL;
            error = _parse_ds3_degraded_blob_response(client, doc, child_node, &degraded_blobs_response);
            response->degraded_blobs = (ds3_degraded_blob_response**)degraded_blobs_array->pdata;
            g_ptr_array_add(degraded_blobs_array, degraded_blobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_degraded_blob_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->degraded_blobs = (ds3_degraded_blob_response**)degraded_blobs_array->pdata;
    response->num_degraded_blobs = degraded_blobs_array->len;
    g_ptr_array_free(degraded_blobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_degraded_blob_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_azure_target_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_azure_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_azure_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_azure_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_azure_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobAzureTarget")) {
            ds3_suspect_blob_azure_target_response* suspect_blob_azure_targets_response = NULL;
            error = _parse_ds3_suspect_blob_azure_target_response(client, doc, child_node, &suspect_blob_azure_targets_response);
            response->suspect_blob_azure_targets = (ds3_suspect_blob_azure_target_response**)suspect_blob_azure_targets_array->pdata;
            g_ptr_array_add(suspect_blob_azure_targets_array, suspect_blob_azure_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_azure_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_azure_targets = (ds3_suspect_blob_azure_target_response**)suspect_blob_azure_targets_array->pdata;
    response->num_suspect_blob_azure_targets = suspect_blob_azure_targets_array->len;
    g_ptr_array_free(suspect_blob_azure_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_azure_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_ds3_target_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_ds3_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_ds3_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_ds3_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_ds3_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobDs3Target")) {
            ds3_suspect_blob_ds3_target_response* suspect_blob_ds3_targets_response = NULL;
            error = _parse_ds3_suspect_blob_ds3_target_response(client, doc, child_node, &suspect_blob_ds3_targets_response);
            response->suspect_blob_ds3_targets = (ds3_suspect_blob_ds3_target_response**)suspect_blob_ds3_targets_array->pdata;
            g_ptr_array_add(suspect_blob_ds3_targets_array, suspect_blob_ds3_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_ds3_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_ds3_targets = (ds3_suspect_blob_ds3_target_response**)suspect_blob_ds3_targets_array->pdata;
    response->num_suspect_blob_ds3_targets = suspect_blob_ds3_targets_array->len;
    g_ptr_array_free(suspect_blob_ds3_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_ds3_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_pool_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_pool_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_pool_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_pools_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_pool_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobPool")) {
            ds3_suspect_blob_pool_response* suspect_blob_pools_response = NULL;
            error = _parse_ds3_suspect_blob_pool_response(client, doc, child_node, &suspect_blob_pools_response);
            response->suspect_blob_pools = (ds3_suspect_blob_pool_response**)suspect_blob_pools_array->pdata;
            g_ptr_array_add(suspect_blob_pools_array, suspect_blob_pools_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_pool_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_pools = (ds3_suspect_blob_pool_response**)suspect_blob_pools_array->pdata;
    response->num_suspect_blob_pools = suspect_blob_pools_array->len;
    g_ptr_array_free(suspect_blob_pools_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_pool_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_s3_target_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_s3_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_s3_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_s3_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_s3_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobS3Target")) {
            ds3_suspect_blob_s3_target_response* suspect_blob_s3_targets_response = NULL;
            error = _parse_ds3_suspect_blob_s3_target_response(client, doc, child_node, &suspect_blob_s3_targets_response);
            response->suspect_blob_s3_targets = (ds3_suspect_blob_s3_target_response**)suspect_blob_s3_targets_array->pdata;
            g_ptr_array_add(suspect_blob_s3_targets_array, suspect_blob_s3_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_s3_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_s3_targets = (ds3_suspect_blob_s3_target_response**)suspect_blob_s3_targets_array->pdata;
    response->num_suspect_blob_s3_targets = suspect_blob_s3_targets_array->len;
    g_ptr_array_free(suspect_blob_s3_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_s3_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_suspect_blob_tape_list_response(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_tape_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_suspect_blob_tape_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* suspect_blob_tapes_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_suspect_blob_tape_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SuspectBlobTape")) {
            ds3_suspect_blob_tape_response* suspect_blob_tapes_response = NULL;
            error = _parse_ds3_suspect_blob_tape_response(client, doc, child_node, &suspect_blob_tapes_response);
            response->suspect_blob_tapes = (ds3_suspect_blob_tape_response**)suspect_blob_tapes_array->pdata;
            g_ptr_array_add(suspect_blob_tapes_array, suspect_blob_tapes_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_suspect_blob_tape_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->suspect_blob_tapes = (ds3_suspect_blob_tape_response**)suspect_blob_tapes_array->pdata;
    response->num_suspect_blob_tapes = suspect_blob_tapes_array->len;
    g_ptr_array_free(suspect_blob_tapes_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_suspect_blob_tape_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3Object")) {
            ds3_s3_object_response* s3_objects_response = NULL;
            error = _parse_ds3_s3_object_response(client, doc, child_node, &s3_objects_response);
            response->s3_objects = (ds3_s3_object_response**)s3_objects_array->pdata;
            g_ptr_array_add(s3_objects_array, s3_objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_objects = (ds3_s3_object_response**)s3_objects_array->pdata;
    response->num_s3_objects = s3_objects_array->len;
    g_ptr_array_free(s3_objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_member_list_response(const ds3_client* client, const ds3_request* request, ds3_group_member_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_member_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* group_members_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_member_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "GroupMember")) {
            ds3_group_member_response* group_members_response = NULL;
            error = _parse_ds3_group_member_response(client, doc, child_node, &group_members_response);
            response->group_members = (ds3_group_member_response**)group_members_array->pdata;
            g_ptr_array_add(group_members_array, group_members_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_member_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->group_members = (ds3_group_member_response**)group_members_array->pdata;
    response->num_group_members = group_members_array->len;
    g_ptr_array_free(group_members_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_member_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_group_list_response(const ds3_client* client, const ds3_request* request, ds3_group_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_group_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* groups_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_group_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Group")) {
            ds3_group_response* groups_response = NULL;
            error = _parse_ds3_group_response(client, doc, child_node, &groups_response);
            response->groups = (ds3_group_response**)groups_array->pdata;
            g_ptr_array_add(groups_array, groups_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_group_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->groups = (ds3_group_response**)groups_array->pdata;
    response->num_groups = groups_array->len;
    g_ptr_array_free(groups_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_group_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_active_job_list_response(const ds3_client* client, const ds3_request* request, ds3_active_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_active_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* active_jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_active_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Job")) {
            ds3_active_job_response* active_jobs_response = NULL;
            error = _parse_ds3_active_job_response(client, doc, child_node, &active_jobs_response);
            response->active_jobs = (ds3_active_job_response**)active_jobs_array->pdata;
            g_ptr_array_add(active_jobs_array, active_jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_active_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->active_jobs = (ds3_active_job_response**)active_jobs_array->pdata;
    response->num_active_jobs = active_jobs_array->len;
    g_ptr_array_free(active_jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_active_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_canceled_job_list_response(const ds3_client* client, const ds3_request* request, ds3_canceled_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_canceled_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* canceled_jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_canceled_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CanceledJob")) {
            ds3_canceled_job_response* canceled_jobs_response = NULL;
            error = _parse_ds3_canceled_job_response(client, doc, child_node, &canceled_jobs_response);
            response->canceled_jobs = (ds3_canceled_job_response**)canceled_jobs_array->pdata;
            g_ptr_array_add(canceled_jobs_array, canceled_jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_canceled_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->canceled_jobs = (ds3_canceled_job_response**)canceled_jobs_array->pdata;
    response->num_canceled_jobs = canceled_jobs_array->len;
    g_ptr_array_free(canceled_jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_canceled_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_completed_job_list_response(const ds3_client* client, const ds3_request* request, ds3_completed_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_completed_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* completed_jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_completed_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CompletedJob")) {
            ds3_completed_job_response* completed_jobs_response = NULL;
            error = _parse_ds3_completed_job_response(client, doc, child_node, &completed_jobs_response);
            response->completed_jobs = (ds3_completed_job_response**)completed_jobs_array->pdata;
            g_ptr_array_add(completed_jobs_array, completed_jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_completed_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->completed_jobs = (ds3_completed_job_response**)completed_jobs_array->pdata;
    response->num_completed_jobs = completed_jobs_array->len;
    g_ptr_array_free(completed_jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_completed_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_node_list_response(const ds3_client* client, const ds3_request* request, ds3_node_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_node_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* nodes_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_node_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Node")) {
            ds3_node_response* nodes_response = NULL;
            error = _parse_ds3_node_response(client, doc, child_node, &nodes_response);
            response->nodes = (ds3_node_response**)nodes_array->pdata;
            g_ptr_array_add(nodes_array, nodes_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_node_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->nodes = (ds3_node_response**)nodes_array->pdata;
    response->num_nodes = nodes_array->len;
    g_ptr_array_free(nodes_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_node_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* azure_target_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureTargetFailureNotificationRegistration")) {
            ds3_azure_target_failure_notification_registration_response* azure_target_failure_notification_registrations_response = NULL;
            error = _parse_ds3_azure_target_failure_notification_registration_response(client, doc, child_node, &azure_target_failure_notification_registrations_response);
            response->azure_target_failure_notification_registrations = (ds3_azure_target_failure_notification_registration_response**)azure_target_failure_notification_registrations_array->pdata;
            g_ptr_array_add(azure_target_failure_notification_registrations_array, azure_target_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->azure_target_failure_notification_registrations = (ds3_azure_target_failure_notification_registration_response**)azure_target_failure_notification_registrations_array->pdata;
    response->num_azure_target_failure_notification_registrations = azure_target_failure_notification_registrations_array->len;
    g_ptr_array_free(azure_target_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_target_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3TargetFailureNotificationRegistration")) {
            ds3_target_failure_notification_registration_response* ds3_target_failure_notification_registrations_response = NULL;
            error = _parse_ds3_target_failure_notification_registration_response(client, doc, child_node, &ds3_target_failure_notification_registrations_response);
            response->ds3_target_failure_notification_registrations = (ds3_target_failure_notification_registration_response**)ds3_target_failure_notification_registrations_array->pdata;
            g_ptr_array_add(ds3_target_failure_notification_registrations_array, ds3_target_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_target_failure_notification_registrations = (ds3_target_failure_notification_registration_response**)ds3_target_failure_notification_registrations_array->pdata;
    response->num_ds3_target_failure_notification_registrations = ds3_target_failure_notification_registrations_array->len;
    g_ptr_array_free(ds3_target_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_completed_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_completed_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* job_completed_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_completed_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "JobCompletedNotificationRegistration")) {
            ds3_job_completed_notification_registration_response* job_completed_notification_registrations_response = NULL;
            error = _parse_ds3_job_completed_notification_registration_response(client, doc, child_node, &job_completed_notification_registrations_response);
            response->job_completed_notification_registrations = (ds3_job_completed_notification_registration_response**)job_completed_notification_registrations_array->pdata;
            g_ptr_array_add(job_completed_notification_registrations_array, job_completed_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_completed_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->job_completed_notification_registrations = (ds3_job_completed_notification_registration_response**)job_completed_notification_registrations_array->pdata;
    response->num_job_completed_notification_registrations = job_completed_notification_registrations_array->len;
    g_ptr_array_free(job_completed_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_completed_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_created_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_created_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* job_created_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_created_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "JobCreatedNotificationRegistration")) {
            ds3_job_created_notification_registration_response* job_created_notification_registrations_response = NULL;
            error = _parse_ds3_job_created_notification_registration_response(client, doc, child_node, &job_created_notification_registrations_response);
            response->job_created_notification_registrations = (ds3_job_created_notification_registration_response**)job_created_notification_registrations_array->pdata;
            g_ptr_array_add(job_created_notification_registrations_array, job_created_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_created_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->job_created_notification_registrations = (ds3_job_created_notification_registration_response**)job_created_notification_registrations_array->pdata;
    response->num_job_created_notification_registrations = job_created_notification_registrations_array->len;
    g_ptr_array_free(job_created_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_created_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_creation_failed_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_creation_failed_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* job_creation_failed_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_creation_failed_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "JobCreationFailedNotificationRegistration")) {
            ds3_job_creation_failed_notification_registration_response* job_creation_failed_notification_registrations_response = NULL;
            error = _parse_ds3_job_creation_failed_notification_registration_response(client, doc, child_node, &job_creation_failed_notification_registrations_response);
            response->job_creation_failed_notification_registrations = (ds3_job_creation_failed_notification_registration_response**)job_creation_failed_notification_registrations_array->pdata;
            g_ptr_array_add(job_creation_failed_notification_registrations_array, job_creation_failed_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_creation_failed_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->job_creation_failed_notification_registrations = (ds3_job_creation_failed_notification_registration_response**)job_creation_failed_notification_registrations_array->pdata;
    response->num_job_creation_failed_notification_registrations = job_creation_failed_notification_registrations_array->len;
    g_ptr_array_free(job_creation_failed_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_creation_failed_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_cached_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_cached_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_object_cached_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_cached_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3ObjectCachedNotificationRegistration")) {
            ds3_s3_object_cached_notification_registration_response* s3_object_cached_notification_registrations_response = NULL;
            error = _parse_ds3_s3_object_cached_notification_registration_response(client, doc, child_node, &s3_object_cached_notification_registrations_response);
            response->s3_object_cached_notification_registrations = (ds3_s3_object_cached_notification_registration_response**)s3_object_cached_notification_registrations_array->pdata;
            g_ptr_array_add(s3_object_cached_notification_registrations_array, s3_object_cached_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_cached_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_object_cached_notification_registrations = (ds3_s3_object_cached_notification_registration_response**)s3_object_cached_notification_registrations_array->pdata;
    response->num_s3_object_cached_notification_registrations = s3_object_cached_notification_registrations_array->len;
    g_ptr_array_free(s3_object_cached_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_cached_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_lost_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_lost_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_object_lost_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_lost_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3ObjectLostNotificationRegistration")) {
            ds3_s3_object_lost_notification_registration_response* s3_object_lost_notification_registrations_response = NULL;
            error = _parse_ds3_s3_object_lost_notification_registration_response(client, doc, child_node, &s3_object_lost_notification_registrations_response);
            response->s3_object_lost_notification_registrations = (ds3_s3_object_lost_notification_registration_response**)s3_object_lost_notification_registrations_array->pdata;
            g_ptr_array_add(s3_object_lost_notification_registrations_array, s3_object_lost_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_lost_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_object_lost_notification_registrations = (ds3_s3_object_lost_notification_registration_response**)s3_object_lost_notification_registrations_array->pdata;
    response->num_s3_object_lost_notification_registrations = s3_object_lost_notification_registrations_array->len;
    g_ptr_array_free(s3_object_lost_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_lost_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_object_persisted_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_object_persisted_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_object_persisted_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_object_persisted_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3ObjectPersistedNotificationRegistration")) {
            ds3_s3_object_persisted_notification_registration_response* s3_object_persisted_notification_registrations_response = NULL;
            error = _parse_ds3_s3_object_persisted_notification_registration_response(client, doc, child_node, &s3_object_persisted_notification_registrations_response);
            response->s3_object_persisted_notification_registrations = (ds3_s3_object_persisted_notification_registration_response**)s3_object_persisted_notification_registrations_array->pdata;
            g_ptr_array_add(s3_object_persisted_notification_registrations_array, s3_object_persisted_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_object_persisted_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_object_persisted_notification_registrations = (ds3_s3_object_persisted_notification_registration_response**)s3_object_persisted_notification_registrations_array->pdata;
    response->num_s3_object_persisted_notification_registrations = s3_object_persisted_notification_registrations_array->len;
    g_ptr_array_free(s3_object_persisted_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_object_persisted_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pool_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PoolFailureNotificationRegistration")) {
            ds3_pool_failure_notification_registration_response* pool_failure_notification_registrations_response = NULL;
            error = _parse_ds3_pool_failure_notification_registration_response(client, doc, child_node, &pool_failure_notification_registrations_response);
            response->pool_failure_notification_registrations = (ds3_pool_failure_notification_registration_response**)pool_failure_notification_registrations_array->pdata;
            g_ptr_array_add(pool_failure_notification_registrations_array, pool_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pool_failure_notification_registrations = (ds3_pool_failure_notification_registration_response**)pool_failure_notification_registrations_array->pdata;
    response->num_pool_failure_notification_registrations = pool_failure_notification_registrations_array->len;
    g_ptr_array_free(pool_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_target_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3TargetFailureNotificationRegistration")) {
            ds3_s3_target_failure_notification_registration_response* s3_target_failure_notification_registrations_response = NULL;
            error = _parse_ds3_s3_target_failure_notification_registration_response(client, doc, child_node, &s3_target_failure_notification_registrations_response);
            response->s3_target_failure_notification_registrations = (ds3_s3_target_failure_notification_registration_response**)s3_target_failure_notification_registrations_array->pdata;
            g_ptr_array_add(s3_target_failure_notification_registrations_array, s3_target_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_target_failure_notification_registrations = (ds3_s3_target_failure_notification_registration_response**)s3_target_failure_notification_registrations_array->pdata;
    response->num_s3_target_failure_notification_registrations = s3_target_failure_notification_registrations_array->len;
    g_ptr_array_free(s3_target_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domain_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomainFailureNotificationRegistration")) {
            ds3_storage_domain_failure_notification_registration_response* storage_domain_failure_notification_registrations_response = NULL;
            error = _parse_ds3_storage_domain_failure_notification_registration_response(client, doc, child_node, &storage_domain_failure_notification_registrations_response);
            response->storage_domain_failure_notification_registrations = (ds3_storage_domain_failure_notification_registration_response**)storage_domain_failure_notification_registrations_array->pdata;
            g_ptr_array_add(storage_domain_failure_notification_registrations_array, storage_domain_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domain_failure_notification_registrations = (ds3_storage_domain_failure_notification_registration_response**)storage_domain_failure_notification_registrations_array->pdata;
    response->num_storage_domain_failure_notification_registrations = storage_domain_failure_notification_registrations_array->len;
    g_ptr_array_free(storage_domain_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* system_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SystemFailureNotificationRegistration")) {
            ds3_system_failure_notification_registration_response* system_failure_notification_registrations_response = NULL;
            error = _parse_ds3_system_failure_notification_registration_response(client, doc, child_node, &system_failure_notification_registrations_response);
            response->system_failure_notification_registrations = (ds3_system_failure_notification_registration_response**)system_failure_notification_registrations_array->pdata;
            g_ptr_array_add(system_failure_notification_registrations_array, system_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->system_failure_notification_registrations = (ds3_system_failure_notification_registration_response**)system_failure_notification_registrations_array->pdata;
    response->num_system_failure_notification_registrations = system_failure_notification_registrations_array->len;
    g_ptr_array_free(system_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeFailureNotificationRegistration")) {
            ds3_tape_failure_notification_registration_response* tape_failure_notification_registrations_response = NULL;
            error = _parse_ds3_tape_failure_notification_registration_response(client, doc, child_node, &tape_failure_notification_registrations_response);
            response->tape_failure_notification_registrations = (ds3_tape_failure_notification_registration_response**)tape_failure_notification_registrations_array->pdata;
            g_ptr_array_add(tape_failure_notification_registrations_array, tape_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_failure_notification_registrations = (ds3_tape_failure_notification_registration_response**)tape_failure_notification_registrations_array->pdata;
    response->num_tape_failure_notification_registrations = tape_failure_notification_registrations_array->len;
    g_ptr_array_free(tape_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_failure_notification_registration_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_failure_notification_registration_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_partition_failure_notification_registrations_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_failure_notification_registration_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartitionFailureNotificationRegistration")) {
            ds3_tape_partition_failure_notification_registration_response* tape_partition_failure_notification_registrations_response = NULL;
            error = _parse_ds3_tape_partition_failure_notification_registration_response(client, doc, child_node, &tape_partition_failure_notification_registrations_response);
            response->tape_partition_failure_notification_registrations = (ds3_tape_partition_failure_notification_registration_response**)tape_partition_failure_notification_registrations_array->pdata;
            g_ptr_array_add(tape_partition_failure_notification_registrations_array, tape_partition_failure_notification_registrations_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_notification_registration_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_partition_failure_notification_registrations = (ds3_tape_partition_failure_notification_registration_response**)tape_partition_failure_notification_registrations_array->pdata;
    response->num_tape_partition_failure_notification_registrations = tape_partition_failure_notification_registrations_array->len;
    g_ptr_array_free(tape_partition_failure_notification_registrations_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_failure_notification_registration_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pool_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PoolFailure")) {
            ds3_pool_failure_response* pool_failures_response = NULL;
            error = _parse_ds3_pool_failure_response(client, doc, child_node, &pool_failures_response);
            response->pool_failures = (ds3_pool_failure_response**)pool_failures_array->pdata;
            g_ptr_array_add(pool_failures_array, pool_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pool_failures = (ds3_pool_failure_response**)pool_failures_array->pdata;
    response->num_pool_failures = pool_failures_array->len;
    g_ptr_array_free(pool_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_partition_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_partition_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_partition_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pool_partitions_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_partition_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "PoolPartition")) {
            ds3_pool_partition_response* pool_partitions_response = NULL;
            error = _parse_ds3_pool_partition_response(client, doc, child_node, &pool_partitions_response);
            response->pool_partitions = (ds3_pool_partition_response**)pool_partitions_array->pdata;
            g_ptr_array_add(pool_partitions_array, pool_partitions_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_partition_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pool_partitions = (ds3_pool_partition_response**)pool_partitions_array->pdata;
    response->num_pool_partitions = pool_partitions_array->len;
    g_ptr_array_free(pool_partitions_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_partition_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_pool_list_response(const ds3_client* client, const ds3_request* request, ds3_pool_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_pool_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* pools_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_pool_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Pool")) {
            ds3_pool_response* pools_response = NULL;
            error = _parse_ds3_pool_response(client, doc, child_node, &pools_response);
            response->pools = (ds3_pool_response**)pools_array->pdata;
            g_ptr_array_add(pools_array, pools_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_pool_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->pools = (ds3_pool_response**)pools_array->pdata;
    response->num_pools = pools_array->len;
    g_ptr_array_free(pools_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_pool_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domain_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomainFailure")) {
            ds3_storage_domain_failure_response* storage_domain_failures_response = NULL;
            error = _parse_ds3_storage_domain_failure_response(client, doc, child_node, &storage_domain_failures_response);
            response->storage_domain_failures = (ds3_storage_domain_failure_response**)storage_domain_failures_array->pdata;
            g_ptr_array_add(storage_domain_failures_array, storage_domain_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domain_failures = (ds3_storage_domain_failure_response**)storage_domain_failures_array->pdata;
    response->num_storage_domain_failures = storage_domain_failures_array->len;
    g_ptr_array_free(storage_domain_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_member_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_member_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domain_members_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_member_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomainMember")) {
            ds3_storage_domain_member_response* storage_domain_members_response = NULL;
            error = _parse_ds3_storage_domain_member_response(client, doc, child_node, &storage_domain_members_response);
            response->storage_domain_members = (ds3_storage_domain_member_response**)storage_domain_members_array->pdata;
            g_ptr_array_add(storage_domain_members_array, storage_domain_members_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_member_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domain_members = (ds3_storage_domain_member_response**)storage_domain_members_array->pdata;
    response->num_storage_domain_members = storage_domain_members_array->len;
    g_ptr_array_free(storage_domain_members_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_member_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_storage_domain_list_response(const ds3_client* client, const ds3_request* request, ds3_storage_domain_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_storage_domain_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* storage_domains_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_storage_domain_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "StorageDomain")) {
            ds3_storage_domain_response* storage_domains_response = NULL;
            error = _parse_ds3_storage_domain_response(client, doc, child_node, &storage_domains_response);
            response->storage_domains = (ds3_storage_domain_response**)storage_domains_array->pdata;
            g_ptr_array_add(storage_domains_array, storage_domains_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_storage_domain_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->storage_domains = (ds3_storage_domain_response**)storage_domains_array->pdata;
    response->num_storage_domains = storage_domains_array->len;
    g_ptr_array_free(storage_domains_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_storage_domain_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_feature_key_list_response(const ds3_client* client, const ds3_request* request, ds3_feature_key_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_feature_key_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* feature_keys_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_feature_key_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "FeatureKey")) {
            ds3_feature_key_response* feature_keys_response = NULL;
            error = _parse_ds3_feature_key_response(client, doc, child_node, &feature_keys_response);
            response->feature_keys = (ds3_feature_key_response**)feature_keys_array->pdata;
            g_ptr_array_add(feature_keys_array, feature_keys_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_feature_key_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->feature_keys = (ds3_feature_key_response**)feature_keys_array->pdata;
    response->num_feature_keys = feature_keys_array->len;
    g_ptr_array_free(feature_keys_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_feature_key_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_system_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_system_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_system_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* system_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_system_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "SystemFailure")) {
            ds3_system_failure_response* system_failures_response = NULL;
            error = _parse_ds3_system_failure_response(client, doc, child_node, &system_failures_response);
            response->system_failures = (ds3_system_failure_response**)system_failures_array->pdata;
            g_ptr_array_add(system_failures_array, system_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_system_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->system_failures = (ds3_system_failure_response**)system_failures_array->pdata;
    response->num_system_failures = system_failures_array->len;
    g_ptr_array_free(system_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_system_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_density_directive_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_density_directive_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_density_directives_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_density_directive_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeDensityDirective")) {
            ds3_tape_density_directive_response* tape_density_directives_response = NULL;
            error = _parse_ds3_tape_density_directive_response(client, doc, child_node, &tape_density_directives_response);
            response->tape_density_directives = (ds3_tape_density_directive_response**)tape_density_directives_array->pdata;
            g_ptr_array_add(tape_density_directives_array, tape_density_directives_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_density_directive_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_density_directives = (ds3_tape_density_directive_response**)tape_density_directives_array->pdata;
    response->num_tape_density_directives = tape_density_directives_array->len;
    g_ptr_array_free(tape_density_directives_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_density_directive_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_drive_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_drive_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_drive_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_drives_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_drive_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeDrive")) {
            ds3_tape_drive_response* tape_drives_response = NULL;
            error = _parse_ds3_tape_drive_response(client, doc, child_node, &tape_drives_response);
            response->tape_drives = (ds3_tape_drive_response**)tape_drives_array->pdata;
            g_ptr_array_add(tape_drives_array, tape_drives_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_drive_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_drives = (ds3_tape_drive_response**)tape_drives_array->pdata;
    response->num_tape_drives = tape_drives_array->len;
    g_ptr_array_free(tape_drives_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_drive_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_detailed_tape_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_detailed_tape_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* detailed_tape_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_detailed_tape_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeFailure")) {
            ds3_detailed_tape_failure_response* detailed_tape_failures_response = NULL;
            error = _parse_ds3_detailed_tape_failure_response(client, doc, child_node, &detailed_tape_failures_response);
            response->detailed_tape_failures = (ds3_detailed_tape_failure_response**)detailed_tape_failures_array->pdata;
            g_ptr_array_add(detailed_tape_failures_array, detailed_tape_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_tape_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->detailed_tape_failures = (ds3_detailed_tape_failure_response**)detailed_tape_failures_array->pdata;
    response->num_detailed_tape_failures = detailed_tape_failures_array->len;
    g_ptr_array_free(detailed_tape_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_tape_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_library_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_library_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_library_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_libraries_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_library_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapeLibrary")) {
            ds3_tape_library_response* tape_libraries_response = NULL;
            error = _parse_ds3_tape_library_response(client, doc, child_node, &tape_libraries_response);
            response->tape_libraries = (ds3_tape_library_response**)tape_libraries_array->pdata;
            g_ptr_array_add(tape_libraries_array, tape_libraries_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_library_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_libraries = (ds3_tape_library_response**)tape_libraries_array->pdata;
    response->num_tape_libraries = tape_libraries_array->len;
    g_ptr_array_free(tape_libraries_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_library_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_partition_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartitionFailure")) {
            ds3_tape_partition_failure_response* tape_partition_failures_response = NULL;
            error = _parse_ds3_tape_partition_failure_response(client, doc, child_node, &tape_partition_failures_response);
            response->tape_partition_failures = (ds3_tape_partition_failure_response**)tape_partition_failures_array->pdata;
            g_ptr_array_add(tape_partition_failures_array, tape_partition_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_partition_failures = (ds3_tape_partition_failure_response**)tape_partition_failures_array->pdata;
    response->num_tape_partition_failures = tape_partition_failures_array->len;
    g_ptr_array_free(tape_partition_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_partition_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_partition_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_partition_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tape_partitions_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_partition_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartition")) {
            ds3_tape_partition_response* tape_partitions_response = NULL;
            error = _parse_ds3_tape_partition_response(client, doc, child_node, &tape_partitions_response);
            response->tape_partitions = (ds3_tape_partition_response**)tape_partitions_array->pdata;
            g_ptr_array_add(tape_partitions_array, tape_partitions_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_partition_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tape_partitions = (ds3_tape_partition_response**)tape_partitions_array->pdata;
    response->num_tape_partitions = tape_partitions_array->len;
    g_ptr_array_free(tape_partitions_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_partition_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_named_detailed_tape_partition_list_response(const ds3_client* client, const ds3_request* request, ds3_named_detailed_tape_partition_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_named_detailed_tape_partition_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* named_detailed_tape_partitions_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_named_detailed_tape_partition_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "TapePartition")) {
            ds3_named_detailed_tape_partition_response* named_detailed_tape_partitions_response = NULL;
            error = _parse_ds3_named_detailed_tape_partition_response(client, doc, child_node, &named_detailed_tape_partitions_response);
            response->named_detailed_tape_partitions = (ds3_named_detailed_tape_partition_response**)named_detailed_tape_partitions_array->pdata;
            g_ptr_array_add(named_detailed_tape_partitions_array, named_detailed_tape_partitions_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_named_detailed_tape_partition_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->named_detailed_tape_partitions = (ds3_named_detailed_tape_partition_response**)named_detailed_tape_partitions_array->pdata;
    response->num_named_detailed_tape_partitions = named_detailed_tape_partitions_array->len;
    g_ptr_array_free(named_detailed_tape_partitions_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_named_detailed_tape_partition_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_tape_list_response(const ds3_client* client, const ds3_request* request, ds3_tape_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_tape_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* tapes_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_tape_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Tape")) {
            ds3_tape_response* tapes_response = NULL;
            error = _parse_ds3_tape_response(client, doc, child_node, &tapes_response);
            response->tapes = (ds3_tape_response**)tapes_array->pdata;
            g_ptr_array_add(tapes_array, tapes_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_tape_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->tapes = (ds3_tape_response**)tapes_array->pdata;
    response->num_tapes = tapes_array->len;
    g_ptr_array_free(tapes_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_tape_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_bucket_name_list_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_bucket_name_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_bucket_name_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* azure_target_bucket_names_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_bucket_name_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureTargetBucketName")) {
            ds3_azure_target_bucket_name_response* azure_target_bucket_names_response = NULL;
            error = _parse_ds3_azure_target_bucket_name_response(client, doc, child_node, &azure_target_bucket_names_response);
            response->azure_target_bucket_names = (ds3_azure_target_bucket_name_response**)azure_target_bucket_names_array->pdata;
            g_ptr_array_add(azure_target_bucket_names_array, azure_target_bucket_names_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_bucket_name_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->azure_target_bucket_names = (ds3_azure_target_bucket_name_response**)azure_target_bucket_names_array->pdata;
    response->num_azure_target_bucket_names = azure_target_bucket_names_array->len;
    g_ptr_array_free(azure_target_bucket_names_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_bucket_name_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* azure_target_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureTargetFailure")) {
            ds3_azure_target_failure_response* azure_target_failures_response = NULL;
            error = _parse_ds3_azure_target_failure_response(client, doc, child_node, &azure_target_failures_response);
            response->azure_target_failures = (ds3_azure_target_failure_response**)azure_target_failures_array->pdata;
            g_ptr_array_add(azure_target_failures_array, azure_target_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->azure_target_failures = (ds3_azure_target_failure_response**)azure_target_failures_array->pdata;
    response->num_azure_target_failures = azure_target_failures_array->len;
    g_ptr_array_free(azure_target_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_read_preference_list_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_read_preference_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_read_preference_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* azure_target_read_preferences_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_read_preference_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureTargetReadPreference")) {
            ds3_azure_target_read_preference_response* azure_target_read_preferences_response = NULL;
            error = _parse_ds3_azure_target_read_preference_response(client, doc, child_node, &azure_target_read_preferences_response);
            response->azure_target_read_preferences = (ds3_azure_target_read_preference_response**)azure_target_read_preferences_array->pdata;
            g_ptr_array_add(azure_target_read_preferences_array, azure_target_read_preferences_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_read_preference_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->azure_target_read_preferences = (ds3_azure_target_read_preference_response**)azure_target_read_preferences_array->pdata;
    response->num_azure_target_read_preferences = azure_target_read_preferences_array->len;
    g_ptr_array_free(azure_target_read_preferences_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_read_preference_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_azure_target_list_response(const ds3_client* client, const ds3_request* request, ds3_azure_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_azure_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* azure_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_azure_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureTarget")) {
            ds3_azure_target_response* azure_targets_response = NULL;
            error = _parse_ds3_azure_target_response(client, doc, child_node, &azure_targets_response);
            response->azure_targets = (ds3_azure_target_response**)azure_targets_array->pdata;
            g_ptr_array_add(azure_targets_array, azure_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_azure_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->azure_targets = (ds3_azure_target_response**)azure_targets_array->pdata;
    response->num_azure_targets = azure_targets_array->len;
    g_ptr_array_free(azure_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_azure_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_target_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_target_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3TargetFailure")) {
            ds3_target_failure_response* ds3_target_failures_response = NULL;
            error = _parse_ds3_target_failure_response(client, doc, child_node, &ds3_target_failures_response);
            response->ds3_target_failures = (ds3_target_failure_response**)ds3_target_failures_array->pdata;
            g_ptr_array_add(ds3_target_failures_array, ds3_target_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_target_failures = (ds3_target_failure_response**)ds3_target_failures_array->pdata;
    response->num_ds3_target_failures = ds3_target_failures_array->len;
    g_ptr_array_free(ds3_target_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_read_preference_list_response(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_read_preference_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_target_read_preferences_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_read_preference_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3TargetReadPreference")) {
            ds3_target_read_preference_response* ds3_target_read_preferences_response = NULL;
            error = _parse_ds3_target_read_preference_response(client, doc, child_node, &ds3_target_read_preferences_response);
            response->ds3_target_read_preferences = (ds3_target_read_preference_response**)ds3_target_read_preferences_array->pdata;
            g_ptr_array_add(ds3_target_read_preferences_array, ds3_target_read_preferences_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_read_preference_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_target_read_preferences = (ds3_target_read_preference_response**)ds3_target_read_preferences_array->pdata;
    response->num_ds3_target_read_preferences = ds3_target_read_preferences_array->len;
    g_ptr_array_free(ds3_target_read_preferences_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_read_preference_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_target_list_response(const ds3_client* client, const ds3_request* request, ds3_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* ds3_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Ds3Target")) {
            ds3_target_response* ds3_targets_response = NULL;
            error = _parse_ds3_target_response(client, doc, child_node, &ds3_targets_response);
            response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
            g_ptr_array_add(ds3_targets_array, ds3_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
    response->num_ds3_targets = ds3_targets_array->len;
    g_ptr_array_free(ds3_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_bucket_name_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_bucket_name_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_bucket_name_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_target_bucket_names_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_bucket_name_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3TargetBucketName")) {
            ds3_s3_target_bucket_name_response* s3_target_bucket_names_response = NULL;
            error = _parse_ds3_s3_target_bucket_name_response(client, doc, child_node, &s3_target_bucket_names_response);
            response->s3_target_bucket_names = (ds3_s3_target_bucket_name_response**)s3_target_bucket_names_array->pdata;
            g_ptr_array_add(s3_target_bucket_names_array, s3_target_bucket_names_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_bucket_name_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_target_bucket_names = (ds3_s3_target_bucket_name_response**)s3_target_bucket_names_array->pdata;
    response->num_s3_target_bucket_names = s3_target_bucket_names_array->len;
    g_ptr_array_free(s3_target_bucket_names_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_bucket_name_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_failure_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_failure_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_failure_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_target_failures_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_failure_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3TargetFailure")) {
            ds3_s3_target_failure_response* s3_target_failures_response = NULL;
            error = _parse_ds3_s3_target_failure_response(client, doc, child_node, &s3_target_failures_response);
            response->s3_target_failures = (ds3_s3_target_failure_response**)s3_target_failures_array->pdata;
            g_ptr_array_add(s3_target_failures_array, s3_target_failures_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_failure_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_target_failures = (ds3_s3_target_failure_response**)s3_target_failures_array->pdata;
    response->num_s3_target_failures = s3_target_failures_array->len;
    g_ptr_array_free(s3_target_failures_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_failure_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_read_preference_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_read_preference_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_read_preference_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_target_read_preferences_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_read_preference_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3TargetReadPreference")) {
            ds3_s3_target_read_preference_response* s3_target_read_preferences_response = NULL;
            error = _parse_ds3_s3_target_read_preference_response(client, doc, child_node, &s3_target_read_preferences_response);
            response->s3_target_read_preferences = (ds3_s3_target_read_preference_response**)s3_target_read_preferences_array->pdata;
            g_ptr_array_add(s3_target_read_preferences_array, s3_target_read_preferences_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_read_preference_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_target_read_preferences = (ds3_s3_target_read_preference_response**)s3_target_read_preferences_array->pdata;
    response->num_s3_target_read_preferences = s3_target_read_preferences_array->len;
    g_ptr_array_free(s3_target_read_preferences_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_read_preference_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_s3_target_list_response(const ds3_client* client, const ds3_request* request, ds3_s3_target_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_s3_target_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* s3_targets_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_s3_target_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "S3Target")) {
            ds3_s3_target_response* s3_targets_response = NULL;
            error = _parse_ds3_s3_target_response(client, doc, child_node, &s3_targets_response);
            response->s3_targets = (ds3_s3_target_response**)s3_targets_array->pdata;
            g_ptr_array_add(s3_targets_array, s3_targets_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_s3_target_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->s3_targets = (ds3_s3_target_response**)s3_targets_array->pdata;
    response->num_s3_targets = s3_targets_array->len;
    g_ptr_array_free(s3_targets_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_s3_target_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_spectra_user_list_response(const ds3_client* client, const ds3_request* request, ds3_spectra_user_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_spectra_user_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* spectra_users_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_spectra_user_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "User")) {
            ds3_spectra_user_response* spectra_users_response = NULL;
            error = _parse_ds3_spectra_user_response(client, doc, child_node, &spectra_users_response);
            response->spectra_users = (ds3_spectra_user_response**)spectra_users_array->pdata;
            g_ptr_array_add(spectra_users_array, spectra_users_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_spectra_user_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->spectra_users = (ds3_spectra_user_response**)spectra_users_array->pdata;
    response->num_spectra_users = spectra_users_array->len;
    g_ptr_array_free(spectra_users_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_spectra_user_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_physical_placement_response(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_physical_placement_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_physical_placement_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "AzureTargets")) {
            GPtrArray* azure_targets_array;
            error = _parse_ds3_azure_target_response_array(client, doc, child_node, &azure_targets_array);
            response->azure_targets = (ds3_azure_target_response**)azure_targets_array->pdata;
            response->num_azure_targets = azure_targets_array->len;
            g_ptr_array_free(azure_targets_array, FALSE);
        } else if (element_equal(child_node, "Ds3Targets")) {
            GPtrArray* ds3_targets_array;
            error = _parse_ds3_target_response_array(client, doc, child_node, &ds3_targets_array);
            response->ds3_targets = (ds3_target_response**)ds3_targets_array->pdata;
            response->num_ds3_targets = ds3_targets_array->len;
            g_ptr_array_free(ds3_targets_array, FALSE);
        } else if (element_equal(child_node, "Pools")) {
            GPtrArray* pools_array;
            error = _parse_ds3_pool_response_array(client, doc, child_node, &pools_array);
            response->pools = (ds3_pool_response**)pools_array->pdata;
            response->num_pools = pools_array->len;
            g_ptr_array_free(pools_array, FALSE);
        } else if (element_equal(child_node, "S3Targets")) {
            GPtrArray* s3_targets_array;
            error = _parse_ds3_s3_target_response_array(client, doc, child_node, &s3_targets_array);
            response->s3_targets = (ds3_s3_target_response**)s3_targets_array->pdata;
            response->num_s3_targets = s3_targets_array->len;
            g_ptr_array_free(s3_targets_array, FALSE);
        } else if (element_equal(child_node, "Tapes")) {
            GPtrArray* tapes_array;
            error = _parse_ds3_tape_response_array(client, doc, child_node, &tapes_array);
            response->tapes = (ds3_tape_response**)tapes_array->pdata;
            response->num_tapes = tapes_array->len;
            g_ptr_array_free(tapes_array, FALSE);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_physical_placement_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_physical_placement_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_capacity_summary_container_response(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_capacity_summary_container_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_capacity_summary_container_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Pool")) {
            error = _parse_ds3_storage_domain_capacity_summary_response(client, doc, child_node, &response->pool);
        } else if (element_equal(child_node, "Tape")) {
            error = _parse_ds3_storage_domain_capacity_summary_response(client, doc, child_node, &response->tape);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_capacity_summary_container_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_capacity_summary_container_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_bulk_object_list_response(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_bulk_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_bulk_object_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_bulk_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_bulk_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_all_my_buckets_result_response(const ds3_client* client, const ds3_request* request, ds3_list_all_my_buckets_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_all_my_buckets_result_response* response;
    ds3_error* error = NULL;

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListAllMyBucketsResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_all_my_buckets_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Buckets")) {
            GPtrArray* buckets_array;
            error = _parse_ds3_bucket_details_response_array(client, doc, child_node, &buckets_array);
            response->buckets = (ds3_bucket_details_response**)buckets_array->pdata;
            response->num_buckets = buckets_array->len;
            g_ptr_array_free(buckets_array, FALSE);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_all_my_buckets_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }


    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_all_my_buckets_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_delete_result_response(const ds3_client* client, const ds3_request* request, ds3_delete_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_delete_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* deleted_objects_array = g_ptr_array_new();
    GPtrArray* errors_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "DeleteResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_delete_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Deleted")) {
            ds3_s3_object_to_delete_response* deleted_objects_response = NULL;
            error = _parse_ds3_s3_object_to_delete_response(client, doc, child_node, &deleted_objects_response);
            response->deleted_objects = (ds3_s3_object_to_delete_response**)deleted_objects_array->pdata;
            g_ptr_array_add(deleted_objects_array, deleted_objects_response);
        } else if (element_equal(child_node, "Error")) {
            ds3_delete_object_error_response* errors_response = NULL;
            error = _parse_ds3_delete_object_error_response(client, doc, child_node, &errors_response);
            response->errors = (ds3_delete_object_error_response**)errors_array->pdata;
            g_ptr_array_add(errors_array, errors_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_delete_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->deleted_objects = (ds3_s3_object_to_delete_response**)deleted_objects_array->pdata;
    response->num_deleted_objects = deleted_objects_array->len;
    g_ptr_array_free(deleted_objects_array, FALSE);
    response->errors = (ds3_delete_object_error_response**)errors_array->pdata;
    response->num_errors = errors_array->len;
    g_ptr_array_free(errors_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_delete_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_objects_response(const ds3_client* client, const ds3_request* request, ds3_objects_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    struct _xmlAttr* attribute;
    ds3_objects_response* response;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Objects");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_objects_response, 1);
    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "ChunkId") == true) {
            response->chunk_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkNumber") == true) {
            response->chunk_number = xml_get_uint16_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "NodeId") == true) {
            response->node_id = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_objects_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_bulk_object_response* objects_response = NULL;
            error = _parse_ds3_bulk_object_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_bulk_object_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_objects_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_bulk_object_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_objects_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_master_object_list_response(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    struct _xmlAttr* attribute;
    ds3_master_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "MasterObjectList");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_master_object_list_response, 1);
    for (attribute = root->properties; attribute != NULL; attribute = attribute->next) {
        if (attribute_equal(attribute, "Aggregating") == true) {
            response->aggregating = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "BucketName") == true) {
            response->bucket_name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "CachedSizeInBytes") == true) {
            response->cached_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "ChunkClientProcessingOrderGuarantee") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->chunk_client_processing_order_guarantee = _match_ds3_job_chunk_client_processing_order_guarantee(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "CompletedSizeInBytes") == true) {
            response->completed_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "EntirelyInCache") == true) {
            response->entirely_in_cache = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "JobId") == true) {
            response->job_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Naked") == true) {
            response->naked = xml_get_bool_from_attribute(client->log, doc, attribute);
        } else if (attribute_equal(attribute, "Name") == true) {
            response->name = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "OriginalSizeInBytes") == true) {
            response->original_size_in_bytes = xml_get_uint64_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Priority") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->priority = _match_ds3_priority(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "RequestType") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->request_type = _match_ds3_job_request_type(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "StartDate") == true) {
            response->start_date = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "Status") == true) {
            xmlChar* text = xmlNodeListGetString(doc, attribute->children, 1);
            if (text == NULL) {
                continue;
            }
            response->status = _match_ds3_job_status(client->log, text);
            xmlFree(text);
        } else if (attribute_equal(attribute, "UserId") == true) {
            response->user_id = xml_get_string_from_attribute(doc, attribute);
        } else if (attribute_equal(attribute, "UserName") == true) {
            response->user_name = xml_get_string_from_attribute(doc, attribute);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown attribute[%s] of ds3_master_object_list_response [%s]\n", attribute->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Nodes")) {
            GPtrArray* nodes_array;
            error = _parse_ds3_job_node_response_array(client, doc, child_node, &nodes_array);
            response->nodes = (ds3_job_node_response**)nodes_array->pdata;
            response->num_nodes = nodes_array->len;
            g_ptr_array_free(nodes_array, FALSE);
        } else if (element_equal(child_node, "Objects")) {
            ds3_objects_response* objects_response = NULL;
            error = _parse_ds3_objects_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_objects_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_master_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->objects = (ds3_objects_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_master_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_job_list_response(const ds3_client* client, const ds3_request* request, ds3_job_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_job_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* jobs_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Jobs");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_job_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Job")) {
            ds3_job_response* jobs_response = NULL;
            error = _parse_ds3_job_response(client, doc, child_node, &jobs_response);
            response->jobs = (ds3_job_response**)jobs_array->pdata;
            g_ptr_array_add(jobs_array, jobs_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_job_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->jobs = (ds3_job_response**)jobs_array->pdata;
    response->num_jobs = jobs_array->len;
    g_ptr_array_free(jobs_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_job_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_parts_result_response(const ds3_client* client, const ds3_request* request, ds3_list_parts_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_parts_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* parts_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListPartsResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_parts_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Key")) {
            response->key = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxParts")) {
            response->max_parts = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "NextPartNumberMarker")) {
            response->next_part_number_marker = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Owner")) {
            error = _parse_ds3_user_response(client, doc, child_node, &response->owner);
        } else if (element_equal(child_node, "PartNumberMarker")) {
            response->part_number_marker = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Part")) {
            ds3_multi_part_upload_part_response* parts_response = NULL;
            error = _parse_ds3_multi_part_upload_part_response(client, doc, child_node, &parts_response);
            response->parts = (ds3_multi_part_upload_part_response**)parts_array->pdata;
            g_ptr_array_add(parts_array, parts_response);
        } else if (element_equal(child_node, "IsTruncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UploadId")) {
            response->upload_id = xml_get_string(doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_parts_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->parts = (ds3_multi_part_upload_part_response**)parts_array->pdata;
    response->num_parts = parts_array->len;
    g_ptr_array_free(parts_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_parts_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_detailed_s3_object_list_response(const ds3_client* client, const ds3_request* request, ds3_detailed_s3_object_list_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_detailed_s3_object_list_response* response;
    ds3_error* error = NULL;
    GPtrArray* detailed_s3_objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "Data");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_detailed_s3_object_list_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Object")) {
            ds3_detailed_s3_object_response* detailed_s3_objects_response = NULL;
            error = _parse_ds3_detailed_s3_object_response(client, doc, child_node, &detailed_s3_objects_response);
            response->detailed_s3_objects = (ds3_detailed_s3_object_response**)detailed_s3_objects_array->pdata;
            g_ptr_array_add(detailed_s3_objects_array, detailed_s3_objects_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_detailed_s3_object_list_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->detailed_s3_objects = (ds3_detailed_s3_object_response**)detailed_s3_objects_array->pdata;
    response->num_detailed_s3_objects = detailed_s3_objects_array->len;
    g_ptr_array_free(detailed_s3_objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_detailed_s3_object_list_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_bucket_result_response(const ds3_client* client, const ds3_request* request, ds3_list_bucket_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_bucket_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* common_prefixes_array = g_ptr_array_new();
    GPtrArray* objects_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListBucketResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_bucket_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "CommonPrefixes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* common_prefixes = xml_get_string(doc, loop_node);
                g_ptr_array_add(common_prefixes_array, common_prefixes);
            }
        } else if (element_equal(child_node, "CreationDate")) {
            response->creation_date = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Delimiter")) {
            response->delimiter = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Marker")) {
            response->marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxKeys")) {
            response->max_keys = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "Name")) {
            response->name = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NextMarker")) {
            response->next_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Contents")) {
            ds3_contents_response* objects_response = NULL;
            error = _parse_ds3_contents_response(client, doc, child_node, &objects_response);
            response->objects = (ds3_contents_response**)objects_array->pdata;
            g_ptr_array_add(objects_array, objects_response);
        } else if (element_equal(child_node, "Prefix")) {
            response->prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsTruncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_bucket_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->common_prefixes = (ds3_str**)common_prefixes_array->pdata;
    response->num_common_prefixes = common_prefixes_array->len;
    g_ptr_array_free(common_prefixes_array, FALSE);
    response->objects = (ds3_contents_response**)objects_array->pdata;
    response->num_objects = objects_array->len;
    g_ptr_array_free(objects_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_bucket_result_response_free(response);
    }

    return error;
}
static ds3_error* _parse_top_level_ds3_list_multi_part_uploads_result_response(const ds3_client* client, const ds3_request* request, ds3_list_multi_part_uploads_result_response** _response, GByteArray* xml_blob) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr child_node;
    ds3_list_multi_part_uploads_result_response* response;
    ds3_error* error = NULL;
    GPtrArray* common_prefixes_array = g_ptr_array_new();
    GPtrArray* uploads_array = g_ptr_array_new();

    error = _get_request_xml_nodes(xml_blob, &doc, &root, "ListMultipartUploadsResult");
    if (error != NULL) {
        return error;
    }

    response = g_new0(ds3_list_multi_part_uploads_result_response, 1);

    for (child_node = root->xmlChildrenNode; child_node != NULL; child_node = child_node->next) {
        if (element_equal(child_node, "Bucket")) {
            response->bucket = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "CommonPrefixes")) {
            xmlNodePtr loop_node;
            int num_nodes = 0;
            for (loop_node = child_node->xmlChildrenNode; loop_node != NULL; loop_node = loop_node->next, num_nodes++) {
                ds3_str* common_prefixes = xml_get_string(doc, loop_node);
                g_ptr_array_add(common_prefixes_array, common_prefixes);
            }
        } else if (element_equal(child_node, "Delimiter")) {
            response->delimiter = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "KeyMarker")) {
            response->key_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "MaxUploads")) {
            response->max_uploads = xml_get_uint16(doc, child_node);
        } else if (element_equal(child_node, "NextKeyMarker")) {
            response->next_key_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "NextUploadIdMarker")) {
            response->next_upload_id_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Prefix")) {
            response->prefix = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "IsTruncated")) {
            response->truncated = xml_get_bool(client->log, doc, child_node);
        } else if (element_equal(child_node, "UploadIdMarker")) {
            response->upload_id_marker = xml_get_string(doc, child_node);
        } else if (element_equal(child_node, "Upload")) {
            ds3_multi_part_upload_response* uploads_response = NULL;
            error = _parse_ds3_multi_part_upload_response(client, doc, child_node, &uploads_response);
            response->uploads = (ds3_multi_part_upload_response**)uploads_array->pdata;
            g_ptr_array_add(uploads_array, uploads_response);
        } else {
            ds3_log_message(client->log, DS3_ERROR, "Unknown node[%s] of ds3_list_multi_part_uploads_result_response [%s]\n", child_node->name, root->name);
        }

        if (error != NULL) {
            break;
        }

    }

    response->common_prefixes = (ds3_str**)common_prefixes_array->pdata;
    response->num_common_prefixes = common_prefixes_array->len;
    g_ptr_array_free(common_prefixes_array, FALSE);
    response->uploads = (ds3_multi_part_upload_response**)uploads_array->pdata;
    response->num_uploads = uploads_array->len;
    g_ptr_array_free(uploads_array, FALSE);

    xmlFreeDoc(doc);

    if (error == NULL) {
        *_response = response;
    } else {
        ds3_list_multi_part_uploads_result_response_free(response);
    }

    return error;
}
static ds3_paging* _parse_paging_headers(ds3_string_multimap* response_headers) {
    ds3_paging* response_paging = NULL;

    ds3_str* page_truncated_key = ds3_str_init("Page-Truncated");
    ds3_str* total_result_count_key = ds3_str_init("Total-Result-Count");

    ds3_string_multimap_entry* page_truncated_entry = ds3_string_multimap_lookup(response_headers, page_truncated_key);
    ds3_string_multimap_entry* total_result_count_entry = ds3_string_multimap_lookup(response_headers, total_result_count_key);

    if ((page_truncated_entry != NULL) || (total_result_count_entry != NULL)) {
        ds3_str* page_truncated_ds3_str = ds3_string_multimap_entry_get_value_by_index(page_truncated_entry, 0);
        ds3_str* total_result_count_ds3_str = ds3_string_multimap_entry_get_value_by_index(total_result_count_entry, 0);

        response_paging = g_new0(ds3_paging, 1);
        if (page_truncated_ds3_str != NULL) {
            response_paging->page_truncated = g_ascii_strtoll(page_truncated_ds3_str->value, NULL, 10);
            ds3_str_free(page_truncated_ds3_str);
        } else {
            response_paging->page_truncated = 0;
        }
        if (total_result_count_ds3_str != NULL) {
            response_paging->total_result_count = g_ascii_strtoll(total_result_count_ds3_str->value, NULL, 10);
            ds3_str_free(total_result_count_ds3_str);
        } else {
            response_paging->total_result_count = 0;
        }
    }

    ds3_str_free(page_truncated_key);
    ds3_str_free(total_result_count_key);
    ds3_string_multimap_entry_free(page_truncated_entry);
    ds3_string_multimap_entry_free(total_result_count_entry);

    return response_paging;
}


ds3_error* ds3_abort_multi_part_upload_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_complete_multi_part_upload_request(const ds3_client* client, const ds3_request* request, ds3_complete_multipart_upload_result_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, COMPLETE_MPU);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_complete_multipart_upload_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bucket_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_put_multi_part_upload_part_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*)) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, user_data, callback, NULL);
}
ds3_error* ds3_put_object_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (*callback)(void*, size_t, size_t, void*)) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, user_data, callback, NULL);
}
ds3_error* ds3_delete_bucket_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_objects_request(const ds3_client* client, const ds3_request* request, ds3_delete_result_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, STRING_LIST);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_delete_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_bucket_request(const ds3_client* client, const ds3_request* request, ds3_list_bucket_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_bucket_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_service_request(const ds3_client* client, const ds3_request* request, ds3_list_all_my_buckets_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_all_my_buckets_result_response(client, request, response, xml_blob);
}

ds3_error* ds3_get_object_request(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*)) {
    ds3_error* error;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    error = _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL, NULL);

    return error;
}

ds3_error* ds3_get_object_with_metadata(const ds3_client* client, const ds3_request* request, void* user_data, size_t (* callback)(void*, size_t, size_t, void*), ds3_metadata** _metadata) {
    ds3_error* error;
    ds3_string_multimap* return_headers;
    ds3_metadata* metadata;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    error = _internal_request_dispatcher(client, request, user_data, callback, NULL, NULL, &return_headers);
    if (error == NULL) {
        metadata = _init_metadata(return_headers);
        *_metadata = metadata;
        ds3_string_multimap_free(return_headers);
    }

    return error;
}
ds3_error* ds3_head_bucket_request(const ds3_client* client, const ds3_request* request) {
    if (client == NULL || request == NULL) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "All arguments must be filled in for request processing");
    }
    if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}

ds3_error* ds3_head_object_request(const ds3_client* client, const ds3_request* request, ds3_head_object_response** response) {
    ds3_error* error;
    ds3_string_multimap* return_headers;
    ds3_metadata* metadata;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    error = _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, &return_headers);

    if (error == NULL) {
        ds3_head_object_response* response_ptr = g_new0(ds3_head_object_response, 1);
        response_ptr->metadata = _init_metadata(return_headers);
        response_ptr->blob_checksum_type = get_blob_checksum_type(client->log, return_headers);
        response_ptr->blob_checksums = get_blob_checksums(client->log, return_headers);
        *response = response_ptr;
        ds3_string_multimap_free(return_headers);
    }

    return error;
}

ds3_error* ds3_initiate_multi_part_upload_request(const ds3_client* client, const ds3_request* request, ds3_initiate_multipart_upload_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_initiate_multipart_upload_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_list_multi_part_upload_parts_request(const ds3_client* client, const ds3_request* request, ds3_list_parts_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The object name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_parts_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_list_multi_part_uploads_request(const ds3_client* client, const ds3_request* request, ds3_list_multi_part_uploads_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The bucket name parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_list_multi_part_uploads_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bucket_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bucket_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_policy_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_policy_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_bucket_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_bucket_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_data_policy_acl_for_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_global_data_policy_acl_for_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_bucket_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_data_policy_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_bucket_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_bucket_acls_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_acl_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_acl_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_data_policy_acl_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_acl_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_policy_acls_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_acl_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_policy_acl_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_put_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_bucket_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bucket_response(client, request, response, xml_blob);
}
ds3_error* ds3_force_full_cache_reclaim_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_cache_filesystem_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_cache_filesystem_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_cache_filesystems_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_cache_filesystem_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_cache_state_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_information_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_cache_information_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_cache_filesystem_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_cache_filesystem_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_cache_filesystem_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_bucket_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_capacity_summary_container_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domain_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_capacity_summary_container_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_system_capacity_summary_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_capacity_summary_container_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_capacity_summary_container_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_path_backend_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_path_backend_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_planner_blob_store_tasks_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_blob_store_tasks_information_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_blob_store_tasks_information_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_data_path_backend_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_path_backend_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_azure_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_persistence_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_ds3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_s3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_azure_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_ds3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_s3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_azure_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_azure_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_azure_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_persistence_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_data_persistence_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_persistence_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_data_policies_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_policy_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_s3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_s3_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_azure_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_data_persistence_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_persistence_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_data_policy_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_ds3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_s3_data_replication_rule_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_data_replication_rule_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_data_replication_rule_response(client, request, response, xml_blob);
}
ds3_error* ds3_clear_suspect_blob_azure_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_suspect_blob_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_suspect_blob_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_suspect_blob_s3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_suspect_blob_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_degraded_azure_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_azure_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_blobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_degraded_blob_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_degraded_blob_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_data_persistence_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_persistence_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_persistence_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_ds3_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_degraded_s3_data_replication_rules_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_data_replication_rule_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_data_replication_rule_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_azure_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_azure_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_azure_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_ds3_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_ds3_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_pool_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_pool_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_s3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_s3_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_s3_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_blob_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_suspect_blob_tape_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_suspect_blob_tape_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_buckets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bucket_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_bucket_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_suspect_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_mark_suspect_blob_azure_targets_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_mark_suspect_blob_ds3_targets_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_mark_suspect_blob_pools_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_mark_suspect_blob_s3_targets_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_mark_suspect_blob_tapes_as_degraded_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_put_group_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_user_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_group_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_group_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_group_members_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_member_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_group_member_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_groups_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_group_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_user_is_member_of_group_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_group_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_group_response(client, request, response, xml_blob);
}
ds3_error* ds3_allocate_job_chunk_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_objects_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_objects_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_all_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_all_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_all_canceled_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_clear_all_completed_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_close_aggregating_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, BULK_GET);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, BULK_PUT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_bulk_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_active_job_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_active_job_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_active_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_active_job_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_canceled_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_canceled_job_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_canceled_job_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_canceled_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_canceled_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_canceled_job_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_completed_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_completed_job_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_completed_job_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_completed_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_completed_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_completed_job_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_chunk_dao_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_chunk_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_chunk_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_chunk_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_objects_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_objects_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_to_replicate_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_str** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_str* _response;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    _response = ds3_str_init_with_size((char*)xml_blob->data, xml_blob->len);
    g_byte_array_free(xml_blob, TRUE);

    *response = _response;
    return error;
}
ds3_error* ds3_get_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_replicate_put_job_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_master_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, STRING);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_master_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_truncate_active_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_truncate_all_active_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_truncate_all_jobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_truncate_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_verify_safe_to_create_put_job_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_node_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_node_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_nodes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_node_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_node_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_node_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_node_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_azure_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_completed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_created_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_creation_failed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_cached_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_lost_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_persisted_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_s3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_system_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_azure_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_s3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_azure_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_azure_target_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_azure_target_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_ds3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_completed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_completed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_completed_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_completed_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_job_completed_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_created_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_created_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_created_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_created_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_job_created_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_job_creation_failed_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_job_creation_failed_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_job_creation_failed_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_job_creation_failed_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_job_creation_failed_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_object_cached_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_cached_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_object_cached_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_cached_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_cached_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_object_lost_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_lost_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_object_lost_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_lost_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_lost_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_object_persisted_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_persisted_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_object_persisted_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_persisted_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_persisted_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_pool_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pool_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_s3_target_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_s3_target_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_target_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_storage_domain_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domain_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_system_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_system_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_system_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_system_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_partition_failure_notification_registration_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_failure_notification_registration_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partition_failure_notification_registrations_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_notification_registration_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_partition_failure_notification_registration_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_delete_folder_recursively_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_blob_persistence_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_str** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;
    ds3_str* _response;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, STRING);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    _response = ds3_str_init_with_size((char*)xml_blob->data, xml_blob->len);
    g_byte_array_free(xml_blob, TRUE);

    *response = _response;
    return error;
}
ds3_error* ds3_get_object_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_object_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_objects_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_object_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_s3_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_detailed_s3_object_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_physical_placement_for_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_physical_placement_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_physical_placement_for_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_physical_placement_for_objects_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_physical_placement_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_physical_placement_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_physical_placement_for_objects_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_import_on_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_import_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_verify_on_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_cancel_verify_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_compact_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_compact_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_deallocate_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_permanently_lost_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_pool_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_force_pool_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request) {



    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_format_all_foreign_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_format_foreign_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_blobs_on_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pool_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pool_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_partition_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_pool_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_import_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_import_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_pool_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_all_pools_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_verify_pool_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_pool_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_pool_response(client, request, response, xml_blob);
}
ds3_error* ds3_convert_storage_domain_to_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_put_pool_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_storage_domain_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_storage_domain_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domain_members_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_member_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_storage_domains_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_storage_domain_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_storage_domain_member_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_member_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_member_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_storage_domain_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_storage_domain_response(client, request, response, xml_blob);
}
ds3_error* ds3_force_feature_key_validation_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_feature_keys_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_feature_key_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_feature_key_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_system_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_system_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_system_information_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_system_information_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_system_information_response(client, request, response, xml_blob);
}
ds3_error* ds3_reset_instance_identifier_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_path_backend_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_path_backend_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_system_health_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_health_verification_result_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;



    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_health_verification_result_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_eject_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_eject_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_format_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_format_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_import_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_import_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_online_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_online_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_verify_on_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_cancel_verify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_clean_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_drive_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_density_directive_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_permanently_lost_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_partition_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_eject_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_eject_storage_domain_blobs_spectra_s3_request(const ds3_client* client, const ds3_request* request) {
    ds3_error* error;
    ds3_xml_send_buff send_buff;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    error = _init_request_payload(request, &send_buff, GET_PHYSICAL_PLACEMENT);
    if (error != NULL) return error;

    error = _internal_request_dispatcher(client, request, NULL, NULL, (void*) &send_buff, _ds3_send_xml_buff, NULL);

    // Clean up the data sent to the server
    xmlFree(send_buff.buff);

    return error;
}
ds3_error* ds3_eject_storage_domain_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_eject_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_force_tape_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request) {



    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_format_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_format_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_blobs_on_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_density_directive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_density_directive_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_density_directives_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_density_directive_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_density_directive_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_drive_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_drives_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_drive_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_detailed_tape_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_libraries_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_library_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_library_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_library_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_library_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_library_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partition_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_partition_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partition_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_detailed_tape_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_detailed_tape_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tape_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_partition_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_partitions_with_full_details_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_named_detailed_tape_partition_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_named_detailed_tape_partition_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_tape_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_import_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_import_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_inspect_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_inspect_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_all_tape_partitions_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_tape_drive_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_drive_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_drive_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_tape_partition_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_partition_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_partition_response(client, request, response, xml_blob);
}
ds3_error* ds3_modify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_online_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_online_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_raw_import_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_raw_import_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_all_tapes_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_failure_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_tape_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_tape_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_tape_response(client, request, response, xml_blob);
}
ds3_error* ds3_force_target_environment_refresh_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_put_azure_target_bucket_name_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_bucket_name_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_bucket_name_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_azure_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_azure_target_bucket_name_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_azure_target_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_azure_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_azure_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_azure_target_bucket_names_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_bucket_name_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_azure_target_bucket_name_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_azure_target_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_azure_target_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_azure_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_azure_target_read_preferences_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_read_preference_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_azure_target_read_preference_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_azure_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_azure_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_azure_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_blobs_on_azure_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_import_azure_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_all_azure_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_azure_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_register_azure_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_azure_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_azure_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_azure_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_ds3_target_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_blobs_on_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_data_policies_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_data_policy_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_data_policy_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_ds3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_target_read_preferences_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_read_preference_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_read_preference_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_all_ds3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_pair_back_registered_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_register_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_ds3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_s3_target_bucket_name_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_bucket_name_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_bucket_name_response(client, request, response, xml_blob);
}
ds3_error* ds3_put_s3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_delete_s3_target_bucket_name_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_s3_target_failure_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_s3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_delete_s3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_blobs_on_s3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_bulk_object_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_bulk_object_list_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_s3_target_bucket_names_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_bucket_name_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_target_bucket_name_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_s3_target_failures_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_failure_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_target_failure_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_s3_target_read_preference_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_read_preference_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_read_preference_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_s3_target_read_preferences_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_read_preference_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_target_read_preference_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_get_s3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_s3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_s3_target_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_import_s3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_all_s3_targets_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_modify_s3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_register_s3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_verify_s3_target_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_s3_target_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_s3_target_response(client, request, response, xml_blob);
}
ds3_error* ds3_delegate_create_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
}
ds3_error* ds3_delegate_delete_user_spectra_s3_request(const ds3_client* client, const ds3_request* request) {

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    return _internal_request_dispatcher(client, request, NULL, NULL, NULL, NULL, NULL);
}
ds3_error* ds3_get_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
}
ds3_error* ds3_get_users_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_list_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;
    ds3_string_multimap* return_headers = NULL;

    if (request->path->size < 2) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, &return_headers);
    if (error != NULL) {
        ds3_string_multimap_free(return_headers);
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    error = _parse_top_level_ds3_spectra_user_list_response(client, request, response, xml_blob);

    (*response)->paging = _parse_paging_headers(return_headers);
    ds3_string_multimap_free(return_headers);

    return error;
}
ds3_error* ds3_modify_user_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
}
ds3_error* ds3_regenerate_user_secret_key_spectra_s3_request(const ds3_client* client, const ds3_request* request, ds3_spectra_user_response** response) {
    ds3_error* error;
    GByteArray* xml_blob;

    int num_slashes = num_chars_in_ds3_str(request->path, '/');
    if (num_slashes < 2 || ((num_slashes == 2) && ('/' == request->path->value[request->path->size-1]))) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource type parameter is required.");
    } else if (g_ascii_strncasecmp(request->path->value, "//", 2) == 0) {
        return ds3_create_error(DS3_ERROR_MISSING_ARGS, "The resource id parameter is required.");
    }

    xml_blob = g_byte_array_new();
    error = _internal_request_dispatcher(client, request, xml_blob, ds3_load_buffer, NULL, NULL, NULL);
    if (error != NULL) {
        g_byte_array_free(xml_blob, TRUE);
        return error;
    }

    return _parse_top_level_ds3_spectra_user_response(client, request, response, xml_blob);
}
