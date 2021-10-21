#include "handler.h"
#include "general_execute.h"

void (*inbuilt_commands[]) (struct command cmd) = {
    echo,
    dpwd,
    cd,
    ls,
    pinfo,
    get_cmd_history,
    jobs,
    sig,
    fg,
    bg
};

char* inbuilt_cmd_strings[] = {
    "echo",
    "pwd",
    "cd",
    "ls",
    "pinfo",
    "history",
    "jobs",
    "sig",
    "fg",
    "bg",
    "repeat"
};

void handle_id(int cmd_id, struct command cmd) {
    if (cmd_id != INBUILT_CMDS-1){
        (*inbuilt_commands[cmd_id])(cmd);
    }
    else{
        // repeat command
        
        struct command rep_cmd;
        int background_p = 0;

        rep_cmd.argc = 0;
        rep_cmd.argv = (char **) malloc(sizeof(char *) * MAX_CMD_ARGS);

        rep_cmd.cmd_type = (char *) malloc(sizeof(char) * (strlen(cmd.argv[2]) + 1));
        strcpy(rep_cmd.cmd_type, cmd.argv[2]);
        for (int arg_id = 2; arg_id < cmd.argc; arg_id++){

            if(strcmp(cmd.argv[arg_id], "&") == 0){
                background_p = 1;
                break;
            };
            
            rep_cmd.argv[rep_cmd.argc] = (char *) malloc(sizeof(char) * (strlen(cmd.argv[arg_id]) + 1));
            strcpy(rep_cmd.argv[rep_cmd.argc], cmd.argv[arg_id]);

            rep_cmd.argc++;
        }

        rep_cmd.argv[rep_cmd.argc] = (char *) malloc(sizeof(NULL));
        rep_cmd.argv[rep_cmd.argc] = NULL;

        for(int rep_id = 0; rep_id < atoi(cmd.argv[1]); rep_id++){
            
            
            int cmd_id = handler_id(rep_cmd);
            if (cmd_id >= 0) {
                handle_id(cmd_id, rep_cmd);
            }
            else {
                execute_gcmd(rep_cmd, background_p);
            }
        }

        free(rep_cmd.cmd_type);
        for (int arg_i = 0; arg_i <= rep_cmd.argc; arg_i++) {
            free(rep_cmd.argv[arg_i]);
        }
    }
}

int handler_id(struct command cmd) {

    for (int i = 0; i < INBUILT_CMDS; i++) {
        if (strcmp(cmd.cmd_type, inbuilt_cmd_strings[i]) == 0) {
            return i;
        }
    }

    return -1;
}
