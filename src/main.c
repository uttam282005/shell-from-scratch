#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_echo.h"
#include "_pwd.h"
#include "_type.h"
#include "executer.h"
#include "utils.h"

void handle_builtin(char **args, int count) {
  const char *command = args[0];

  if (strcmp(command, "echo") == 0) {
    _echo(args, count);
  }
  if (strcmp(command, "exit") == 0)
    exit(0);
  if (strcmp(command, "type") == 0) {
    _type(args, count);
  }
  if (strcmp(command, "pwd") == 0) {
    _pwd();
  }
}

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  // Wait for user input

  while (1) {
    char input[100];

    printf("$ ");
    if (!fgets(input, sizeof(input), stdin))
      break;

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
      input[len - 1] = '\0';

    int count = 0;
    char **args = split_string(input, ' ', &count);
    if (count == 0) {
      free(args);
      continue;
    }

    char *command = args[0];

    if (is_builtin(command)) {
      handle_builtin(args, count);
    } else {
      execute(args);
    }

    for (int i = 0; i < count; ++i)
      free(args[i]);
    free(args);
  }

  return 0;
}
