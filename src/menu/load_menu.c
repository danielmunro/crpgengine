int getLoadCursorLength(MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

void drawLoadMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawSmallMenu();
    FontStyle *defaultFont = getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT);
    TextBox *b = createTextBox(rect, defaultFont, LOAD_BOX);
    for (int i = 0; i < menuContext->saveFiles->count; i++) {
        drawInMenu(b, menuContext->saveFiles->saveNames[i]);
    }
    drawText(
            ">",
            (Vector2) {rect.x, rect.y + UI_PADDING +
                    line(menuContext->cursorLine, defaultFont->lineHeight)},
            defaultFont);
    free(b);
}

MenuSelectResponse *loadMenuItemSelected(MenuContext *menuContext) {
    printf("SELECTED: %d\n", menuContext->cursorLine);
    return createMenuSelectResponse(CLOSE_MENU, LOAD_MENU);
}
