#include "redirection.h"

int r_stdin = -1;
int r_stdout = -1;

void set_redirect(struct command cmd){

    r_stdin = -1;
    r_stdout = -1;

    if (strlen(cmd.output) > 0){
        // if there is an output file, make it STDOUT

        char output_file[PATH_MAX];

        r_stdout = dup(STDOUT_FILENO);

        if (cmd.output[0] - '~' == 0){
            sprintf(output_file, "%s%s", prompt_home_dir, cmd.output+1);
        }
        else{
            strcpy(output_file, cmd.output);
        }

        int outfile_fd = -1;
        outfile_fd = open(output_file, cmd.append ? O_CREAT | O_WRONLY | O_APPEND : O_CREAT | O_WRONLY | O_TRUNC, 0644);

        if (outfile_fd < 0) {
            perror("Was not able to open output file");
        }

        if (dup2(outfile_fd, STDOUT_FILENO) < 0) {
            perror("Unable to redirect to STDOUT");
        }

        close(outfile_fd);
    }

    if (strlen(cmd.input) > 0){
        // if there is an input file, make it STDIN
        char input_file[PATH_MAX];

        r_stdin = dup(STDIN_FILENO);

        if (cmd.input[0] - '~' == 0){
            sprintf(input_file, "%s%s", prompt_home_dir, cmd.input+1);
        }
        else{
            strcpy(input_file, cmd.input);
        }

        int input_fd = -1;
        input_fd = open(input_file, O_RDONLY);

        if (input_fd < 0) {
            perror("Unable to open input file for the command, using STDIN (write the entire file with the EOF symbol)");
            return;
        }

        if(dup2(input_fd, STDIN_FILENO) < 0) {
            perror("Unable to redirect STDIN");
        }

        close(input_fd);
    }
}

void reset_redirect() {

    if (r_stdout > 0){
        if(dup2(r_stdout, STDOUT_FILENO) < 0) {
            close(r_stdout);
            perror("Unable to restore STDOUT");
            exit(errno);
        }
    }

    if (r_stdin > 0){
        if(dup2(r_stdin, STDIN_FILENO) < 0) {
            close(r_stdin);
            perror("Unable to restore STDIN");
            exit(errno);
        }
    }

    r_stdout = -1;
    r_stdin = -1;
}
