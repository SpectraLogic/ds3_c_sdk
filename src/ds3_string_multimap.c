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

static void _free_pointer_array(gpointer pointer) {
    GPtrArray* array = (GPtrArray*) pointer;

    g_ptr_array_free(array, TRUE);
}

ds3_string_multimap* ds3_string_multimap_init(void) {
    struct _ds3_string_multimap* multimap = g_new0(struct _ds3_string_multimap, 1);
    multimap->hash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, _free_pointer_array);

    return (ds3_string_multimap*) multimap;
}

void ds3_string_multimap_insert(ds3_string_multimap* _hashtable, char* key, char* value) {
    struct _ds3_string_multimap* map = (struct _ds3_string_multimap*) _hashtable;
    GPtrArray* entries = (GPtrArray*) g_hash_table_lookup(map->hash, key);
    if (entries == NULL) {
        entries = g_ptr_array_new_with_free_func(g_free);
        g_hash_table_insert(map->hash, key, entries);
    }
    g_ptr_array_add(entries, value);
}

GPtrArray* ds3_string_multimap_lookup(ds3_string_multimap* hashtable, char* key) {
    struct _ds3_string_multimap* map = (struct _ds3_string_multimap*) hashtable;
    return g_hash_table_lookup(map->hash, key);
}

void ds3_string_multimap_free(ds3_string_multimap* _map) {
    struct _ds3_string_multimap* map;
    if (_map == NULL) return;
    map = (struct _ds3_string_multimap*) _map;

    if (map->hash != NULL) {
        g_hash_table_destroy(map->hash);
    }

    g_free(map);
}

