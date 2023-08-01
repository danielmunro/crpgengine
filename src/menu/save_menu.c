int getSaveCursorLength(MenuContext *menuContext) {
    return 2;
}

void drawSaveMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawAlertBox();
    TextBox *b = createTextBox(rect);
    drawInTextBox(b, "Save game?");
    drawInTextBox(b, "no");
    drawInTextBox(b, "yes");
    drawText(
            ">",
            (Vector2D) {
                    (int) rect.x,
                    (int) rect.y + line(1 + menuContext->cursorLine) + UI_PADDING
            }
    );
}

MenuSelectResponse *saveMenuItemSelected(MenuContext *menuContext, MenuType menuType) {
    if (strcmp(QuitMenuItems[menuType], QUIT_MENU_YES) == 0) {
        save(
                menuContext->player,
                menuContext->scene,
                menuContext->indexDir);
        return createMenuSelectResponse(OPEN_MENU, ACKNOWLEDGE_MENU);
    }
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
