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

#define MAX_OBJECTS 255
#define MAX_SCENES    256
#define MAX_LAYER_SIZE 1024
#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440
#define MAX_EXITS 64
#define MAX_MOBILES 255
#define MAX_INSTRUCTIONS 255
#define MAX_CONTROLS 255
#define MAX_AUDIO 1024
#define MAX_CHARACTERS_PER_LINE 60
#define MAX_STORIES 1024
#define MAX_BEASTIARY_SIZE 1024
#define MAX_BEASTS 255
#define MAX_BEASTS_IN_FIGHT 9

#define LAYER_COUNT 3

typedef enum {
    BACKGROUND = 0,
    MIDGROUND,
    FOREGROUND,
} LayerType;

#define TARGET_FRAMERATE 60
#define MOB_COLLISION_WIDTH 16
#define MOB_COLLISION_HEIGHT 12
#define MOB_COLLISION_HEIGHT_OFFSET 12

#define SCENE_TYPE_TOWN 1
#define SCENE_TYPE_DUNGEON 2

typedef enum {
    UP = 1,
    DOWN,
    LEFT,
    RIGHT
} AnimationDirection;

#define CONTROL_TYPE_NONE 0
#define CONTROL_TYPE_WHEN 1
#define CONTROL_TYPE_THEN 2

#define WHEN_ENGAGED "engaged"
#define WHEN_HAS "has"
#define WHEN_NOT_HAS "not_has"
#define WHEN_AT "at"
#define WHEN_HAS_STORY "has_story"
#define WHEN_NOT_HAS_STORY "not_has_story"

#define CONDITION_ENGAGED 1
#define CONDITION_HAS 2
#define CONDITION_NOT_HAS 3
#define CONDITION_AT 4
#define CONDITION_HAS_STORY 5
#define CONDITION_NOT_HAS_STORY 6

#define THEN_SPEAK "speak"
#define THEN_MOVE_TO "move_to"
#define THEN_DIRECTION "direction"
#define THEN_SPRITE "sprite"
#define THEN_WAIT "wait"
#define THEN_GIVE_ITEM "give_item"
#define THEN_TAKE "take"
#define THEN_ADD_STORY "add_story"

#define OUTCOME_SPEAK 1
#define OUTCOME_MOVE_TO 2
#define OUTCOME_DIRECTION 3
#define OUTCOME_SPRITE 4
#define OUTCOME_WAIT 5
#define OUTCOME_GIVE_ITEM 6
#define OUTCOME_TAKE 7
#define OUTCOME_ADD_STORY 8

#define FONT_SIZE 20
#define LINE_HEIGHT 30

typedef struct Mobile Mobile;

#include "util.c"
#include "yaml/animation.c"
#include "yaml/config.c"
#include "yaml/music.c"
#include "yaml/attributes.c"
#include "yaml/player.c"
#include "yaml/scene.c"
#include "yaml/sound.c"
#include "yaml/beastiary.c"
#include "yaml/mobile.c"
#include "cyaml.c"
#include "window.c"
#include "ui.c"
#include "audio.c"
#include "control_block.c"
#include "sprite.c"
#include "attributes.c"
#include "item.c"
#include "beastiary.c"
#include "mobile.c"
#include "player.c"
#include "fight.c"
#include "tilemap.c"
#include "scene.c"
#include "runtime_args.c"
#include "db.c"
#include "game.c"
#include "validate.c"
#include "dump.c"

#endif /* CJRPGENGINE_H_INCLUDED */
