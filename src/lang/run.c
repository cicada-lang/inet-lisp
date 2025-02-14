#include "index.h"

void
step_op(vm_t *vm, frame_t *frame, op_t *op) {
    switch (op->kind) {
    case OP_APPLY: {
        value_t target = stack_pop(vm->value_stack);
        apply(vm, target, op->apply.arity);
        return;
    }

    case OP_LITERAL: {
        stack_push(vm->value_stack, op->literal.value);
        return;
    }

    case OP_LOCAL_GET: {
        value_t value = frame_local_get(frame, op->local_get.index);
        stack_push(vm->value_stack, value);
        return;
    }

    case OP_LOCAL_SET: {
        value_t value = stack_pop(vm->value_stack);
        frame_local_set(frame, op->local_set.index, value);
        return;
    }
    }
}

void
step(vm_t *vm) {
    if (stack_is_empty(vm->return_stack)) return;

    frame_t *frame = stack_pop(vm->return_stack);
    if (frame_is_finished(frame)) {
        frame_destroy(&frame);
        return;
    }

    op_t *op = frame_fetch_op(frame);

    // proper tail-call = do not push finished frame.
    bool finished = frame_is_finished(frame);
    if (!finished) {
        stack_push(vm->return_stack, frame);
    }

    step_op(vm, frame, op);

    if (finished) {
        frame_destroy(&frame);
    }
}

void
run_until(vm_t *vm, size_t base_length) {
#if DEBUG
    vm_print(vm, stdout);
    fprintf(stdout, "\n");
#endif

    while (stack_length(vm->return_stack) > base_length) {
        step(vm);

#if DEBUG
        vm_print(vm, stdout);
        fprintf(stdout, "\n");
#endif
    }
}

static void
return_local_wires(vm_t *vm, net_matcher_t *net_matcher) {
    list_t *local_name_list =
        net_pattern_local_name_list(net_matcher->net_pattern);
    char *name = list_first(local_name_list);
    while (name) {
        wire_t *wire = hash_get(net_matcher->wire_hash, name);
        assert(wire);
        wire_free_from_node(wire);
        stack_push(vm->value_stack, wire);
        name = list_next(local_name_list);
    }
}

static void
delete_match_nodes(vm_t *vm, net_matcher_t *net_matcher) {
    size_t length = net_pattern_length(net_matcher->net_pattern);
    for (size_t i = 0; i < length; i++) {
        node_t *matched_node = net_matcher->matched_nodes[i];
        assert(matched_node);
        for (size_t k = 0; k < matched_node->ctor->arity; k++) {
            if (!is_wire(matched_node->ports[k]))
                continue;

            wire_t *wire = as_wire(matched_node->ports[k]);
            if (wire_is_principal(wire))
                vm_delete_wire(vm, wire);
        }

        vm_delete_node(vm, matched_node);
    }
}

void
step_net(vm_t *vm) {
    activity_t *activity = list_shift(vm->activity_list);
    if (activity == NULL) return;

    return_local_wires(vm, activity->net_matcher);
    delete_match_nodes(vm, activity->net_matcher);

    size_t base_length = stack_length(vm->return_stack);
    frame_t *frame = frame_new(activity->rule->function);

    activity_destroy(&activity);

    stack_push(vm->return_stack, frame);
    run_until(vm, base_length);
}

void
run_net(vm_t *vm) {
    while (!list_is_empty(vm->activity_list)) {
        step_net(vm);
    }
}
