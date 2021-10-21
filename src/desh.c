#include "libraries.h"
#include "utils.h"
#include "structs.h"
#include "constants.h"
#include "handler.h"
#include "general_execute.h"
#include "history.h"
#include "pipeline.h"
#include "jobs.h"

extern int errno;

int main() {
    print_startup();

    initialize_histories();
    initialize_history();

    char *inp_cmd;
    size_t inp_len = 0, line_read;

    bg_processes = (struct bg_process *) malloc(sizeof(struct bg_process) * 1);

    struct command sh_cmd = {0};
    sh_cmd.argv = (char **) malloc(sizeof(char *) * MAX_CMD_ARGS);

    struct pipline_cmd sh_pipe = {0};
    sh_pipe.cmdv = (struct command *) malloc(sizeof(struct command) * MAX_IN_PIPE);

    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    while(1) {

        print_prompt();

        line_read = getline(&inp_cmd, &inp_len, stdin);

        if ((int)line_read <= 0) {
            break;
        }

        if(inp_cmd[0] == '\0') {
            perror("Getline error");
            break;
        }

        if (inp_cmd[line_read-1] - '\n' == 0){
            inp_cmd[line_read-1] = '\0';
        }

        char* save_ptr[3];
        
        char *pipeline = strtok_r(inp_cmd, ";", &save_ptr[0]);
        while (pipeline != NULL){
            // Iterate through the ; separated commands

            pipeline = strip(pipeline);

            sh_pipe.cmdc = 0;
            sh_pipe.background = 0;

            char* cmd = strtok_r(pipeline, "|", &save_ptr[1]);
            while (cmd != NULL){
                // Iterate through the | separated pipes

                cmd = strip(cmd);

                sh_cmd.cmd_type = NULL;
                sh_cmd.input[0] = '\0';
                sh_cmd.output[0] = '\0';
                sh_cmd.argc = 0;
                sh_cmd.append = 0;
                char* cmd_arg = strtok_r(cmd, " \t\n", &save_ptr[2]);
                while (cmd_arg != NULL){
                    // Iterate through the arguments in each command

                    cmd_arg = strip(cmd_arg);

                    if(strcmp(cmd_arg, "&") == 0){
                        sh_pipe.background = 1;
                        break;
                    }
                    else if (strcmp(cmd_arg, "<") == 0) {
                        // input file for the command
                        cmd_arg = strtok_r(NULL, " \t\n", &save_ptr[2]);
                        if (cmd_arg == NULL) {
                            perror("Input file not given, using STDIN");
                            break;
                        }
                        strcpy(sh_cmd.input, cmd_arg);
                    }
                    else if (strcmp(cmd_arg, ">") == 0) {
                        // get output file name
                        cmd_arg = strtok_r(NULL, " \t\n", &save_ptr[2]);
                        if (cmd_arg == NULL) {
                            perror("Output file not given, using STDOUT");
                            break;
                        }
                        strcpy(sh_cmd.output, cmd_arg);
                    }
                    else if (strcmp(cmd_arg, ">>") == 0) {
                        // get output file name to append to
                        cmd_arg = strtok_r(NULL, " \t\n", &save_ptr[2]);
                        if (cmd_arg == NULL) {
                            perror("Output file not given, using STDOUT");
                            break;
                        }
                        strcpy(sh_cmd.output, cmd_arg);
                        sh_cmd.append = 1;
                    }
                    else{
                        if (sh_cmd.cmd_type == NULL){
                            sh_cmd.cmd_type = strdup(cmd_arg);
                        }

                        sh_cmd.argv[sh_cmd.argc] = strdup(cmd_arg);
                        sh_cmd.argc++;
                    }

                    cmd_arg = strtok_r(NULL, " \t\n", &save_ptr[2]);
                }

                sh_cmd.argv[sh_cmd.argc] = (char *) malloc(sizeof(NULL));
                sh_cmd.argv[sh_cmd.argc] = NULL;

                // copy sh_cmd data to pipe struct
                sh_pipe.cmdv[sh_pipe.cmdc].argc = sh_cmd.argc;
                sh_pipe.cmdv[sh_pipe.cmdc].append = sh_cmd.append;
                sh_pipe.cmdv[sh_pipe.cmdc].argv = malloc(sizeof(char *) * sh_cmd.argc+1);
                sh_pipe.cmdv[sh_pipe.cmdc].cmd_type = strdup(sh_cmd.cmd_type);
                strcpy(sh_pipe.cmdv[sh_pipe.cmdc].output, sh_cmd.output);
                strcpy(sh_pipe.cmdv[sh_pipe.cmdc].input, sh_cmd.input);

                // TODO change history to take pipe
                add_cmd_history(sh_cmd);

                // Free the memory allocated in the command struct
                free(sh_cmd.cmd_type);
                for (int arg_i = 0; arg_i < sh_cmd.argc; arg_i++) {
                    sh_pipe.cmdv[sh_pipe.cmdc].argv[arg_i] = strdup(sh_cmd.argv[arg_i]);
                    free(sh_cmd.argv[arg_i]);
                }
                sh_pipe.cmdv[sh_pipe.cmdc].argv[sh_cmd.argc] = (char *) malloc(sizeof(NULL));
                sh_pipe.cmdv[sh_pipe.cmdc].argv[sh_cmd.argc] = NULL;
                free(sh_cmd.argv[sh_cmd.argc]);

                sh_pipe.cmdc += 1;

                cmd = strtok_r(NULL, "|", &save_ptr[1]);
            }

            execute_pipeline(sh_pipe);

            // Free memory allocated to the pipeline
            for (int pipe_id = 0; pipe_id < sh_pipe.cmdc; pipe_id++){
                for (int cmd_id = 0; cmd_id <= sh_pipe.cmdv[pipe_id].argc; cmd_id++){
                    free(sh_pipe.cmdv[pipe_id].argv[cmd_id]);
                }
                free(sh_pipe.cmdv[pipe_id].argv);
                free(sh_pipe.cmdv[pipe_id].cmd_type);
            }

            pipeline = strtok_r(NULL, ";", &save_ptr[0]);
        }

    }

    free(inp_cmd);
    free(sh_cmd.argv);
    free(bg_processes);

    printf("Adios!\n");
    return 0;
}