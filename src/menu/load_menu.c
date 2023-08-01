int getLoadCursorLength(MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

void drawLoadMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawSaveBox();
    TextBox *b = createTextBox(rect);
    for (int i = 0; i < menuContext->saveFiles->count; i++) {
        drawInTextBox(b, menuContext->saveFiles->saveNames[i]);
    }
    drawText(">", (Vector2D) {(int) rect.x, (int) rect.y + UI_PADDING + line(menuContext->cursorLine)});
}

MenuSelectResponse *loadMenuItemSelected(MenuContext *menuContext) {
    printf("SELECTED: %d\n", menuContext->cursorLine);
    return createMenuSelectResponse(CLOSE_MENU, LOAD_MENU);
}
