#include "history.h"

void initialize_history(){

    char homedir[PATH_MAX + 1] = {'\0'};
    get_homedir(homedir);

    char his_file[PATH_MAX + 100] = {'\0'};
    strcpy(his_file, homedir);
    strcat(his_file, "/.desh_history");

    FILE *hist_file;
    char * line = NULL;

    hist_file = fopen(his_file, "r");
    if (hist_file == NULL)
        hist_file = fopen(his_file, "w");

    fclose(hist_file);
    if (line)
        free(line);
}

void add_cmd_history(struct command cmd){
    char homedir[PATH_MAX + 1] = {'\0'};
    get_homedir(homedir);

    char his_file[PATH_MAX + 100] = {'\0'};
    strcpy(his_file, homedir);
    strcat(his_file, "/.desh_history");

    FILE *hist_file;
    char * line = NULL;
    size_t len = 0;

    hist_file = fopen(his_file, "r");

    char history_cmds[20][2048];
    int num_history = 0;
    while (getline(&line, &len, hist_file) != -1) {
        line = strip(line);
        strcpy(history_cmds[num_history], line);
        num_history++;
    }
    fclose(hist_file);

    hist_file = fopen(his_file, "w");

    char actual_cmd[2048] = {'\0'};
    for(int i = 0; i < cmd.argc; i++){
        strcat(actual_cmd, cmd.argv[i]);
        strcat(actual_cmd, " ");
    }
    strip(actual_cmd);

        
    if(strcmp(actual_cmd, history_cmds[num_history-1]) != 0){
        if (num_history >= 20){
                for(int i = 1; i < num_history; i++){
                    fprintf(hist_file, "%s\n", history_cmds[i]);
                }
            }
        else {
            for(int i = 0; i < num_history; i++){
                fprintf(hist_file, "%s\n", history_cmds[i]);
            }
        }
        fprintf(hist_file, "%s\n", actual_cmd);
    }
    else{
        for(int i = 0; i < num_history; i++){
            fprintf(hist_file, "%s\n", history_cmds[i]);
        }
    }

    fclose(hist_file);
    
    if (line)
        free(line);

}

void get_cmd_history(struct command cmd) {
    char homedir[PATH_MAX + 1] = {'\0'};
    get_homedir(homedir);

    char his_file[PATH_MAX + 100] = {'\0'};
    strcpy(his_file, homedir);
    strcat(his_file, "/.desh_history");

    FILE *hist_file;
    char * line = NULL;
    size_t len = 0;

    hist_file = fopen(his_file, "r");

    char history_cmds[20][2048];
    int num_history = 0;
    while (getline(&line, &len, hist_file) != -1) {
        line = strip(line);
        strcpy(history_cmds[num_history], line);
        num_history++;
    }
    fclose(hist_file);

    int print_cmds = 0;
    if (cmd.argc > 1) {
        print_cmds = atoi(cmd.argv[1]);
    }
    else{
        print_cmds = num_history;
    }

    if (print_cmds < 0){
        perror("Invalid history requested, showing default history");
    }

    print_cmds = num_history < print_cmds ? num_history : print_cmds;
    print_cmds = 10 < print_cmds ? 10 : print_cmds;

    for(int i = num_history-print_cmds; i < num_history; i++){
        print(history_cmds[i]);
        print("\n");
    }

}
