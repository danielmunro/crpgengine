TextBox *createItemsTextBox(MenuContext *mc) {
    return createTextBox(
            drawFullscreenMenu(),
            mc->fonts->default_,
            ITEMS_BOX);
}

void drawItemsMenuScreen(MenuContext *menuContext) {
    drawFullscreenMenu();
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            menuContext,
            ITEMS_BOX,
            createItemsTextBox);
    for (int i = 0; i < menuContext->player->itemCount; i++) {
        drawInMenu(textBox, menuContext->player->items[i]->name);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    0,
                    line(menuContext->cursorLine, defaultFont->lineHeight),
            });
}

int getItemsCursorLength(MenuContext *menuContext) {
    return menuContext->player->itemCount;
}

MenuSelectResponse *itemMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, ITEMS_MENU);
}
