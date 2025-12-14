#include "../include/threadpool.h"
#include <stdlib.h>
#include <stdio.h>

static void *worker_thread(void *arg) {
    threadpool_t *pool = (threadpool_t *)arg;

    while (1) {
        pthread_mutex_lock(&pool->lock);

        while (!pool->shutdown && pool->task_head == NULL) {
            pthread_cond_wait(&pool->task_available, &pool->lock);
        }

        if (pool->shutdown && pool->task_head == NULL) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }

        task_t *task = pool->task_head;
        pool->task_head = task->next;
        if (pool->task_head == NULL) {
            pool->task_tail = NULL;
        }

        pthread_mutex_unlock(&pool->lock);

        task->function(task->argument);
        free(task);
    }

    return NULL;
}

void threadpool_init(threadpool_t *pool, int num_threads) {
    pool->thread_count = num_threads;
    pool->shutdown = 0;
    pool->task_head = NULL;
    pool->task_tail = NULL;

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->task_available, NULL);

    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    }
}

void threadpool_submit(threadpool_t *pool,
                       void (*function)(void *),
                       void *argument) {
    task_t *task = malloc(sizeof(task_t));
    task->function = function;
    task->argument = argument;
    task->next = NULL;

    pthread_mutex_lock(&pool->lock);

    if (pool->task_tail == NULL) {
        pool->task_head = task;
        pool->task_tail = task;
    } else {
        pool->task_tail->next = task;
        pool->task_tail = task;
    }

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
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->task_available);
}


