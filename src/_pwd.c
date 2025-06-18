#include "_pwd.h"

void _pwd() {
  char cwd[MAX_PATH_LEN];

  if (getcwd(cwd, MAX_PATH_LEN) == NULL) {
    perror("sys call failed cannot fetch working directory\n");
    return;
  }

  printf("%s\n", cwd);
}
