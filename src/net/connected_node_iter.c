#include "index.h"

connected_node_iter_t *
connected_node_iter_new(node_t *root, hash_t *node_neighborhood_hash) {
    assert(root);
    connected_node_iter_t *self = new(connected_node_iter_t);
    self->root = root;
    self->node_neighborhood_hash = node_neighborhood_hash;
    self->occurred_node_set = set_new();
    self->remaining_node_list = list_new();
    return self;
}

void
connected_node_iter_destroy(connected_node_iter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    connected_node_iter_t *self = *self_pointer;
    set_destroy(&self->occurred_node_set);
    list_destroy(&self->remaining_node_list);
    free(self);
    *self_pointer = NULL;
}

static void
take_node(connected_node_iter_t *self, node_t *node) {
    set_add(self->occurred_node_set, node);
    array_t *node_neighbor_array = hash_get(self->node_neighborhood_hash, node);
    for (size_t i = 0; i < array_length(node_neighbor_array); i++) {
        node_neighbor_t *node_neighbor = array_get(node_neighbor_array, i);
        list_push(self->remaining_node_list, node_neighbor->end_node);
    }
}

node_t *
connected_node_iter_first(connected_node_iter_t *self) {
    take_node(self, self->root);
    return self->root;
}

node_t *
connected_node_iter_next(connected_node_iter_t *self) {
    node_t *node = list_pop(self->remaining_node_list);
    if (!node) return NULL;

    if (set_has(self->occurred_node_set, node))
        return connected_node_iter_next(self);

    take_node(self, node);
    return node;
}

array_t *
connected_node_array(node_t *root, hash_t *node_neighborhood_hash) {
    array_t *node_array = array_new_auto();
    connected_node_iter_t *connected_node_iter =
        connected_node_iter_new(root, node_neighborhood_hash);
    node_t *node = connected_node_iter_first(connected_node_iter);
    while (node) {
        array_push(node_array, node);
        node = connected_node_iter_next(connected_node_iter);
    }

    connected_node_iter_destroy(&connected_node_iter);
    return node_array;
}
