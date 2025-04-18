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

typedef struct Velocity {
  float x;
  float y;
} Velocity;

typedef struct Player {
  Color color;
  float width;
  float height;
  Position position;
  Velocity velocity;
} Player;

typedef struct World {
  Player player;
  enum State state;
} World;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int SCREEN_HALF_W;
int SCREEN_HALF_H;
int N_COLORS=12;
Color COLORS[13]={RED, WHITE, BLUE, ORANGE, PINK, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, DARKBLUE, PURPLE, YELLOW};
Color PLAYER_COLOR=RED;
int PLAYER_WIDTH=100;
int PLAYER_HEIGHT=100;
unsigned int SEED=69;

World initialize_world() {
  SetRandomSeed(SEED);
  InitWindow(0, 0, "The Game");
  ToggleFullscreen();
  SCREEN_WIDTH = GetRenderWidth();
  SCREEN_HEIGHT = GetRenderHeight();
  SCREEN_HALF_W = SCREEN_WIDTH/2;
  SCREEN_HALF_H = SCREEN_HEIGHT/2;
  Player player;
  player.color = PLAYER_COLOR;
  player.width = PLAYER_WIDTH;
  player.height = PLAYER_HEIGHT;
  Position position = {SCREEN_HALF_W-PLAYER_WIDTH/2, SCREEN_HALF_H-PLAYER_HEIGHT/2};
  player.position = position;
  Velocity velocity = {0.0, 0.0};
  player.velocity = velocity;
  uint8_t state = START;
  World world = {player, state};
  return world;
}

void DrawTextCenter(const char *text, int fontSize, Color color) {
  int textHeight = fontSize;
  int textWidth = MeasureText(text, fontSize);
  DrawText(text, SCREEN_HALF_W-textWidth/2, SCREEN_HALF_H-textHeight/2, fontSize, color);
}

void DrawPlayer(World *world) {
  // Check position boundaries.
  Player pl = world->player;
  Position pos = pl.position;

  float overflowX = pos.x + pl.width - SCREEN_WIDTH;
  bool resetX = overflowX > pl.width;
  if (resetX) {
    world->player.position.x = 0;
  }
  else if (overflowX > 0) {
    DrawRectangle(0, pos.y, overflowX, pl.height, pl.color);
  }
  else if (pos.x + pl.width < 0) {
    world->player.position.x = SCREEN_WIDTH - pl.width;
  }
  else if (pos.x < 0) {
    DrawRectangle(SCREEN_WIDTH + pos.x, pos.y, SCREEN_WIDTH + pos.x, pl.height, pl.color);
  }

  float overflowY = pos.y + pl.height - SCREEN_HEIGHT;
  bool resetY = overflowY > pl.width;
  if (resetY) {
    world->player.position.y = 0;
  }
  else if (overflowY > 0) {
    DrawRectangle(pos.x, 0, pl.width, overflowY, pl.color);
  }
  else if (pos.y + pl.height < 0) {
    world->player.position.y = SCREEN_HEIGHT - pl.height;
  }
  else if (pos.y < 0) {
    DrawRectangle(pos.x, SCREEN_HEIGHT + pos.y, pl.width, SCREEN_HEIGHT + pos.y, pl.color);
  }
  
  DrawRectangle(pos.x, pos.y, pl.width, pl.height, pl.color);
}

void ChangeColor(Player *player) {
  if (GetRandomValue(0, 1) == 0) {
    Color c = COLORS[GetRandomValue(0, N_COLORS-1)];
    player->color = c;
  }
}

void HandleInput(World *world) {
  // Update player velocity.
  if (IsKeyPressed(KEY_RIGHT)) {
    ChangeColor(&(world->player));
    world->player.velocity.x += 100;
  }
  if (IsKeyPressed(KEY_LEFT)) {
    ChangeColor(&(world->player));
    world->player.velocity.x -= 100;
  }
  if (IsKeyPressed(KEY_UP)) {
    ChangeColor(&(world->player));
    world->player.velocity.y -= 100;
  }
  if (IsKeyPressed(KEY_DOWN)) {
    ChangeColor(&(world->player));
    world->player.velocity.y += 100;
  }
}

void DrawWorld(World *world) {
  switch (world->state) {
    case START: {
      DrawTextCenter("Press Enter to start!", 50, RED);
      if (IsKeyPressed(KEY_ENTER)) world->state = RUN;
    } break;
    case RUN: {
      // Update player position.
      float dt = GetFrameTime();
      Velocity v = world->player.velocity;
      world->player.position.x += dt*v.x;
      world->player.position.y += dt*v.y;

      // Draw player.
      DrawPlayer(world);
    } break;
    default: break;
  }
}

void MainLoop(World *world) {
  BeginDrawing();
  ClearBackground(GRAY);
  HandleInput(world);
  DrawWorld(world);
  EndDrawing();
}

int main(int argc, char **argv) {
  World world = initialize_world();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    MainLoop(&world);
  }
  
  CloseWindow();
  return 0;
}
