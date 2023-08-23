void drawActionSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = createTextBox(
            drawActionSelectMenu(),
            getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT),
            ACTION_SELECT_BOX);
    drawInMenu(t, "Attack");
    drawInMenu(t, "Items");
    drawInMenu(t, "Magic");
    drawInMenu(t, "Defend");
    drawInMenu(t, "Run");
}

int getActionSelectCursorLength(MenuContext *menuContext) {
    return -1;
}

MenuSelectResponse *actionSelectMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, BEAST_LIST_FIGHT_MENU);
}
