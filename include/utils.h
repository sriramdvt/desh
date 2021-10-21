#ifndef __UTILS_H
#define __UTILS_H

extern char *prompt_home_dir;

void print(char* message);

void print_startup();

void initialize_histories();

void update_directory_history(char* old_dir);

void get_directory_history(char* old_dir);

void get_homedir(char* homedir);

void print_prompt();

char* strip(char* command);

#endif