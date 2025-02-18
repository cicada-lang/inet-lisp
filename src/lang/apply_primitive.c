#include "index.h"

static bool
is_directly_appliable(vm_t *vm, primitive_t *primitive, size_t arity) {
    if (primitive->input_arity != arity) return false;

    for (size_t i = 0; i < primitive->input_arity; i++) {
        if (primitive->node_ctor == NULL ||
            primitive->node_ctor->port_infos[i]->is_principal)
        {
            value_t value = stack_pick(vm->value_stack, i);
            if (is_wire(value)) return false;
        }
    }

    return true;
}

static void
apply_primitive_directly(vm_t *vm, primitive_t *primitive) {
    switch (primitive->fn_kind) {
    case PRIMITIVE_FN: {
        primitive->primitive_fn(vm);
        return;
    }

    case PRIMITIVE_FN_0: {
        value_t result = primitive->primitive_fn_0();
        stack_push(vm->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_1: {
        value_t x = stack_pop(vm->value_stack);
        value_t result = primitive->primitive_fn_1(x);
        stack_push(vm->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_2: {
        value_t y = stack_pop(vm->value_stack);
        value_t x = stack_pop(vm->value_stack);
        value_t result = primitive->primitive_fn_2(x, y);
        stack_push(vm->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_3: {
        value_t z = stack_pop(vm->value_stack);
        value_t y = stack_pop(vm->value_stack);
        value_t x = stack_pop(vm->value_stack);
        value_t result = primitive->primitive_fn_3(x, y, z);
        stack_push(vm->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_4: {
        value_t w = stack_pop(vm->value_stack);
        value_t z = stack_pop(vm->value_stack);
        value_t y = stack_pop(vm->value_stack);
        value_t x = stack_pop(vm->value_stack);
        value_t result = primitive->primitive_fn_4(x, y, z, w);
        stack_push(vm->value_stack, result);
        return;
    }
    }
}

void
apply_primitive(vm_t *vm, primitive_t *primitive, size_t arity) {
    // TODO supplement wire if arity is short
    assert(primitive->input_arity == arity);

    if (primitive->node_ctor == NULL ||
        is_directly_appliable(vm, primitive, arity))
    {
        apply_primitive_directly(vm, primitive);
        return;
    }

    node_t *node = vm_add_node(vm, primitive->node_ctor);
    apply_node_input_ports(vm, node, arity);
    apply_node_output_ports(vm, node, arity);
    // TODO maybe no need to call `activate_node_and_neighbor`,
    activate_node_and_neighbor(vm, node);
}
