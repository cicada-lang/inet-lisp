#include "index.h"

value_t x_print(value_t x) {
    value_print(x, stdout);
    return x;
}

value_t x_newline(void) {
    printf("\n");
    return NULL;
}

value_t x_println(value_t x) {
    x_print(x);
    x_newline();
    return x;
}
