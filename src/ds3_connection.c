
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

#include <string.h>
#include <curl/curl.h>
#include <glib.h>
#include "ds3_net.h"

ds3_connection_pool* ds3_connection_pool_init(void) {
    ds3_connection_pool* pool = g_new0(ds3_connection_pool, 1);
    g_mutex_init(&pool->mutex);
    g_cond_init(&pool->available_connections);
    return pool;
}

void ds3_connection_pool_clear(ds3_connection_pool* pool) {
    int index;

    if (pool == NULL) {
        return;
    }

    g_mutex_lock(&pool->mutex);

    for (index = 0; index < CONNECTION_POOL_SIZE; index++) {
        if (pool->connections[index] != NULL) {
            curl_easy_cleanup(pool->connections[index]);
        }
    }

    g_mutex_unlock(&pool->mutex);
    g_mutex_clear(&pool->mutex);
    g_cond_clear(&pool->available_connections);
}

static int _pool_inc(ds3_connection_pool* pool, int index) {
    return (index+1) % CONNECTION_POOL_SIZE;
}

static int _pool_full(ds3_connection_pool* pool) {
    return (_pool_inc(pool, pool->tail) == pool->head);
}


ds3_connection* ds3_connection_acquire(ds3_connection_pool* pool) {
    ds3_connection* connection = NULL;

    g_mutex_lock(&pool->mutex);
    while (_pool_full(pool)) {
        g_cond_wait(&pool->available_connections, &pool->mutex);
    }

    if (pool->connections[pool->head] == NULL) {
        connection = curl_easy_init();

        pool->connections[pool->head] = connection;
    } else {
        connection = pool->connections[pool->head];
    }
    pool->head = _pool_inc(pool, pool->head);

    g_mutex_unlock(&pool->mutex);

    return connection;
}

void ds3_connection_release(ds3_connection_pool* pool, ds3_connection* connection) {
    g_mutex_lock(&pool->mutex);

    curl_easy_reset(connection);
    pool->tail = _pool_inc(pool, pool->tail);

    g_mutex_unlock(&pool->mutex);
    g_cond_signal(&pool->available_connections);
}

