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
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    partyMenuItemSelected),
            createMenu(
                    ITEMS_MENU,
                    getItemsCursorLength,
                    drawItemsMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    itemMenuItemSelected),
            createMenu(
                    LOAD_MENU,
                    getLoadCursorLength,
                    drawLoadMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    loadMenuItemSelected),
            createMenu(
                    QUIT_MENU,
                    getQuitCursorLength,
                    drawQuitMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    quitMenuItemSelected),
            createMenu(
                    ACKNOWLEDGE_MENU,
                    getAcknowledgeCursorLength,
                    drawAcknowledgeMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    acknowledgeMenuItemSelected),
            createMenu(
                    MOBILE_SELECT_FIGHT_MENU,
                    getMobileSelectMenuCursorLength,
                    drawMobileSelectMenuScreen,
                    getPreviousMobileSelectCursorPosition,
                    getNextMobileSelectCursorPosition,
                    mobileSelectMenuItemSelected),
            createMenu(
                    BEAST_LIST_FIGHT_MENU,
                    getBeastSelectCursorLength,
                    drawBeastSelectMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    beastSelectMenuItemSelected),
            createMenu(
                    ACTION_SELECT_FIGHT_MENU,
                    getActionSelectCursorLength,
                    drawActionSelectMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    actionSelectMenuItemSelected),
            createMenu(
                    BEAST_TARGET_FIGHT_MENU,
                    geBeastTargetMenuCursorLength,
                    drawBeastTargetMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    beastTargetMenuItemSelected),
            createMenu(
                    MAGIC_FIGHT_MENU,
                    getMagicFightCursorLength,
                    drawMagicFightMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    magicFightMenuItemSelected),
            createMenu(
                    MOBILE_TARGET_FIGHT_MENU,
                    getMobileTargetMenuCursorLength,
                    drawMobileTargetMenuScreen,
                    getPreviousMobileTargetCursorPosition,
                    getNextMobileTargetCursorPosition,
                    mobileTargetMenuItemSelected)
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
