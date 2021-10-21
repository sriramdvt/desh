#include "fg.h"

void fg(struct command cmd){

    if (cmd.argc != 2) {
        perror("Not the right number of commands for fg");
        return;
    }

    int bg_id, status, job_id = atoi(cmd.argv[1]);
    pid_t pid = -1;
    
    for (bg_id = 0; bg_id < num_bg_processes; bg_id++){
        if ((bg_processes+bg_id)->job_number+1 == job_id) {
            pid = (bg_processes+bg_id)->proc_id;
            break;
        }
    }

    if (pid < 0) {
        errno = 3;
        perror("Invalid Job ID, process not found");
        return;
    }

    // Give terminal to the process group of pid
    tcsetpgrp(STDIN_FILENO, getpgid(pid));

    // Ask the process to continue by sending SIGCONT
    if (kill(pid, SIGCONT) < 0) {
        perror("Unable to bring to foreground");
        return;
    }

    strcpy((bg_processes+bg_id)->status, "Not");

    // Shell process should ignore anything that is given from or sent to the terminal
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    // Wait for the foreground process
    waitpid(pid, &status, WUNTRACED);

    // if the process is stopped again for some reason, change the status in the bg_process struct
    if (WIFSTOPPED(status)) {
        strcpy((bg_processes+bg_id)->status, "Stp");
    }

    // Take back the control of the terminal
    tcsetpgrp(STDIN_FILENO, getpgrp());

    // Restore being able to read from and write to the terminal
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

}