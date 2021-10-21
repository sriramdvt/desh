#ifndef __HANDLER_H
#define __HANDLER_H

#include "structs.h"
#include "constants.h"
#include "libraries.h"
#include "echo.h"
#include "dpwd.h"
#include "cd.h"
#include "ls.h"
#include "pinfo.h"
#include "history.h"
#include "jobs.h"
#include "sig.h"
#include "fg.h"
#include "bg.h"

extern void (*inbuilt_commands[]) (struct command cmd);

extern char* inbuilt_cmd_strings[];

void handle_id(int cmd_id, struct command cmd);

int handler_id(struct command cmd);

#endif