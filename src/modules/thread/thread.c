#include "index.h"

thread_id_t
thread_start(thread_fn_t *thread_fn, void *arg) {
    thread_id_t thread_id;
    int ok = pthread_create(&thread_id, NULL, thread_fn, arg);
    assert(ok == 0);
    return thread_id;
}

void *
thread_wait(thread_id_t thread_id) {
    void *result;
    int ok = pthread_join(thread_id, &result);
    assert(ok == 0);
    return result;
}

double
clock_elapsed_seconds(clock_t start_clock) {
    clock_t end_clock = clock();
    return ((double) (end_clock - start_clock)) / CLOCKS_PER_SEC;
}
