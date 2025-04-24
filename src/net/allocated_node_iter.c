#include "index.h"

allocated_node_iter_t *
allocated_node_iter_new(node_allocator_t *node_allocator) {
    allocated_node_iter_t *self = new(allocated_node_iter_t);
    self->node_allocator = node_allocator;
    self->cursor = 0;
    return self;
}

void
allocated_node_iter_destroy(allocated_node_iter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    allocated_node_iter_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}

node_t *
allocated_node_iter_first(allocated_node_iter_t *self) {
    self->cursor = 0;

    node_t *node = array_get(self->node_allocator->node_array, self->cursor++);
    while (!node || (node && !node->is_allocated)) {
        if (self->cursor >= array_length(self->node_allocator->node_array))
            return NULL;

        node = array_get(self->node_allocator->node_array, self->cursor++);
    }

    return node;
}

node_t *
allocated_node_iter_next(allocated_node_iter_t *self) {
    node_t *node = array_get(self->node_allocator->node_array, self->cursor++);
    while (!node || (node && !node->is_allocated)) {
        if (self->cursor >= array_length(self->node_allocator->node_array))
            return NULL;

        node = array_get(self->node_allocator->node_array, self->cursor++);
    }

    return node;
}

array_t *
allocated_node_array(node_allocator_t *node_allocator) {
    mutex_lock(node_allocator->allocator->mutex);

    array_t *node_array = array_new_auto();
    allocated_node_iter_t *allocated_node_iter = allocated_node_iter_new(node_allocator);
    node_t *node = allocated_node_iter_first(allocated_node_iter);
    while (node) {
        array_push(node_array, node);
        node = allocated_node_iter_next(allocated_node_iter);
    }

    allocated_node_iter_destroy(&allocated_node_iter);
    mutex_unlock(node_allocator->allocator->mutex);
    return node_array;
}
