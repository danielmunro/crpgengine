int getQuitCursorLength(MenuContext *menuContext) {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

void drawQuitMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawAlertBox();
    rect.x += UI_PADDING;
    rect.y += UI_PADDING;
    TextBox *textBox = createTextBox(fromRectangle(rect));
    drawInTextBox(textBox, "Are you sure?");
    drawInTextBox(textBox, "");
    int cursorLength = getQuitCursorLength(menuContext);
    for (int i = 0; i < cursorLength; i++) {
        drawInTextBox(textBox, QuitMenuItems[i]);
    }
    drawText(
            ">",
            (Vector2D) {
                    (int) rect.x - UI_PADDING,
                    (int) rect.y + line(cursorLength + menuContext->cursorLine)
            }
    );
}

MenuSelectResponse *quitMenuItemSelected(MenuType menuType) {
    if (strcmp(QuitMenuItems[menuType], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
