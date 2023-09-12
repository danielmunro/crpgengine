int geBeastTargetMenuCursorLength(MenuContext *menuContext) {
    return getBeastSelectCursorLength(menuContext);
}

void drawBeastTargetMenuScreen(MenuContext *menuContext) {
    drawBeastSelectMenuScreen(menuContext);
    TextBox *t = findOrCreateTextBox(
            menuContext,
            BEAST_SELECT_BOX,
            createBeastSelectTextBox);
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    menuContext->fonts->default_->lineHeight *
                    (float) menuContext->cursorLine,
            });
    Rectangle p = menuContext->fight->beasts[menuContext->cursorLine]->position;
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    p.x - (p.width / 2),
                    p.y - (p.height / 2),
            });
}

MenuSelectResponse *beastTargetMenuItemSelected(MenuContext *menuContext) {
    menuContext->targetBeast = menuContext->fight->beasts[menuContext->cursorLine];
    return beastSelectMenuItemSelected(menuContext);
}
