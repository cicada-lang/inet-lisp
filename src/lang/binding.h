#pragma once

struct binding_t {
    list_t *name_list;
    list_t *body;
};

binding_t *binding_new();
