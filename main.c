#include <stdio.h>
#include <stdint.h>
#include "raylib.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int SCREEN_HALF_W;
int SCREEN_HALF_H;
int DECAY=5;
int ACCELERATION=25;
int CAP_SPEED=1500;
int N_COLORS=11;
Color COLORS[12]={RED, BLUE, ORANGE, PINK, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, DARKBLUE, PURPLE, YELLOW};
Color PLAYER_COLOR=WHITE;
Rectangle PLAYER_RECTANGLE={0.0, 0.0, 200.0, 200.0};
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

World InitializeWorld() {
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
  Rectangle rec = {
    SCREEN_HALF_W-PLAYER_RECTANGLE.width/2,
    SCREEN_HALF_H-PLAYER_RECTANGLE.height/2,
    PLAYER_RECTANGLE.width,
    PLAYER_RECTANGLE.height,
  };
  player.rectangle = rec;
  Velocity velocity = {0.0, 0.0};
  player.velocity = velocity;
  Texture2D bunny = LoadTexture(BUNNY_SRC);
  player.texture2d = bunny;
  Rectangle srcRec = {
    0.0f,
    0.0f,
    (float)player.texture2d.width,
    (float)player.texture2d.height,
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

void DrawPlayer(Player *player) {
  DrawTexturePro(
    player->texture2d,
    player->srcRec,
    player->rectangle,
    (Vector2){ 0.0f, 0.0f },
    0.0f,
    player->color);
}

void ChangeColor(Player *player) {
  if (GetRandomValue(0, 1) == 0) {
    Color c = COLORS[GetRandomValue(0, N_COLORS-1)];
    player->color = c;
  }
}

bool PressedOrHeld(int key) {
  return (IsKeyPressed(key) || IsKeyDown(key));
}

void HandleInput(World *world) {
  switch (world->state) {
    case START: {
      if (IsKeyPressed(KEY_ENTER)) world->state = RUN;
    } break;
    case RUN: {
      // Update player velocity.
      if (world->player.velocity.x < 0) {
	world->player.velocity.x += DECAY;
      } else if (world->player.velocity.x > 0) {
	world->player.velocity.x -= DECAY;
      }

      if (world->player.velocity.y < 0) {
	world->player.velocity.y += DECAY;
      } else if (world->player.velocity.y > 0) {
	world->player.velocity.y -= DECAY;
      }

      if (PressedOrHeld(KEY_RIGHT)) {
	ChangeColor(&(world->player));
	world->player.velocity.x += ACCELERATION;
      }
      if (PressedOrHeld(KEY_LEFT)) {
	ChangeColor(&(world->player));
	world->player.velocity.x -= ACCELERATION;
      }
      if (PressedOrHeld(KEY_UP)) {
	ChangeColor(&(world->player));
	world->player.velocity.y -= ACCELERATION;
      }
      if (PressedOrHeld(KEY_DOWN)) {
	ChangeColor(&(world->player));
	world->player.velocity.y += ACCELERATION;
      }
      if (world->player.velocity.x > CAP_SPEED) world->player.velocity.x = CAP_SPEED;
      if (world->player.velocity.x < -CAP_SPEED) world->player.velocity.x = -CAP_SPEED;
      if (world->player.velocity.y > CAP_SPEED) world->player.velocity.y = CAP_SPEED;
      if (world->player.velocity.y < -CAP_SPEED) world->player.velocity.y = -CAP_SPEED;
    } break;
    default: break;
  }
}

void DrawWorld(World *world) {
  BeginDrawing();
  ClearBackground(GRAY);
  switch (world->state) {
    case START: {
      DrawTextCenter("Press Enter to start!", 50, RED);
    } break;
    case RUN: {
      DrawPlayer(&(world->player));
      world->player.color = WHITE;
    } break;
    default: break;
  }
  EndDrawing();
}

void UpdatePlayer(Player *player, float dt) {
  Velocity v = player->velocity;
  if ((v.x < 0 && player->srcRec.width < 0)
      ||
      (v.x > 0 && player->srcRec.width > 0)) player->srcRec.width *= -1;

  Rectangle *rec = &(player->rectangle);

  float nextX = rec->x + dt*v.x;
  if (nextX > SCREEN_WIDTH) {
    rec->x = -rec->width;
  } else if (nextX + rec->width < 0) {
    rec->x = SCREEN_WIDTH;
  } else {
    rec->x = nextX;
  }

  float nextY = rec->y + dt*v.y;
  if (nextY > SCREEN_HEIGHT) {
    rec->y = -rec->height;
  } else if (nextY + rec->height < 0) {
    rec->y = SCREEN_HEIGHT;
  } else {
    rec->y = nextY;
  }
}

void MainLoop(World *world) {
  float dt = GetFrameTime();

  HandleInput(world);

  UpdatePlayer(&(world->player), dt);

  DrawWorld(world);
}

int main() {
  World world = InitializeWorld();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    MainLoop(&world);
  }

  CloseWindow();
  return 0;
}
