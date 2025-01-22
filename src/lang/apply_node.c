#include "index.h"

static void
node_apply_input_ports(vm_t *vm, node_t *node, size_t arity) {
    for (size_t c = 0; c < arity; c++) {
        wire_t *wire = stack_pop(vm->value_stack);
        size_t i = arity - 1 - c;
        wire->node = node;
        wire->index = i;
        node->wires[i] = wire;

        vm_maybe_add_active_wire(vm, wire, wire->opposite);
    }
}

static void
node_return_output_ports(vm_t *vm, node_t *node, size_t arity) {
    size_t output_arity = node->def->arity - arity;
    for (size_t c = 0; c < output_arity; c++) {
        wire_t *node_wire = wire_new();
        wire_t *free_wire = wire_new();

        node_wire->opposite = free_wire;
        free_wire->opposite = node_wire;

        size_t i = arity + c;
        node_wire->node = node;
        node_wire->index = i;
        node->wires[i] = node_wire;

        stack_push(vm->value_stack, free_wire);
    }
}

void
apply_node(vm_t *vm, node_t *node, size_t arity) {
    node_apply_input_ports(vm, node, arity);
    node_return_output_ports(vm, node, arity);
    return;
}
