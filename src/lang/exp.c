#include "index.h"

exp_t *
exp_var(char *name) {
    exp_t *self = new(exp_t);
    self->kind = EXP_VAR;
    self->var.name = name;
    return self;
}

exp_t *
exp_ap(exp_t *target, list_t *arg_list) {
    exp_t *self = new(exp_t);
    self->kind = EXP_AP;
    self->ap.target = target;
    self->ap.arg_list = arg_list;
    return self;
}

exp_t *
exp_bind(list_t *name_list) {
    exp_t *self = new(exp_t);
    self->kind = EXP_BIND;
    self->bind.name_list = name_list;
    return self;
}

list_t *
exp_list_new(void) {
    return list_new_with((destroy_fn_t *) exp_destroy);
}

void
exp_destroy(exp_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        exp_t *self = *self_pointer;
        switch (self->kind) {
        case EXP_VAR: {
            string_destroy(&self->var.name);
            break;
        }

        case EXP_AP: {
            exp_destroy(&self->ap.target);
            list_destroy(&self->ap.arg_list);
            break;
        }

        case EXP_BIND: {
            list_destroy(&self->bind.name_list);
            break;
        }
        }

        free(self);
        *self_pointer = NULL;
        return;
    }
}

void
exp_list_print(list_t *exp_list, file_t *file) {
    exp_t *exp = list_first(exp_list);
    while (exp) {
        exp_print(exp, file);
        if (!list_cursor_is_end(exp_list))
            fprintf(file, " ");
        exp = list_next(exp_list);
    }
}

void
exp_list_print_as_tail(list_t *exp_list, file_t *file) {
    if (list_is_empty(exp_list)) {
        fprintf(file, ")");
    } else {
        fprintf(file, " ");
        exp_list_print(exp_list, file);
        fprintf(file, ")");
    }
}

void
name_list_print(list_t *name_list, file_t *file) {
    char *name = list_first(name_list);
    while (name) {
        if (list_cursor_is_end(name_list))
            fprintf(file, "%s", name);
        else
            fprintf(file, "%s ", name);
        name = list_next(name_list);
    }
}

void
exp_print(const exp_t *self, file_t *file) {
    switch (self->kind) {
    case EXP_VAR: {
        fprintf(file, "%s", self->var.name);
        return;
    }

    case EXP_AP: {
        if (list_is_empty(self->ap.arg_list)) {
            fprintf(file, "(");
            exp_print(self->ap.target, file);
            fprintf(file, ")");
            return;
        }

        fprintf(file, "(");
        exp_print(self->ap.target, file);
        fprintf(file, " ");
        exp_list_print(self->ap.arg_list, file);
        fprintf(file, ")");
        return;
    }

    case EXP_BIND: {
        if (list_is_empty(self->bind.name_list)) {
            fprintf(file, "(=)");
            return;
        }

        fprintf(file, "(= ");
        name_list_print(self->bind.name_list, file);
        fprintf(file, ")");
        return;
    }
    }
}
