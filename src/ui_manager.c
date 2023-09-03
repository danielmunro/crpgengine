typedef struct {
    SpritesheetManager *sprites;
    MenuContext *menuContext;
    Menu *menus[MAX_MENUS];
    int menuCount;
    FontStyle **fonts;
    Spritesheet *menuSprite;
    UIData *config;
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
        UIData *config,
        SpritesheetManager *sprites) {
    UIManager *ui = malloc(sizeof(UIManager));
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/fonts/%s", runtimeArgs->indexDir, config->fonts->default_->filename);
    Font font = LoadFont(path);
    ui->fonts = calloc(FONT_STYLE_COUNT, sizeof(FontStyle));
    ui->fonts[0] = createFontFromData(config->fonts->default_, font);
    ui->fonts[1] = createFontFromData(config->fonts->disable, font);
    ui->fonts[2] = createFontFromData(config->fonts->highlight, font);
    ui->fonts[3] = createFontFromData(config->fonts->warning, font);
    ui->fonts[4] = createFontFromData(config->fonts->danger, font);
    ui->menuCount = getMenuList(ui);
    ui->sprites = sprites;
    ui->menuSprite = findSpritesheetByName(sprites, SPRITESHEET_NAME_UI);
    ui->config = config;
    return ui;
}

void drawAllMenus(
        MenuContext *menuContext,
        Menu **menus) {
    for (int i = 0; i < MAX_MENUS; i++) {
        if (menus[i] == NULL) {
            break;
        }
        menuContext->cursorLine = menus[i]->cursor;
        menus[i]->draw(menuContext);
    }
}
