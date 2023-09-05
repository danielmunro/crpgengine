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

#include "header/animation.h"
#include "header/control.h"
#include "header/exit.h"
#include "header/intent.h"
#include "header/item.h"
#include "header/layer.h"
#include "header/menu.h"
#include "header/spell.h"
#include "header/ui.h"

#define MOB_COLLISION_WIDTH 12
#define MOB_COLLISION_WIDTH_OFFSET 2
#define MOB_COLLISION_HEIGHT 12
#define MOB_COLLISION_HEIGHT_OFFSET 12
#define MOB_HEIGHT 24

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

#define START_ENTRANCE "start"

#define STARTING_HP 20
#define STARTING_MANA 10

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
#define MAX_FS_PATH_LENGTH 255
#define MAX_MOBILE_MOVEMENTS 64
#define MAX_SPRITES 1024
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

#include "log.c"
#include "util.c"
#include "runtime_args.c"
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
#include "yaml/ui.c"
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
#include "global_setup.c"

#endif /* CJRPGENGINE_H_INCLUDED */
