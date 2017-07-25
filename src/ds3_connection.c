
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
#include "ds3_connection.h"

//-- Opaque struct
struct _ds3_connection_pool{
    ds3_connection** connections;
    uint16_t         num_connections; // the number of connections created
    ds3_connection** connection_queue;
    uint16_t         max_connections; // max number of possible connections, which the connections and queue arrays will be initialized to
    int              queue_head;
    int              queue_tail;
    ds3_mutex        mutex;
    ds3_condition    available_connection_notifier;
    uint16_t         ref_count;
};

ds3_connection_pool* ds3_connection_pool_init(void) {
    return ds3_connection_pool_init_with_size(DEFAULT_CONNECTION_POOL_SIZE);
}

ds3_connection_pool* ds3_connection_pool_init_with_size(uint16_t pool_size) {
    ds3_connection_pool* pool = g_new0(ds3_connection_pool, 1);

    pool->connections = g_new0(ds3_connection*, pool_size);
    pool->connection_queue = g_new0(ds3_connection*, pool_size);

    pool->max_connections = pool_size;

    g_mutex_init(&pool->mutex);
    g_cond_init(&pool->available_connection_notifier);
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

    for (index = 0; index < pool->num_connections; index++) {
        if (pool->connections[index] != NULL) {
            curl_easy_cleanup(pool->connections[index]);
        }
    }

    g_free(pool->connections);
    g_free(pool->connection_queue);
    g_mutex_unlock(&pool->mutex);
    g_mutex_clear(&pool->mutex); // an attempt to clear a locked mutex is undefined
    g_cond_clear(&pool->available_connection_notifier);
}

static int _queue_inc(int index, uint16_t size) {
    return (index+1) % size;
}

static int _queue_is_empty(ds3_connection_pool* pool) {
    int queue_head = pool->queue_head;
    return pool->queue_tail == queue_head && pool->connection_queue[queue_head] == NULL;
}

ds3_connection* ds3_connection_acquire(ds3_connection_pool* pool) {
    ds3_connection* connection = NULL;

    g_mutex_lock(&pool->mutex);
    while (_queue_is_empty(pool) && pool->num_connections >= pool->max_connections) {
        g_cond_wait(&pool->available_connection_notifier, &pool->mutex);
    }

    if (_queue_is_empty(pool)) {
        connection = curl_easy_init();

        pool->connections[pool->num_connections] = connection;
        pool->num_connections++;
    } else {
        connection = pool->connection_queue[pool->queue_tail];
        pool->connection_queue[pool->queue_tail] = NULL;
        pool->queue_tail = _queue_inc(pool->queue_tail, pool->max_connections);
    }

    g_mutex_unlock(&pool->mutex);

    return connection;
}

void ds3_connection_release(ds3_connection_pool* pool, ds3_connection* connection) {
    g_mutex_lock(&pool->mutex);

    curl_easy_reset(connection);

    pool->connection_queue[pool->queue_head] = connection;

    pool->queue_head = _queue_inc(pool->queue_head, pool->max_connections);

    g_cond_signal(&pool->available_connection_notifier);
    g_mutex_unlock(&pool->mutex);
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
