#pragma once

queue_t *queue_new(size_t size);
void queue_destroy(queue_t **self_pointer);

void queue_set_destroy_fn(queue_t *self, destroy_fn_t *destroy_fn);
queue_t *queue_new_with(size_t size, destroy_fn_t *destroy_fn);

bool queue_is_full(const queue_t *self);

// fail on full
void queue_enqueue(queue_t *self, void *value);
