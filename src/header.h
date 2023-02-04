#ifndef CJRPGENGINE_H_INCLUDED
#define CJRPGENGINE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <./_deps/raylib-src/src/raylib.h>
#include <libxml/xmlreader.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define MAX_LAYER_COUNT 3
#define MAX_SPRITES   128
#define MAX_OBJECTS 255
#define MAX_SCENES    256
#define MAX_LAYER_SIZE 1024
#define MAX_TILES 2048

#define SCENE_NAME_MAX_LENGTH 64

#define SCENE_TYPE_FREE_MOVE 1
#define SCENE_TYPE_FIGHT 2
#define SCENE_TYPE_MENU 3

typedef char Layer[MAX_LAYER_COUNT][MAX_LAYER_SIZE][MAX_LAYER_SIZE];

#include "util.c"
#include "sprite.c"
#include "player.c"
#include "tilemap.c"
#include "scene.c"
#include "db.c"
#include "game.c"

#endif /* CJRPGENGINE_H_INCLUDED */
