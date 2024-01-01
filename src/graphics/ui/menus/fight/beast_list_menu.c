void drawBeastSelectMenuScreen(MenuContext *mc) {
    TextBox *t = findOrCreateTextBox(
            mc,
            BEAST_SELECT_BOX,
            mc->context->ui->textAreas->bottomLeft);
    drawMenuRect(t->area);
    int count = mc->fight->beastCount > MAX_BEAST_NAMES_IN_FIGHT ?
                MAX_BEAST_NAMES_IN_FIGHT : mc->fight->beastCount;
    for (int i = 0; i < count; i++) {
        if (t->area.height > (float) i * mc->fonts->default_->lineHeight) {
            drawInMenu(t, mc->fight->beasts[i]->name);
        }
    }
}

int getBeastSelectCursorLength(const MenuContext *menuContext) {
    return menuContext->fight->beastCount;
}

MenuSelectResponse *beastSelectMenuItemSelected() {
    return createMenuSelectResponse(RESPONSE_TYPE_FIND_TARGET_MENU, BEAST_LIST_FIGHT_MENU);
}
