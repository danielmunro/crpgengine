typedef struct {
    int cursor;
} Menu;

Menu *createMenu() {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    return menu;
}

void drawPlayer(Player *player) {
    drawAnimation(
            findAnimation(player->mob->animations, DOWN),
            (Vector2){20, 20}
            );
}

void drawStats(Menu *menu, Player *player) {
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
