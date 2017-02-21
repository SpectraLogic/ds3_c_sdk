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


#include <glib.h>

#include "ds3.h"
#include "ds3_utils.h"

ds3_error* ds3_create_error(ds3_error_code code, const char * message) {
    ds3_error* error = g_new0(ds3_error, 1);
    error->code = code;
    error->message = ds3_str_init(message);
    error->error = NULL;
    return error;
}

size_t ds3_load_buffer(void* buffer, size_t size, size_t nmemb, void* user_data) {
    size_t realsize = size * nmemb;
    GByteArray* blob = (GByteArray*) user_data;

    g_byte_array_append(blob, (const guint8 *) buffer, realsize);
    return realsize;
}

void ds3_log_message(const ds3_log* log, ds3_log_lvl lvl, const char* message, ...) {
    if (log == NULL) {
        return;
    }

    if (log->log_callback == NULL) {
        fprintf(stderr, "ERROR: ds3_c_sdk - User supplied log_callback is null, failed to log message.\n");
        return;
    }

    if (lvl <= log->log_lvl) {
        va_list args;
        char * log_message;

        va_start(args, message);
        log_message = g_strdup_vprintf(message, args);
        va_end(args);

        log->log_callback(log_message, log->user_data);

        g_free(log_message);
    }
}

