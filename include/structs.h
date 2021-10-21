#ifndef __STRUCTS_H
#define __STRUCTS_H

#include "libraries.h"

struct command {
    char* cmd_type;
    int argc;
    char** argv;
    int append;
    char input[PATH_MAX];
    char output[PATH_MAX];
};

struct pipline_cmd {
    int cmdc;
    struct command *cmdv;
    int background;
};

struct bg_process {
    int job_number;
    int proc_id;
    char proc_name[2048];
    char status[3];
};

#endif