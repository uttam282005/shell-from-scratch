#include "executer.h"
#include <dirent.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM_ALLOWED_PATHS 15
#define MAXIMUM_PATH_LENGTH 100

// TODO: Trim input of spaces
// TODO: Refactor

bool is_command_exist(char *path, const char *command) {
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

  char *path_b = (char *)malloc(MAXIMUM_PATH_LENGTH);
  char path[MAXIMUM_PATH_LENGTH];
  char path_arr[MAXIMUM_ALLOWED_PATHS][MAXIMUM_PATH_LENGTH];

  int c = 0;
  int path_c = 0;
  int path_cnt = 0;

  while (path_cnt < MAXIMUM_ALLOWED_PATHS) {
    if (paths[c] == ':' || paths[c] == '\0') {
      path[path_c] = '\0';
      strcpy(path_arr[path_cnt++], path);
      path_c = 0;
    } else
      path[path_c++] = paths[c];
    if (paths[c] == '\0')
      break;
    c++;
  }

  for (int cur_path = 0; cur_path < path_cnt; cur_path++) {
    if (is_command_exist(path_arr[cur_path], command)) {
      strcpy(path_b, path_arr[cur_path]);
      return path_b;
    }
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage

  // Wait for user input
  char input[100];

  while (1) {
    bool is_valid_command = false;

    printf("$ ");
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';

    if (strcmp(input, "exit 0") == 0)
      break;

    int first_char = 0;
    for (int charc = 0; input[charc] != '\0'; charc++) {
      if (input[charc] != ' ') {
        first_char = charc;
        break;
      }
    }

    char word[100];

    bool is_echo = false;
    bool is_type = false;

    char *command;
    int c = first_char;
    int character = 0;

    for (;; c++) {
      if (input[c] == ' ' || input[c] == '\0') {
        word[character] = '\0';
        command = word;
        break;
      } else
        word[character++] = input[c];
    }

    c++;

    if (strcmp(command, "echo") == 0) {
      is_echo = true;
      is_valid_command = true;
    }

    if (strcmp(command, "type") == 0) {
      is_type = true;
      is_valid_command = true;
    }

    if (strcmp(command, "exit") == 0) {
      break;
    }

    character = 0;
    char *path;
    while (1) {
      if (input[c] == ' ' || input[c] == '\0') {

        word[character] = '\0';

        if (is_echo) {
          printf("%s ", word);
        }

        if (is_type) {
          if (strcmp(word, "echo") == 0 || strcmp(word, "type") == 0 ||
              strcmp(word, "exit") == 0)
            printf("%s is a shell builtin\n", word);
          else if ((path = get_path(word)) != NULL) {
            printf("%s is %s/", word, path);
            printf("%s\n", word);
          } else {
            printf("%s: not found\n", word);
          }
        }

        character = 0;

      } else
        word[character++] = input[c];
      if (input[c] == '\0')
        break;
      c++;
    }

    if (is_echo)
      printf("%s\n", "");

    if (!is_valid_command) {
      printf("%s: command not found\n", input);
    }
  }

  return 0;
}
