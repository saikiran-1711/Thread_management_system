#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/threadpool.h"

/* Mutex only for clean printing */
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

/* Task function executed by threads */
void demo_task(void *arg) {
    int task_id = *(int *)arg;

    pthread_mutex_lock(&print_lock);
    printf("Task %d executed successfully by a thread\n", task_id);
    fflush(stdout);
    pthread_mutex_unlock(&print_lock);

    usleep(100000); // simulate work (0.1s)
}

int main() {
    int task_count;

    printf("Enter number of tasks to run: ");
    scanf("%d", &task_count);

    if (task_count <= 0) {
        printf("Invalid number of tasks.\n");
        return 1;
    }

    threadpool_t pool;

    /* Create thread pool with fixed threads (e.g., 8) */
    threadpool_init(&pool, 8);

    int *task_ids = malloc(sizeof(int) * task_count);

    for (int i = 0; i < task_count; i++) {
        task_ids[i] = i + 1;
        threadpool_submit(&pool, demo_task, &task_ids[i]);
    }

    /* Give threads time to finish */
    sleep(2);

    threadpool_destroy(&pool);
    free(task_ids);

    printf("All tasks completed.\n");
    return 0;
}
