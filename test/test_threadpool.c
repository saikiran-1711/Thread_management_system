#include "threadpool.h"
#include <stdio.h>

int main() {
    threadpool_t pool;
    threadpool_init(&pool, 2);
    threadpool_destroy(&pool);
    printf("Thread pool test passed\n");
    return 0;
}
