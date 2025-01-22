#pragma once

void apply(vm_t *vm, value_t *target, size_t arity);

void apply_function(vm_t *vm, function_t *function, size_t arity);
void apply_node(vm_t *vm, node_t *node, size_t arity);
