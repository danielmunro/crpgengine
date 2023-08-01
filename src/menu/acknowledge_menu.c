int getAcknowledgeCursorLength(MenuContext *menuContext) {
    return 1;
}

void drawAcknowledgeMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawAlertBox();
    TextBox *b = createTextBox(rect);
    drawInTextBox(b, "Your game has been saved.");
    drawInTextBox(b, "Reminder: your progress will save");
    drawInTextBox(b, "automatically.");
    drawInTextBox(b, "");
    drawInTextBox(b, "ok");
    drawText(
            ">",
            (Vector2D) {
                    (int) rect.x,
                    (int) rect.y + line(4 + menuContext->cursorLine) + UI_PADDING
            }
    );
}

MenuSelectResponse *acknowledgeMenuItemSelected(MenuContext *menuContext, MenuType menuType) {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
