int getQuitCursorLength(MenuContext *menuContext) {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

void drawQuitMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = createTextBox(rect, defaultFont, QUIT_BOX);
    drawInMenu(textBox, "Are you sure?");
    drawInMenu(textBox, "");
    int cursorLength = getQuitCursorLength(menuContext);
    for (int i = 0; i < cursorLength; i++) {
        drawInMenu(textBox, QuitMenuItems[i]);
    }
    drawText(
            ">",
            (Vector2) {
                    rect.x,
                    rect.y +
                            line(cursorLength + menuContext->cursorLine, defaultFont->lineHeight)
                            + UI_PADDING
            },
            defaultFont);
    free(textBox);
}

MenuSelectResponse *quitMenuItemSelected(MenuContext *menuContext) {
    if (strcmp(QuitMenuItems[menuContext->cursorLine], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
