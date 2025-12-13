#include "threadpool.h"
#include <stdlib.h>
#include <stdio.h>

void* worker(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d is running\n", id);
    return NULL;
}

int threadpool_init(threadpool_t *pool, int num_threads) {
    pool->num_threads = num_threads;
    pool->stop = 0;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&pool->threads[i], NULL, worker, id);
    }

    return 0;
}

void threadpool_destroy(threadpool_t *pool) {
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
}
