void drawAllMenus(
        MenuContext *menuContext,
        Menu *menus[MAX_MENUS],
        int menuCount) {
    BeginDrawing();
    for (int i = 0; i < menuCount; i++) {
        menuContext->cursorLine = menus[i]->cursor;
        menus[i]->draw(menuContext);
    }
    EndDrawing();
}
