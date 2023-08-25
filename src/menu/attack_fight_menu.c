int getAttackFightMenuCursorLength(MenuContext *menuContext) {
    return getBeastSelectCursorLength(menuContext);
}

void drawAttackFightMenuScreen(MenuContext *menuContext) {
    drawBeastSelectMenuScreen(menuContext);
    TextBox *t = findOrCreateTextBox(
            menuContext,
            BEAST_SELECT_BOX,
            createBeastSelectTextBox);
    drawCursor(
            menuContext->fight->menuSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y + (float) (LINE_HEIGHT * menuContext->cursorLine),
            });
//    menuContext->fight->beasts[menuContext->cursorLine]->
}

MenuSelectResponse *attackFightMenuItemSelected(MenuContext *menuContext) {
    return beastSelectMenuItemSelected(menuContext);
}
