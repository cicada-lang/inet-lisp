#pragma once

void apply(vm_t *vm, value_t target, size_t arity);

void apply_primitive(vm_t *vm, primitive_t *primitive, size_t arity);
void apply_function(vm_t *vm, function_t *function, size_t arity);

void apply_node_input_ports(vm_t *vm, node_t *node, size_t arity);
void apply_node_output_ports(vm_t *vm, node_t *node, size_t arity);
void apply_node_ctor(vm_t *vm, node_ctor_t *node_ctor, size_t arity);
