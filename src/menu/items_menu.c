void drawItemsMenuScreen(MenuContext *menuContext) {
    TextBox *textBox = createTextBox(drawInGameMenuBox());
    for (int i = 0; i < menuContext->player->itemCount; i++) {
        drawInTextBox(textBox, menuContext->player->items[i]->name);
    }
    drawText(">", (Vector2D) {0, line(menuContext->cursorLine) + UI_PADDING});
}

int getItemsCursorLength(MenuContext *menuContext) {
    return menuContext->player->itemCount;
}

MenuSelectResponse *itemMenuItemSelected(MenuType menuType) {
    return createMenuSelectResponse(CLOSE_MENU, ITEMS_MENU);
}
