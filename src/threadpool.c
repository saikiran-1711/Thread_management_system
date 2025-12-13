#include "threadpool.h"
#include <stdlib.h>

int threadpool_init(threadpool_t *pool, int num_threads) {
    pool->num_threads = num_threads;
    pool->stop = 0;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    return pool->threads ? 0 : -1;
}

void threadpool_destroy(threadpool_t *pool) {
    free(pool->threads);
}
