void drawBeastSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = createTextBox(
            drawBottomLeftMenu(),
            getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT));
    int count = menuContext->fight->beastCount > MAX_MOB_NAMES_IN_FIGHT ?
            MAX_MOB_NAMES_IN_FIGHT : menuContext->fight->beastCount;
    for (int i = 0; i < count; i++) {
        drawInMenu(t, menuContext->fight->beasts[i]->name);
    }
    free(t);
}

int getBeastSelectCursorLength(MenuContext *menuContext) {
    return menuContext->fight->beastCount;
}

MenuSelectResponse *beastSelectMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, BEAST_LIST_FIGHT_MENU);
}
