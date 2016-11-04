/*
 * ******************************************************************************
 *   Copyright 2014-2016 Spectra Logic Corporation. All Rights Reserved.
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

#include "ds3.h"
#include "ds3_string.h"
#include "ds3_string_multimap.h"
#include <boost/test/unit_test.hpp>
#include <glib.h>

BOOST_AUTO_TEST_CASE( string_multimap_insert_and_lookup ) {
    printf("-----Testing string_multimap insert_and_lookup-------\n");
    ds3_string_multimap* map = ds3_string_multimap_init();
    ds3_str* key = ds3_str_init("key");
    ds3_str* value = ds3_str_init("value");

    ds3_string_multimap_insert(map, key, value);

    ds3_string_multimap_entry* entry = ds3_string_multimap_lookup(map, key);

    BOOST_CHECK(ds3_string_multimap_entry_get_num_values(entry) == 1);
    ds3_str* entry_key = ds3_string_multimap_entry_get_key(entry);
    ds3_str* entry_value = ds3_string_multimap_entry_get_value_by_index(entry, 0);
    BOOST_CHECK(g_strcmp0(entry_key->value, key->value) == 0);
    BOOST_CHECK(g_strcmp0(entry_value->value, value->value) == 0);

    ds3_str_free(key);
    ds3_str_free(value);
    ds3_str_free(entry_key);
    ds3_str_free(entry_value);
    ds3_string_multimap_free(map);
    ds3_string_multimap_entry_free(entry);
}
