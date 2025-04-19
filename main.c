#include <stdio.h>
#include <stdint.h>
#include "raylib.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int SCREEN_HALF_W;
int SCREEN_HALF_H;
int N_COLORS=12;
Color COLORS[13]={RED, WHITE, BLUE, ORANGE, PINK, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, DARKBLUE, PURPLE, YELLOW};
Color PLAYER_COLOR=RED;
Rectangle PLAYER_RECTANGLE={0.0, 0.0, 100.0, 100.0};
unsigned int SEED=69;

const char* BUNNY_SRC = "resources/bunny.png";

enum State {
  START,
  RUN
};

typedef struct Velocity {
  float x;
  float y;
} Velocity;

typedef struct Player {
  Color color;
  Rectangle rectangle;
  Velocity velocity;
  Texture2D texture2d;
  Rectangle srcRec;
} Player;

typedef struct World {
  Player player;
  enum State state;
} World;

World initialize_world() {
  // Seed
  SetRandomSeed(SEED);

  // Window
  InitWindow(0, 0, "The Bunny Game");
  ToggleFullscreen();
  SCREEN_WIDTH = GetRenderWidth();
  SCREEN_HEIGHT = GetRenderHeight();
  SCREEN_HALF_W = SCREEN_WIDTH/2;
  SCREEN_HALF_H = SCREEN_HEIGHT/2;

  // Player
  Player player;
  player.color = PLAYER_COLOR;
  player.rectangle = PLAYER_RECTANGLE;
  Velocity velocity = {0.0, 0.0};
  player.velocity = velocity;
  Texture2D bunny = LoadTexture(BUNNY_SRC);
  player.texture2d = bunny;
  Rectangle srcRec = {
    0.0f,
    0.0f,
    (float)player.texture2d.width,
    (float)player.texture2d.height
  };
  player.srcRec = srcRec;

  // State
  uint8_t state = START;

  // World
  World world = {player, state};
  return world;
}

void DrawTextCenter(const char *text, int fontSize, Color color) {
  int textHeight = fontSize;
  int textWidth = MeasureText(text, fontSize);
  DrawText(text, SCREEN_HALF_W-textWidth/2, SCREEN_HALF_H-textHeight/2, fontSize, color);
}

void DrawPlayer(World *world) {
  Player pl = world->player;
  Rectangle rec = pl.rectangle;

  Vector2 origin = { 0.0f, 0.0f };

  float overflowX = rec.x + rec.width - SCREEN_WIDTH;
  bool resetX = overflowX > rec.width;
  if (resetX) {
    world->player.rectangle.x = 0;
  }
  else if (overflowX > 0) {
    Rectangle destRec = {-rec.width+overflowX, rec.y, rec.width, rec.height};
    DrawTexturePro(pl.texture2d, pl.srcRec, destRec, origin, 0.0f, pl.color);
  }
  else if (rec.x + rec.width < 0) {
    world->player.rectangle.x = SCREEN_WIDTH - rec.width;
  }
  else if (rec.x < 0) {
    Rectangle destRec = {SCREEN_WIDTH + rec.x, rec.y, rec.width, rec.height};
    DrawTexturePro(pl.texture2d, pl.srcRec, destRec, origin, 0.0f, pl.color);
  }

  float overflowY = rec.y + rec.height - SCREEN_HEIGHT;
  bool resetY = overflowY > rec.width;
  if (resetY) {
    world->player.rectangle.y = 0;
  }
  else if (overflowY > 0) {
    Rectangle destRec = {rec.x, -rec.height+overflowY, rec.width, rec.height};
    DrawTexturePro(pl.texture2d, pl.srcRec, destRec, origin, 0.0f, pl.color);
  }
  else if (rec.y + rec.height < 0) {
    world->player.rectangle.y = SCREEN_HEIGHT - rec.height;
  }
  else if (rec.y < 0) {
    Rectangle destRec = {rec.x, SCREEN_HEIGHT+rec.y, rec.width, rec.height};
    DrawTexturePro(pl.texture2d, pl.srcRec, destRec, origin, 0.0f, pl.color);
  }

  DrawTexturePro(pl.texture2d, pl.srcRec, rec, origin, 0.0f, pl.color);
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
      DrawPlayer(world);
    } break;
    default: break;
  }
}

void MainLoop(World *world) {
  float dt = GetFrameTime();
  
  BeginDrawing();
  ClearBackground(GRAY);
  HandleInput(world);

  // Update player position.
  Velocity v = world->player.velocity;
  world->player.rectangle.x += dt*v.x;
  world->player.rectangle.y += dt*v.y;

  DrawWorld(world);
  EndDrawing();
}

int main() {
  World world = initialize_world();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    MainLoop(&world);
  }
  
  CloseWindow();
  return 0;
}
