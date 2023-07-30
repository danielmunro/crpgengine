typedef struct {
    const char *indexDir;
    Player *player;
    int cursorLine;
} MenuContext;

typedef struct {
    MenuSelectResponseType type;
    MenuType menuType;
} MenuSelectResponse;

typedef struct {
    MenuType type;
    int cursor;
    int (*getCursorLength)(MenuContext *);
    void (*draw)(MenuContext *);
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
        int (getCursorLength)(MenuContext *),
        void (draw)(MenuContext *),
        MenuSelectResponse *(*selected)()) {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    menu->type = type;
    menu->getCursorLength = getCursorLength;
    menu->draw = draw;
    menu->selected = selected;
    return menu;
}

MenuContext *createMenuContext(Player *player, const char *indexDir, int cursorLine) {
    MenuContext *context = malloc(sizeof(MenuContext));
    context->player = player;
    context->indexDir = indexDir;
    context->cursorLine = cursorLine;
    return context;
}

Menu *findMenu(Menu *menus[MAX_MENUS], int menuCount, MenuType type) {
    for (int i = 0; i < menuCount; i++) {
        if (menus[i]->type == type) {
            return menus[i];
        }
    }
    return NULL;
}

void drawAllMenus(Player *player, Menu *menus[MAX_MENUS], int menuCount, const char *indexDir) {
    BeginDrawing();
    for (int i = 0; i < menuCount; i++) {
        MenuContext *c = createMenuContext(player, indexDir, menus[i]->cursor);
        menus[i]->draw(c);
    }
    EndDrawing();
}

void normalizeMenuCursor(Menu *menu, MenuContext *menuContext) {
    if (menu->cursor >= menu->getCursorLength(menuContext)) {
        menu->cursor = 0;
    }

    if (menu->cursor < 0) {
        menu->cursor = menu->getCursorLength(menuContext) - 1;
    }
}
