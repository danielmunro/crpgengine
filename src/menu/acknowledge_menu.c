int getAcknowledgeCursorLength(MenuContext *menuContext) {
    return 1;
}

void drawAcknowledgeMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    FontStyle *defaultFont = getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT);
    TextBox *b = createTextBox(rect, defaultFont, ACKNOWLEDGE_BOX);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawText(
            ">",
            (Vector2) {
                    rect.x,
                    rect.y + line(4 + menuContext->cursorLine) + UI_PADDING
            },
            defaultFont);
    free(b);
}

MenuSelectResponse *acknowledgeMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
