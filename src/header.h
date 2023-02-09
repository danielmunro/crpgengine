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
#define LAYER_COUNT 3
#define LAYER_TYPE_BACKGROUND 1
#define LAYER_TYPE_MIDGROUND 2
#define LAYER_TYPE_FOREGROUND 3

#define SCENE_NAME_MAX_LENGTH 64

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

#include "util.c"
#include "sprite.c"
#include "player.c"
#include "tilemap.c"
#include "scene.c"
#include "db.c"
#include "game.c"

#endif /* CJRPGENGINE_H_INCLUDED */
