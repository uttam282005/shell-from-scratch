#include "string.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage

  // Wait for user input
  char input[100];

  while (1) {
    printf("$ ");
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    printf("%s: command not found\n", input);
  }

  return 0;
}
