int getAcknowledgeCursorLength(MenuContext *menuContext) {
    return 1;
}

TextBox *createAcknowledgeBox(MenuContext *mc) {
    FontStyle *defaultFont = mc->fonts->default_;
    return createTextBox(getMediumMenu(), defaultFont, ACKNOWLEDGE_BOX);
}

void drawAcknowledgeMenuScreen(MenuContext *menuContext) {
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            ACKNOWLEDGE_BOX,
            createAcknowledgeBox);
    drawMenuRect(b->area);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(4 + menuContext->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *acknowledgeMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
