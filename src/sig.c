#include "sig.h"

void sig (struct command cmd) {

    if (cmd.argc != 3) {
        perror("Not the right number of commands for sig");
        return;
    }

    int status, bg_id, job_id = atoi(cmd.argv[1]);
    pid_t pid = -1;
    
    for (bg_id = 0; bg_id < num_bg_processes; bg_id++){
        if ((bg_processes+bg_id)->job_number+1 == job_id) {
            pid = (bg_processes+bg_id)->proc_id;
        }
    }

    if (pid < 0) {
        errno = 3;
        perror("Invalid Job ID, process not found");
        return;
    }

    int proc_sig = atoi(cmd.argv[2]);
    if (kill(pid, proc_sig) < 0) {
        errno = 95;
        perror("Unable to send the signal to the process");
    }

    waitpid(pid, &status, WNOHANG);

    if (WIFSTOPPED(status)) {
        strcpy((bg_processes+bg_id)->status, "Stp");
    }

}