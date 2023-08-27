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
#define MOB_HEIGHT 24

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

#define START_ENTRANCE "start"

#define FONT_SIZE 20
#define LINE_HEIGHT 30
#define UI_PADDING 20
#define HIGHLIGHT_COLOR YELLOW
#define DISABLED_COLOR GRAY
#define DEFAULT_COLOR WHITE
#define WARNING_COLOR YELLOW
#define DANGER_COLOR RED
#define BOTTOM_MENU_HEIGHT 200
#define BOTTOM_MENU_PLAYER_WIDTH 560
#define BOTTOM_MENU_ACTION_SELECT_WIDTH 300

#define FIGHT_PLAYER_X (SCREEN_WIDTH * 0.8)
#define FIGHT_PLAYER_Y_MARGIN 40
#define FIGHT_PLAYER_Y_PADDING 24

#define FIGHT_CURSOR_X_OFFSET 5
#define FIGHT_CURSOR_Y_OFFSET 22

#define FONT_WARNING_THRESHOLD 0.6
#define FONT_DANGER_THRESHOLD 0.3

#define HP_X_OFFSET 200
#define MANA_X_OFFSET 320
#define ACTION_GAUGE_WIDTH 100
#define ACTION_GAUGE_HEIGHT 10

#define EXIT_NO_INDEX_DIR 1
#define EXIT_MISSING_SPRITESHEET 2
#define EXIT_MENU_NOT_DEFINED 3
#define EXIT_MOBILE_NOT_FOUND 4
#define EXIT_FORCE_NEW_AND_LOAD_SAVE 5
#define EXIT_FONT_STYLE_NOT_FOUND 6
#define EXIT_TEXT_BOX_NOT_FOUND 7

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
#define MAX_TEXT_BOXES 32
#define MAX_SPELLS 256

#define COLLIDE_TYPE_OBJECTS "objects"
#define COLLIDE_TYPE_PLAYER "player"
#define COLLIDE_TYPE_WARPS "warps"
#define COLLIDE_TYPE_COUNT 3

#define BEAST_AREA (Rectangle) {20, 20, 240, 240}

typedef enum {
    ACKNOWLEDGE_BOX,
    ACTION_SELECT_BOX,
    BEAST_SELECT_BOX,
    ITEMS_BOX,
    LOAD_BOX,
    MOBILE_SELECT_BOX,
    PARTY_BOX,
    QUIT_BOX,
    MAGIC_SELECT_BOX,
} TextBoxLabel;

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
        "has_story",
        "not_has_story",
        "scene_loaded",
        "arrive_at",
};

typedef enum {
    ENGAGED = 0,
    HAS_STORY,
    NOT_HAS_STORY,
    SCENE_LOADED,
    ARRIVE_AT,
} Condition;

const char *outcomes[] = {
        "speak",
        "move_to",
        "set_direction",
        "sprite",
        "wait",
        "give_item",
        "take",
        "add_story",
        "set_position",
        "lock",
        "unlock",
        "save",
};

typedef enum {
    SPEAK = 0,
    MOVE_TO,
    SET_DIRECTION,
    SPRITE,
    WAIT,
    GIVE_ITEM,
    TAKE,
    ADD_STORY,
    SET_POSITION,
    LOCK,
    UNLOCK,
    SAVE,
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
    PARTY_MENU,
    ITEMS_MENU,
    LOAD_MENU,
    SAVE_MENU,
    QUIT_MENU,
    ACKNOWLEDGE_MENU,
    BEAST_LIST_FIGHT_MENU,
    BEAST_TARGET_FIGHT_MENU,
    MOBILE_SELECT_FIGHT_MENU,
    MOBILE_TARGET_FIGHT_MENU,
    ACTION_SELECT_FIGHT_MENU,
    MAGIC_FIGHT_MENU,
    ITEMS_FIGHT_MENU,
} MenuType;

typedef enum {
    RECEIVE_QUEST_ITEM,
    LOSE_QUEST_ITEM,
    RECEIVE_EXPERIENCE,
    RECEIVE_GOLD,
    LOSE_GOLD,
    SAVED,
} NotificationType;

#define PARTY_MENU_ITEMS "Items"
#define PARTY_MENU_REARRANGE "Rearrange"
#define PARTY_MENU_CONFIG "Config"
#define PARTY_MENU_SAVE "Save"
#define PARTY_MENU_LOAD "Load"
#define PARTY_MENU_QUIT "Quit"

char *PartyMenuItems[] = {
        PARTY_MENU_ITEMS,
        PARTY_MENU_REARRANGE,
        PARTY_MENU_CONFIG,
        PARTY_MENU_SAVE,
        PARTY_MENU_LOAD,
        PARTY_MENU_QUIT,
};

#define QUIT_MENU_YES "Yes"
#define QUIT_MENU_NO "No"

char *QuitMenuItems[] = {
        QUIT_MENU_NO,
        QUIT_MENU_YES,
};

const char *ItemTypes[] = {
        "consumable",
        "equipment",
        "crafting material",
        "quest",
};

typedef enum {
    ITEM_TYPE_NONE,
    ITEM_TYPE_CONSUMABLE,
    ITEM_TYPE_EQUIPMENT,
    ITEM_TYPE_CRAFTING_MATERIAL,
    ITEM_TYPE_QUEST,
} ItemType;

const char *EquipmentPositions[] = {
        "none",
        "weapon",
        "torso",
        "wrist",
        "accessory",
};

typedef enum {
    POSITION_NONE,
    POSITION_WEAPON,
    POSITION_TORSO,
    POSITION_WRIST,
    POSITION_ACCESSORY,
} EquipmentPosition;

typedef enum {
    OPEN_MENU,
    CLOSE_MENU,
    TARGET_FOR_ATTACK,
} MenuSelectResponseType;

typedef enum {
    EVENT_GAME_LOOP,
    EVENT_SCENE_LOADED,
} EventType;

const char *Events[MAX_EVENTS] = {
        "game_loop",
        "scene_loaded",
};

#define FONT_STYLE_COUNT 5

typedef enum {
    FONT_STYLE_DEFAULT,
    FONT_STYLE_DISABLED,
    FONT_STYLE_HIGHLIGHTED,
    FONT_STYLE_WARNING,
    FONT_STYLE_DANGER,
} FontStyleType;

const char *Spells[] = {
        "cure",
        "fire",
        "lightning",
        "ice",
        "energy",
};

typedef enum {
    CURE,
    FIRE,
    LIGHTNING,
    ICE,
    ENERGY,
} SpellType;

typedef enum {
    INTENT_HARM,
    INTENT_HELP,
} Intent;

#include "log.c"
#include "util.c"
#include "yaml/spritesheet.c"
#include "yaml/animation.c"
#include "yaml/config.c"
#include "yaml/music.c"
#include "yaml/attributes.c"
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
