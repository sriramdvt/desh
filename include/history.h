#ifndef __HISTORY_H
#define __HISTORY_H

#include "structs.h"
#include "libraries.h"
#include "constants.h"
#include "utils.h"

void initialize_history();

void add_cmd_history(struct command cmd);

void get_cmd_history(struct command cmd);

#endif