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
#include "../src/ds3_response_header_utils.h"

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

void _put_test_headers(ds3_string_multimap* map, const char* key, const char* value) {
    ds3_str* ds3_key = ds3_str_init(key);
    ds3_str* ds3_value = ds3_str_init(value);
    ds3_string_multimap_insert(map, ds3_key, ds3_value);
    ds3_str_free(ds3_key);
    ds3_str_free(ds3_value);
}

ds3_string_multimap* _get_head_object_test_headers(void) {
    ds3_string_multimap* map = ds3_string_multimap_init();
    _put_test_headers(map, "x-amz-meta-key", "value");
    _put_test_headers(map, "ds3-blob-checksum-type", "CRC_32");
    _put_test_headers(map, "ds3-blob-checksum-offset-0", "4nQGNX4nyz0pi8Hvap79PQ==");
    _put_test_headers(map, "ds3-blob-checksum-offset-10485760", "965Aa0/n8DlO1IwXYFh4bg==");
    _put_test_headers(map, "ds3-blob-checksum-offset-20971520", "iV2OqJaXJ/jmqgRSb1HmFA==");
    return map;
}

BOOST_AUTO_TEST_CASE( get_blob_checksum_type_CRC_32 ) {
    ds3_string_multimap* response_headers = _get_head_object_test_headers();

    ds3_checksum_type* type = get_blob_checksum_type(NULL, response_headers);
    BOOST_CHECK(type != NULL);
    BOOST_CHECK(*type == DS3_CHECKSUM_TYPE_CRC_32);

    if (type != NULL) {
        g_free(type);
    }
    ds3_string_multimap_free(response_headers);
}

BOOST_AUTO_TEST_CASE( get_blob_checksum_type_empty_map ) {
    ds3_string_multimap* response_headers = ds3_string_multimap_init();

    ds3_checksum_type* type = get_blob_checksum_type(NULL, response_headers);
    BOOST_CHECK(type == NULL);

    if (type != NULL) {
        g_free(type);
    }
    ds3_string_multimap_free(response_headers);
}

void _verify_blob_checksum_value(ds3_uint64_string_map* checksums, uint64_t key, const char* expected_checksum) {
    ds3_str* blob_checksum;
    BOOST_CHECK_EQUAL(ds3_uint64_string_map_contains(checksums, &key), TRUE);

    blob_checksum = ds3_uint64_string_map_lookup(checksums, &key);

    BOOST_CHECK(blob_checksum != NULL);
    BOOST_CHECK_EQUAL(blob_checksum->value, expected_checksum);

    ds3_str_free(blob_checksum);
}

BOOST_AUTO_TEST_CASE( get_blob_checksums_test ) {
    ds3_string_multimap* response_headers = _get_head_object_test_headers();

    ds3_uint64_string_map* checksums = get_blob_checksums(NULL, response_headers);
    BOOST_CHECK(checksums != NULL);
    BOOST_CHECK(ds3_uint64_string_map_size(checksums) == 3);

    _verify_blob_checksum_value(checksums, 0, "4nQGNX4nyz0pi8Hvap79PQ==");
    _verify_blob_checksum_value(checksums, 10485760, "965Aa0/n8DlO1IwXYFh4bg==");
    _verify_blob_checksum_value(checksums, 20971520, "iV2OqJaXJ/jmqgRSb1HmFA==");

    ds3_uint64_string_map_free(checksums);
    ds3_string_multimap_free(response_headers);
}

BOOST_AUTO_TEST_CASE( get_blob_checksums_empty_map ) {
    ds3_string_multimap* response_headers = ds3_string_multimap_init();

    ds3_uint64_string_map* checksums = get_blob_checksums(NULL, response_headers);
    BOOST_CHECK(checksums != NULL);
    BOOST_CHECK(ds3_uint64_string_map_size(checksums) == 0);

    ds3_uint64_string_map_free(checksums);
    ds3_string_multimap_free(response_headers);
}
