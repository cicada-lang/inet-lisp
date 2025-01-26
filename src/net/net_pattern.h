#pragma once

struct net_pattern_t {
    const list_t *node_pattern_list;
    size_t starting_index;
};

net_pattern_t *net_pattern_new(const list_t *node_pattern_list, size_t starting_index);
void net_pattern_destroy(net_pattern_t **self_pointer);

size_t net_pattern_length(const net_pattern_t *self);
size_t net_pattern_starting_index(const net_pattern_t *self);
