void drawPartyMenuScreen(int cursorLine, Player *player) {
    int column1 = (UI_PADDING * 2) + MOB_COLLISION_WIDTH;
    int column2 = SCREEN_WIDTH - 200;
    drawText(player->mob->name, (Vector2d) {column1, UI_PADDING});
    char hp[64];
    sprintf(hp, "hp %d/%d", 20, 20);
    drawText(hp, (Vector2d) {column1, UI_PADDING + line(1)});
    char mp[64];
    sprintf(mp, "mp %d/%d", 20, 20);
    drawText(mp, (Vector2d) {column1, UI_PADDING + line(2)});

    int count = sizeof(PartyMenu) + 1;
    for (int i = 0; i < count; i++) {
        drawText(MainInGameMenuItems[i], (Vector2d) {column2, UI_PADDING + line(i)});
    }
    drawText(">", (Vector2d) {column2 - 20, UI_PADDING + line(cursorLine)});
}