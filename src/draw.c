void drawAllMenus(
        Fight *fight,
        Menu *menus[MAX_MENUS],
        int menuCount,
        FontStyle **fonts,
        const char *scene,
        const char *indexDir) {
    BeginDrawing();
    MenuContext *c = createMenuContext(
            fight,
            fonts,
            scene,
            indexDir,
            0);
    for (int i = 0; i < menuCount; i++) {
        c->cursorLine = menus[i]->cursor;
        menus[i]->draw(c);
    }
    free(c);
    EndDrawing();
}
