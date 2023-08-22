typedef struct {
    const char *scene;
    Player *player;
    SaveFiles *saveFiles;
    const char *indexDir;
    int cursorLine;
    FontStyle *defaultFont;
    FontStyle **fonts;
    Fight *fight;
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
    MenuSelectResponse *(*selected)(MenuContext *menuContext);
    MenuContext *context;
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
    menu->context = NULL;
    return menu;
}

MenuContext *createMenuContext(
        Fight *fight,
        Player *player,
        FontStyle **fonts,
        const char *scene,
        const char *indexDir,
        int cursorLine) {
    MenuContext *context = malloc(sizeof(MenuContext));
    context->fight = fight;
    context->player = player;
    context->scene = scene;
    context->indexDir = indexDir;
    context->cursorLine = cursorLine;
    context->fonts = fonts;
    return context;
}

void normalizeMenuCursor(Menu *menu, MenuContext *menuContext) {
    if (menu->cursor >= menu->getCursorLength(menuContext)) {
        menu->cursor = 0;
    }

    if (menu->cursor < 0) {
        menu->cursor = menu->getCursorLength(menuContext) - 1;
    }
}
