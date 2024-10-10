//------------------------------------------------------------------------------
//  Noel Ball njball 02/26/2024
//  queue.c for CSE 130 Programming Assignment 3
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "queue.h"

// struct definition queue_t
typedef struct queue {
    // struct fields
    int cap;
    int size;
    int front;
    int back;
    void **queue;
    // threads
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} queue_t;

// queue constructor
queue_t *queue_new(int size) {
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (q == NULL) {
        // memory allocation failed
        return NULL;
    }

    // initializing fields
    q->cap = size;
    q->size = 0;
    q->front = 0;
    q->back = -1;
    q->queue = (void **) malloc(size * sizeof(void *));
    if (q->queue == NULL) {
        // memory allocation failed
        free(q);
        return NULL;
    }

    // initializing threads
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);

    return q;
}

// queue destructor
void queue_delete(queue_t **q) {
    if (q == NULL || *q == NULL) {
        return;
    }

    // safely clear struct fields
    pthread_mutex_lock(&((*q)->mutex));
    free((*q)->queue);
    (*q)->queue = NULL;
    (*q)->cap = 0;
    (*q)->size = 0;
    (*q)->front = 0;
    (*q)->back = -1;
    pthread_mutex_unlock(&((*q)->mutex));

    // destroy threads
    pthread_mutex_destroy(&((*q)->mutex));
    pthread_cond_destroy(&((*q)->not_full));
    pthread_cond_destroy(&((*q)->not_empty));

    // free memory
    free(*q);
    *q = NULL;
}

// queue push an element
bool queue_push(queue_t *q, void *elem) {
    if (q == NULL) {
        return false;
    }

    // safely wait for mutex and non-full queue
    pthread_mutex_lock(&(q->mutex));
    while (q->size == q->cap) {
        pthread_cond_wait(&(q->not_full), &(q->mutex));
    }
    // push element
    q->back = (q->back + 1) % q->cap;
    q->queue[q->back] = elem;
    q->size++;
    // signal not_empty since element pushed
    pthread_cond_signal(&(q->not_empty));
    pthread_mutex_unlock(&(q->mutex));

    return true;
}

// queue pop an element
bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL) {
        return false;
    }

    // safely wait for mutex and non-empty queue
    pthread_mutex_lock(&(q->mutex));
    while (q->size == 0) {
        pthread_cond_wait(&(q->not_empty), &(q->mutex));
    }
    // pop element
    *elem = q->queue[q->front];
    q->front = (q->front + 1) % q->cap;
    q->size--;
    // signal not_full since element popped
    pthread_cond_signal(&(q->not_full));
    pthread_mutex_unlock(&(q->mutex));

    return true;
}
