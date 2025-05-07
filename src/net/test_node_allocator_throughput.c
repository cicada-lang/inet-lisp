#include "index.h"

#define BATCH_SIZE 1000
#define REPEATION_COUNT 10000

static void
thread_fn(thread_t *thread) {
    node_allocator_t *node_allocator = thread->arg;
    stack_t *stack = stack_new();

    stack_t *allocated_stack = stack_new();
    for (size_t r = 0; r < REPEATION_COUNT; r++) {
        for (size_t i = 0; i < BATCH_SIZE; i++) {
            node_t *node = node_allocator_allocate(node_allocator, stack);
            stack_push(allocated_stack, node);
        }

        for (size_t i = 0; i < BATCH_SIZE; i++) {
            node_t *node = stack_pop(allocated_stack);
            node_allocator_recycle(node_allocator, stack, &node);
        }
    }
}

void
test_node_allocator_throughput(void) {
    test_start();

    node_allocator_t *node_allocator = node_allocator_new();

    double start_second = time_second();

    size_t thread_count = 3;
    array_t *thread_array = array_new_auto();
    for (size_t i = 0; i < thread_count; i++) {
        thread_t *T = thread_start(thread_fn, node_allocator);
        array_push(thread_array, T);
    }

    for (size_t i = 0; i < thread_count; i++) {
        thread_t *T = array_pop(thread_array);
        thread_join(T);
    }

    node_allocator_destroy(&node_allocator);

    who_printf("thread_count: %lu\n", thread_count);
    double throughput = REPEATION_COUNT * BATCH_SIZE / 1000 / time_passed_second(start_second);
    who_printf("throughput: %.f k/s\n", throughput);

    test_end();
}
