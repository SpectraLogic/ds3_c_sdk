
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


#include <curl/curl.h>
#include <glib.h>
#include <inttypes.h>
#include "ds3_net.h"


ds3_connection_pool* ds3_connection_pool_init(void) {
    return ds3_connection_pool_init_with_size(CONNECTION_POOL_SIZE);
}

void _ds3_queue_init(ds3_connection_pool* pool) {
    int index;
    pool->queue = g_new0(int, pool->size);
    for (index = 0; index < pool->size; index++) {
        pool->queue[index] = index; // init to default
    }
}

void _ds3_queue_free(ds3_connection_pool* pool) {
    g_free(pool->queue);
}

ds3_connection_pool* ds3_connection_pool_init_with_size(uint16_t pool_size) {
    printf("ds3_connection_pool_init_with_size(%u)\n", pool_size);
    ds3_connection_pool* pool = g_new0(ds3_connection_pool, 1);
    pool->connections = g_new0(ds3_connection*, pool_size);
    pool->size = pool_size;

    _ds3_queue_init(pool);

    g_mutex_init(&pool->mutex);
    g_cond_init(&pool->available_connections);
    pool->ref_count = 1;
    return pool;
}

void ds3_connection_pool_clear(ds3_connection_pool* pool, ds3_bool already_locked) {
    int index;

    if (pool == NULL) {
        return;
    }

    if (already_locked == False) {
        g_mutex_lock(&pool->mutex);
    }
    printf("ds3_connection_pool_clear(%s)\n", (already_locked ? "locked" : "not locked"));

    for (index = 0; index < pool->size; index++) {
        if (pool->connections[index] != NULL) {
            curl_easy_cleanup(pool->connections[index]);
        }
    }

    _ds3_queue_free(pool);

    g_free(pool->connections);
    g_mutex_unlock(&pool->mutex);
    g_mutex_clear(&pool->mutex); // an attempt to clear a locked mutex is undefined
    g_cond_clear(&pool->available_connections);
}

static int _queue_inc(int index, uint16_t size) {
    printf("_pool_inc(%d, %u) :[%d]\n", index, size, (index+1) % size);
    return (index+1) % size;
}

static int _queue_full(ds3_connection_pool* pool) {
    return (_pool_inc(pool->head, pool->size) == pool->tail);
}

ds3_connection* ds3_connection_acquire(ds3_connection_pool* pool) {
    ds3_connection* connection = NULL;
    int next_connection_index;

    g_mutex_lock(&pool->mutex);
    printf("ds3_connection_acquire() BEGIN: head[%d] tail[%d]\n", pool->head, pool->tail);
    while (_pool_full(pool)) {
        g_cond_wait(&pool->available_connections, &pool->mutex);
    }

    next_connection_index = pool->queue[pool->head];
    pool->queue[pool->head] = -1; // invalid while in use
    if (pool->connections[next_connection_index] == NULL) {
        connection = g_new0(ds3_connection, 1);
        connection = curl_easy_init();

        pool->connections[next_connection_index] = connection;
    } else {
        connection = pool->connections[next_connection_index];
    }
    pool->queue[pool->tail] = pool->head;
    pool->head = _pool_inc(pool->head, pool->size);

    printf("ds3_connection_acquire() END: head[%d] tail[%d]\n", pool->head, pool->tail);
    g_mutex_unlock(&pool->mutex);

    return connection;
}

void ds3_connection_release(ds3_connection_pool* pool, ds3_connection* connection) {
    g_mutex_lock(&pool->mutex);
    printf("ds3_connection_release() BEGIN: head[%d] tail[%d]\n", pool->head, pool->tail);

    curl_easy_reset(connection);

    pool->tail = _pool_inc(pool->tail, pool->size);

    printf("ds3_connection_release() END: head[%d] tail[%d]\n", pool->head, pool->tail);
    g_mutex_unlock(&pool->mutex);
    g_cond_signal(&pool->available_connections);
}

void ds3_connection_pool_inc_ref(ds3_connection_pool* pool) {
    g_mutex_lock(&pool->mutex);
    pool->ref_count++;
    g_mutex_unlock(&pool->mutex);
}

void ds3_connection_pool_dec_ref(ds3_connection_pool* pool) {
    g_mutex_lock(&pool->mutex);
    pool->ref_count--;

    if (pool->ref_count == 0) {
        ds3_connection_pool_clear(pool, True);
        g_free(pool);
    } else {
        g_mutex_unlock(&pool->mutex);
    }
}
