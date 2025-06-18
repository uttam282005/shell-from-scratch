#include "executer.h"
#include <stdlib.h>

void execute(char **args) {
  char *program = args[0];

  pid_t pid = fork();

  if (pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    execvp(program, args);
    if (errno == 2)
      printf("%s: command not found\n", program);
    exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
      printf("%s: abnormal program termination\n", program);
    }
  }
}
