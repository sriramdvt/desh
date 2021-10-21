#ifndef __JOBS_H
#define __JOBS_H

#include "structs.h"
#include "libraries.h"
#include "constants.h"
#include "utils.h"

extern int num_bg_processes;
extern struct bg_process *bg_processes;

int bg_cmp_alpha(const void * a, const void * b);

void add_bg_job(int proc_id, char *proc_name, char *p_status);

void print_bg_jobs();

void jobs(struct command cmd);

#endif