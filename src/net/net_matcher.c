#include "index.h"

net_matcher_t *
net_matcher_new(const net_pattern_t *net_pattern) {
    net_matcher_t *self = new(net_matcher_t);
    self->net_pattern = net_pattern;
    self->wire_hash = hash_of_string_key();
    self->matched_nodes = allocate_pointers(
        array_length(net_pattern->node_pattern_array));
    self->principle_name_list = string_list_new();
    self->matched_principle_name_list = string_list_new();
    return self;
}

void
net_matcher_destroy(net_matcher_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        net_matcher_t *self = *self_pointer;
        hash_destroy(&self->wire_hash);
        free(self->matched_nodes);
        list_destroy(&self->principle_name_list);
        list_destroy(&self->matched_principle_name_list);
        free(self);
        *self_pointer = NULL;
    }
}

static void
matcher_match_node(net_matcher_t *self, size_t index, const node_t *node) {
    const node_pattern_t *node_pattern =
        array_get(self->net_pattern->node_pattern_array, index);

    if (node_pattern->ctor != node->ctor)
        return;

    for (size_t i = 0; i < node->ctor->arity; i++) {
        port_info_t *port_info = node_pattern->port_infos[i];
        wire_t *wire = node->wires[i];
        if (port_info->is_principal) {
            if (hash_has(self->wire_hash, port_info->name)) {
                wire_t *existing_wire = hash_get(self->wire_hash, port_info->name);
                if (!wire->opposite) return;
                if (!existing_wire->opposite) return;
                if (wire->opposite != existing_wire->opposite) return;
            } else {
                list_push(self->principle_name_list, string_copy(port_info->name));
                hash_set(self->wire_hash, port_info->name, wire);
            }
        } else {
            if (!hash_set(self->wire_hash, port_info->name, wire))
                return;
        }
    }

    self->matched_nodes[index] = node;
}

static const char *
matcher_next_principle_name(net_matcher_t *self) {
    char *name = list_pop(self->principle_name_list);
    if (!name) return NULL;

    list_push(self->matched_principle_name_list, name);
    return name;
}

static bool
matcher_index_is_used(net_matcher_t *self, size_t index) {
    return self->matched_nodes[index] != NULL;
}

static size_t
matcher_next_index(net_matcher_t *self, const char *name) {
    (void) matcher_index_is_used;
    (void) self;
    (void) name;
    return 0;
}

static const node_t *
matcher_next_node(net_matcher_t *self, const char *name) {
    wire_t *wire = hash_get(self->wire_hash, name);
    if (!wire) return NULL;
    if (!wire->opposite) return NULL;
    if (!wire->opposite->node) return NULL;

    return wire->opposite->node;
}

static bool
matcher_is_success(const net_matcher_t *self) {
    size_t size = array_size(self->net_pattern->node_pattern_array);
    for (size_t i = 0; i < size; i++) {
        if (self->matched_nodes[i] == NULL) return false;
    }

    return true;
}

static void
matcher_start(net_matcher_t *self, const node_t *node) {
    size_t index = self->net_pattern->starting_index;
    matcher_match_node(self, index, node);
    const char *name = matcher_next_principle_name(self);
    while (name) {
        index = matcher_next_index(self, name);
        node = matcher_next_node(self, name);
        if (node) {
            matcher_match_node(self, index, node);
        }
    }
}

net_matcher_t *
match_net(const net_pattern_t *net_pattern, const node_t *node) {
    net_matcher_t *self = net_matcher_new(net_pattern);
    matcher_start(self, node);
    if (!matcher_is_success(self)) {
        net_matcher_destroy(&self);
        return NULL;
    }

    return self;
}
