#include "bg.h"

void bg(struct command cmd){

    if (cmd.argc != 2) {
        perror("Not the right number of commands for bg");
        return;
    }

    int bg_id, job_id = atoi(cmd.argv[1]);
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

    // Ask the process to continue by sending SIGCONT
    if (kill(pid, SIGCONT) < 0) {
        perror("Unable to continue the background process");
        return;
    }

    strcpy((bg_processes+bg_id)->status, "Run");

}