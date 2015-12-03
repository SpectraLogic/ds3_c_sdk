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

/* This is used to free the entires in the values ptr array in the ds3_string_multimap_entry
 */
static void _internal_str_free(gpointer data) {
    ds3_str_free((ds3_str*)data);
}

static guint _ds3_str_hash(gconstpointer key) {
    if (key == NULL) return 0;

    const ds3_str* d_str = (const ds3_str*)key;
    return g_str_hash(d_str->value);
}

static gboolean _ds3_str_equal(gconstpointer a, gconstpointer b) {
    if ((a == NULL) || (b == NULL)) return FALSE;

    const ds3_str* _a = (const ds3_str*) a;
    const ds3_str* _b = (const ds3_str*) b;

    return g_str_equal(_a->value, _b->value);
}

ds3_string_multimap* ds3_string_multimap_init(void) {
    struct _ds3_string_multimap* multimap = g_new0(struct _ds3_string_multimap, 1);
    multimap->hash = g_hash_table_new_full(_ds3_str_hash, _ds3_str_equal, _internal_str_free, _free_pointer_array);

    return (ds3_string_multimap*) multimap;
}

void ds3_string_multimap_insert(ds3_string_multimap* map, ds3_str* key, ds3_str* value) {
    struct _ds3_string_multimap* _map = (struct _ds3_string_multimap*) map;
    GPtrArray* entries = g_hash_table_lookup(_map->hash, key);
    if (entries == NULL) {
        entries = g_ptr_array_new_with_free_func(_internal_str_free);
        g_hash_table_insert(_map->hash, ds3_str_dup(key), entries);
    }
    g_ptr_array_add(entries, ds3_str_dup(value));
}

// Add an entry (key & 1+ values) to a map
// caller frees all passed in values
void ds3_string_multimap_insert_entry(ds3_string_multimap* map, const ds3_string_multimap_entry* entry) {
    if ((map == NULL) || (entry == NULL)) return;

    int index;
    int num_values = ds3_string_multimap_entry_get_num_values(entry);
    for (index = 0; index < num_values; index++) {
        ds3_str* key = ds3_string_multimap_entry_get_key(entry);
        ds3_str* value = ds3_string_multimap_entry_get_value_by_index(entry, index);
        ds3_string_multimap_insert(map, key, value);
        ds3_str_free(key);
        ds3_str_free(value);
    }
}

ds3_string_multimap_entry* ds3_string_multimap_lookup(ds3_string_multimap* map, ds3_str* key) {
    if ((map == NULL) || (key == NULL)) return NULL;

    struct _ds3_string_multimap* _map = (struct _ds3_string_multimap*)map;
    GPtrArray* values = g_hash_table_lookup(_map->hash, key);
    if (values == NULL) return NULL;

    ds3_string_multimap_entry* entry = ds3_string_multimap_entry_init(key);
    int index;
    unsigned int num_values = values->len;
    for (index=0; index < num_values; index++) {
        ds3_str* current_value = g_ptr_array_index(values, index);
        ds3_string_multimap_entry_add_value(entry, current_value);
    }

    return entry;
}

void ds3_string_multimap_free(ds3_string_multimap* map) {
    if (map == NULL) return;

    struct _ds3_string_multimap* _map = (struct _ds3_string_multimap*) map;
    if (_map->hash != NULL) {
        g_hash_table_destroy(_map->hash);
    }

    g_free(map);
}


// The following functions manipulate ds3_string_multimap_entry
ds3_string_multimap_entry* ds3_string_multimap_entry_init(const ds3_str* key) {
    struct _ds3_string_multimap_entry* _entry = g_new0(ds3_string_multimap_entry, 1);
    _entry->key = ds3_str_dup(key);
    _entry->values = g_ptr_array_new_with_free_func(_internal_str_free);
    return (ds3_string_multimap_entry*)_entry;
}

void ds3_string_multimap_entry_add_value(ds3_string_multimap_entry* entry, ds3_str* value) {
    if ((entry == NULL) || (value == NULL)) return;

    struct _ds3_string_multimap_entry* _entry = entry;
    g_ptr_array_add(_entry->values, (gpointer)ds3_str_dup(value));
}

ds3_string_multimap_entry* ds3_string_multimap_entry_dupe(const ds3_string_multimap_entry* entry) {
    const struct _ds3_string_multimap_entry* _entry = entry;
    ds3_string_multimap_entry* duped_entry = ds3_string_multimap_entry_init(_entry->key);
    unsigned int index;
    unsigned int num_values = ds3_string_multimap_entry_get_num_values(entry);
    for (index = 0; index < num_values; index++) {
        ds3_string_multimap_entry_add_value(duped_entry, ds3_string_multimap_entry_get_value_by_index(entry, index));
    }

    return duped_entry;
}

// caller frees returned ds3_str
ds3_str* ds3_string_multimap_entry_get_key(const ds3_string_multimap_entry* entry) {
    const struct _ds3_string_multimap_entry* _entry = entry;
    if (entry == NULL) {
        return NULL;
    }

    return ds3_str_dup(_entry->key);
}

// caller frees returned ds3_str
ds3_str* ds3_string_multimap_entry_get_value_by_index(const ds3_string_multimap_entry* entry, int index) {
    const struct _ds3_string_multimap_entry* _entry = entry;
    if ((entry == NULL) || (index > ds3_string_multimap_entry_get_num_values(entry))) {
        return NULL;
    }

    ds3_str* value = ds3_str_dup(g_ptr_array_index(_entry->values, index));
    return value;
}

unsigned int ds3_string_multimap_entry_get_num_values(const ds3_string_multimap_entry* map_entry) {
    if (map_entry == NULL) return 0;

    const struct _ds3_string_multimap_entry* _entry = map_entry;
    return _entry->values->len;
}

void ds3_string_multimap_entry_free(ds3_string_multimap_entry* entry) {
    if (entry == NULL) {
        return;
    }

    struct _ds3_string_multimap_entry* _entry = (struct _ds3_string_multimap_entry*) entry;
    ds3_str_free(_entry->key);
    g_ptr_array_free(_entry->values, TRUE);
    g_free(entry);
}

