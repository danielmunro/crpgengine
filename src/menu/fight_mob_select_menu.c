int getFightMobSelectMenuCursorLength(MenuContext *menuContext) {
    return MAX_PARTY_SIZE;
}

void drawFightMobSelectMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawMediumMenu();
    TextBox *b = createTextBox(rect, menuContext->fontStyle);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawText(
            ">",
            (Vector2) {
                    rect.x,
                    rect.y + line(4 + menuContext->cursorLine) + UI_PADDING
            },
            menuContext->fontStyle);
    free(b);
}

MenuSelectResponse *fightMobSelectMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(OPEN_MENU, FIGHT_MENU_ACTION_SELECT);
}
