# include "general_execute.h"

char* bg_jobs[MAX_PID];

void onChildExit() {

    pid_t p;
    int status;

    p = waitpid(-1, &status, WNOHANG);

    if (p <= 0)
        return;
    else {
        char bg_death[2048];
        sprintf(bg_death, "\n%s with pid %d ended %s\n", bg_jobs[p], p, !status ? "normally" : "abnormally");
        print(bg_death);
        print_prompt();
    }

    free(bg_jobs[p]);

    for (int bg_id = 0; bg_id < num_bg_processes; bg_id++){
        if ((bg_processes+bg_id)->proc_id == p) {
            strcpy((bg_processes+bg_id)->status, "Not");
            break;
        }
    }
}

void execute_gcmd(struct command cmd, int background_p) {

    signal(SIGCHLD, onChildExit);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Unable to create child process");
        return;
    }
    else if (pid == 0) {
        // in child process
        if (background_p == 1) {
            char b_pid[64] = {'\0'};
            sprintf(b_pid, "%d\n", getpid());
            print(b_pid);
        }

        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        setpgrp();

        if (execvp(cmd.cmd_type, cmd.argv) == -1) {
            perror("Unable to execvp the command");
            exit(1);
        }
        exit(0);
    }
    else{
        // in parent process
        if (background_p == 0) {
            // if it is not a background process

            tcsetpgrp(STDIN_FILENO, pid); // give terminal control to the child
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            int status;
            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, getpgrp()); // get back control from the child after it is done
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (WIFSTOPPED(status)) {
                // if the foreground process was stopped with ctrl+z
                
                int found_id = -1;
                for (int bg_id = 0; bg_id < num_bg_processes; bg_id++){
                    if ((bg_processes+bg_id)->proc_id == pid)   found_id = bg_id;
                }

                if (found_id < 0){
                    // the process doesn't exist in the list of bg_processes
                    add_bg_job(pid, cmd.cmd_type, "Stp");
                }
                else{
                    // the process exists in the list of bg_processes
                    strcpy((bg_processes+found_id)->status, "Stp");
                }
            }

        }
        else {
            bg_jobs[pid] = (char *) malloc(sizeof(char) * strlen(cmd.cmd_type));
            strcpy(bg_jobs[pid], cmd.cmd_type);

            add_bg_job(pid, cmd.cmd_type, NULL);
        }
    }

}
