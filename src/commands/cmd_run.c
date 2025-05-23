#include "index.h"

static int run(commander_t *commander);

void
cmd_run(commander_t *runner) {
    command_t *command = command_new("run");
    command->description = "run files";
    command->run = run;
    commander_add(runner, command);
}

int
run(commander_t *commander) {
    char **argv = commander_rest_argv(commander);
    while (*argv) {
        char *arg = *argv++;
        if (string_equal(arg, "--single-threaded") || string_equal(arg, "-s")) {
            single_threaded_flag = true;
            continue;
        }

        if (string_equal(arg, "--print-top-level-exp") || string_equal(arg, "-p")) {
            if (DEBUG_NODE_ALLOCATOR_DISABLED) {
                printf("[run] can not print when compiled with DEBUG_NODE_ALLOCATOR_DISABLED");
                exit(1);
            }

            print_top_level_exp_flag = true;
            continue;
        }

        if (string_equal(arg, "--no-run-top-level-exp")) {
            no_run_top_level_exp = true;
            continue;
        }

        path_t *path = path_new_cwd();
        path_resolve(path, arg);
        load_mod(path);
    }

    return 0;
}
