#include "index.h"

static stmt_t *
parse_define_node(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);
    sexp_t *name_sexp = list_next(sexp_list);
    char *name = string_copy(sexp_string(name_sexp));
    list_t *port_name_list = string_list_new();
    sexp_t *port_name_sexp = list_next(sexp_list);
    while (port_name_sexp) {
        list_push(port_name_list, string_copy(sexp_string(port_name_sexp)));
        port_name_sexp = list_next(sexp_list);
    }

    return stmt_define_node(name, port_name_list);
}

static stmt_t *
parse_define_function(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);

    list_t *name_sexp_list = sexp_sexp_list(list_next(sexp_list));
    sexp_t *first_name_sexp = list_first(name_sexp_list);
    char *name = string_copy(sexp_string(first_name_sexp));
    list_t *arg_name_list = string_list_new();
    sexp_t *name_sexp = list_next(name_sexp_list);
    while (name_sexp) {
        list_push(arg_name_list, string_copy(sexp_string(name_sexp)));
        name_sexp = list_next(name_sexp_list);
    }

    list_t *exp_list = exp_list_new();
    sexp_t *exp_sexp = list_next(sexp_list);
    while (exp_sexp) {
        list_push(exp_list, parse_exp(exp_sexp));
        exp_sexp = list_next(sexp_list);
    }

    return stmt_define_function(name, arg_name_list, exp_list);
}

static stmt_t *
parse_define_rule(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);

    exp_t *pattern_exp = parse_exp(list_next(sexp_list));

    list_t *exp_list = exp_list_new();
    sexp_t *exp_sexp = list_next(sexp_list);
    while (exp_sexp) {
        list_push(exp_list, parse_exp(exp_sexp));
        exp_sexp = list_next(sexp_list);
    }

    return stmt_define_rule(pattern_exp, exp_list);
}

static stmt_t *
parse_define_rule_star(sexp_t *sexp) {
    (void) sexp;
    return NULL;
}

static stmt_t *
parse_compute_exp(sexp_t *sexp) {
    return stmt_compute_exp(parse_exp(sexp));
}

stmt_t *
parse_stmt(sexp_t *sexp) {
    if (sexp_starts_with(sexp, "define-node"))
        return parse_define_node(sexp);
    if (sexp_starts_with(sexp, "define-rule"))
        return parse_define_rule(sexp);
    if (sexp_starts_with(sexp, "define-rule*"))
        return parse_define_rule_star(sexp);
    else if (sexp_starts_with(sexp, "define"))
        return parse_define_function(sexp);
    else if (is_list_sexp(sexp))
        return parse_compute_exp(sexp);
    else
        assert(false && "[parse_stmt] can not handle atom sexp");
}

list_t *
parse_stmt_list(list_t *sexp_list) {
    list_t *stmt_list = list_new_with((destroy_fn_t *) stmt_destroy);
    sexp_t *sexp = list_first(sexp_list);
    while (sexp) {
        list_push(stmt_list, parse_stmt(sexp));
        sexp = list_next(sexp_list);
    }

    return stmt_list;
}
