#ifndef TYPE
#define TYPE

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM_ALLOWED_PATHS 15
#define MAXIMUM_PATH_LENGTH 4096

bool is_builtin(char *command);
void _type(char **args, int count);

#endif
