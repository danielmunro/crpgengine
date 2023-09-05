int getQuitCursorLength(MenuContext *menuContext) {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

TextBox *createQuitTextBox(MenuContext *mc) {
    return createTextBox(
            drawMediumMenu(),
            mc->fonts->default_,
            QUIT_BOX);
}

void drawQuitMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(menuContext, QUIT_BOX, createQuitTextBox);
    drawInMenu(textBox, "Are you sure?");
    drawInMenu(textBox, "");
    int cursorLength = getQuitCursorLength(menuContext);
    for (int i = 0; i < cursorLength; i++) {
        drawInMenu(textBox, QuitMenuItems[i]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    rect.x,
                    rect.y +
                    line(cursorLength + menuContext->cursorLine, defaultFont->lineHeight),
            });
}

MenuSelectResponse *quitMenuItemSelected(MenuContext *menuContext) {
    if (strcmp(QuitMenuItems[menuContext->cursorLine], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
