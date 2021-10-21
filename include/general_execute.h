#ifndef __GEXECUTE_H
#define __GEXECUTE_H

#include "structs.h"
#include "libraries.h"
#include "constants.h"
#include "utils.h"
#include "jobs.h"

void onChildExit();

void execute_gcmd(struct command cmd, int background);

#endif