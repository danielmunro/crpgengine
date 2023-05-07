#ifndef CJRPGENGINE_H_INCLUDED
#define CJRPGENGINE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define MAX_OBJECTS 255
#define MAX_SCENES    256
#define MAX_LAYER_SIZE 1024
#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440
#define MAX_EXITS 64
#define LAYER_COUNT 3
#define LAYER_TYPE_BACKGROUND 0
#define LAYER_TYPE_MIDGROUND 1
#define LAYER_TYPE_FOREGROUND 2
#define TARGET_FRAMERATE 60

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

#define HUMANOID_WIDTH     16.0f
#define HUMANOID_HEIGHT    24.0f
#define DIRECTION_UP       1
#define DIRECTION_DOWN     2
#define DIRECTION_LEFT     3
#define DIRECTION_RIGHT    4

#define ANIM_UP 1
#define ANIM_DOWN 2
#define ANIM_LEFT 3
#define ANIM_RIGHT 4

#include "util.c"
#include "sprite.c"
#include "mobile.c"
#include "player.c"
#include "tilemap.c"
#include "scene.c"
#include "db.c"
#include "game.c"
#include "validate.c"
#include "dump.c"

#endif /* CJRPGENGINE_H_INCLUDED */
