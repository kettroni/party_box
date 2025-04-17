#include <stdio.h>
#include <stdint.h>
#include "raylib.h"

enum State {
  START,
  RUN
};

typedef struct Position {
  float x;
  float y;
} Position;

typedef struct Player {
  Position position;
} Player;

typedef struct World {
  Player player;
  enum State state;
} World;

int WIDTH;
int HEIGHT;
int HALF_W;
int HALF_H;

World initialize_world() {
  
  InitWindow(0, 0, "The Game");
  ToggleFullscreen();
  WIDTH = GetRenderWidth();
  HEIGHT = GetRenderHeight();
  HALF_W = WIDTH/2;
  HALF_H = HEIGHT/2;
  Player player;
  Position position = {HALF_W, HALF_H};
  player.position = position;
  uint8_t state = START;
  World world = {player, state};
  return world;
}

void DrawTextCenter(const char *text, int fontSize, Color color) {
  int textHeight = fontSize;
  int textWidth = MeasureText(text, fontSize);
  DrawText(text, HALF_W-textWidth/2, HALF_H-textHeight/2, fontSize, color);
}

void DrawPlayer(World world) {
  DrawRectangle(world.player.position.x, world.player.position.y, 20, 20, PINK);
}

int main(int argc, char **argv) {
  World world = initialize_world();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(GRAY);

    switch (world.state) {
      case START: {
	DrawTextCenter("Press Enter to start!", 20, RED);
	if (IsKeyPressed(KEY_ENTER)) world.state = RUN;
      } break;
      case RUN: {
	DrawPlayer(world);
      } break;
      default: break;
    }
      
    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}
