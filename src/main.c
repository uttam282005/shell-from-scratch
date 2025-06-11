#include "string.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    input[strlen(input) - 1] = ' ';
    input[strlen(input)] = '\0';

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

    int character = 0;

    bool is_echo = false;
    bool is_type = false;

    char *command;
    int c = 0;

    for (c = first_char; input[c] != '\0'; c++) {
      if (input[c] == ' ') {
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

    character = 0;
    while (input[c] != '\0') {
      if (input[c] == ' ') {

        word[character] = '\0';

        if (is_echo) {
          printf("%s ", word);
        }

        if (is_type) {
          if (strcmp(word, "echo") == 0 || strcmp(word, "type") == 0)
            printf("%s is a shell builtin\n", word);
          else
            printf("%s: not found\n", word);
        }

        character = 0;

      } else
        word[character++] = input[c];
      c++;
    }

    if (is_echo)
      printf("%s\n", "");

    if (!is_valid_command)
      printf("%s: command not found\n", input);
  }

  return 0;
}
