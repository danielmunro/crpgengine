int getQuitCursorLength(MenuContext *menuContext) {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

void drawQuitMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    TextBox *textBox = createTextBox(rect, menuContext->defaultFont);
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
                    rect.y + line(cursorLength + menuContext->cursorLine) + UI_PADDING
            },
            menuContext->defaultFont);
    free(textBox);
}

MenuSelectResponse *quitMenuItemSelected(MenuContext *menuContext) {
    if (strcmp(QuitMenuItems[menuContext->cursorLine], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
