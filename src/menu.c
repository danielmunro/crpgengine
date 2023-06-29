typedef struct {
    int type;
    int cursor;
    int (*getCursorLength)(Player *p);
    void (*draw)(Player *p, int cursor);
} Menu;

Menu *createMenu(MenuType type, int (getCursorLength)(Player *), void (draw)(Player *, int)) {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    menu->type = type;
    menu->getCursorLength = getCursorLength;
    menu->draw = draw;
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
        return min(menu->cursor + 1, sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]) - 1);
    } else if (menu->type == ITEMS_MENU) {
        return player->itemCount - 1;
    } else if (menu->type == QUIT_MENU) {
        return 2;
    }
}
