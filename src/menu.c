typedef struct {
    MenuSelectResponseType type;
    MenuType menuType;
} MenuSelectResponse;

typedef struct {
    MenuType type;
    int cursor;

    int (*getCursorLength)(Player *p);

    void (*draw)(Player *p, int cursor);

    MenuSelectResponse *(*selected)(MenuType menuType);
} Menu;

MenuSelectResponse *createMenuSelectResponse(MenuSelectResponseType type, MenuType menuType) {
    MenuSelectResponse *response = malloc(sizeof(MenuSelectResponse));
    response->type = type;
    response->menuType = menuType;
    return response;
}

Menu *createMenu(
        MenuType type,
        int (getCursorLength)(Player *),
        void (draw)(Player *, int),
        MenuSelectResponse *(*selected)()) {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    menu->type = type;
    menu->getCursorLength = getCursorLength;
    menu->draw = draw;
    menu->selected = selected;
    return menu;
}

Menu *findMenu(Menu *menus[MAX_MENUS], int menuCount, MenuType type) {
    for (int i = 0; i < menuCount; i++) {
        if (menus[i]->type == type) {
            return menus[i];
        }
    }
    return NULL;
}

void drawAllMenus(Player *player, Menu *menus[MAX_MENUS], int menuCount) {
    BeginDrawing();
    for (int i = 0; i < menuCount; i++) {
        menus[i]->draw(player, menus[i]->cursor);
    }
    EndDrawing();
}

int getPartyMenuCursorLength(Player *player) {
    return sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]) - 1;
}

int getItemsCursorLength(Player *player) {
    return player->itemCount - 1;
}

int getQuitCursorLength(Player *player) {
    return 2;
}

int getCursorLengthForMenu(Menu *menu, Player *player) {
    if (menu->type == PARTY_MENU) {
        return sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]) - 1;
    } else if (menu->type == ITEMS_MENU) {
        return player->itemCount - 1;
    } else if (menu->type == QUIT_MENU) {
        return 2;
    }
    return 0;
}

void normalizeMenuCursor(Menu *menu, Player *player) {
    if (menu->cursor > getCursorLengthForMenu(menu, player)) {
        menu->cursor = 0;
    }

    if (menu->cursor < 0) {
        menu->cursor = getCursorLengthForMenu(menu, player);
    }
}

MenuSelectResponse *partyMenuItemSelected(MenuType menuType) {
    if (strcmp(PartyMenuItems[menuType], PARTY_MENU_ITEMS) == 0) {
        return createMenuSelectResponse(OPEN_MENU, ITEMS_MENU);
    }
    if (strcmp(PartyMenuItems[menuType], PARTY_MENU_QUIT) == 0) {
        return createMenuSelectResponse(OPEN_MENU, QUIT_MENU);
    }
    return NULL;
}

MenuSelectResponse *quitMenuItemSelected(MenuType menuType) {
    if (strcmp(QuitMenuItems[menuType], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
