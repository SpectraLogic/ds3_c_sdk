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

#ifndef __DS3_STRING_MULTIMAP__
#define __DS3_STRING_MULTIMAP__

#include "ds3_string.h"

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

typedef struct _ds3_string_multimap       ds3_string_multimap;
typedef struct _ds3_string_multimap_entry ds3_string_multimap_entry;


//operations for manipulating a hash map as a ds3_str multi map
LIBRARY_API ds3_string_multimap*       ds3_string_multimap_init(void);
LIBRARY_API void                       ds3_string_multimap_insert(ds3_string_multimap* map, const ds3_str* key, const ds3_str* value);
LIBRARY_API void                       ds3_string_multimap_insert_entry(ds3_string_multimap* map, const ds3_string_multimap_entry* entry);  // caller frees all passed in values
LIBRARY_API ds3_string_multimap_entry* ds3_string_multimap_lookup(ds3_string_multimap* map, const ds3_str* key);
LIBRARY_API void                       ds3_string_multimap_free(ds3_string_multimap* map);


//operations for manipulating a ds3_string_multi_map_entry
LIBRARY_API ds3_string_multimap_entry* ds3_string_multimap_entry_init(const ds3_str* key);
LIBRARY_API ds3_str*                   ds3_string_multimap_entry_get_key(const ds3_string_multimap_entry* entry);
LIBRARY_API void                       ds3_string_multimap_entry_add_value(ds3_string_multimap_entry* entry, const ds3_str* value);
LIBRARY_API unsigned int               ds3_string_multimap_entry_get_num_values(const ds3_string_multimap_entry* map_entry);
LIBRARY_API ds3_str*                   ds3_string_multimap_entry_get_value_by_index(const ds3_string_multimap_entry* entry, unsigned int index);
LIBRARY_API ds3_string_multimap_entry* ds3_string_multimap_entry_dup(const ds3_string_multimap_entry* entry);
LIBRARY_API void                       ds3_string_multimap_entry_free(ds3_string_multimap_entry* entry);


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
