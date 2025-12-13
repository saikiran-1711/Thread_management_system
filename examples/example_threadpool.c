#include "threadpool.h"
#include <stdio.h>

int main() {
    threadpool_t pool;
    threadpool_init(&pool, 2);
    printf("Thread pool initialized\n");
    threadpool_destroy(&pool);
    return 0;
}
