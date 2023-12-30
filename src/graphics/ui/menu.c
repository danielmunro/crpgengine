#include "headers/player.h"
#include "headers/fight.h"
#include "headers/user_config.h"

const char *PartyMenuItems[] = {
        PARTY_MENU_ITEMS,
        PARTY_MENU_MAGIC,
        PARTY_MENU_REARRANGE,
        PARTY_MENU_CONFIG,
        PARTY_MENU_SAVE,
        PARTY_MENU_LOAD,
        PARTY_MENU_QUIT,
};

const char *QuitMenuItems[] = {
        QUIT_MENU_NO,
        QUIT_MENU_YES,
};

typedef enum {
    RESPONSE_TYPE_OPEN_MENU,
    RESPONSE_TYPE_CLOSE_MENU,
    RESPONSE_TYPE_FIND_TARGET_MENU,
    RESPONSE_TYPE_DEFEND_SELECTED,
    RESPONSE_TYPE_RUN,
    RESPONSE_TYPE_PARTY_MEMBER_SELECTED,
    RESPONSE_TYPE_NONE,
    RESPONSE_TYPE_EXIT,
    RESPONSE_TYPE_NEW_GAME,
    RESPONSE_TYPE_CONTINUE_GAME,
    RESPONSE_TYPE_SAVE_GAME,
    RESPONSE_TYPE_LOAD_GAME,
} MenuSelectResponseType;

typedef enum {
    KEY_PRESSED_CLOSE_MENU,
    KEY_PRESSED_DECREMENT_CURSOR,
    KEY_PRESSED_INCREMENT_CURSOR,
    KEY_PRESSED_EXECUTE,
    KEY_PRESSED_DECREMENT_QUANTITY,
    KEY_PRESSED_INCREMENT_QUANTITY,
    KEY_PRESSED_NOTHING_TO_DO,
} MenuKeyPressedType;

typedef struct {
    Spritesheet *sprite;
    UIData *config;
} UISprite;

typedef struct {
    MenuType menuType;
    const char *scene;
    Player *player;
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
    Shop *shop;
    ItemWithMarkup *itemToBuy;
    SaveFiles *saveFiles;
    UserConfig *userConfig;
    int quantity;
    int scrollOffset;
} MenuContext;

typedef struct {
    MenuSelectResponseType type;
    MenuType menuType;
} MenuSelectResponse;

typedef struct {
    MenuType type;
    int cursor;

    int (*getCursorLength)(const MenuContext *);

    void (*draw)(MenuContext *);

    int (*getNextOption)(const MenuContext *, const int maxCursorLine);

    int (*getPreviousOption)(const MenuContext *, const int maxCursorLine);

    MenuKeyPressedType (*keyPressed)(const MenuContext *);

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
        int (getCursorLength)(const MenuContext *),
        void (*draw)(MenuContext *),
        int (*getPreviousOption)(const MenuContext *, const int maxCursorLine),
        int (*getNextOption)(const MenuContext *, const int maxCursorLine),
        MenuKeyPressedType (*keyPressed)(const MenuContext *),
        MenuSelectResponse *(*selected)()) {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    menu->type = type;
    menu->getCursorLength = getCursorLength;
    menu->draw = draw;
    menu->getPreviousOption = getPreviousOption;
    menu->getNextOption = getNextOption;
    menu->keyPressed = keyPressed;
    menu->selected = selected;
    return menu;
}

MenuContext *createMenuContext(
        Player *player,
        Fonts *fonts,
        UISprite *uiSprite,
        Spell **spells,
        SaveFiles *saveFiles,
        UserConfig *userConfig,
        const char *scene,
        int cursorLine) {
    MenuContext *context = malloc(sizeof(MenuContext));
    context->fight = NULL;
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
    context->shop = NULL;
    context->itemToBuy = NULL;
    context->saveFiles = saveFiles;
    context->userConfig = userConfig;
    context->quantity = 1;
    context->scrollOffset = 0;
    return context;
}

void resetMenuContext(MenuContext *mc) {
    mc->quantity = 1;
}

void resetItemList(MenuContext *mc) {
    free(mc->itemList);
    mc->itemList = NULL;
    mc->itemListCount = 0;
}

UISprite *createUISprite(Spritesheet *sprite, UIData *uiData) {
    UISprite *uiSprite = malloc(sizeof(UISprite));
    uiSprite->sprite = sprite;
    uiSprite->config = uiData;
    return uiSprite;
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

int removeMenu(Menu **menus, MenuType menuType) {
    bool startRemoving = false;
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            return i - 1;
        }
        if (menus[i]->type == menuType) {
            startRemoving = true;
        }
        if (startRemoving) {
            menus[i] = NULL;
        }
    }
    return -1;
}

int removeLastMenu(Menu **menus) {
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
    addError("could not find text box :: %d", label);
    exit(GameEngineErrorTextBoxNotFound);
}

void clearTextBoxes(TextBox **textBoxes) {
    for (int i = 0; i < MAX_TEXT_BOXES; i++) {
        textBoxes[i] = NULL;
    }
}

int getDefaultNextOption(const MenuContext *mc, const int maxCursorLine) {
    if (mc->cursorLine + 1 == maxCursorLine) {
        return 0;
    }
    return mc->cursorLine + 1;
}

int getDefaultPreviousOption(const MenuContext *mc, const int maxCursorLine) {
    if (mc->cursorLine == 0) {
        return maxCursorLine - 1;
    }
    return mc->cursorLine - 1;
}

MenuKeyPressedType menuKeyPressed(const MenuContext *mc) {
    if (IsKeyPressed(KEY_ESCAPE) && mc->menuType != MAIN_MENU) {
        return KEY_PRESSED_CLOSE_MENU;
    } else if (IsKeyPressed(KEY_DOWN)) {
        return KEY_PRESSED_INCREMENT_CURSOR;
    } else if (IsKeyPressed(KEY_UP)) {
        return KEY_PRESSED_DECREMENT_CURSOR;
    } else if (IsKeyPressed(KEY_SPACE)) {
        return KEY_PRESSED_EXECUTE;
    }
    return KEY_PRESSED_NOTHING_TO_DO;
}

MenuSelectResponse *menuItemSelected(Menu **menus, Menu **allMenus, MenuContext *menuContext) {
    Menu *menu = getCurrentMenu(menus);
    MenuSelectResponse *response = menu->selected(menuContext);
    if (response == NULL) {
        return NULL;
    }
    if (response->type == RESPONSE_TYPE_OPEN_MENU) {
        addMenu(menus, findMenu(allMenus, response->menuType));
    } else if (response->type == RESPONSE_TYPE_CLOSE_MENU) {
        removeMenu(menus, response->menuType);
    } else if (response->type == RESPONSE_TYPE_PARTY_MEMBER_SELECTED) {
        Menu *partyMenu = findMenu(menus, PARTY_MENU);
        if (strcmp(PartyMenuItems[partyMenu->cursor], PARTY_MENU_MAGIC) == 0) {
            addMenu(menus, findMenu(allMenus, MAGIC_MENU));
        }
    } else if (response->type == RESPONSE_TYPE_EXIT) {
        addDebug("player exiting game");
        exit(0);
    }
    return response;
}

void drawRightCursor(const UISprite *sprites, Vector2 position) {
    drawImageFromSprite(
            sprites->sprite,
            (Vector2) {
                position.x - 8,
                position.y + 14,
            },
            sprites->config->sprite->cursors->right);
}

void drawDownCursor(const UISprite *sprites, Vector2 position) {
    drawImageFromSprite(
            sprites->sprite,
            position,
            sprites->config->sprite->cursors->down);
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
