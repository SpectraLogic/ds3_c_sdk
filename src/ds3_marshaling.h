/*
 * ******************************************************************************
 *   Copyright 2014-2018 Spectra Logic Corporation. All Rights Reserved.
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

#ifndef DS3_MARSHALING_H
#define DS3_MARSHALING_H

#include <libxml/parser.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BULK_PUT,
    BULK_GET,
    BULK_DELETE,
    GET_PHYSICAL_PLACEMENT,
    COMPLETE_MPU,
    STRING,
    STRING_LIST,
    ID_LIST,
    DATA
}object_list_type;

typedef struct {
    char* buff;
    size_t size;
    size_t total_read;
}ds3_xml_send_buff;

xmlDocPtr ds3_generate_xml_ids(ds3_ids_list* ids_list);
xmlDocPtr ds3_generate_xml_delete_objects(ds3_delete_objects_response* keys_list);
xmlDocPtr ds3_generate_xml_complete_mpu(const ds3_complete_multipart_upload_response* mpu_list);
xmlDocPtr ds3_generate_xml_bulk_objects_list(const ds3_bulk_object_list_response* obj_list, object_list_type list_type);

#ifdef __cplusplus
}
#endif
#endif
