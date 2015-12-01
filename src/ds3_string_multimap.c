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
#include "ds3.h"
#include "ds3_string_multimap.h"
#include "ds3_string_multimap_impl.h"

static void _free_pointer_array(gpointer pointer) {
    GPtrArray* array = (GPtrArray*) pointer;

    g_ptr_array_free(array, TRUE);
}

static void _internal_string_entry_free(gpointer data) {
    ds3_str_free((ds3_str*)data);
}

static gboolean _ds3_str_equal(gconstpointer a, gconstpointer b) {
    if ((a == NULL) || (b == NULL)) return FALSE;

    const ds3_str* _a = a;
    const ds3_str* _b = b;

    return g_strcmp0(_a->value, _b->value);
}

ds3_string_multimap* ds3_string_multimap_init(void) {
    struct _ds3_string_multimap* multimap = g_new0(struct _ds3_string_multimap, 1);
    multimap->hash = g_hash_table_new_full(g_str_hash, _ds3_str_equal, g_free, _free_pointer_array);

    return (ds3_string_multimap*) multimap;
}

void ds3_string_multimap_insert(ds3_string_multimap* map, ds3_str* key, ds3_str* value) {
    struct _ds3_string_multimap* _map = (struct _ds3_string_multimap*) map;
    GPtrArray* entries = (GPtrArray*) g_hash_table_lookup(_map->hash, key->value);
    if (entries == NULL) {
        entries = g_ptr_array_new_with_free_func(_internal_string_entry_free);
        g_hash_table_insert(_map->hash, key, entries);
    }
    g_ptr_array_add(entries, value);
}

ds3_string_multimap_entry* ds3_string_multimap_lookup(ds3_string_multimap* map, ds3_str* key) {
    struct _ds3_string_multimap* _map = (struct _ds3_string_multimap*)map;
    return g_hash_table_lookup(_map->hash, key->value);
}

void ds3_string_multimap_free(ds3_string_multimap* map) {
    struct _ds3_string_multimap* _map;
    if (map == NULL) return;
    _map = (struct _ds3_string_multimap*) map;

    if (_map->hash != NULL) {
        g_hash_table_destroy(_map->hash);
    }

    g_free(map);
}


/* This is used to free the entires in the values ptr array in the ds3_string_multimap_entry
 */
static void _ds3_internal_str_free(gpointer data) {
    ds3_str_free((ds3_str*)data);
}

ds3_string_multimap_entry* ds3_init_string_multimap_entry(const ds3_str* key) {
    struct _ds3_string_multimap_entry* _entry = g_new0(ds3_string_multimap_entry, 1);
    _entry->key = ds3_str_dup(key);
    _entry->values = g_ptr_array_new_with_free_func(_ds3_internal_str_free);
    return (ds3_string_multimap_entry*)_entry;
}

// caller frees all passed in values
void ds3_insert_string_multimap_entry(ds3_string_multimap* map, const ds3_str* key, const ds3_str* value) {
    struct _ds3_string_multimap* _map = map;
    struct _ds3_string_multimap_entry* _entry = (ds3_string_multimap_entry*)g_hash_table_lookup(_map->hash, key->value);

    if (_entry == NULL) {
        _entry = ds3_init_string_multimap_entry(key);
        g_hash_table_insert(map->hash, g_strdup(key->value), _map);
    }

    g_ptr_array_add(_entry->values, ds3_str_dup(value));
}

ds3_str* ds3_string_multimap_entry_get_value_by_index(const ds3_string_multimap_entry* entry, int index) {
    const struct _ds3_string_multimap_entry* _entry = entry;
    if (entry == NULL || index >_entry->values->len) {
        return NULL;
    }

    return (ds3_str*)g_ptr_array_index(entry->values, index);
}

void ds3_free_string_multimap_entry(gpointer data) {
    ds3_string_multimap_entry* entry;
    if (data == NULL) {
        return;
    }

    entry = (ds3_string_multimap_entry*) data;
    ds3_str_free(entry->key);
    g_ptr_array_free(entry->values, TRUE);
    g_free(data);
}

size_t ds3_string_multimap_entry_get_num_values(const ds3_string_multimap_entry* map_entry) {
    const struct _ds3_string_multimap_entry* _entry = map_entry;
    return _entry->values->len;
}
