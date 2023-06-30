void drawItemsMenuScreen(Player *player, int cursorLine) {
    drawInGameMenuBox();
    for (int i = 0; i < player->itemCount; i++) {
        drawText(
                player->items[i]->name,
                (Vector2D) {UI_PADDING + 20, UI_PADDING + line(i)}
                );
    }
    drawText(">", (Vector2D) {UI_PADDING, UI_PADDING + line(cursorLine)});
}
