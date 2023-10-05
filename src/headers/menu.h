#ifndef CJRPGENGINE_MENU_H
#define CJRPGENGINE_MENU_H

#define MAX_MENUS 255
#define MAX_TEXT_BOXES 32

#define PARTY_MENU_ITEMS "Items"
#define PARTY_MENU_MAGIC "Magic"
#define PARTY_MENU_REARRANGE "Rearrange"
#define PARTY_MENU_CONFIG "Config"
#define PARTY_MENU_SAVE "Save"
#define PARTY_MENU_LOAD "Load"
#define PARTY_MENU_QUIT "Quit"

char *PartyMenuItems[] = {
        PARTY_MENU_ITEMS,
        PARTY_MENU_MAGIC,
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

typedef enum {
    OPEN_MENU,
    CLOSE_MENU,
    FIND_TARGET_MENU,
    DEFEND_SELECTED,
    RESPONSE_TYPE_RUN,
    PARTY_MEMBER_SELECTED,
    NO_OP,
} MenuSelectResponseType;

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

#endif //CJRPGENGINE_MENU_H
