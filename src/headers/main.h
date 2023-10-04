#ifndef CJRPGENGINE_H_INCLUDED
#define CJRPGENGINE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "_deps/raylib-src/src/raylib.h"
#include <libxml/xmlreader.h>
#include "cyaml/cyaml.h"
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <ctype.h>

#include "globals.h"
#include "errors.h"
#include "log.h"
#include "collision.h"
#include "util.h"
#include "animation.h"
#include "control.h"
#include "db.h"
#include "exploration.h"
#include "fight.h"
#include "intent.h"
#include "item.h"
#include "layer.h"
#include "menu.h"
#include "notification.h"
#include "player.h"
#include "scene.h"
#include "spell.h"
#include "timing.h"
#include "ui.h"
#include "xmlparser.h"

#define MAX_AUDIO 1024
#define MAX_SPRITES 1024

#include "src/persistence/yaml/spritesheet.c"
#include "src/persistence/yaml/animation.c"
#include "src/persistence/yaml/music.c"
#include "src/persistence/yaml/attributes.c"
#include "src/persistence/yaml/spell.c"
#include "src/persistence/yaml/item.c"
#include "src/persistence/yaml/item_reference.c"
#include "src/persistence/yaml/storyline.c"
#include "src/persistence/yaml/scene.c"
#include "src/persistence/yaml/sound.c"
#include "src/persistence/yaml/beastiary.c"
#include "src/persistence/yaml/mobile.c"
#include "src/persistence/yaml/player.c"
#include "src/persistence/yaml/save.c"
#include "src/persistence/yaml/start_party.c"
#include "src/persistence/yaml/ui.c"
#include "src/persistence/cyaml.c"
#include "src/graphics/avatar.c"
#include "src/graphics/font.c"
#include "src/graphics/window.c"
#include "src/graphics/ui/ui.c"
#include "src/notification.c"
#include "src/audio.c"
#include "src/graphics/spritesheet.c"
#include "src/graphics/animation.c"
#include "src/attributes.c"
#include "src/item.c"
#include "src/item_manager.c"
#include "src/warp.c"
#include "src/spell.c"
#include "src/beastiary.c"
#include "src/mobile.c"
#include "src/player.c"
#include "src/control.c"
#include "src/object.c"
#include "src/timing.c"
#include "src/graphics/tilemap.c"
#include "src/action.c"
#include "src/fight.c"
#include "src/graphics/ui/menu.c"
#include "src/graphics/ui/menus/acknowledge_menu.c"
#include "src/graphics/ui/menus/items_menu.c"
#include "src/graphics/ui/menus/load_menu.c"
#include "src/graphics/ui/menus/magic_menu.c"
#include "src/graphics/ui/menus/party_apply_menu.c"
#include "src/graphics/ui/menus/party_menu.c"
#include "src/graphics/ui/menus/party_select_menu.c"
#include "src/graphics/ui/menus/quit_menu.c"
#include "src/graphics/ui/menus/fight/action_select_menu.c"
#include "src/graphics/ui/menus/fight/beast_list_menu.c"
#include "src/graphics/ui/menus/fight/beast_target_menu.c"
#include "src/graphics/ui/menus/fight/item_select_menu.c"
#include "src/graphics/ui/menus/fight/magic_select_menu.c"
#include "src/graphics/ui/menus/fight/mobile_select_menu.c"
#include "src/graphics/ui/menus/fight/mobile_target_menu.c"
#include "src/exploration.c"
#include "src/scene.c"
#include "src/persistence/xmlparser.c"
#include "src/spell_manager.c"
#include "src/mobile_manager.c"
#include "src/player_manager.c"
#include "src/graphics/ui/ui_manager.c"
#include "src/fight_manager.c"
#include "src/control_manager.c"
#include "src/scene_manager.c"
#include "src/graphics/draw_fight.c"
#include "src/persistence/loader.c"
#include "src/game.c"
#include "src/util/validate.c"
#include "src/util/dump.c"
#include "src/util/global_setup.c"

#endif /* CJRPGENGINE_H_INCLUDED */
