#include <stdlib.h>

#define MAX_SPRITES   128
#define MAX_SCENES    256

typedef struct Game GameT;
typedef struct Sprite SpriteT;
SpriteT *createTestHumanoid();

#include "raylib.h"
#include "player.c"
#include "scene.c"
#include "sprite.c"
#include "game.c"
#include "instance.c"
