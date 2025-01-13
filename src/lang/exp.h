#pragma once

typedef enum {
    EXP_VAR,
    EXP_AP,
    EXP_LET,
} exp_kind_t;

struct exp_t {
    exp_kind_t kind;
    union {
        struct { char *name; } var;
        struct { exp_t *target; size_t size; exp_t **args; } ap;
        // TODO let;
    };
};
