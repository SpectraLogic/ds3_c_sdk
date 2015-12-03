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


#ifndef __DS3_STRING_MULTIMAP_IMPL__
#define __DS3_STRING_MULTIMAP_IMPL__

#include <glib.h>

struct _ds3_string_multimap {
    GHashTable* hash; //key is ds3_str, values are a GPtrArray of ds3_str
};

typedef struct _ds3_string_multimap_entry{
    ds3_str*   key;
    GPtrArray* values; // A ds3_str list of the header values
}ds3_string_multimap_entry;

GHashTable* ds3_string_multimap_get_hashtable(const ds3_string_multimap* mp);

void ds3_string_multimap_set_hashtable(ds3_string_multimap* mp, GHashTable* ht);

//void ds3_string_multimap_dupe_response_headers_hashtable(const GHashTable* src_ht, const ds3_string_multimap* dest_map );

#endif

