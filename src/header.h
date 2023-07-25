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

#define TARGET_FRAMERATE 60

#define MOB_COLLISION_WIDTH 12
#define MOB_COLLISION_WIDTH_OFFSET 2
#define MOB_COLLISION_HEIGHT 12
#define MOB_COLLISION_HEIGHT_OFFSET 12

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

#define START_ENTRANCE "start"

#define FONT_SIZE 20
#define LINE_HEIGHT 30
#define UI_PADDING 20

#define EXIT_NO_INDEX_DIR 1
#define EXIT_MISSING_SPRITESHEET 2

#define MAX_OBJECTS 255
#define MAX_SCENES    256
#define MAX_FILES 256
#define MAX_LAYERS 1024
#define MAX_LAYER_SIZE 1024
#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440
#define MAX_EXITS 64
#define MAX_ENTRANCES 64
#define MAX_MOBILES 255
#define MAX_INSTRUCTIONS 255
#define MAX_CONTROLS 255
#define MAX_ACTIVE_CONTROLS 64
#define MAX_AUDIO 1024
#define MAX_CHARACTERS_PER_LINE 60
#define MAX_STORIES 1024
#define MAX_BEASTIARY_SIZE 1024
#define MAX_BEASTS 255
#define MAX_BEASTS_IN_FIGHT 9
#define MAX_LOG_LINE_LENGTH 2048
#define MAX_DATA_SIZE 2048
#define MAX_MENUS 255
#define MAX_ITEMS 255
#define MAX_PARTY_SIZE 4
#define MAX_TEAM_SIZE 255
#define MAX_FS_PATH_LENGTH 255
#define MAX_MOBILE_MOVEMENTS 64
#define MAX_SPRITES 1024

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

LayerType LAYERS[] = {
    BACKGROUND,
    MIDGROUND,
    FOREGROUND,
};

#define LAYER_COUNT sizeof(LAYERS) / sizeof(LAYERS[0])

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} AnimationType;
const int MOVE_KEYS[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
const AnimationType DIRECTIONS[] = {UP, DOWN, LEFT, RIGHT};
#define DIRECTION_COUNT sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0])
AnimationType getOppositeDirection(AnimationType direction) {
    if (direction == UP) {
        return DOWN;
    } else if (direction == DOWN) {
        return UP;
    } else if (direction == LEFT) {
        return RIGHT;
    } else if (direction == RIGHT) {
        return LEFT;
    }
    return DOWN;
}
const AnimationType ANIMATION_TYPES[] = {UP, DOWN, LEFT, RIGHT};
const char *AnimationTypeStrings[] = {
        "up",
        "down",
        "left",
        "right",
};
#define ANIMATION_TYPE_COUNT sizeof(AnimationTypeStrings) / sizeof(AnimationTypeStrings[0])

typedef enum {
    EXIT = 1,
    ENTRANCE,
} ObjectType;

typedef enum {
    PARTY_MENU = 0,
    ITEMS_MENU,
    QUIT_MENU,
} MenuType;

#define PARTY_MENU_ITEMS "Items"
#define PARTY_MENU_REARRANGE "Rearrange"
#define PARTY_MENU_CONFIG "Config"
#define PARTY_MENU_SAVE "Save"
#define PARTY_MENU_QUIT "Quit"

char *PartyMenuItems[] = {
        PARTY_MENU_ITEMS,
        PARTY_MENU_REARRANGE,
        PARTY_MENU_CONFIG,
        PARTY_MENU_SAVE,
        PARTY_MENU_QUIT,
};

#define QUIT_MENU_YES "Yes"
#define QUIT_MENU_NO "No"

char *QuitMenuItems[] = {
        QUIT_MENU_NO,
        QUIT_MENU_YES,
};

#define QUIT_MENU_ITEM_COUNT 2

typedef enum {
    CONSUMABLE,
    EQUIPMENT,
    CRAFTING_MATERIAL,
} ItemType;

typedef enum {
    OPEN_MENU,
    CLOSE_MENU,
} MenuSelectResponseType;

#define COLLIDE_TYPE_OBJECTS "objects"
#define COLLIDE_TYPE_PLAYER "player"
#define COLLIDE_TYPE_WARPS "warps"
#define COLLIDE_TYPE_COUNT 3

#include "log.c"
#include "util.c"
#include "yaml/spritesheet.c"
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
#include "yaml/save.c"
#include "runtime_args.c"
#include "cyaml.c"
#include "window.c"
#include "ui.c"
#include "audio.c"
#include "spritesheet.c"
#include "animation.c"
#include "attributes.c"
#include "item.c"
#include "beastiary.c"
#include "mobile.c"
#include "player.c"
#include "control_block.c"
#include "menu.c"
#include "menu/items_menu.c"
#include "menu/party_menu.c"
#include "menu/quit_menu.c"
#include "menu_list.c"
#include "object.c"
#include "fight.c"
#include "tilemap.c"
#include "exploration.c"
#include "scene.c"
#include "tilemap_xmlreader.c"
#include "xmlparser.c"
#include "db.c"
#include "game.c"
#include "validate.c"
#include "dump.c"

#endif /* CJRPGENGINE_H_INCLUDED */
