void drawAllMenus(
        MenuContext *menuContext,
        Menu **menus) {
    BeginDrawing();
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            break;
        }
        menuContext->cursorLine = menus[i]->cursor;
        menus[i]->draw(menuContext);
    }
    EndDrawing();
}
