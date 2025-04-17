#include <stdio.h>
#include <stdlib.h>

const char *old_cmd = "cc -Wall -Wextra -g main.c greet.c -o ./output/main";

const char *raylib = "";
const char *cmd = "cc main.c -I./external/raylib-5.5/src/ -L./external/raylib-5.5/src/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o ./output/main";

int main(int argc, char **argv) {
  return system(cmd);
}
