typedef struct {
    Spritesheet *sprite;
    UIData *config;
} UISprite;

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
    Item *selectedItem;
    Spell *selectedSpell;
    UISprite *uiSprite;
    Spell **spells;
    ItemList *itemList;
    int itemListCount;
    Beast *targetBeast;
    Mobile *targetMob;
    ActionType actionType;
    int scrollOffset;
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
        UISprite *uiSprite,
        Spell **spells,
        const char *scene,
        int cursorLine) {
    MenuContext *context = malloc(sizeof(MenuContext));
    context->fight = fight;
    context->player = player;
    context->scene = scene;
    context->cursorLine = cursorLine;
    context->fonts = fonts;
    context->uiSprite = uiSprite;
    context->spells = spells;
    context->textBoxes = calloc(MAX_TEXT_BOXES, sizeof(TextBox));
    context->selectedMob = NULL;
    context->itemList = calloc(MAX_ITEMS, sizeof(ItemList));
    context->itemListCount = 0;
    context->targetBeast = NULL;
    context->targetMob = NULL;
    context->actionType = NONE;
    context->scrollOffset = 0;
    return context;
}

void resetItemList(MenuContext *mc) {
    free(mc->itemList);
    mc->itemList = NULL;
    mc->itemListCount = 0;
}

UISprite *createUISprite(Spritesheet *sprite, UIData *config) {
    UISprite *uiSprite = malloc(sizeof(UISprite));
    uiSprite->sprite = sprite;
    uiSprite->config = config;
    return uiSprite;
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
            menus[i]->cursor = 0;
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

Menu *removeMenusTo(Menu **menus, MenuType type) {
    for (int i = 10; i > 0; i--) {
        if (menus[i] != NULL) {
            if (menus[i]->type == type) {
                return menus[i];
            }
            menus[i] = NULL;
        }
    }
    return NULL;
}

TextBox *findOrCreateTextBox(MenuContext *mc, TextBoxLabel label, Rectangle area) {
    for (int i = 0; i < MAX_TEXT_BOXES; i++) {
        if (mc->textBoxes[i] == NULL) {
            TextBox *t = createTextBox(
                    area,
                    mc->fonts->default_,
                    label);
            mc->textBoxes[i] = t;
            return t;
        } else if (mc->textBoxes[i]->label == label) {
            mc->textBoxes[i]->cursor = 0;
            return mc->textBoxes[i];
        }
    }
    fprintf(stderr, "could not find text box :: %d", label);
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
    if (response == NULL) {
        return NULL;
    }
    if (response->type == OPEN_MENU) {
        addMenu(menus, findMenu(allMenus, response->menuType));
    } else if (response->type == CLOSE_MENU) {
        removeMenu(menus);
    } else if (response->type == PARTY_MEMBER_SELECTED) {
        Menu *partyMenu = findMenu(menus, PARTY_MENU);
        if (strcmp(PartyMenuItems[partyMenu->cursor], PARTY_MENU_MAGIC) == 0) {
            addMenu(menus, findMenu(allMenus, MAGIC_MENU));
        }
    }
    return response;
}

void drawUpCursor(UISprite *sprites, Vector2 position) {
    drawImageFromSprite(sprites->sprite, (Vector2) {
            position.x + FIGHT_CURSOR_X_OFFSET,
            position.y + FIGHT_CURSOR_Y_OFFSET,
    }, sprites->config->sprite->cursors->up);
}

void drawRightCursor(UISprite *sprites, Vector2 position) {
    drawImageFromSprite(sprites->sprite, (Vector2) {
            position.x + FIGHT_CURSOR_X_OFFSET,
            position.y + FIGHT_CURSOR_Y_OFFSET,
    }, sprites->config->sprite->cursors->right);
}

void drawDownCursor(UISprite *sprites, Vector2 position) {
    drawImageFromSprite(sprites->sprite, (Vector2) {
            position.x + FIGHT_CURSOR_X_OFFSET,
            position.y + FIGHT_CURSOR_Y_OFFSET,
    }, sprites->config->sprite->cursors->down);
}

void drawAllMenus(MenuContext *mc, Menu **menus) {
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            break;
        }
        mc->cursorLine = menus[i]->cursor;
        menus[i]->draw(mc);
    }
}
