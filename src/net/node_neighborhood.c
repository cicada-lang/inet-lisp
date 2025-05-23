#include "index.h"

struct node_neighborhood_t {
    node_t *node;
    array_t *node_neighbor_array;
};

node_neighborhood_t *
node_neighborhood_new(node_t *node) {
    node_neighborhood_t *self = new(node_neighborhood_t);
    self->node = node;
    self->node_neighbor_array = array_new_auto_with((destroy_fn_t *) node_neighbor_destroy);
    return self;
}

void
node_neighborhood_destroy(node_neighborhood_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_neighborhood_t *self = *self_pointer;
    array_destroy(&self->node_neighbor_array);
    free(self);
    *self_pointer = NULL;
}

node_t *
node_neighborhood_node(node_neighborhood_t *self) {
    return self->node;
}

node_neighbor_t *
node_neighborhood_get(node_neighborhood_t *self, size_t index) {
    return array_get(self->node_neighbor_array, index);
}

void
node_neighborhood_maybe_add_node(node_neighborhood_t *self, node_t *node) {
    node_neighbor_t *node_neighbor = node_neighbor_new_maybe(self->node, node);
    if (node_neighbor) {
        array_set(
            self->node_neighbor_array,
            node_neighbor->start_port_index,
            node_neighbor);
    }
}

static void
node_neighbor_print_end_port(const node_neighbor_t *self, file_t *file) {
    fprintf(file, "(");
    node_print_port_info(self->end_node, self->end_port_index, file);
    fprintf(file, " ");
    node_print(self->end_node, file);
    fprintf(file, ")");
}

static void
node_neighborhood_print_one(node_neighborhood_t *self, size_t i, file_t *file) {
    node_print_port_info(self->node, i, file);
    fprintf(file, " ");

    node_neighbor_t *node_neighbor = array_get(self->node_neighbor_array, i);
    if (node_neighbor) {
        node_neighbor_print_end_port(node_neighbor, file);
        return;
    }

    value_t value = node_get_value(self->node, i);
    if (!value) {
        fprintf(file, "____");
        return;
    }

    value = walk(value);
    if (is_non_wire(value)) {
        value_print(value, file);
    } else {
        fprintf(file, "____");
    }
}

void
node_neighborhood_print(node_neighborhood_t *self, const char *prefix, file_t *file) {
    fprintf(file, "%s", prefix);
    fprintf(file, "(");
    node_print_name(self->node, file);
    fprintf(file, "\n");

    size_t length = self->node->ctor->arity;
    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%s", prefix);
        fprintf(file, " ");
        node_neighborhood_print_one(self, i, file);
        if (i == length - 1) {
            fprintf(file, ")");
        } else {
            fprintf(file, "\n");
        }
    }
}

hash_t *
build_node_neighborhood_hash(node_allocator_t *node_allocator) {
    hash_t *node_neighborhood_hash = hash_new();
    hash_set_destroy_fn(node_neighborhood_hash, (destroy_fn_t *) node_neighborhood_destroy);

    array_t *node_array = allocated_node_array(node_allocator);
    size_t length = array_length(node_array);
    for (size_t i = 0; i < length; i++) {
        node_t *x = array_get(node_array, i);
        node_neighborhood_t *node_neighborhood = node_neighborhood_new(x);
        hash_set(node_neighborhood_hash, x, node_neighborhood);
        for (size_t j = 0; j < length; j++) {
            node_t *y = array_get(node_array, j);
            node_neighborhood_maybe_add_node(node_neighborhood, y);
        }
    }

    array_destroy(&node_array);
    return node_neighborhood_hash;
}
