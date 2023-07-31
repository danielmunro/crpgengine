int getQuitCursorLength(MenuContext *menuContext) {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

void drawQuitMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawAlertBox();
    TextBox *textBox = createTextBox(rect);
    drawInTextBox(textBox, "Are you sure?");
    drawInTextBox(textBox, "");
    int cursorLength = getQuitCursorLength(menuContext);
    for (int i = 0; i < cursorLength; i++) {
        drawInTextBox(textBox, QuitMenuItems[i]);
    }
    drawText(
            ">",
            (Vector2D) {
                    (int) rect.x,
                    (int) rect.y + line(cursorLength + menuContext->cursorLine) + UI_PADDING
            }
    );
}

MenuSelectResponse *quitMenuItemSelected(MenuType menuType) {
    if (strcmp(QuitMenuItems[menuType], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
