#include "index.h"

manager_t *
manager_new(mod_t *mod, size_t worker_pool_size) {
    manager_t *self = new_shared(manager_t);
    self->mod = mod;

    self->worker_pool_size = worker_pool_size;
    self->workers = allocate_pointers(worker_pool_size);
    for (size_t i = 0; i < worker_pool_size; i++) {
        self->workers[i] = worker_new(mod);
        self->workers[i]->manager = self;
        self->workers[i]->index = i;
    }

    self->task_queue_size = SCHEDULER_TASK_QUEUE_SIZE;
    self->task_queues = allocate_pointers(worker_pool_size);
    for (size_t i = 0; i < worker_pool_size; i++) {
        self->task_queues[i] = queue_new_with(
            self->task_queue_size,
            (destroy_fn_t *) task_destroy);
    }

    return self;
}

void
manager_destroy(manager_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        manager_t *self = *self_pointer;

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

static void *
manager_thread_fn(manager_t *self) {
    (void) self;
    return NULL;
}

void
manager_start(manager_t *self) {
    self->thread_id = thread_start((thread_fn_t *) manager_thread_fn, self);
    self->is_started = true;
}

void
manager_wait(manager_t *self) {
    assert(self->thread_id);
    thread_wait(self->thread_id);
}
