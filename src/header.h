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

#define TARGET_FRAMERATE 60

#define MOB_COLLISION_WIDTH 12
#define MOB_COLLISION_WIDTH_OFFSET 2
#define MOB_COLLISION_HEIGHT 12
#define MOB_COLLISION_HEIGHT_OFFSET 12
#define MOB_HEIGHT 24

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

#define START_ENTRANCE "start"

#define EXIT_NO_INDEX_DIR 1
#define EXIT_MENU_NOT_DEFINED 100
#define EXIT_MOBILE_NOT_FOUND 200
#define EXIT_FORCE_NEW_AND_LOAD_SAVE 300
#define EXIT_FONT_STYLE_NOT_FOUND 400
#define EXIT_TEXT_BOX_NOT_FOUND 401
#define EXIT_UNABLE_TO_CREATE_SPELL 500
#define EXIT_UNABLE_TO_FIND_SPELL 501
#define EXIT_UNKNOWN_INTENT 600

#define SPRITESHEET_NAME_UI "uipack"

#define CURSOR_RIGHT_INDEX 801
#define CURSOR_DOWN_INDEX 769

#define NOTIFICATION_DECAY_SECONDS 5
#define NOTIFICATION_HEIGHT 75

#define STARTING_HP 20
#define STARTING_MANA 10

#define LIGHTBLUE (Color){100, 134, 175, 255}

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
#define MAX_EQUIPMENT 4
#define MAX_PARTY_SIZE 4
#define MAX_TEAM_SIZE 255
#define MAX_FS_PATH_LENGTH 255
#define MAX_MOBILE_MOVEMENTS 64
#define MAX_SPRITES 1024
#define MAX_EVENTS 64
#define MAX_ARRIVE_AT 64
#define MAX_SAVE_FILES 255
#define MAX_SAVE_NAME 255
#define MAX_DATETIME_LENGTH 255
#define MAX_NOTIFICATIONS 64
#define MAX_LINE_BUFFER 128
#define MAX_MESSAGE_BUFFER 1024
#define MAX_BEAST_NAMES_IN_FIGHT 6
#define MAX_CURSORS 4
#define MAX_ACTION_GAUGE 300
#define MAX_ACTION_GAUGE_RAISE 10
#define MAX_TEXT_BOXES 32
#define MAX_SPELLS 256

#define COLLIDE_TYPE_OBJECTS "objects"
#define COLLIDE_TYPE_PLAYER "player"
#define COLLIDE_TYPE_WARPS "warps"
#define COLLIDE_TYPE_COUNT 3

#define BEAST_AREA (Rectangle) {20, 20, 240, 240}

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
    RECEIVE_QUEST_ITEM,
    LOSE_QUEST_ITEM,
    RECEIVE_EXPERIENCE,
    RECEIVE_GOLD,
    LOSE_GOLD,
    SAVED,
} NotificationType;

typedef enum {
    EVENT_GAME_LOOP,
    EVENT_SCENE_LOADED,
} EventType;

const char *Events[MAX_EVENTS] = {
        "game_loop",
        "scene_loaded",
};

const char *Spells[] = {
        "cure",
        "fire",
        "lightning",
        "ice",
        "energy",
};

typedef enum {
    SPELL_CURE,
    SPELL_FIRE,
    SPELL_LIGHTNING,
    SPELL_ICE,
    SPELL_ENERGY,
} SpellType;

typedef enum {
    INTENT_HARM,
    INTENT_HELP,
} Intent;

#include "header/control.h"
#include "header/item.h"
#include "header/menu.h"
#include "header/ui.h"
#include "log.c"
#include "util.c"
#include "yaml/spritesheet.c"
#include "yaml/animation.c"
#include "yaml/config.c"
#include "yaml/music.c"
#include "yaml/attributes.c"
#include "yaml/spell.c"
#include "yaml/storyline.c"
#include "yaml/item.c"
#include "yaml/player_item.c"
#include "yaml/scene.c"
#include "yaml/sound.c"
#include "yaml/beastiary.c"
#include "yaml/mobile.c"
#include "yaml/player.c"
#include "yaml/save.c"
#include "runtime_args.c"
#include "cyaml.c"
#include "font.c"
#include "window.c"
#include "ui.c"
#include "notification.c"
#include "audio.c"
#include "spritesheet.c"
#include "animation.c"
#include "attributes.c"
#include "intent.c"
#include "item.c"
#include "item_manager.c"
#include "beastiary.c"
#include "warp.c"
#include "spell.c"
#include "mobile.c"
#include "player.c"
#include "control.c"
#include "object.c"
#include "timing.c"
#include "tilemap.c"
#include "fight.c"
#include "menu.c"
#include "menu/items_menu.c"
#include "menu/party_menu.c"
#include "menu/quit_menu.c"
#include "menu/load_menu.c"
#include "menu/acknowledge_menu.c"
#include "menu/action_select_menu.c"
#include "menu/beast_list_menu.c"
#include "menu/beast_target_menu.c"
#include "menu/mobile_select_menu.c"
#include "menu/mobile_target_menu.c"
#include "menu/magic_fight_menu.c"
#include "draw.c"
#include "exploration.c"
#include "scene.c"
#include "tilemap_xmlreader.c"
#include "xmlparser.c"
#include "spell_manager.c"
#include "ui_manager.c"
#include "fight_manager.c"
#include "control_manager.c"
#include "scene_manager.c"
#include "draw_fight.c"
#include "db.c"
#include "game.c"
#include "validate.c"
#include "dump.c"

#endif /* CJRPGENGINE_H_INCLUDED */
