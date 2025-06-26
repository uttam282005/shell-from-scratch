// executer.h

#ifndef EXECUTER_H
#define EXECUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

void execute(char **args, int count);

#endif
