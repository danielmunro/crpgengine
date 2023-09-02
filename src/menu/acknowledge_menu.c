int getAcknowledgeCursorLength(MenuContext *menuContext) {
    return 1;
}

TextBox *createAcknowledgeBox(MenuContext *mc) {
    Rectangle rect = drawMediumMenu();
    FontStyle *defaultFont = getFontStyle(mc->fonts, FONT_STYLE_DEFAULT);
    return createTextBox(rect, defaultFont, ACKNOWLEDGE_BOX);
}

void drawAcknowledgeMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    FontStyle *defaultFont = getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT);
    TextBox *b = findOrCreateTextBox(
            menuContext,
            ACKNOWLEDGE_BOX,
            createAcknowledgeBox);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawText(
            ">",
            (Vector2) {
                    rect.x,
                    rect.y + line(4 + menuContext->cursorLine, defaultFont->lineHeight) + UI_PADDING
            },
            defaultFont);
}

MenuSelectResponse *acknowledgeMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
