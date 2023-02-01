#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define MAX_SPRITES   128
#define MAX_SCENES    256
#define MAX_LAYER_SIZE 1024
#define MAX_TILES 512

#define SCENE_NAME_MAX_LENGTH 64

#define SCENE_TYPE_FREE_MOVE 1
#define SCENE_TYPE_FIGHT 2
#define SCENE_TYPE_MENU 3

#include "sprite.c"
#include "player.c"
#include "tilemap.c"
#include "scene.c"
#include "game.c"
