#include "threadpool.h"
#include <stdio.h>

int main() {
    threadpool_t pool;
    threadpool_init(&pool, 3);
    threadpool_destroy(&pool);
    printf("All threads completed successfully\n");
    return 0;
}
