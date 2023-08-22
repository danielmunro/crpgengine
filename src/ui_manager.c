typedef struct {
    Log *log;
    SpritesheetManager *sprites;
    MenuContext *menuContext;
    Menu *menus[MAX_MENUS];
    int menuCount;
    FontStyle **fonts;
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
                    mobileSelectFightMenuItemSelected),
            createMenu(
                    BEAST_LIST_FIGHT_MENU,
                    getBeastSelectCursorLength,
                    drawBeastSelectMenuScreen,
                    beastSelectMenuItemSelected),
            createMenu(
                    ACTION_SELECT_FIGHT_MENU,
                    getActionSelectCursorLength,
                    drawActionSelectMenuScreen,
                    actionSelectMenuItemSelected)
    };
    int count = sizeof(list) / sizeof(list[0]);
    for (int i = 0; i < count; i++) {
        ui->menus[i] = list[i];
    }
    return count;
}

UIManager *createUIManager(
        Log *log,
        SpritesheetManager *sprites,
        const char *indexDir,
        const char *fontName) {
    UIManager *ui = malloc(sizeof(UIManager));
    ui->log = log;
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/fonts/%s", indexDir, fontName);
    Font font = LoadFont(path);
    ui->fonts = calloc(FONT_STYLE_COUNT, sizeof(FontStyle));
    ui->fonts[0] = createDefaultFontStyle(font);
    ui->fonts[1] = createDisabledFontStyle(font);
    ui->fonts[2] = createHighlightedFontStyle(font);
    ui->fonts[3] = createWarningFontStyle(font);
    ui->fonts[4] = createDangerFontStyle(font);
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
