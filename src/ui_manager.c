typedef struct {
    Font font;
    FontStyle *fontStyle;
    Menu *menus[MAX_MENUS];
    int menuCount;
} UIManager;

int getMenuList(UIManager *ui) {
    Menu *list[] = {
            createMenu(
                    PARTY_MENU,
                    getPartyMenuCursorLength,
                    drawPartyMenuScreen,
                    partyMenuItemSelected),
            createMenu(
                    ITEMS_MENU,
                    getItemsCursorLength,
                    drawItemsMenuScreen,
                    itemMenuItemSelected),
            createMenu(
                    LOAD_MENU,
                    getLoadCursorLength,
                    drawLoadMenuScreen,
                    loadMenuItemSelected),
            createMenu(
                    QUIT_MENU,
                    getQuitCursorLength,
                    drawQuitMenuScreen,
                    quitMenuItemSelected),
            createMenu(
                    ACKNOWLEDGE_MENU,
                    getAcknowledgeCursorLength,
                    drawAcknowledgeMenuScreen,
                    acknowledgeMenuItemSelected),
    };
    int count = sizeof(list) / sizeof(list[0]);
    for (int i = 0; i < count; i++) {
        ui->menus[i] = list[i];
    }
    return count;
}

UIManager *createUIManager(const char *indexDir, const char *font) {
    UIManager *ui = malloc(sizeof(UIManager));
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/fonts/%s", indexDir, font);
    ui->font = LoadFont(path);
    ui->fontStyle = createDefaultFontStyle(LoadFont(path));
    ui->menuCount = getMenuList(ui);
    return ui;
}

Menu *findMenu(UIManager *ui, MenuType type) {
    for (int i = 0; i < ui->menuCount; i++) {
        if (ui->menus[i]->type == type) {
            return ui->menus[i];
        }
    }
    return NULL;
}
