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
        menuContext->fonts->default_,
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
    drawRightCursor(
            menuContext->menuSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                            menuContext->fonts->default_->lineHeight *
                            (float) menuContext->cursorLine,
            });
}

MenuSelectResponse *actionSelectMenuItemSelected(MenuContext *menuContext) {
    if (menuContext->cursorLine == 0) {
        return createMenuSelectResponse(OPEN_MENU, BEAST_TARGET_FIGHT_MENU);
    } else if (menuContext->cursorLine == 1) {
        return createMenuSelectResponse(OPEN_MENU, MAGIC_FIGHT_MENU);
    } else if (menuContext->cursorLine == 2) {
        return createMenuSelectResponse(OPEN_MENU, ITEMS_FIGHT_MENU);
    }
    return createMenuSelectResponse(CLOSE_MENU, ACTION_SELECT_FIGHT_MENU);
}
