#include "libraries.h"
#include "utils.h"
#include "constants.h"

char *prompt_home_dir;

void print(char* message) {
    write(STDOUT_FILENO, message, strlen(message));
    fflush(stdout);
}

void initialize_histories() {
    char homedir[PATH_MAX + 1];
    get_homedir(homedir);

    char pd_file[PATH_MAX + 100];
    strcpy(pd_file, homedir);
    strcat(pd_file, "/.desh_pd");

    FILE *fptr;
    fptr = fopen(pd_file,"w+");

    if (fptr != NULL){
        fprintf(fptr,"%s", homedir);
        fclose(fptr);
    }

    prompt_home_dir = getcwd(NULL, 0);
}

void update_directory_history(char* old_dir) {
    char homedir[PATH_MAX + 1] = {'\0'};
    get_homedir(homedir);

    char pd_file[PATH_MAX + 100] = {'\0'};
    strcat(pd_file, homedir);
    strcat(pd_file, "/.desh_pd");

    FILE *fptr;
    fptr = fopen(pd_file,"w+");

    if (fptr != NULL){
        fprintf(fptr,"%s", old_dir);
        fclose(fptr);
    }
    else {
        printf("Could not update dir %s\n", pd_file);
    }
}

void get_directory_history(char* old_dir) {
    char homedir[PATH_MAX + 1] = {'\0'};
    get_homedir(homedir);

    char pd_file[PATH_MAX + 100] = {'\0'};
    strcat(pd_file, homedir);
    strcat(pd_file, "/.desh_pd");

    FILE *fptr;
    fptr = fopen(pd_file,"r");

    if (fptr != NULL){
        fscanf(fptr, "%[^\n]", old_dir);
        fclose(fptr);
    }

}

void print_startup() {
    print("  _\n");
    print(" (_)\n");
    print("<___>\n");
    print(" | |__________\n");
    print(" | |\033[0;31m%%%%%%%%%\033[0m|\n");
    print(" | |   \033[0;34m(*)\033[0m   |\n");
    print(" | |\033[0;32m:::::::::\033[0m|\n");
    print(" | |----------\n");
    print(" | |\n");
    print(" | |\n");
    print(" | |\n");

    fflush(stdout);
}

void get_homedir(char* homedir) {
    if(readlink("/proc/self/exe", homedir, PATH_MAX + 1) < -1) {
        perror("Error while finding home directory");
        exit(errno);
    }
    dirname(homedir);
}

void print_prompt() {

    char *cwd;
    cwd = getcwd(NULL, 0);

    char username[LOGIN_NAME_MAX + 1];
    getlogin_r(username, LOGIN_NAME_MAX + 1);

    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, HOST_NAME_MAX + 1);

    char homedir[PATH_MAX + 1] = {'\0'};
    get_homedir(homedir);

    char* prompt_dir;
    prompt_dir = strstr(cwd, prompt_home_dir);
    if (prompt_dir != NULL){
        sprintf(prompt_dir, "~%s", cwd+strlen(prompt_home_dir));
    }
    else {
        prompt_dir = cwd;
    }

    char prompt[2048];
    sprintf(prompt,"<\033[0;31m%s\033[0;34m@%s\033[0;32m:%s\033[0m> ", username, hostname, prompt_dir);
    print(prompt);

}

char* strip(char* command) {
    
    if (strlen(command) <= 0) {
        return command;
    }

    char* end = command + strlen(command) - 1;

    while (isspace((unsigned char) *command))
        command++;
    
    if (*command == '\0')
        return command;
    
    while (isspace((unsigned char) *end))
        *end-- = '\0';

    return command;
}
