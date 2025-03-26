#include "index.h"

object_spec_t function_object_spec = {
    .name = "function",
    .print_fn = (print_fn_t *) function_print_name,
};

function_t *
function_new(size_t arity) {
    function_t *self = new(function_t);
    self->spec = &function_object_spec;
    self->arity = arity;
    self->local_index_hash = hash_of_string_key();
    self->op_array = array_auto_with((destroy_fn_t *) op_destroy);
    return self;
}

void
function_destroy(function_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        function_t *self = *self_pointer;
        string_destroy(&self->name);
        hash_destroy(&self->local_index_hash);
        array_destroy(&self->op_array);
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
    return array_length(self->op_array);
}

void
function_add_op(function_t *self, op_t *op) {
    array_push(self->op_array, op);
}

op_t *
function_get_op(const function_t *self, size_t index) {
    return array_get(self->op_array, index);
}

void
function_print_name(const function_t *self, file_t *file) {
    fprintf(file, "%s", self->name);
}

void
function_print(const function_t *self, file_t *file) {
    fprintf(file, "<function %s>\n", self->name);
    for (size_t i = 0; i < function_length(self); i++) {
        op_t *op = array_get(self->op_array, i);
        op_print(op, file);
        fprintf(file, "\n");
    }
    fprintf(file, "</function>\n");
}

void
function_print_with_cursor(const function_t *self, file_t *file, size_t cursor) {
    fprintf(file, "<function %s>\n", self->name);
    for (size_t i = 0; i < function_length(self); i++) {
        if (i == cursor) {
            op_print(function_get_op(self, i), file);
            fprintf(file, " <<<");
            fprintf(file, "\n");
        } else {
            op_print(function_get_op(self, i), file);
            fprintf(file, "\n");
        }
    }
    fprintf(file, "</function>\n");
}
