#include "index.h"

void
worker_add_task(worker_t *worker, task_t *task) {
    queue_push_back(worker->task_queue, task);

    if (worker->scheduler) {
        scheduler_task_count_add1(worker->scheduler, worker->id);
    }
}
