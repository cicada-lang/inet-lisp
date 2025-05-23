#include "index.h"

inline void
worker_apply_primitive(worker_t *worker, const primitive_t *primitive) {
    switch (primitive->fn_kind) {
    case PRIMITIVE_FN: {
        primitive->primitive_fn(worker);
        return;
    }

    case PRIMITIVE_FN_0: {
        value_t result = primitive->primitive_fn_0();
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_1: {
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_1(x);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_2: {
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_2(x, y);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_3: {
        value_t z = stack_pop(worker->value_stack);
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_3(x, y, z);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_4: {
        value_t w = stack_pop(worker->value_stack);
        value_t z = stack_pop(worker->value_stack);
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_4(x, y, z, w);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }
    }
}
