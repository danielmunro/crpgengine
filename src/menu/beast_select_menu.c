TextBox *createBeastSelectTextBox(MenuContext *mc) {
    return createTextBox(
            drawBottomLeftMenu(),
            getFontStyle(mc->fonts, FONT_STYLE_DEFAULT),
            BEAST_SELECT_BOX);
}

void drawBeastSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            BEAST_SELECT_BOX,
            createBeastSelectTextBox);
    drawMenuRect(t->area);
    int count = menuContext->fight->beastCount > MAX_BEAST_NAMES_IN_FIGHT ?
                MAX_BEAST_NAMES_IN_FIGHT : menuContext->fight->beastCount;
    for (int i = 0; i < count; i++) {
        drawInMenu(t, menuContext->fight->beasts[i]->name);
    }
}

int getBeastSelectCursorLength(MenuContext *menuContext) {
    return menuContext->fight->beastCount;
}

MenuSelectResponse *beastSelectMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(TARGET_FOR_ATTACK, BEAST_LIST_FIGHT_MENU);
}
