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
#include "direction.h"
#include "util.h"
#include "cyaml.h"
#include "db.h"
#include "exploration.h"
#include "fight.h"
#include "intent.h"
#include "layer.h"
#include "menu.h"
#include "scene.h"
#include "xmlparser.h"
#include "warp.h"
#include "audio.h"
#include "attributes.h"
#include "spell.h"
#include "beastiary.h"
#include "item.h"
#include "ui.h"
#include "animation.h"
#include "mobile.h"
#include "player.h"
#include "control.h"
#include "notification.h"
#include "timing.h"

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
