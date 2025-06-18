#ifndef UTILS
#define UTILS

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM_SEGMENT_LENGTH 100
#define ALLOC_FAILED                                                           \
  fprintf(stderr, "Memory allocation failed: %s\n", strerror(errno))

bool is_whitespace_segment(const char *s);
char **split_string(const char *string, char separater, int *count);

#endif
