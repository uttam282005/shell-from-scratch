#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_cd.h"
#include "_echo.h"
#include "_pwd.h"
#include "_type.h"
#include "executer.h"
#include "redirect.h"
#include "utils.h"

// TODO: refactor this
#include "_type.h"
#include "redirect.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_builtin(char **args, int count);

bool handle_execute_in_main_process(char **args) {
  char *command = args[0];

  if (strcmp(command, "exit") == 0) {
    exit(0);
  }

  if (strcmp(command, "cd") == 0) {
    char *path = args[1];
    _cd(path);
    return true;
  }

  if (strcmp(command, "pwd") == 0) {
    _pwd();

    return true;
  }

  return false;
}

char ***group(char **args, int count, int *process) {
  char ***cmds = malloc(sizeof(args) * 10);

  cmds[0] = args;
  int j = 1;
  int i = 0;
  for (; i < count; i++) {
    if (args[i][0] == '|') {
      args[i] = NULL;
      if (i + 1 < count) {
        cmds[j++] = args + i + 1;
      }
    }
  }

  *process = j;

  return cmds;
}

int _pipe(char ***cmds, int process) {
  int previn = -1;
  pid_t pids[process];

  for (int i = 0; i < process; i++) {
    int fd[2];

    if (i != process - 1 && pipe(fd) == -1) {
      perror("pipe");
      return -1;
    }

    pid_t child = fork();
    if (child < 0) {
      perror("fork");
      return -1;
    } else if (child == 0) {
      // In child
      if (previn != -1) {
        dup2(previn, STDIN_FILENO);
        close(previn);
      }

      if (i != process - 1) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
      }

      char **args = cmds[i];
      char *command = cmds[i][0];
      int count = 0;
      for (char **temp = args; *temp; temp++)
        count++;

      int savedfd, oldfd;
      int newfd = -1;

      if (is_builtin(command)) {
        newfd = handle_redirect(args, &count, &savedfd, &oldfd);
        handle_builtin(args, count);
      } else {
        execute(args, count);
      }

      if (newfd != -1) {
        restore_default_fd(oldfd, savedfd);
        close(newfd);
      }

      exit(0);
    } else {
      // In parent
      pids[i] = child;

      if (previn != -1)
        close(previn);
      if (i != process - 1) {
        close(fd[1]);
        previn = fd[0]; // Save the read end for next command
      }
    }
  }

  for (int i = 0; i < process; i++) {
    waitpid(pids[i], NULL, 0);
  }

  return 1;
}

void handle_builtin(char **args, int count) {
  const char *command = args[0];

  if (strcmp(command, "echo") == 0) {
    _echo(args, count);
  } else if (strcmp(command, "exit") == 0)
    exit(0);
  else if (strcmp(command, "type") == 0) {
    _type(args, count);
  } else if (strcmp(command, "pwd") == 0) {
    _pwd();
  } else if (strcmp(command, "cd") == 0) {
    if (count > 2) {
      printf("Too many arguments.\n");
      return;
    }
    _cd(args[1]);
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
    char **args = tokenize(input, ' ', &count);

    if (count == 0) {
      free(args);
      continue;
    }

    if (handle_execute_in_main_process(args)) {
      goto clean_up;
    }

    int process;
    char ***cmds;

    cmds = group(args, count, &process);
    _pipe(cmds, process);

  clean_up:
    for (int i = 0; i < count; ++i)
      free(args[i]);
    free(args);

    free(cmds);
  }

  return 0;
}
