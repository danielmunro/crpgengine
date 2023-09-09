int getLoadCursorLength(MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

TextBox *createLoadMenuBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->small,
            mc->fonts->default_,
            LOAD_BOX);
}

void drawLoadMenuScreen(MenuContext *menuContext) {
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(menuContext, LOAD_BOX, createLoadMenuBox);
    for (int i = 0; i < menuContext->saveFiles->count; i++) {
        drawInMenu(b, menuContext->saveFiles->saveNames[i]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y
                        + ui->menu->padding
                        + line(menuContext->cursorLine, defaultFont->lineHeight),
            });
    free(b);
}

MenuSelectResponse *loadMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, LOAD_MENU);
}
