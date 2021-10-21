#include "pipeline.h"

void execute_command(struct command cmd, int is_background) {
    // set the required file descriptors to STDOUT and STDIN
    set_redirect(cmd);

    if (cmd.argc <= 0) return;

    int cmd_id = handler_id(cmd);
    if (cmd_id >= 0) {
        handle_id(cmd_id, cmd);
    }
    else {
        execute_gcmd(cmd, is_background);
    }

    // reset the STDOUT and STDIN
    reset_redirect();

}

void execute_pipeline(struct pipline_cmd p){

    int pipes[MAX_IN_PIPE][2];

    // Copy STDIN and STDOUT file descriptors
    int o_stdout = dup(STDOUT_FILENO);
    int o_stdin = dup(STDIN_FILENO);

    for (int cmd_id = 0; cmd_id < p.cmdc; cmd_id++){

        if(cmd_id <= p.cmdc - 2){
            pipe(pipes[cmd_id]);
        }

        struct command p_cmd = p.cmdv[cmd_id];

        if (cmd_id == p.cmdc - 1) {
            // for the last command, change STDIN to the read pipe of last but one command
            dup2(o_stdout, STDOUT_FILENO);
            dup2(pipes[cmd_id-1][0], STDIN_FILENO);
            execute_command(p_cmd, p.background);
            close(pipes[cmd_id-1][0]);

        }
        else if (cmd_id == 0) {
            // for the first command, the write pipe is STDOUT
            dup2(pipes[cmd_id][1], STDOUT_FILENO);
            execute_command(p_cmd, p.background);
            close(pipes[cmd_id][1]);
        }
        else {
            // for all other commands, read from pipe of previous command, write to pipe of next command
            dup2(pipes[cmd_id][1], STDOUT_FILENO);
            dup2(pipes[cmd_id-1][0], STDIN_FILENO);
            execute_command(p_cmd, p.background);
            close(pipes[cmd_id][1]);
            close(pipes[cmd_id-1][0]);
        }
    }

    // Restore original STDIN and STDOUT
    dup2(o_stdout, STDOUT_FILENO);
    dup2(o_stdin, STDIN_FILENO);
}
