#pragma once

extern object_spec_t wire_object_spec;

struct wire_t {
    object_spec_t *spec;
    // might be `wire_t` or `principal_wire_t`
    atomic_value_t atomic_fuzed_value;
};

wire_t *wire_new(void);
void wire_destroy(wire_t **self_pointer);

inline bool
is_wire(value_t value) {
    if (!is_xobject(value)) return false;
    return as_object(value)->spec == &wire_object_spec;
}

inline wire_t *
as_wire(value_t value) {
    assert(is_wire(value));
    return (wire_t *) value;
}

value_t walk(value_t value);
value_t defuze(value_t value);
bool is_fuzed(value_t x, value_t y);
bool is_connected(value_t x, value_t y);
bool is_non_wire(value_t value);
