
#include <glib.h>
#include "countdown_latch.h"

struct _countdown_latch {
    guint initial_count;
    volatile guint count;
    GCond waiters;
    GMutex lock;
};

countdown_latch* countdown_latch_new(guint count) {
    countdown_latch* latch = g_new0(countdown_latch, 1);
    latch->initial_count = count;
    latch->count = count;
    g_mutex_init(&(latch->lock));
    g_cond_init(&(latch->waiters));
    return latch;
}

void countdownlatch_free(countdown_latch* latch) {
    g_cond_clear(&(latch->waiters));
    g_mutex_clear(&(latch->lock));
    g_free(latch);
}

void countdownlatch_wait(countdown_latch* latch) {
    g_mutex_lock(&(latch->lock));
    while(latch->count > 0) {
        g_cond_wait(&(latch->waiters), &(latch->lock));
    }
    g_mutex_unlock(&(latch->lock));
}

void countdownlatch_count_down(countdown_latch* latch) {
    g_mutex_lock(&(latch->lock));
    (latch->count)--;
    if(latch->count == 0) {
        g_cond_broadcast(&(latch->waiters));
    }
    g_mutex_unlock(&(latch->lock));
}

void countdownlatch_count_down_wait(countdown_latch* latch) {
    g_mutex_lock(&(latch->lock));
    (latch->count)--;
    if(latch->count == 0) {
        g_cond_broadcast(&(latch->waiters));
    } else {
        g_cond_wait(&(latch->waiters), &(latch->lock));
    }
    g_mutex_unlock(&(latch->lock));
}

