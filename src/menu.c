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
            (Vector2){20, 20}
            );
}

void drawInGameMainMenuScreen(Menu *menu, Player *player) {
    int column1 = (UI_PADDING * 2) + MOB_COLLISION_WIDTH;
    int column2 = SCREEN_WIDTH - 200;
    drawText(player->mob->name, (Vector2d){column1, UI_PADDING});
    char hp[64];
    sprintf(hp, "hp %d/%d", 20, 20);
    drawText(hp, (Vector2d){column1, UI_PADDING + line(1)});
    char mp[64];
    sprintf(mp, "mp %d/%d", 20, 20);
    drawText(mp, (Vector2d){column1, UI_PADDING + line(2)});

    int count = sizeof(MainInGameMenuItem) + 1;
    for (int i = 0; i < count; i++) {
        drawText(MainInGameMenuItems[i], (Vector2d){column2, UI_PADDING + line(i)});
    }
    drawText(">", (Vector2d){column2 - 20, UI_PADDING + line(menu->cursor)});
}

void drawAllMenus(Player *player, Menu *menu[MAX_MENUS], int menuCount) {
    for (int i = 0; i < menuCount; i++) {
        if (menu[i]->type == MENU_IN_GAME_MAIN) {
            drawInGameMainMenuScreen(menu[i], player);
        }
    }
}
