#include "utils.h"

bool is_whitespace_segment(const char *s) {
  while (*s) {
    if (!isspace((unsigned char)*s))
      return false;
    s++;
  }
  return true;
}

char **split_string(const char *string, char separater, int *count) {
  int initial_capacity = 100;

  char **parsed_args = malloc(initial_capacity * sizeof(char *));

  int parsed_args_index = 0;
  if (parsed_args == NULL) {
    ALLOC_FAILED;
    return NULL;
  }

  const char *ptr = string;
  const char *start = string;

  while (1) {
    if (*ptr == separater || *ptr == '\0') {
      int len = ptr - start;

      char *segment = malloc(len + 1);

      if (segment == NULL) {
        for (int i = 0; i < parsed_args_index; i++) {
          free(parsed_args[i]);
        }
        free(parsed_args);
        ALLOC_FAILED;
        return NULL;
      }

      strncpy(segment, start, len);
      segment[len] = '\0';

      if (!is_whitespace_segment(segment))
        parsed_args[parsed_args_index++] = segment;

      if (parsed_args_index >= initial_capacity) {
        int capacity = initial_capacity * 2;
        char **temp = realloc(parsed_args, capacity * sizeof(char *));
        if (temp == NULL) {
          for (int i = 0; i < parsed_args_index; i++)
            free(parsed_args[i]);
          free(parsed_args);
          ALLOC_FAILED;
          return NULL;
        }
        parsed_args = temp;
      }

      if (*ptr == '\0') {
        parsed_args[parsed_args_index] = NULL;
        break;
      }

      start = ptr + 1;
    }

    ptr++;
  }

  *count = parsed_args_index;

  return parsed_args;
}
