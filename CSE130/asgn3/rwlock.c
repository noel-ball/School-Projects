//------------------------------------------------------------------------------
//  Noel Ball njball 02/26/2024
//  rwlock.c for CSE 130 Programming Assignment 3
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "rwlock.h"

// struct declaration rwlock
typedef struct rwlock {
    // struct fields
    pthread_rwlock_t lock;
    PRIORITY priority;
    int n_init;
    int n_limit;
    int waiting_readers;
    int active_readers;
    int waiting_writers;
    int active_writers;
    // threads
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} rwlock_t;

// lock constructor
rwlock_t *rwlock_new(PRIORITY p, uint32_t n) {
    //fprintf(stderr, "In rwlock_new!\n");
    rwlock_t *lock = (rwlock_t *) malloc(sizeof(rwlock_t));
    if (lock == NULL) {
        // memory allocation failed
        return NULL;
    }

    // initializing fields
    lock->priority = p;
    lock->n_init = lock->n_limit = n;
    lock->active_readers = 0;
    lock->waiting_readers = 0;
    lock->active_writers = 0;
    lock->waiting_writers = 0;

    // initializing threads
    pthread_rwlock_init(&(lock->lock), NULL);
    pthread_mutex_init(&(lock->mutex), NULL);
    pthread_cond_init(&(lock->cond), NULL);

    //fprintf(stderr, "Out rwlock_new!\n");
    return lock;
}

// lock destructor
void rwlock_delete(rwlock_t **rw) {
    //fprintf(stderr, "In rwlock_delete!\n");

    if (rw == NULL || *rw == NULL) {
        return;
    }

    // destroy threads
    pthread_rwlock_destroy(&((*rw)->lock));
    pthread_mutex_destroy(&((*rw)->mutex));
    pthread_cond_destroy(&((*rw)->cond));

    // free memory
    free(*rw);
    *rw = NULL;
    //fprintf(stderr, "Out rwlock_delete!\n");
}

// acquire read lock
void reader_lock(rwlock_t *rw) {
    //fprintf(stderr, "In reader_lock!\n");

    // safely wait for mutex and reader priority
    pthread_mutex_lock(&(rw->mutex));
    while ((rw->priority == WRITERS && rw->waiting_writers > 0)
           || (rw->priority == N_WAY && rw->active_readers >= rw->n_limit)
           || rw->active_writers > 0) {
        rw->waiting_readers++;
        pthread_cond_wait(&(rw->cond), &(rw->mutex));
        rw->waiting_readers--;
    }
    rw->active_readers++;
    pthread_mutex_unlock(&(rw->mutex));

    pthread_rwlock_rdlock(&(rw->lock));
    //fprintf(stderr, "Out reader_lock!\n");
}

// release read lock
void reader_unlock(rwlock_t *rw) {
    //fprintf(stderr, "In reader_unlock!\n");

    pthread_rwlock_unlock(&(rw->lock));

    // safely signal potential writers
    pthread_mutex_lock(&(rw->mutex));
    rw->active_readers--;
    if (rw->priority == N_WAY && rw->active_readers == 0) {
        // decrement n_limit with active readers until writer gets lock, then reset to n_init
        rw->n_limit--;
    }
    if (rw->waiting_writers > 0 && rw->active_readers == 0) {
        pthread_cond_signal(&(rw->cond));
    } else {
        pthread_cond_broadcast(&(rw->cond));
    }
    pthread_mutex_unlock(&(rw->mutex));
    //fprintf(stderr, "Out reader_unlock!\n");
}

// acquire write lock
void writer_lock(rwlock_t *rw) {
    //fprintf(stderr, "In writer_lock!\n");

    // safely wait for mutex and writer priority
    pthread_mutex_lock(&(rw->mutex));
    while ((rw->active_readers > 0) || (rw->active_writers > 0)) {
        rw->waiting_writers++;
        pthread_cond_wait(&(rw->cond), &(rw->mutex));
        rw->waiting_writers--;
    }
    rw->active_writers++;
    pthread_mutex_unlock(&(rw->mutex));

    pthread_rwlock_wrlock(&(rw->lock));
    //fprintf(stderr, "Out writer_lock!\n");
}

// release write lock
void writer_unlock(rwlock_t *rw) {
    //fprintf(stderr, "In writer_unlock!\n");

    pthread_rwlock_unlock(&(rw->lock));
    //fprintf(stderr, "rwlock unlocked, waiting for mutex\n");

    // safely signal next writer or broadcast condition
    pthread_mutex_lock(&(rw->mutex));
    //fprintf(stderr, "mutex acquired\n");
    rw->active_writers--;
    if (rw->priority == N_WAY) {
        // writer gained access to lock, another n readers may continue
        rw->n_limit = rw->n_init;
    }
    if (rw->waiting_writers > 0 && rw->active_readers == 0) {
        pthread_cond_signal(&(rw->cond));
        //fprintf(stderr, "cond signaled\n");
    } else if (rw->waiting_readers > 0) {
        pthread_cond_broadcast(&(rw->cond));
        //fprintf(stderr, "cond broadcast\n");
    }
    pthread_mutex_unlock(&(rw->mutex));
    //fprintf(stderr, "Out writer_unlock!\n");
}
