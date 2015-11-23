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

#ifndef __DS3_STRING_MULTIMAP__
#define __DS3_STRING_MULTIMAP__

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ds3_string_multimap ds3_string_multimap;

//opertions for manipulating a hash map as a string multi map
ds3_string_multimap* ds3_string_multimap_init(void);
void ds3_string_multimap_insert(ds3_string_multimap* hashtable, char* key, char* value);
GPtrArray* ds3_string_multimap_lookup(ds3_string_multimap* hashtable, char* key);
void ds3_string_multimap_free(ds3_string_multimap* map);


/*
// TODO implement this as a future refactor
// operations for multimaps
typedef struct _ds3_multi_map ds3_multi_map;

typedef unsigned int (*ds3_hash_func)(void* pointer);
typedef int (*ds3_equal_func)(void* pointer);

LIBRARY_API ds3_multi_map* ds3_multi_map_init(ds3_hash_func hash, ds3_equal_func equal);
LIBRARY_API void ds3_multi_map_put(void* key, void* data);
LIBRARY_API void* ds3_multi_map_get(void* key);
LIBRARY_API void ds3_multi_map_free(ds3_multi_map* map);
*/
#ifdef __cplusplus
}
#endif
#endif
