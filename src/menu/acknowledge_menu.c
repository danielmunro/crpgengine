int getAcknowledgeCursorLength(MenuContext *menuContext) {
    return 1;
}

void drawAcknowledgeMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    TextBox *b = createTextBox(rect);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawText(
            ">",
            (Vector2D) {
                    (int) rect.x,
                    (int) rect.y + line(4 + menuContext->cursorLine) + UI_PADDING
            }
    );
    free(b);
}

MenuSelectResponse *acknowledgeMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
