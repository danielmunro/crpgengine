typedef struct {
    SpritesheetManager *sprites;
    Menu *menus[MAX_MENUS];
    int menuCount;
    FontStyle *defaultFont;
    FontStyle *disabledFont;
    FontStyle *highlightedFont;
    FontStyle *warningFont;
    FontStyle *dangerFont;
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
            createMenu(
                    MOBILE_SELECT_FIGHT_MENU,
                    getMobileSelectFightMenuCursorLength,
                    drawMobileSelectFightMenuScreen,
                    mobileSelectFightMenuItemSelected
            )
    };
    int count = sizeof(list) / sizeof(list[0]);
    for (int i = 0; i < count; i++) {
        ui->menus[i] = list[i];
    }
    return count;
}

UIManager *createUIManager(SpritesheetManager *sprites, const char *indexDir, const char *fontName) {
    UIManager *ui = malloc(sizeof(UIManager));
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/fonts/%s", indexDir, fontName);
    Font font = LoadFont(path);
    ui->defaultFont = createDefaultFontStyle(font);
    ui->disabledFont = createDefaultDisabledFontStyle(font);
    ui->highlightedFont = createHighlightedFontStyle(font);
    ui->warningFont = createWarningFontStyle(font);
    ui->dangerFont = createDangerFontStyle(font);
    ui->menuCount = getMenuList(ui);
    ui->sprites = sprites;
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
