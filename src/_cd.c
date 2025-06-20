#include "_cd.h"
#include "_type.h"

void _cd(char *path) {
  char full_path[MAXIMUM_PATH_LENGTH];

  if (path[0] == '~') {
    const char *home = getenv("HOME");
    if (home == NULL) {
      fprintf(stderr, "cd: HOME not set\n");
      return;
    }
    snprintf(full_path, sizeof(full_path), "%s%s", home, path + 1);
    path = full_path;
  }

  if (chdir(path) == -1) {
    fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
    return;
  }
}
