#include "executer.h"

void execute(const char *program, char *args[]) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    int errno = 0;
    execvp(program, args);
    if (errno == 2) {
      printf("%s: command not found\n", program);
    }
  } else {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
      printf("%s: abnormal program termination\n", program);
    }
  }
}
