int getMainMenuCursorLength(const MenuContext *menuContext) {
    return 3;
}

void drawMainMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MAIN_MENU_BOX,
            ui->textAreas->full);
    drawMenuRect(t->area);
    drawInMenu(t, "new");
    drawInMenu(t, "load");
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

MenuSelectResponse *mainMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(NO_OP, MAIN_MENU);
}
