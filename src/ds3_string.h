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

#ifndef __DS3_STRING__
#define __DS3_STRING__

#include <stdlib.h>
#include "ds3_library_exports.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    char* value;
    size_t size;
}ds3_str;

LIBRARY_API ds3_str* ds3_str_init(const char* string);
LIBRARY_API ds3_str* ds3_str_init_with_size(const char* string, size_t size);
LIBRARY_API char* ds3_str_value(const ds3_str* string);
LIBRARY_API size_t ds3_str_size(const ds3_str* string);
LIBRARY_API ds3_str* ds3_str_dup(const ds3_str* string);
LIBRARY_API void ds3_str_free(ds3_str* string);

#ifdef __cplusplus
}
#endif
#endif
