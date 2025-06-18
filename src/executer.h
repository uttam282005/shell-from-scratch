// executer.h

#ifndef EXECUTRE_H
#define EXECUTRE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute(const char *program, char *args[]);

#endif
