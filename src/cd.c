#include "cd.h"

void cd(struct command cmd) {

    if (cmd.argc > 2) {
        errno = 22;
        perror("Too many arguments for cd");
        return;
    }

    char *cwd;
    cwd = getcwd(NULL, 0);

    char target_path[PATH_MAX + 1] = {'\0'};
    char homedir[PATH_MAX + 1] = {'\0'};
    get_homedir(homedir);
    
    if (cmd.argc > 1){

        if (cmd.argv[1][0] - '~' == 0){
            sprintf(target_path, "%s%s", homedir, cmd.argv[1]+1);
        }
        else if (cmd.argv[1][0] - '-' == 0) {
            get_directory_history(target_path);
        }
        else {
            strcpy(target_path, cmd.argv[1]);
        }

    }
    else {
        strcpy(target_path, homedir);
    }

    if (chdir(target_path) < 0) {
        errno = 20;
        perror("Unable to cd into given path");
        return;
    }

    update_directory_history(cwd);

}
