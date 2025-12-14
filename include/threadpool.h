#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

/* Task structure */
typedef struct task {
    void (*function)(void *);
    void *argument;
    struct task *next;
} task_t;

/* Thread state (for monitoring & explanation) */
typedef enum {
    THREAD_IDLE,
    THREAD_BUSY
} thread_state_t;

/* Thread pool structure */
typedef struct threadpool {
    pthread_t *threads;
    thread_state_t *states;

    int thread_count;
    int shutdown;

    task_t *task_head;
    task_t *task_tail;

    pthread_mutex_t lock;
    pthread_cond_t task_available;
} threadpool_t;

/* Public API */
void threadpool_init(threadpool_t *pool, int num_threads);
void threadpool_submit(threadpool_t *pool,
                       void (*function)(void *),
                       void *argument);
void threadpool_destroy(threadpool_t *pool);

#endif

