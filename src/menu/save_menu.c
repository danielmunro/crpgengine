int getSaveCursorLength(MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

void drawSaveMenuScreen(MenuContext *menuContext) {
    TextBox *b = createTextBox(drawSaveBox());
    for (int i = 0; i < menuContext->saveFiles->count; i++) {
        drawInTextBox(b, menuContext->saveFiles->saveNames[i]);
    }
}

MenuSelectResponse *saveMenuItemSelected(MenuType menuType) {
    if (strcmp(QuitMenuItems[menuType], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
