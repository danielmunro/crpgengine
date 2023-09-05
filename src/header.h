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

#include "header/errors.h"
#include "header/animation.h"
#include "header/collision.h"
#include "header/control.h"
#include "header/db.h"
#include "header/exploration.h"
#include "header/fight.h"
#include "header/intent.h"
#include "header/item.h"
#include "header/layer.h"
#include "header/log.h"
#include "header/menu.h"
#include "header/notification.h"
#include "header/player.h"
#include "header/scene.h"
#include "header/spell.h"
#include "header/ui.h"

#define MAX_AUDIO 1024
#define MAX_SPRITES 1024

#include "log.c"
#include "util.c"
#include "runtime_args.c"
#include "yaml/spritesheet.c"
#include "yaml/animation.c"
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
