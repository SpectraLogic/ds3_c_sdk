
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

/* This Code is Auto-Generated; DO NOT MODIFY! */


#ifndef __DS3_CONNECTION_H__
#define __DS3_CONNECTION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <curl/curl.h>
#include <glib.h>
#include "ds3.h"

#define DEFAULT_CONNECTION_POOL_SIZE 10

typedef GMutex ds3_mutex;
typedef GCond ds3_condition;

typedef CURL ds3_connection;

typedef struct _ds3_connection_pool ds3_connection_pool;

ds3_connection_pool* ds3_connection_pool_init(void);
ds3_connection_pool* ds3_connection_pool_init_with_size(uint16_t pool_size);
void ds3_connection_pool_clear(ds3_connection_pool* pool, ds3_bool already_locked);

ds3_connection* ds3_connection_acquire(ds3_connection_pool* pool);
void ds3_connection_release(ds3_connection_pool* pool, ds3_connection* handle);

void ds3_connection_pool_inc_ref(ds3_connection_pool* pool);
void ds3_connection_pool_dec_ref(ds3_connection_pool* pool);

#ifdef __cplusplus
}
#endif
#endif
