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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define MAX_OBJECTS 255
#define MAX_SCENES    256
#define MAX_LAYER_SIZE 1024
#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440
#define MAX_EXITS 64
#define MAX_MOBILES 255
#define MAX_INSTRUCTIONS 255
#define MAX_CONTROLS 255
#define MAX_ITEMS 255
#define MAX_AUDIO 1024
#define MAX_CHARACTERS_PER_LINE 72
#define MAX_STORIES 1024
#define MAX_STORY_LENGTH 255

#define LAYER_COUNT 3
#define LAYER_TYPE_BACKGROUND 0
#define LAYER_TYPE_MIDGROUND 1
#define LAYER_TYPE_FOREGROUND 2
#define TARGET_FRAMERATE 60
#define MOB_COLLISION_WIDTH 16
#define MOB_COLLISION_HEIGHT 12
#define MOB_COLLISION_HEIGHT_OFFSET 12

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

#define CONTROL_TYPE_NONE 0
#define CONTROL_TYPE_WHEN 1
#define CONTROL_TYPE_THEN 2

#define CONTROL_ID "id"
#define CONTROL_WHEN "when"
#define CONTROL_THEN "then"
#define CONTROL_TITLE "title"
#define CONTROL_END "end"
#define CONTROL_ANIMATIONS "animations"
#define CONTROL_NAME "name"
#define CONTROL_COORDINATES "coordinates"
#define CONTROL_DIRECTION "direction"
#define CONTROL_HAS_STORY "has_story"
#define CONTROL_NOT_HAS_STORY "not_has_story"

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

#define SCENE_ATTRIBUTE_TYPE "type"

#define FONT_SIZE 20
#define LINE_HEIGHT 30

typedef struct Mobile Mobile;

#include "util.c"
#include "yaml/animation.c"
#include "yaml/config.c"
#include "yaml/music.c"
#include "yaml/player.c"
#include "yaml/scene.c"
#include "yaml/sound.c"
#include "yaml/mobile.c"
#include "cyaml.c"
#include "ui.c"
#include "audio.c"
#include "control_block.c"
#include "sprite.c"
#include "attributes.c"
#include "item.c"
#include "mobile.c"
#include "player.c"
#include "tilemap.c"
#include "scene.c"
#include "runtime_args.c"
#include "parser.c"
#include "db.c"
#include "game.c"
#include "validate.c"
#include "dump.c"

#endif /* CJRPGENGINE_H_INCLUDED */
