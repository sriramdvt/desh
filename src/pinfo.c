#include "pinfo.h"

void pinfo(struct command cmd){

    if (cmd.argc > 2) {
        errno = 22;
        perror("Too many arguments");
        return;
    }
    ssize_t read_suc;

    char proc_path[PATH_MAX] = {'\0'};
    if (cmd.argc == 2) {
        sprintf(proc_path, "/proc/%s", cmd.argv[1]);
    }
    else if (cmd.argc == 1) {
        sprintf(proc_path, "/proc/%d", getpid());
    }

    // Stat of the process
    char proc_stat[PATH_MAX + 5] = {'\0'};
    sprintf(proc_stat, "%s/stat", proc_path);
    FILE *p_stat = fopen(proc_stat, "r");
    if (p_stat != NULL){

        int pid;
        char state;
        fscanf(p_stat, "%d %*s %c", &pid, &state);

        char print_pid[PATH_MAX] = {'\0'}, print_pstatus[PATH_MAX] = {'\0'};

        sprintf(print_pid, "pid -- %d\n", pid);

        if (state == 'R' && getpgrp() == tcgetpgrp(STDOUT_FILENO)) {
            sprintf(print_pstatus, "Process Status -- %c+\n", state);
        }
        else {
            sprintf(print_pstatus, "Process Status -- %c\n", state);
        }

        print(print_pid);
        print(print_pstatus);

    }
    else {
        perror("Unable to read process info");
        return;
    }
    fclose(p_stat);

    // Virtual memory of the process
    char proc_statm[PATH_MAX + 6] = {'\0'};
    sprintf(proc_statm, "%s/statm", proc_path);
    FILE *p_statm = fopen(proc_statm, "r");
    if (p_statm != NULL) {
        int memory;

        fscanf(p_statm, "%d", &memory);
        printf("memory -- %d\n", memory);
    }
    else {
        perror("Unable to read process info");
        return; 
    }
    fclose(p_statm);

    // Path of the executable of the process
    char proc_exec[PATH_MAX + 4] = {'\0'};
    char exec_path[PATH_MAX] = {'\0'};
    sprintf(proc_exec, "%s/exe", proc_path);
    read_suc = readlink(proc_exec, exec_path, PATH_MAX);
    if (read_suc < 0) {
        perror("Unable to read process info");
        return;
    }
    char print_message[20 + PATH_MAX + 4];
    sprintf(print_message, "Executable Path -- %s\n", exec_path);
    print(print_message);


}
