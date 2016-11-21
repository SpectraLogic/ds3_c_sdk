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

#ifndef __DS3_CONNECTION_H__
#define __DS3_CONNECTION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <curl/curl.h>
#include <glib.h>

#define CONNECTION_POOL_SIZE 100

typedef GMutex ds3_mutex;
typedef GCond ds3_condition;

typedef CURL ds3_connection;

//-- Opaque struct
struct _ds3_connection_pool{
    ds3_connection* connections[CONNECTION_POOL_SIZE];
    int head;
    int tail;
    ds3_mutex mutex;
    ds3_condition available_connections;
};

ds3_connection_pool* ds3_connection_pool_init(void);
void ds3_connection_pool_clear(ds3_connection_pool* pool);

ds3_connection* ds3_connection_acquire(ds3_connection_pool* pool);
void ds3_connection_release(ds3_connection_pool* pool, ds3_connection* handle);

#ifdef __cplusplus
}
#endif
#endif
