int getQuitCursorLength(MenuContext *menuContext) {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

TextBox *createQuitTextBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->medium,
            mc->fonts->default_,
            QUIT_BOX);
}

void drawQuitMenuScreen(MenuContext *menuContext) {
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(menuContext, QUIT_BOX, createQuitTextBox);
    drawMenuRect(textBox->area);
    drawInMenu(textBox, "Are you sure?");
    drawInMenu(textBox, "");
    int cursorLength = getQuitCursorLength(menuContext);
    for (int i = 0; i < cursorLength; i++) {
        drawInMenu(textBox, QuitMenuItems[i]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    textBox->area.x,
                    textBox->area.y +
                    line(cursorLength + menuContext->cursorLine, defaultFont->lineHeight),
            });
}

MenuSelectResponse *quitMenuItemSelected(MenuContext *menuContext) {
    if (strcmp(QuitMenuItems[menuContext->cursorLine], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
