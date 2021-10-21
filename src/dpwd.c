#include "dpwd.h"

void dpwd(struct command cmd) {

    if (cmd.argc > 1) {
        errno = 22;
        perror("Too many arguments for cwd");
        return;
    }

    char *cwd;
    cwd = getcwd(NULL, 0);
    char print_string[MAX_ARG_LEN];
    sprintf(print_string, "%s\n", cwd);
    print(print_string);

}
