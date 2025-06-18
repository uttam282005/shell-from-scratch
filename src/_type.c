#include "_type.h"
#include "utils.h"

const char *builtin_commands[] = {"echo", "cd", "exit", "type", NULL};

bool is_builtin(char *command) {
  for (int i = 0; builtin_commands[i]; i++) {
    if (strcmp(command, builtin_commands[i]) == 0)
      return true;
  }

  return false;
}

bool command_found(char *path, const char *command) {
  DIR *dp;
  struct dirent *entry;

  if ((dp = opendir(path)) == NULL) {
    return false;
  }

  while ((entry = readdir(dp)) != NULL) {
    if (strcmp(entry->d_name, command) == 0) {
      closedir(dp);
      return true;
    }
  }

  closedir(dp);
  return false;
}

char *get_path(const char *command) {
  if (command == NULL)
    return NULL;

  char *paths = getenv("PATH");

  if (paths == NULL)
    return NULL;

  int path_count = 0;
  char *path_b = malloc(MAXIMUM_PATH_LENGTH);
  char **parsed_paths = split_string(paths, ':', &path_count);

  for (int cur_path = 0; cur_path < path_count; cur_path++) {
    if (command_found(parsed_paths[cur_path], command)) {
      strcpy(path_b, parsed_paths[cur_path]);
      return path_b;
    }
  }

  return NULL;
}

void _type(char **args, int count) {
  char *path;

  for (int i = 1; i < count; i++) {
    if (is_builtin(args[i])) {
      printf("%s is a shell builitn\n", args[i]);
      continue;
    }
    if ((path = get_path(args[i])) != NULL) {
      printf("%s is %s/", args[i], path);
      printf("%s\n", args[i]);
      continue;
    }
    printf("%s: not found\n", args[i]);
  }
}
