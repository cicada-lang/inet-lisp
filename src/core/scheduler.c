#include "index.h"

scheduler_t *
scheduler_new(mod_t *mod, size_t worker_pool_size) {
    scheduler_t *self = new_shared(scheduler_t);
    self->mod = mod;
    self->task_queue_size = SCHEDULER_TASK_QUEUE_SIZE;

    self->worker_pool_size = worker_pool_size;
    self->workers = allocate_pointers(worker_pool_size);
    for (size_t i = 0; i < worker_pool_size; i++)
        self->workers[i] = worker_new(mod);

    self->task_queues = allocate_pointers(worker_pool_size);
    for (size_t i = 0; i < worker_pool_size; i++)
        self->task_queues[i] = queue_new_with(
            self->task_queue_size,
            (destroy_fn_t *) task_destroy);

    return self;
}

void
scheduler_destroy(scheduler_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        scheduler_t *self = *self_pointer;

        for (size_t i = 0; i < self->worker_pool_size; i++)
            worker_destroy(&self->workers[i]);
        free(self->workers);

        for (size_t i = 0; i < self->worker_pool_size; i++)
            queue_destroy(&self->task_queues[i]);
        free(self->task_queues);

        free(self);
        *self_pointer = NULL;
    }
}
