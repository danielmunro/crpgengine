typedef struct {
    const char *scene;
    Player *player;
    SaveFiles *saveFiles;
    const char *indexDir;
    int cursorLine;
    Font font;
    FontStyle *fontStyle;
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
    Font font;
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

MenuContext *createMenuContext(
        Player *player,
        const char *scene,
        const char *indexDir,
        FontStyle *fontStyle,
        int cursorLine) {
    MenuContext *context = malloc(sizeof(MenuContext));
    context->player = player;
    context->scene = scene;
    context->indexDir = indexDir;
    context->cursorLine = cursorLine;
    context->fontStyle = fontStyle;
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
