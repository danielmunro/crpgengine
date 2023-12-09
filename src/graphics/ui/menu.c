#include "headers/player.h"
#include "headers/fight.h"

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
    OPEN_MENU,
    CLOSE_MENU,
    FIND_TARGET_MENU,
    DEFEND_SELECTED,
    RESPONSE_TYPE_RUN,
    PARTY_MEMBER_SELECTED,
    NO_OP,
} MenuSelectResponseType;

typedef enum {
    NOTHING_TO_DO,
} MenuKeyPressedResponseType;

typedef struct {
    Spritesheet *sprite;
    UIData *config;
} UISprite;

typedef struct {
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
    int quantity;
    int scrollOffset;
} MenuContext;

typedef struct {
    MenuSelectResponseType type;
    MenuType menuType;
} MenuSelectResponse;

typedef struct {
    MenuKeyPressedResponseType type;
} MenuKeyPressedResponse;

typedef struct {
    MenuType type;
    int cursor;

    int (*getCursorLength)(const MenuContext *);

    void (*draw)(MenuContext *);

    int (*getNextOption)(const MenuContext *);

    int (*getPreviousOption)(const MenuContext *);

    MenuKeyPressedResponse *(*keyPressed)(MenuContext *);

    MenuSelectResponse *(*selected)(MenuContext *menuContext);
} Menu;

MenuSelectResponse *createMenuSelectResponse(MenuSelectResponseType type, MenuType menuType) {
    MenuSelectResponse *response = malloc(sizeof(MenuSelectResponse));
    response->type = type;
    response->menuType = menuType;
    return response;
}

MenuKeyPressedResponse *createMenuKeyPressedResponse(MenuKeyPressedResponseType type) {
    MenuKeyPressedResponse *response = malloc(sizeof(MenuKeyPressedResponse ));
    response->type = type;
    return response;
}

Menu *createMenu(
        MenuType type,
        int (getCursorLength)(const MenuContext *),
        void (*draw)(MenuContext *),
        int (*getPreviousOption)(const MenuContext *),
        int (*getNextOption)(const MenuContext *),
        MenuKeyPressedResponse *(*keyPressed)(MenuContext *),
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

void normalizeMenuCursor(Menu *menu, const MenuContext *menuContext) {
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

int getDefaultNextOption(const MenuContext *mc) {
    return mc->cursorLine + 1;
}

int getDefaultPreviousOption(const MenuContext *mc) {
    return mc->cursorLine - 1;
}

MenuKeyPressedResponse *menuKeyPressed(MenuContext *mc) {
    return createMenuKeyPressedResponse(NOTHING_TO_DO);
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
        removeMenu(menus, response->menuType);
    } else if (response->type == PARTY_MEMBER_SELECTED) {
        Menu *partyMenu = findMenu(menus, PARTY_MENU);
        if (strcmp(PartyMenuItems[partyMenu->cursor], PARTY_MENU_MAGIC) == 0) {
            addMenu(menus, findMenu(allMenus, MAGIC_MENU));
        }
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
