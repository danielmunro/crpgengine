#ifndef CJRPGENGINE_MENU_H
#define CJRPGENGINE_MENU_H

#define MAX_MENUS 255

const char *MenuStyles[] = {
        "verticalGradient",
};

typedef enum {
    VERTICAL_GRADIENT,
} MenuStyle;

MenuStyle getMenuStyleFromString(const char *style) {
    int count = sizeof(MenuStyles) / sizeof(MenuStyles[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(style, MenuStyles[i]) == 0) {
            return i;
        }
    }
    fprintf(stderr, "cannot get menu style");
    exit(EXIT_MENU_STYLE_NOT_DEFINED);
}

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

typedef enum {
    OPEN_MENU,
    CLOSE_MENU,
    FIND_TARGET_MENU,
} MenuSelectResponseType;

#endif //CJRPGENGINE_MENU_H
