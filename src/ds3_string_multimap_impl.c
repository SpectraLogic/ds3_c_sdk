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

#include <glib.h>
#include "ds3_string_multimap.h"
#include "ds3_string_multimap_impl.h"

GHashTable* ds3_string_multimap_get_hashtable(const ds3_string_multimap* mp) {
    const struct _ds3_string_multimap* _mp = mp;
    return _mp->hash;
}

void ds3_string_multimap_set_hashtable(ds3_string_multimap* mp, GHashTable* ht) {
    if (mp != NULL) {
        ds3_string_multimap_free(mp);
    }

    struct _ds3_string_multimap* _mp = mp;
    _mp->hash = ht;
}

ds3_string_multimap* ds3_string_multimap_dupe(GHashTable* response_headers) {
    ds3_string_multimap* map = ds3_string_multimap_init();
    GHashTableIter iter;
    gpointer _key, _value;

    g_hash_table_iter_init(&iter, response_headers);
    while (g_hash_table_iter_next(&iter, &_key, &_value)) {
        int index;
        struct _ds3_string_multimap_entry* _entry = (ds3_string_multimap_entry*)_value;
        if (_value == NULL) {
            continue;
        }

        for (index=0; index < _entry->values->len; index++) {
            ds3_string_multimap_insert(map, _key, ds3_string_multimap_entry_get_value_by_index(_value, index));
        }
    }
    return map;
}

