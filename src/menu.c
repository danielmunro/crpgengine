typedef struct {
    const char *scene;
    Player *player;
    SaveFiles *saveFiles;
    const char *indexDir;
    int cursorLine;
    Fonts *fonts;
    Fight *fight;
    TextBox **textBoxes;
    Mobile *selectedMob;
    Spritesheet *menuSprite;
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
    int (*getNextOption)(MenuContext *);
    int (*getPreviousOption)(MenuContext *);
    MenuSelectResponse *(*selected)(MenuContext *menuContext);
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
        int (*getPreviousOption)(MenuContext *),
        int (*getNextOption)(MenuContext *),
        MenuSelectResponse *(*selected)()) {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    menu->type = type;
    menu->getCursorLength = getCursorLength;
    menu->draw = draw;
    menu->getPreviousOption = getPreviousOption;
    menu->getNextOption = getNextOption;
    menu->selected = selected;
    return menu;
}

MenuContext *createMenuContext(
        Fight *fight,
        Player *player,
        Fonts *fonts,
        Spritesheet *menuSprite,
        const char *scene,
        int cursorLine) {
    MenuContext *context = malloc(sizeof(MenuContext));
    context->fight = fight;
    context->player = player;
    context->scene = scene;
    context->cursorLine = cursorLine;
    context->fonts = fonts;
    context->menuSprite = menuSprite;
    context->textBoxes = calloc(MAX_TEXT_BOXES, sizeof(TextBox));
    context->selectedMob = NULL;
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

int addMenu(Menu **menus, Menu *m) {
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            menus[i] = m;
            return i;
        }
    }
    fprintf(stderr, "could not add menu");
    return MAX_MENUS;
}

Menu *findMenu(Menu **menus, MenuType type) {
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            return NULL;
        } else if (menus[i]->type == type) {
            return menus[i];
        }
    }
    return NULL;
}

Menu *getCurrentMenu(Menu **menus) {
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            return i > 0 ? menus[i - 1] : NULL;
        }
    }
    return menus[MAX_MENUS - 1];
}

Menu *getPreviousMenu(Menu **menus) {
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            return i > 1 ? menus[i - 2] : NULL;
        }
    }
    return menus[MAX_MENUS - 2];
}

int removeMenu(Menu **menus) {
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            if (i > 0) {
                menus[i - 1] = NULL;
            }
            return i - 1;
        }
    }
    return -1;
}

TextBox *findOrCreateTextBox(MenuContext *mc, TextBoxLabel label, TextBox *(createTextBox)(MenuContext *)) {
    for (int i = 0; i < MAX_TEXT_BOXES; i++) {
        if (mc->textBoxes[i] == NULL) {
            TextBox *t = createTextBox(mc);
            mc->textBoxes[i] = t;
            return t;
        }
        if (mc->textBoxes[i]->label == label) {
            mc->textBoxes[i]->cursor = 0;
            return mc->textBoxes[i];
        }
    }
    fprintf(stderr, "could not find text box");
    exit(EXIT_TEXT_BOX_NOT_FOUND);
}

int getDefaultNextOption(MenuContext *mc) {
    return mc->cursorLine + 1;
}

int getDefaultPreviousOption(MenuContext *mc) {
    return mc->cursorLine - 1;
}

MenuSelectResponse *menuItemSelected(Menu **menus, Menu **allMenus, MenuContext *menuContext) {
    Menu *menu = getCurrentMenu(menus);
    MenuSelectResponse *response = menu->selected(menuContext);
    if (response->type == OPEN_MENU) {
        addMenu(menus, findMenu(allMenus, response->menuType));
    } else if (response->type == CLOSE_MENU) {
        removeMenu(menus);
    }
    return response;
}
