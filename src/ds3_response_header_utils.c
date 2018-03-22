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

#include "ds3_response_header_utils.h"
#include "ds3_string_multimap_impl.h"
#include "ds3_utils.h"

#define BLOB_CHECKSUM_HEADER "ds3-blob-checksum-offset-"
#define BLOB_CHECKSUM_TYPE_HEADER "ds3-blob-checksum-type"

// Converts a ds3_str* containing a checksum value into a ds3_checksum_type*.
// If conversion is not possible, then NULL is returned.
static ds3_checksum_type* _convert_str_to_checksum_type(const ds3_log* log, const ds3_str* checksum_str) {
    if (checksum_str == NULL || checksum_str->value == NULL) {
        return NULL;
    }
    ds3_checksum_type* checksum_type = g_new0(ds3_checksum_type, 1);
    if (strcmp(checksum_str->value, "CRC_32") == 0) {
        *checksum_type = DS3_CHECKSUM_TYPE_CRC_32;
        return checksum_type;
    }
    if (strcmp(checksum_str->value, "CRC_32C") == 0) {
        *checksum_type = DS3_CHECKSUM_TYPE_CRC_32C;
        return checksum_type;
    }
    if (strcmp(checksum_str->value, "MD5") == 0) {
        *checksum_type = DS3_CHECKSUM_TYPE_MD5;
        return checksum_type;
    }
    if (strcmp(checksum_str->value, "SHA_256") == 0) {
        *checksum_type = DS3_CHECKSUM_TYPE_SHA_256;
        return checksum_type;
    }
    if (strcmp(checksum_str->value, "SHA_512") == 0) {
        *checksum_type = DS3_CHECKSUM_TYPE_SHA_512;
        return checksum_type;
    }
    g_free(checksum_type);
    ds3_log_message(log, DS3_ERROR, "ERROR: Unknown value of '%s' for ds3_checksum_type.", checksum_str->value);
    return NULL;
}

// Retrieves the blob checksum type from the response headers.
ds3_checksum_type* get_blob_checksum_type(const ds3_log* log, ds3_string_multimap* response_headers) {
    ds3_checksum_type* checksum_type = NULL;

    ds3_str* header_key = ds3_str_init(BLOB_CHECKSUM_TYPE_HEADER);
    ds3_string_multimap_entry* entry = ds3_string_multimap_lookup(response_headers, header_key);
    ds3_str_free(header_key);

    if (entry == NULL) {
        return NULL;
    }

    ds3_str* value = ds3_string_multimap_entry_get_value_by_index(entry, 0);
    ds3_string_multimap_entry_free(entry);


    checksum_type = _convert_str_to_checksum_type(log, value);
    ds3_str_free(value);


    return checksum_type;
}

// Retrieves the offset value at the end of a blob checksum header
static uint64_t* _get_offset_from_key(const ds3_str* key) {
    if (key == NULL) {
        return NULL;
    }

    uint64_t* offset = g_new0(uint64_t, 1);
    *offset = strtoull(key->value+strlen(BLOB_CHECKSUM_HEADER), NULL, 10);
    return offset;
}

// Retrieves the blob checksums from the response headers.
ds3_uint64_string_map* get_blob_checksums(const ds3_log* log, ds3_string_multimap* response_headers) {
    if (response_headers == NULL) {
        ds3_log_message(log, DS3_WARN, "Cannot parse blob checksum headers: response headers was null\n");
        return NULL;
    }

    ds3_uint64_string_map* blob_map = ds3_uint64_string_map_init();

    GHashTableIter iter;
    gpointer _key, _value;
    ds3_str* key = NULL;

    g_hash_table_iter_init(&iter, ds3_string_multimap_get_hashtable(response_headers));
    while(g_hash_table_iter_next(&iter, &_key, &_value)) {
        key = (ds3_str*) _key;
        if (g_str_has_prefix(key->value, BLOB_CHECKSUM_HEADER)) {
            ds3_string_multimap_entry* entry = ds3_string_multimap_lookup(response_headers, key);
            if (entry != NULL && ds3_string_multimap_entry_get_num_values(entry) > 0) {
                uint64_t* offset = _get_offset_from_key(key);
                if (offset != NULL) {
                    ds3_str* value = ds3_string_multimap_entry_get_value_by_index(entry, 0);
                    ds3_uint64_string_map_insert(blob_map, offset, value);
                    ds3_str_free(value);
                }
                g_free(offset);
            }
            ds3_string_multimap_entry_free(entry);
        }
    }
    return blob_map;
}
