#pragma once

typedef void (primitive_vm_fn_t)(vm_t *vm);
typedef value_t (primitive_0_fn_t)(void);
typedef value_t (primitive_1_fn_t)(value_t x);
typedef value_t (primitive_2_fn_t)(value_t x, value_t y);
typedef value_t (primitive_3_fn_t)(value_t x, value_t y, value_t z);
typedef value_t (primitive_4_fn_t)(value_t x, value_t y, value_t z, value_t w);

typedef enum {
    PRIMITIVE_VM_FN,
    PRIMITIVE_0_FN,
    PRIMITIVE_1_FN,
    PRIMITIVE_2_FN,
    PRIMITIVE_3_FN,
    PRIMITIVE_4_FN,
} primitive_fn_kind_t;

extern object_spec_t primitive_object_spec;

struct primitive_t {
    object_spec_t *spec;
    char *name;
    size_t arity;
    primitive_fn_kind_t fn_kind;
    union {
        primitive_vm_fn_t *primitive_vm_fn;
        primitive_0_fn_t *primitive_0_fn;
        primitive_1_fn_t *primitive_1_fn;
        primitive_2_fn_t *primitive_2_fn;
        primitive_3_fn_t *primitive_3_fn;
        primitive_4_fn_t *primitive_4_fn;
    };
};

primitive_t *primitive_from_vm_fn(const char *name, size_t arity, primitive_vm_fn_t *primitive_vm_fn);
primitive_t *primitive_from_0_fn(const char *name, primitive_0_fn_t *primitive_0_fn);
primitive_t *primitive_from_1_fn(const char *name, primitive_1_fn_t *primitive_1_fn);
primitive_t *primitive_from_2_fn(const char *name, primitive_2_fn_t *primitive_2_fn);
primitive_t *primitive_from_3_fn(const char *name, primitive_3_fn_t *primitive_3_fn);
primitive_t *primitive_from_4_fn(const char *name, primitive_4_fn_t *primitive_4_fn);

void primitive_destroy(primitive_t **self_pointer);

bool is_primitive(value_t value);
primitive_t *as_primitive(value_t value);

void primitive_print(primitive_t *self, file_t *file);
