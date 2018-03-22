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

#include <glib.h>

#include "ds3_uint64_string_map.h"

struct _ds3_uint64_string_map {
    GHashTable* hash; //key is uint64_t*, value is ds3_str*
};

// Used to iterate through a ds3_uint64_string_map
struct _ds3_uint64_string_map_iter {
    GHashTableIter* g_iter;
};

static void _internal_uint64_free(gpointer data) {
    g_free((uint64_t*) data);
}

static void _internal_str_free(gpointer data) {
    ds3_str_free((ds3_str*) data);
}

ds3_uint64_string_map* ds3_uint64_string_map_init(void) {
    // Create a hash table with uint64_t* as the key, and ds3_str* as the value
    struct _ds3_uint64_string_map* map = g_new0(struct _ds3_uint64_string_map, 1);
    map->hash = g_hash_table_new_full(g_int64_hash, g_int64_equal, _internal_uint64_free, _internal_str_free);
    return (ds3_uint64_string_map*) map;
}

// Inserts a safe copy of the key-value pair into the map. Returns true if the key did not exist yet.
ds3_bool ds3_uint64_string_map_insert(ds3_uint64_string_map* map, const uint64_t* key, const ds3_str* value) {
    if (map == NULL || map->hash == NULL) {
        return FALSE;
    }
    ds3_str* value_cpy = ds3_str_dup(value);
    uint64_t* key_cpy = g_new0(uint64_t, 1);
    *key_cpy = *key;
    gboolean result = g_hash_table_insert(map->hash, key_cpy, value_cpy);
    if (result == FALSE) {
        return False;
    }
    return True;
}

ds3_bool ds3_uint64_string_map_contains(ds3_uint64_string_map* map, uint64_t* key) {
    if (map == NULL || map->hash == NULL) {
        return FALSE;
    }
    gboolean result = g_hash_table_contains(map->hash, key);
    if (result == FALSE) {
        return False;
    }
    return True;
}

// Looks up the value for the provided key and returns a duplicate of the ds3_str value if found.
ds3_str* ds3_uint64_string_map_lookup(ds3_uint64_string_map* map, uint64_t* key) {
    if (map == NULL || map->hash == NULL) {
        return NULL;
    }
    GPtrArray* value = g_hash_table_lookup(map->hash, key);
    if (value == NULL) {
        return NULL;
    }
    return ds3_str_dup((ds3_str*)value);
}

uint64_t ds3_uint64_string_map_size(ds3_uint64_string_map* map) {
    return (uint64_t) g_hash_table_size(map->hash);
}

void ds3_uint64_string_map_free(ds3_uint64_string_map* map) {
    if (map == NULL) {
        return;
    }
    if (map->hash != NULL) {
        // Release all elements in the hash
        g_hash_table_remove_all(map->hash);

        // Release the hash
        g_hash_table_unref(map->hash);
    }

    // Release the map
    g_free(map);
}

ds3_uint64_string_map_iter* ds3_uint64_string_map_iter_init(ds3_uint64_string_map* map) {
    // Create iterator for ds3_uint64_string_map
    struct _ds3_uint64_string_map_iter* iter = g_new0(struct _ds3_uint64_string_map_iter, 1);
    iter->g_iter = g_new0(GHashTableIter, 1);
    g_hash_table_iter_init(iter->g_iter, map->hash);
    return (ds3_uint64_string_map_iter*) iter;
}

void ds3_uint64_string_map_iter_free(ds3_uint64_string_map_iter* iter) {
    if (iter == NULL) {
        return;
    }
    if (iter->g_iter != NULL) {
        g_free(iter->g_iter);
    }
    // Release the iterator
    g_free(iter);
}

// Returns the next pair of key-value in the map if exists. The pair must be deallocate
// using ds3_uint64_string_pair_free. The returned pair is a copy of the values within
// the map, so modification of the pair does not affect the map. If there is no next
// entry then NULL is returned.
ds3_uint64_string_pair* ds3_uint64_string_map_iter_next(ds3_uint64_string_map_iter* iter) {
    gpointer g_key;
    gpointer g_value;
    gboolean has_next = g_hash_table_iter_next(iter->g_iter, &g_key, &g_value);
    if (has_next == FALSE) {
        return NULL;
    }
    // Allocate the pair ds3_uint64_string_pair
    struct _ds3_uint64_string_pair* pair = g_new0(struct _ds3_uint64_string_pair, 1);

    // Set key and value of the pair
    uint64_t* key_ptr = (uint64_t*) g_key;
    pair->key = *key_ptr;
    pair->value = ds3_str_dup((ds3_str*)g_value);

    return (ds3_uint64_string_pair*) pair;
}

void ds3_uint64_string_pair_free(ds3_uint64_string_pair* pair) {
    if (pair == NULL) {
        return;
    }
    if (pair->value != NULL) {
        ds3_str_free(pair->value);
    }
    g_free(pair);
}
