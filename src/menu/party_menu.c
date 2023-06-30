void drawPlayer(Player *player) {
    drawAnimation(
            findAnimation(player->mob->animations, DOWN),
            (Vector2) {20, 20}
    );
}

void drawPartyMenuScreen(Player *player, int cursorLine) {
    drawInGameMenuBox();
    drawPlayer(player);
    int column1 = (UI_PADDING * 2) + MOB_COLLISION_WIDTH;
    int column2 = SCREEN_WIDTH - 200;
    drawText(player->mob->name, (Vector2D) {column1, UI_PADDING});
    char hp[64];
    sprintf(hp, "hp %d/%d", 20, 20);
    drawText(hp, (Vector2D) {column1, UI_PADDING + line(1)});
    char mp[64];
    sprintf(mp, "mp %d/%d", 20, 20);
    drawText(mp, (Vector2D) {column1, UI_PADDING + line(2)});
    int count = sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
    TextBox *textBox = createTextBox((RectangleD){
            column2,
            UI_PADDING,
            200,
            SCREEN_HEIGHT - (UI_PADDING * 2)
    });
    for (int i = 0; i < count; i++) {
        drawInTextBox(textBox, PartyMenuItems[i]);
    }
    drawText(">", (Vector2D) {column2 - 20, UI_PADDING + line(cursorLine)});
}
