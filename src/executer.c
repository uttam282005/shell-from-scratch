#include "executer.h"
#include "redirect.h"
#include <stdlib.h>

void execute(char **args, int count) {
  char *program = args[0];

  pid_t pid = fork();

  if (pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    int savedfd, oldfd, newfd = -1;
    newfd = handle_redirect(args, &count, &savedfd, &oldfd);

    execvp(program, args);

    if (newfd != -1)
      restore_default_fd(oldfd, savedfd);

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
