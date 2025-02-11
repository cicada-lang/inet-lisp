#pragma once

typedef enum {
    DEF_PRIMITIVE,
    DEF_FUNCTION,
    DEF_NODE,
} def_kind_t;

struct def_t {
    def_kind_t kind;
    union {
        primitive_t *primitive;
        function_t *function;
        struct { node_ctor_t *ctor; } node;
    };
};

def_t *def_primitive(primitive_t *primitive);
def_t *def_function(function_t *function);
def_t *def_node(node_ctor_t *ctor);

void def_destroy(def_t **self_pointer);

const char *def_name(const def_t *self);
const char *def_kind_name(def_kind_t kind);

void def_print(const def_t *self, file_t *file);
