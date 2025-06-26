#include "redirect.h"
#include <stdio.h>
#include <stdlib.h>

int handle_redirect(char **args, int *count, int *savedfd, int *oldfd) {
  int newfd = -1;
  regex_t regex;
  const char *pattern = "^[0-9]*>$";

  int ret = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);
  if (ret) {
    perror("Failed to compile regex.");
    return -1;
  }

  for (int i = 0; i < *count; i++) {
    ret = regexec(&regex, args[i], 0, NULL, 0);
    if (!ret) {
      char *arg = args[i];
      int fd = strlen(arg) == 1 ? 1 : arg[0] - '0';

      if (i + 1 >= *count) {
        perror("No file path provided.");
        return -1;
      }

      char *filename = args[i + 1];

      *savedfd = dup(fd);
      *oldfd = fd;
      newfd = redirect(fd, filename);

      *count = i;
      args[*count] = NULL;
    }
  }

  regfree(&regex);

  return newfd;
}

int redirect(int fd, char *filename) {
  FILE *file;

  if (fd == 1 || fd == 3) {
    file = fopen(filename, "w");
  } else {
    file = fopen(filename, "r");
  }

  if (file == NULL) {
    perror("open");
    return -1;
  }

  int newfd = fileno(file);

  if (newfd == -1) {
    perror("Error getting file descriptor");
    return -1;
  }

  if (dup2(newfd, fd) == -1) {
    fclose(file);
    perror("Error redirecting");
    return -1;
  }

  return newfd;
}

int restore_default_fd(int oldfd, int savedfd) {
  if (dup2(savedfd, oldfd) == -1) {
    perror("Default fd restoration failed");
    exit(1);
  }

  close(savedfd);

  return 1;
}
