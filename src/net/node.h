#pragma once

struct node_t {
    const node_ctor_t *ctor;
    fast_spinlock_t *fast_spinlock;
    void *locked_by_worker;
    size_t id;
    bool is_allocated;
    array_t *value_array;

    // for primitive node
    atomic_size_t atomic_primitive_arg_count;
};

node_t *node_new(void);
void node_destroy(node_t **self_pointer);
void node_clean(node_t *self);

void node_set_value(node_t *self, size_t index, value_t value);
value_t node_get_value(const node_t *self, size_t index);
port_info_t *node_get_port_info(const node_t *self, size_t index);

void node_print_name(const node_t *self, file_t *file);
void node_print(const node_t *self, file_t *file);
void node_print_port_info(const node_t *self, size_t index, file_t *file);

bool node_has_wire(node_t *node, wire_t *wire);
size_t node_wire_index(node_t *node, wire_t *wire);

bool node_is_primitive(const node_t *self);
size_t node_primitive_arg_count_fetch_add1(node_t *self);

void node_lock(node_t *self);
bool node_try_lock(node_t *self);
void node_unlock(node_t *self);

size_t node_principal_port_count(const node_t *self);
