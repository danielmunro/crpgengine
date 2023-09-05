int getLoadCursorLength(MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

void drawLoadMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawSmallMenu();
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = createTextBox(rect, defaultFont, LOAD_BOX);
    for (int i = 0; i < menuContext->saveFiles->count; i++) {
        drawInMenu(b, menuContext->saveFiles->saveNames[i]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                rect.x,
                rect.y + UI_PADDING + line(menuContext->cursorLine, defaultFont->lineHeight),
            });
    free(b);
}

MenuSelectResponse *loadMenuItemSelected(MenuContext *menuContext) {
    printf("SELECTED: %d\n", menuContext->cursorLine);
    return createMenuSelectResponse(CLOSE_MENU, LOAD_MENU);
}
