/*
* ******************************************************************************
*   Copyright 2014-2015 Spectra Logic Corporation. All Rights Reserved.
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

#ifndef __DS3_REQUEST_H__
#define __DS3_REQUEST_H__

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

    //These next few elements are only for the bulk commands
    ds3_bulk_object_list* object_list;
    ds3_chunk_ordering    chunk_ordering;
};

#endif

