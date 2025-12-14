#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

/* Task node */
typedef struct task {
    void (*function)(void *);
    void *argument;
    struct task *next;
} task_t;

/* Thread pool structure */
typedef struct threadpool {
    pthread_t *threads;
    int thread_count;

    task_t *task_head;
    task_t *task_tail;

    pthread_mutex_t lock;
    pthread_cond_t task_available;

    int shutdown;
} threadpool_t;

/* Public API */
void threadpool_init(threadpool_t *pool, int num_threads);
void threadpool_submit(threadpool_t *pool,
                       void (*function)(void *),
                       void *argument);
void threadpool_destroy(threadpool_t *pool);

#endif

