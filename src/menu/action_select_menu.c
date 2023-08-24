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

void drawActionSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = createTextBox(
            drawActionSelectMenu(),
            getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT),
            ACTION_SELECT_BOX);
    for (int i = 0; i < getActionSelectCursorLength(menuContext); i++) {
        drawInMenu(t, actionMenu[i]);
    }
}

MenuSelectResponse *actionSelectMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, BEAST_LIST_FIGHT_MENU);
}
