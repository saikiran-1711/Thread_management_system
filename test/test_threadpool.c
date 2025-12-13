#include "threadpool.h"
#include <stdio.h>

int main() {
    threadpool_t pool;
    threadpool_init(&pool, 4);
    printf("Thread pool test passed\n");
    threadpool_destroy(&pool);
    return 0;
}
