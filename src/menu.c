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
        }
    }
}
