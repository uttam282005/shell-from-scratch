#include "_echo.h"

void _echo(char **args, int count) {
  for (int i = 1; i < count; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
}
