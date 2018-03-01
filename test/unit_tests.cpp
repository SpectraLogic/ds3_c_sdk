/*   Copyright 2014-2018 Spectra Logic Corporation. All Rights Reserved.
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

#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

#include "../src/ds3_uint64_string_map.h"

BOOST_AUTO_TEST_CASE( ds3_unit64_string_map ) {
    const char* const_value = "my value";
    uint64_t key = 10;

    // Create map
    ds3_uint64_string_map* map = ds3_uint64_string_map_init();

    // Verify map does not have content
    gboolean contains = ds3_uint64_string_map_contains(map, &key);
    BOOST_CHECK(contains == FALSE);

    // Insert stuff into map
    ds3_str* value = ds3_str_init(const_value);
    ds3_uint64_string_map_insert(map, &key, value);
    ds3_str_free(value);

    // Verify map has content
    contains = ds3_uint64_string_map_contains(map, &key);
    BOOST_CHECK(contains == TRUE);

    // Iterate through map
    ds3_uint64_string_map_iter* iter = ds3_uint64_string_map_iter_init(map);
    ds3_uint64_string_pair* pair = NULL;
    while((pair = ds3_uint64_string_map_iter_next(iter)) != NULL) {
        BOOST_CHECK(pair != NULL);
        BOOST_CHECK(pair->value != NULL);
        if (pair->value != NULL) {
            BOOST_CHECK(strcmp(const_value, ds3_str_value(pair->value)) == 0);
        }
        BOOST_CHECK_EQUAL(pair->key, key);

        ds3_uint64_string_pair_free(pair);
    }

    ds3_uint64_string_map_iter_free(iter);

    // Get stuff from map
    value = ds3_uint64_string_map_lookup(map, &key);
    BOOST_CHECK(value != NULL);
    BOOST_CHECK(strcmp(const_value, ds3_str_value(value)) == 0);
    ds3_str_free(value);

    // Free map
    ds3_uint64_string_map_free(map);
}
