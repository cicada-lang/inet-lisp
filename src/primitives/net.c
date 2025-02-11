#include "index.h"

void
x_connect(vm_t *vm) {
    wire_t *second_wire = stack_pop(vm->value_stack);
    wire_t *first_wire = stack_pop(vm->value_stack);

    wire_t *first_opposite = vm_wire_connect(vm, second_wire, first_wire);

    if (first_opposite->node) {
        vm_maybe_add_activity(vm, first_opposite->node);
    }
}

void
x_wire_print_net(vm_t *vm) {
    wire_t *wire = stack_top(vm->value_stack);
    wire_print_net(wire, stdout);
    fprintf(stdout, "\n");
}

void
x_link(vm_t *vm) {
    wire_t *first_wire = wire_new();
    wire_t *second_wire = wire_new();

    first_wire->opposite = second_wire;
    second_wire->opposite = first_wire;

    stack_push(vm->value_stack, first_wire);
    stack_push(vm->value_stack, second_wire);
}

void
x_run(vm_t *vm) {
    run_net(vm);
}

void
x_inspect_run(vm_t *vm) {
    x_wire_print_net(vm);
    x_run(vm);
    x_wire_print_net(vm);
}

void
x_debug(vm_t *vm) {
    fprintf(stdout, "[debug] start\n");
    debug_start(vm);
    fprintf(stdout, "[debug] end\n");
    fprintf(stdout, "\n");
}
