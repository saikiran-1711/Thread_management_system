int main() {
    const int TASK_COUNT = 100;

    printf("Running %d tasks using thread pool\n", TASK_COUNT);

    threadpool_t pool;

    /* Create thread pool with fixed number of worker threads */
    threadpool_init(&pool, 8);

    int task_ids[TASK_COUNT];

    for (int i = 0; i < TASK_COUNT; i++) {
        task_ids[i] = i + 1;
        threadpool_submit(&pool, demo_task, &task_ids[i]);
    }

    /* Give enough time for all tasks to finish */
    sleep(3);

    threadpool_destroy(&pool);

    printf("All %d tasks executed successfully.\n", TASK_COUNT);
    return 0;
}
