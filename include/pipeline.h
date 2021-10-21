#ifndef __PIPE_H
#define __PIPE_H

#include "structs.h"
#include "libraries.h"
#include "constants.h"
#include "utils.h"
#include "handler.h"
#include "general_execute.h"
#include "redirection.h"

void execute_command(struct command cmd, int is_background);

void execute_pipeline(struct pipline_cmd p);

#endif