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


#ifndef __DS3_RESPONSE_HEADER_UTILS__
#define __DS3_RESPONSE_HEADER_UTILS__

#include <stdlib.h>
#include <glib.h>

#include "ds3_string.h"
#include "stdint.h"
#include "ds3_string_multimap.h"
#include "ds3.h"

#ifdef __cplusplus
extern "C" {
#endif

// For windows DLL symbol exports.
#ifdef _WIN32
#    ifdef LIBRARY_EXPORTS
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#else
#    define LIBRARY_API
#endif

LIBRARY_API ds3_checksum_type* get_blob_checksum_type(const ds3_log* log, ds3_string_multimap* response_headers);
LIBRARY_API ds3_uint64_string_map* get_blob_checksums(const ds3_log* log, ds3_string_multimap* response_headers);

#ifdef __cplusplus
}
#endif
#endif
