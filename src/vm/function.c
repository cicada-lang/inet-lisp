#include "index.h"

object_spec_t function_object_spec = {
    .name = "function",
    .print_fn = (print_fn_t *) function_print,
};

struct function_t {
    object_spec_t *spec;
    size_t arity;
    function_ctx_t *ctx;
    list_t *op_list;
    size_t length;
    op_t **ops;
};

function_t *
function_new(size_t arity) {
    function_t *self = new(function_t);
    self->spec = &function_object_spec;
    self->arity = arity;
    self->ctx = function_ctx_new();
    self->op_list = list_new_with((destroy_fn_t *) op_destroy);
    self->length = 0;
    self->ops = NULL;
    return self;
}

void
function_destroy(function_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        function_t *self = *self_pointer;
        list_destroy(&self->op_list);
        function_ctx_destroy(&self->ctx);
        if (self->ops) free(self->ops);
        free(self);
        *self_pointer = NULL;
    }
}

bool
is_function(value_t value) {
    if (!is_xobject(value)) return false;
    return as_object(value)->spec == &function_object_spec;
}

function_t *
as_function(value_t value) {
    assert(is_function(value));
    return (function_t *) value;
}

size_t
function_length(const function_t *self) {
    return self->length;
}

void
function_add_op(function_t *self, op_t *op) {
    list_push(self->op_list, op);
}

void
function_build(function_t *self) {
    self->length = list_length(self->op_list);
    self->ops = allocate_pointers(self->length);
    size_t index = 0;
    op_t *op = list_first(self->op_list);
    while (op) {
        self->ops[index] = op;
        op = list_next(self->op_list);
        index++;
    }
}

op_t *
function_get_op(const function_t *self, size_t index) {
    return self->ops[index];
}

void
function_print(const function_t *self, file_t *file) {
    for (size_t i = 0; i < self->length; i++) {
        op_print(self->ops[i], file);
        fprintf(file, "\n");
    }
}

void
function_print_with_cursor(const function_t *self, file_t *file, size_t cursor) {
    for (size_t i = 0; i < self->length; i++) {
        if (i == cursor) {
            op_print(self->ops[i], file);
            fprintf(file, " <<<");
            fprintf(file, "\n");
        } else {
            op_print(self->ops[i], file);
            fprintf(file, "\n");
        }
    }
}
