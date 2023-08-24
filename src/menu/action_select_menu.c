const char *actionMenu[] = {
        "Attack",
        "Magic",
        "Items",
        "Defend",
        "Run",
};

int getActionSelectCursorLength(MenuContext *menuContext) {
    return sizeof(actionMenu) / sizeof(char *);
}

TextBox *createActionSelectMenuTextBox(MenuContext *menuContext) {
    return createTextBox(
        drawActionSelectMenu(),
        getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT),
        ACTION_SELECT_BOX);
}

void drawActionSelectMenuScreen(MenuContext *menuContext) {
    drawActionSelectMenu();
    TextBox *t = findOrCreateTextBox(
            menuContext,
            ACTION_SELECT_BOX,
            createActionSelectMenuTextBox);
    for (int i = 0; i < getActionSelectCursorLength(menuContext); i++) {
        drawInMenu(t, actionMenu[i]);
    }
    drawCursor(
            menuContext->fight->menuSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y + (float) (LINE_HEIGHT * menuContext->cursorLine),
            });
}

MenuSelectResponse *actionSelectMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, ACTION_SELECT_FIGHT_MENU);
}
