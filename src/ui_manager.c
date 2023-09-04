typedef struct {
    SpritesheetManager *sprites;
    MenuContext *menuContext;
    Menu *menus[MAX_MENUS];
    int menuCount;
    Fonts *fonts;
    Spritesheet *spritesheet;
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

UIManager *createUIManager(UIData *config, Spritesheet *uiSprite) {
    UIManager *ui = malloc(sizeof(UIManager));
    ui->fonts = malloc(sizeof(Fonts));
    ui->fonts->default_ = createFontFromData(config->fonts->default_);
    ui->fonts->disable = createFontFromData(config->fonts->disable);
    ui->fonts->highlight = createFontFromData(config->fonts->highlight);
    ui->fonts->warning = createFontFromData(config->fonts->warning);
    ui->fonts->danger = createFontFromData(config->fonts->danger);
    ui->menuCount = getMenuList(ui);
    ui->spritesheet = uiSprite;
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
