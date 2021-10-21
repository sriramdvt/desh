#include "echo.h"

void echo(struct command cmd) {

    char print_string[MAX_ARG_LEN];
    for (int i = 1; i < cmd.argc; i++) {
        sprintf(print_string, i == cmd.argc-1 ? "%s" : "%s ", cmd.argv[i]);
        print(print_string);
    }
    print("\n");

}

