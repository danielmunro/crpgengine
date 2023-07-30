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

void normalizeMenuCursor(Menu *menu, Player *player) {
    if (menu->cursor >= menu->getCursorLength(player)) {
        menu->cursor = 0;
    }

    if (menu->cursor < 0) {
        menu->cursor = menu->getCursorLength(player) - 1;
    }
}
