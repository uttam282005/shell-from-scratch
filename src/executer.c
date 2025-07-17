#include "executer.h"
#include "redirect.h"
#include <stdlib.h>

void execute(char **args, int count) {
  char *program = args[0];

  execvp(program, args);

  if (errno == 2)
    printf("%s: command not found\n", program);
}
