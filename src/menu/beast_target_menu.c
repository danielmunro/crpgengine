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
            menuContext->menuSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y + (float) (LINE_HEIGHT * menuContext->cursorLine),
            });
    Rectangle p = menuContext->fight->beasts[menuContext->cursorLine]->position;
    drawRightCursor(
            menuContext->menuSprite,
            (Vector2) {
                    p.x - (p.width / 2),
                    p.y - (p.height / 2),
            });
}

MenuSelectResponse *beastTargetMenuItemSelected(MenuContext *menuContext) {
    return beastSelectMenuItemSelected(menuContext);
}
