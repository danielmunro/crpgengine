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

void drawPlayer(Player *player) {
    drawAnimation(
            findAnimation(player->mob->animations, DOWN),
            (Vector2) {20, 20}
    );
}

void drawAllMenus(Player *player, Menu *menu[MAX_MENUS], int menuCount) {
    for (int i = 0; i < menuCount; i++) {
        if (menu[i]->type == MENU_IN_GAME_MAIN) {
            drawInGameMainMenuScreen(menu[i]->cursor, player);
        }
    }
}
