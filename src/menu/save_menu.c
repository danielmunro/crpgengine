int getSaveCursorLength(MenuContext *menuContext) {
    return menuContext->fileCount;
}

void drawSaveMenuScreen(MenuContext *menuContext) {
    TextBox *b = createTextBox(drawSaveBox());
    for (int i = 0; i < menuContext->fileCount; i++) {
        drawInTextBox(b, menuContext->files[i]);
    }
}

MenuSelectResponse *saveMenuItemSelected(MenuType menuType) {
    if (strcmp(QuitMenuItems[menuType], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
