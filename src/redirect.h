#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int handle_redirect(char **args, int *count, int *savedfd, int *oldfd);
int redirect(int fd, char *filename);
int restore_default_fd(int oldfd, int savedfd);
