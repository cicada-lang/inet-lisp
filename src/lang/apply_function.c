#include "index.h"

void
apply_function(vm_t *vm, function_t *function, size_t arity) {
    // TODO supplement wire if arity is short
    assert(function->arity == arity);
    frame_t *frame = frame_new(function);
    stack_push(vm->return_stack, frame);
    return;
}
