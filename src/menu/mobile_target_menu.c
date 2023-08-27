int getMobileTargetMenuCursorLength(MenuContext *menuContext) {
    return getMobileSelectMenuCursorLength(menuContext);
}

void drawMobileTargetMenuScreen(MenuContext *menuContext) {
    drawMobileSelectMenuScreen(menuContext);
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MOBILE_SELECT_BOX,
            createMobileSelectTextBox);
    drawCursor(
            menuContext->fight->menuSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y + (float) (LINE_HEIGHT * menuContext->cursorLine),
            });
}

MenuSelectResponse *mobileTargetMenuItemSelected(MenuContext *menuContext) {
    return beastSelectMenuItemSelected(menuContext);
}
