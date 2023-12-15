int getMainMenuCursorLength() {
    return 4;
}

void drawMainMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MAIN_MENU_BOX,
            ui->textAreas->full);
    drawMenuRect(t->area);
    drawInMenu(t, "new");
    drawInMenu(t, "load");
    drawInMenu(t, "settings");
    drawInMenu(t, "exit");
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    ui->textAreas->full.x,
                    line(
                            menuContext->cursorLine,
                            menuContext->fonts->default_->lineHeight),
            });
}

MenuSelectResponse *mainMenuItemSelected(MenuContext *mc) {
    if (mc->cursorLine == 4) {
        return createMenuSelectResponse(RESPONSE_TYPE_EXIT, MAIN_MENU);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_NONE, MAIN_MENU);
}
