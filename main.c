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
Rectangle PLAYER_RECTANGLE={0.0, 0.0, 64.0, 64.0};
unsigned int SEED=69;

const char* BUNNY_SRC = "resources/bunny2.png";
const char* EGG_SRC = "resources/egg1.png";
const char* BG_SRC = "resources/grass3.png";
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
  Texture2D background;
  Rectangle srcRec;
  Rectangle dstRec;
  Texture2D egg;
  Vector2 eggPosition;
  int eggFrame;
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

  // Background
  Texture2D background = LoadTexture(BG_SRC);
  Rectangle bgSrc = {
    0.0f,
    0.0f,
    (float)SCREEN_HALF_W/2,
    (float)SCREEN_HALF_H/2,
  };
  Rectangle bgDst = {
    0.0f,
    0.0f,
    (float)SCREEN_WIDTH,
    (float)SCREEN_HEIGHT,
  };

  Texture2D egg = LoadTexture(EGG_SRC);
  Vector2 eggPosition = {50.0f, 200.0f};
  int eggFrame = 0;
  // World
  World world = {player, state, background, bgSrc, bgDst, egg, eggPosition, eggFrame};
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

void DrawEgg(World *world) {
  DrawTextureEx(
    world->egg,
    world->eggPosition,
    0.0f,
    3.0f,
    GetColor(0xf1f2daff));
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
  ClearBackground(GetColor(0x00303B00));
  DrawTexturePro(
    world->background,
    world->srcRec,
    world->dstRec,
    (Vector2){ 0.0f, 0.0f },
    0.0f,
    GetColor(0xf1f2daff));
  switch (world->state) {
    case START: {
      DrawTextCenter("Press Enter to start!", 50, GetColor(0xF1F2DAFF));
    } break;
    case RUN: {
      DrawEgg(world);
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

void UpdateEgg(World *world) {
  world->eggFrame++;
  if (world->eggFrame > 60) {world->eggFrame = 0;}
  if (world->eggFrame <= 15) {
    world->eggPosition.x++;
    world->eggPosition.y--;
  } else if (world->eggFrame <= 30) {
    world->eggPosition.x++;
    world->eggPosition.y++;
  } else if (world->eggFrame <= 45) {
    world->eggPosition.x--;
    world->eggPosition.y--;
  } else if (world->eggFrame <= 60) {
    world->eggPosition.x--;
    world->eggPosition.y++;
  }
}

bool CatchEgg(World *world) {
  return CheckCollisionPointRec(world->eggPosition, world->player.rectangle);
}

void NewEgg(World *world) {
  world->eggPosition.x = GetRandomValue(0, SCREEN_WIDTH);
  world->eggPosition.y = GetRandomValue(0, SCREEN_HEIGHT);
  world->eggFrame = 0;
}

void MainLoop(World *world) {
  float dt = GetFrameTime();

  HandleInput(world);
  
  UpdatePlayer(&(world->player), dt);
  UpdateEgg(world);

  if (CatchEgg(world)) NewEgg(world);
  
  DrawWorld(world);
}

int main() {
  World world = InitializeWorld();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    MainLoop(&world);
  }

  UnloadTexture(world.player.texture2d);
  UnloadTexture(world.background);
  UnloadTexture(world.egg);
  
  CloseWindow();
  return 0;
}
