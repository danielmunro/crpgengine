void drawItemsMenuScreen(MenuContext *menuContext) {
    Rectangle rect = drawInGameMenuBox();
    rect.x += UI_PADDING;
    rect.y += UI_PADDING;
    TextBox *textBox = createTextBox(fromRectangle(rect));
    for (int i = 0; i < menuContext->player->itemCount; i++) {
        drawInTextBox(textBox, menuContext->player->items[i]->name);
    }
    drawText(">", (Vector2D) {0, UI_PADDING + line(menuContext->cursorLine)});
}

MenuSelectResponse *partyMenuItemSelected(MenuType menuType) {
    if (strcmp(PartyMenuItems[menuType], PARTY_MENU_ITEMS) == 0) {
        return createMenuSelectResponse(OPEN_MENU, ITEMS_MENU);
    }
    if (strcmp(PartyMenuItems[menuType], PARTY_MENU_QUIT) == 0) {
        return createMenuSelectResponse(OPEN_MENU, QUIT_MENU);
    }
    return NULL;
}

int getItemsCursorLength(MenuContext *menuContext) {
    return menuContext->player->itemCount;
}
