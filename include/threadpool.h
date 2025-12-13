#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

typedef struct {
    pthread_t *threads;
    int num_threads;
    int stop;
} threadpool_t;

int threadpool_init(threadpool_t *pool, int num_threads);
void threadpool_destroy(threadpool_t *pool);

#endif
