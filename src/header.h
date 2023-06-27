#ifndef CJRPGENGINE_H_INCLUDED
#define CJRPGENGINE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include <cyaml/cyaml.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 450
#define SCALE (float) 1.0

#define LAYER_COUNT 3

#define TARGET_FRAMERATE 60

#define MOB_COLLISION_WIDTH 16
#define MOB_COLLISION_HEIGHT 12
#define MOB_COLLISION_HEIGHT_OFFSET 12

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

#define FONT_SIZE 20
#define LINE_HEIGHT 30
#define UI_PADDING 20

#define EXIT_NO_INDEX_DIR 1

#define MAX_OBJECTS 255
#define MAX_SCENES    256
#define MAX_LAYER_SIZE 1024
#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440
#define MAX_EXITS 64
#define MAX_ENTRANCES 64
#define MAX_MOBILES 255
#define MAX_INSTRUCTIONS 255
#define MAX_CONTROLS 255
#define MAX_AUDIO 1024
#define MAX_CHARACTERS_PER_LINE 60
#define MAX_STORIES 1024
#define MAX_BEASTIARY_SIZE 1024
#define MAX_BEASTS 255
#define MAX_BEASTS_IN_FIGHT 9
#define MAX_LOG_LINE_LENGTH 2048
#define MAX_DATA_SIZE 2048
#define MAX_MENUS 12
#define MAX_ITEMS 255

const char *logLevels[] = {
        "error",
        "warn",
        "info",
        "debug"
};

typedef enum {
    ERROR = 0,
    WARN = 1,
    INFO = 2,
    DEBUG = 3,
} LogLevel;

const char *conditions[] = {
        "engaged",
        "has",
        "not_has",
        "at",
        "has_story",
        "not_has_story",
};

typedef enum {
    ENGAGED = 0,
    HAS,
    NOT_HAS,
    AT,
    HAS_STORY,
    NOT_HAS_STORY,
} Condition;

const char *outcomes[] = {
        "speak",
        "move_to",
        "direction",
        "sprite",
        "wait",
        "give_item",
        "take",
        "add_story",
};

typedef enum {
    SPEAK = 0,
    MOVE_TO,
    DIRECTION,
    SPRITE,
    WAIT,
    GIVE_ITEM,
    TAKE,
    ADD_STORY,
} Outcome;

typedef enum {
    BACKGROUND = 0,
    MIDGROUND,
    FOREGROUND,
} LayerType;

typedef enum {
    UP = 1,
    DOWN,
    LEFT,
    RIGHT
} AnimationDirection;

typedef enum {
    EXIT = 1,
    ENTRANCE,
} ObjectType;

typedef enum {
    PARTY_MENU = 0,
    ITEMS_MENU,
} MenuType;

typedef enum {
    CONSUMABLE,
    EQUIPMENT,
    CRAFTING_MATERIAL,
} ItemType;

int MOVE_KEYS[] = {KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT};
int DIRECTIONS[] = {UP, DOWN, RIGHT, LEFT};
int DIRECTION_COUNT = sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0]);
char *PartyMenuItems[] = {
        "Items",
        "Rearrange",
        "Config",
        "Save",
        "Quit",
};

#include "log.c"
#include "util.c"
#include "yaml/animation.c"
#include "yaml/config.c"
#include "yaml/music.c"
#include "yaml/attributes.c"
#include "yaml/player.c"
#include "yaml/storyline.c"
#include "yaml/scene.c"
#include "yaml/sound.c"
#include "yaml/beastiary.c"
#include "yaml/mobile.c"
#include "cyaml.c"
#include "window.c"
#include "ui.c"
#include "audio.c"
#include "sprite.c"
#include "attributes.c"
#include "item.c"
#include "beastiary.c"
#include "mobile.c"
#include "control_block.c"
#include "player.c"
#include "menu/items_menu.c"
#include "menu/party_menu.c"
#include "menu.c"
#include "object.c"
#include "fight.c"
#include "tilemap.c"
#include "exploration.c"
#include "scene.c"
#include "runtime_args.c"
#include "tilemap_xml_reader.c"
#include "xmlparser.c"
#include "db.c"
#include "game.c"
#include "validate.c"
#include "dump.c"

#endif /* CJRPGENGINE_H_INCLUDED */
