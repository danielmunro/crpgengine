int getAcknowledgeCursorLength(MenuContext *menuContext) {
    return 1;
}

TextBox *createAcknowledgeBox(MenuContext *mc) {
    Rectangle rect = drawMediumMenu();
    FontStyle *defaultFont = mc->fonts->default_;
    return createTextBox(rect, defaultFont, ACKNOWLEDGE_BOX);
}

void drawAcknowledgeMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            ACKNOWLEDGE_BOX,
            createAcknowledgeBox);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    rect.x,
                    rect.y + line(4 + menuContext->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *acknowledgeMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
