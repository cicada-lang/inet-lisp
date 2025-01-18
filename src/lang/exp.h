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
        struct { exp_t *target; list_t *arg_list; } ap;
        struct { list_t *binding_list; list_t *body; } let;
    };
};

exp_t *exp_var(char *name);
exp_t *exp_ap(exp_t *target);
exp_t *exp_let(void);

void exp_destroy(exp_t **self_pointer);
