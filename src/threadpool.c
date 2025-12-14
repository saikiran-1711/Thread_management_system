#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../include/threadpool.h"


#define MAX_THREADS 256   /* Safe upper bound for scalability */

typedef enum {
    THREAD_IDLE,
    THREAD_BUSY,
    THREAD_STOPPED
} thread_state_t;

typedef struct task {
    void (*function)(void *);
    void *argument;
    struct task *next;
} task_t;

/* THREAD POOL STRUCTURE */

struct threadpool {
    pthread_t *threads;
    thread_state_t *states;

    int thread_count;
    int shutdown;

    task_t *task_head;
    task_t *task_tail;

    pthread_mutex_t lock;
    pthread_cond_t task_available;
};

/*  WORKER THREAD FUNCTION*/
/*
   EXPLAINED BY:
   - Saikiran: lifecycle, execution loop
   - Narahari: mutex + condition variable logic
*/

static void *worker_thread(void *arg) {
    threadpool_t *pool = (threadpool_t *)arg;

    while (1) {
        pthread_mutex_lock(&pool->lock);

        while (!pool->task_head && !pool->shutdown) {
            pthread_cond_wait(&pool->task_available, &pool->lock);
        }

        if (pool->shutdown && !pool->task_head) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        
        task_t *task = pool->task_head;
        pool->task_head = task->next;
        if (!pool->task_head) {
            pool->task_tail = NULL;
        }

        pthread_mutex_unlock(&pool->lock);

        
        if (task) {
            task->function(task->argument);
            free(task);
        }
    }
    return NULL;
}

/*  THREAD POOL INITIALIZATION */
/* Saikiran*/

void threadpool_init(threadpool_t *pool, int num_threads) {
    if (num_threads <= 0) {
        num_threads = 1;
    }

    if (num_threads > MAX_THREADS) {
        num_threads = MAX_THREADS;
    }

    pool->thread_count = num_threads;
    pool->shutdown = 0;

    pool->task_head = NULL;
    pool->task_tail = NULL;

    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    pool->states = malloc(sizeof(thread_state_t) * num_threads);

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->task_available, NULL);

    for (int i = 0; i < num_threads; i++) {
        pool->states[i] = THREAD_IDLE;
        pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    }
}

/* TASK SUBMISSION*/
/*Narahari*/

void threadpool_submit(threadpool_t *pool,
                       void (*function)(void *),
                       void *argument) {
    task_t *task = malloc(sizeof(task_t));
    task->function = function;
    task->argument = argument;
    task->next = NULL;

    pthread_mutex_lock(&pool->lock);

    if (pool->task_tail) {
        pool->task_tail->next = task;
    } else {
        pool->task_head = task;
    }
    pool->task_tail = task;

    pthread_cond_signal(&pool->task_available);
    pthread_mutex_unlock(&pool->lock);
}

void threadpool_destroy(threadpool_t *pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->task_available);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    free(pool->states);

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->task_available);
}

