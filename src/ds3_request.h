/*
 * ******************************************************************************
 *   Copyright 2014-2016 Spectra Logic Corporation. All Rights Reserved.
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


#ifndef __DS3_REQUEST_H__
#define __DS3_REQUEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>
#include "ds3.h"

//---------- Define opaque struct ----------//
struct _ds3_request{
    http_verb         verb;
    ds3_str*          path;
    uint64_t          length;
    ds3_str*          checksum;
    ds3_checksum_type checksum_type;
    GHashTable*       headers;
    GHashTable*       query_params;

    //These next few elements are for request payload
    ds3_bulk_object_list_response*                  object_list;
    ds3_job_chunk_client_processing_order_guarantee chunk_ordering;

    ds3_complete_multipart_upload_response*         mpu_list;

    ds3_delete_objects_response*                    delete_objects;
};

typedef struct {
    // These attributes are used when processing a response header
    uint64_t status_code;
    ds3_str* status_message;
    size_t header_count;
    ds3_string_multimap* headers;

    // These attributes are used when processing a response body
    GByteArray* body; // this will only be used when getting errors
    void* user_data;
    size_t (*user_func)(void*, size_t, size_t, void*);
}ds3_response_data;


#ifdef __cplusplus
}
#endif
#endif
