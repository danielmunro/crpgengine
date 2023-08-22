void drawBeastSelectMenuScreen(MenuContext *menuContext) {
//    TextBox *left = createTextBox(
//            drawBottomLeftMenu(),
//            getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT));
//    int count = menuContext->fight->beastCount > MAX_MOB_NAMES_IN_FIGHT ?
//            MAX_MOB_NAMES_IN_FIGHT : menuContext->fight->beastCount;
//    for (int i = 0; i < count; i++) {
//        drawInMenu(left, menuContext->fight->beasts[i]->name);
//    }
//    free(left);
}

int getBeastSelectCursorLength(MenuContext *menuContext) {
    return -1;
}

MenuSelectResponse *beastSelectMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, BEAST_LIST_FIGHT_MENU);
}
