void drawItemsMenuScreen(Player *player, int cursorLine) {
    Rectangle rect = drawInGameMenuBox();
    rect.x += UI_PADDING;
    rect.y += UI_PADDING;
    TextBox *textBox = createTextBox(fromRectangle(rect));
    for (int i = 0; i < player->itemCount; i++) {
        drawInTextBox(textBox, player->items[i]->name);
    }
    drawText(">", (Vector2D) {UI_PADDING, UI_PADDING + line(cursorLine)});
}
