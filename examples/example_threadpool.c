#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/threadpool.h"

/* Mutex only for clean printing */
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

/* Task function */
void demo_task(void *arg) {
    int id = *(int *)arg;

    pthread_mutex_lock(&print_lock);
printf("Task %d executed by thread ID %lu\n", id, pthread_self());
    fflush(stdout);
    pthread_mutex_unlock(&print_lock);

    usleep(100000); // simulate work
}

int main() {
    const int TASK_COUNT = 100;

    printf("Running %d tasks using thread pool\n", TASK_COUNT);

    threadpool_t pool;
    threadpool_init(&pool, 8);

    int task_ids[TASK_COUNT];

    for (int i = 0; i < TASK_COUNT; i++) {
        task_ids[i] = i + 1;
        threadpool_submit(&pool, demo_task, &task_ids[i]);
    }

    sleep(3);

    threadpool_destroy(&pool);

    printf("All %d tasks executed successfully.\n", TASK_COUNT);
    return 0;
}
