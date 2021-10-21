#ifndef __LS_H
#define __LS_H

#include "structs.h"
#include "libraries.h"
#include "constants.h"
#include "utils.h"

void ls(struct command cmd);

int filter_flag_a(const struct dirent* f_path);

void print_info (struct stat f_st);

int lengthsort (const struct dirent **d1, const struct dirent **d2);

void ls_path(char* ls_pathname, int flag_l, int flag_a);

#endif