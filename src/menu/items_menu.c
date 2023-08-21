void drawItemsMenuScreen(MenuContext *menuContext) {
    FontStyle *defaultFont = getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT);
    TextBox *textBox = createTextBox(drawFullscreenMenu(), defaultFont);
    for (int i = 0; i < menuContext->player->itemCount; i++) {
        drawInMenu(textBox, menuContext->player->items[i]->name);
    }
    drawText(
            ">",
            (Vector2) {0, line(menuContext->cursorLine) + UI_PADDING},
            defaultFont);
    free(textBox);
}

int getItemsCursorLength(MenuContext *menuContext) {
    return menuContext->player->itemCount;
}

MenuSelectResponse *itemMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, ITEMS_MENU);
}
