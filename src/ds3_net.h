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

#ifndef __DS3_NET_H__
#define __DS3_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ds3.h"
#include "ds3_string_multimap.h"
#include "ds3_connection.h"

char* escape_url(const char* url);
char* escape_url_extended(const char* url, const char** delimiters, uint32_t num_delimiters);
char* escape_url_object_name(const char* url);
char* escape_url_range_header(const char* url);

ds3_error* net_process_request(
   const ds3_client* client,
   const ds3_request* _request,
   void* read_user_struct,
   size_t (*read_handler_func)(void*, size_t, size_t, void*),
   void* write_user_struct,
   size_t (*write_handler_func)(void*, size_t, size_t, void*),
   ds3_string_multimap** return_headers);

void net_cleanup(void);

#ifdef __cplusplus
}
#endif
#endif
