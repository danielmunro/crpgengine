typedef struct {
    int type;
    int cursor;
} Menu;

Menu *createMenu(int type) {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    menu->type = type;
    return menu;
}

void drawAllMenus(Player *player, Menu *menus[MAX_MENUS], int menuCount) {
    for (int i = 0; i < menuCount; i++) {
        if (menus[i]->type == PARTY_MENU) {
            drawPartyMenuScreen(menus[i]->cursor, player);
        } else if (menus[i]->type == ITEMS_MENU) {
            drawItemsMenuScreen(menus[i]->cursor, player);
        } else if (menus[i]->type == QUIT_MENU) {
            drawQuitMenuScreen(menus[i]->cursor);
        }
    }
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
