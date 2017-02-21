/*
 * ******************************************************************************
 *   Copyright 2014-2017 Spectra Logic Corporation. All Rights Reserved.
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

#include "ds3_string.h"
#include <glib.h>
#include <string.h>

ds3_str* ds3_str_init(const char* string) {
    size_t size = strlen(string);
    return ds3_str_init_with_size(string, size);
}

ds3_str* ds3_str_init_with_size(const char* string, size_t size) {
    ds3_str* str = g_new0(ds3_str, 1);
    str->value = g_strndup(string, size);
    str->size = size;
    return str;
}

ds3_str* ds3_str_dup(const ds3_str* string) {
    ds3_str* str = g_new0(ds3_str, 1);
    str->value = g_strndup(string->value, string->size);
    str->size = string->size;
    return str;
}

char* ds3_str_value(const ds3_str* string) {
    return string->value;
}

size_t ds3_str_size(const ds3_str* string) {
    return string->size;
}

void ds3_str_free(ds3_str* string) {
    if (string == NULL) return;

    if (string->value != NULL) {
        g_free(string->value);
    }
    g_free(string);
}

