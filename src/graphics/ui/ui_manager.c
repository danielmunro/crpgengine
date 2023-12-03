#include "headers/graphics/ui/menus/all_menus.h"

typedef struct {
    SpritesheetManager *sprites;
    MenuContext *menuContext;
    Menu *menus[MAX_MENUS];
    int menuCount;
    Fonts *fonts;
    UISprite *uiSprite;
    UIData *config;
} UIManager;

int getMenuList(UIManager *uiMan) {
    Menu *list[] = {
            createMenu(
                    PARTY_MENU,
                    &getPartyMenuCursorLength,
                    drawPartyMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &partyMenuItemSelected),
            createMenu(
                    PARTY_SELECT_MENU,
                    &getPartySelectMenuCursorLength,
                    drawPartySelectMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &partySelectMenuItemSelected),
            createMenu(
                    MAGIC_MENU,
                    &getMagicMenuCursorLength,
                    drawMagicMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &magicMenuItemSelected),
            createMenu(
                    ITEMS_MENU,
                    &getItemsCursorLength,
                    drawItemsMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &itemMenuItemSelected),
            createMenu(
                    LOAD_MENU,
                    &getLoadCursorLength,
                    drawLoadMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &loadMenuItemSelected),
            createMenu(
                    QUIT_MENU,
                    &getQuitCursorLength,
                    drawQuitMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &quitMenuItemSelected),
            createMenu(
                    ACKNOWLEDGE_MENU,
                    &getAcknowledgeCursorLength,
                    drawAcknowledgeMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &acknowledgeMenuItemSelected),
            createMenu(
                    MOBILE_SELECT_FIGHT_MENU,
                    &getMobileSelectMenuCursorLength,
                    drawMobileSelectMenuScreen,
                    getPreviousMobileSelectCursorPosition,
                    getNextMobileSelectCursorPosition,
                    &mobileSelectMenuItemSelected),
            createMenu(
                    BEAST_LIST_FIGHT_MENU,
                    &getBeastSelectCursorLength,
                    drawBeastSelectMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &beastSelectMenuItemSelected),
            createMenu(
                    ACTION_SELECT_FIGHT_MENU,
                    &getActionSelectCursorLength,
                    drawActionSelectMenuScreen,
                    getPreviousActionSelectCursorPosition,
                    getNextActionSelectCursorPosition,
                    &actionSelectMenuItemSelected),
            createMenu(
                    BEAST_TARGET_FIGHT_MENU,
                    &geBeastTargetMenuCursorLength,
                    drawBeastTargetMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &beastTargetMenuItemSelected),
            createMenu(
                    MAGIC_FIGHT_MENU,
                    getMagicSelectCursorLength,
                    drawMagicSelectMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &magicSelectMenuItemSelected),
            createMenu(
                    MOBILE_TARGET_FIGHT_MENU,
                    &getMobileTargetMenuCursorLength,
                    drawMobileTargetMenuScreen,
                    getPreviousMobileTargetCursorPosition,
                    getNextMobileTargetCursorPosition,
                    &mobileTargetMenuItemSelected),
            createMenu(
                    PARTY_APPLY_MENU,
                    &getPartyApplyMenuCursorLength,
                    drawPartyApplyMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &partyApplyMenuItemSelected),
            createMenu(
                    ITEMS_FIGHT_MENU,
                    &getItemSelectCursorLength,
                    drawItemSelectMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &itemSelectMenuItemSelected),
            createMenu(
                    SHOP_MENU,
                    &getShopCursorLength,
                    drawShopMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &shopMenuItemSelected),
    };
    int count = sizeof(list) / sizeof(list[0]);
    for (int i = 0; i < count; i++) {
        uiMan->menus[i] = list[i];
    }
    return count;
}

Fonts *createFonts(UIData *uiConfig) {
    Fonts *fonts = malloc(sizeof(Fonts));
    fonts->default_ = createFontFromData(uiConfig->fonts->default_);
    fonts->disable = createFontFromData(uiConfig->fonts->disable);
    fonts->highlight = createFontFromData(uiConfig->fonts->highlight);
    fonts->warning = createFontFromData(uiConfig->fonts->warning);
    fonts->danger = createFontFromData(uiConfig->fonts->danger);
    return fonts;
}

UIManager *createUIManager(UIData *uiConfig, UISprite *uiSprite, Fonts *fonts, MenuContext *menuContext) {
    UIManager *uiMan = malloc(sizeof(UIManager));
    uiMan->fonts = fonts;
    uiMan->menuCount = getMenuList(uiMan);
    uiMan->uiSprite = uiSprite;
    uiMan->config = uiConfig;
    uiMan->menuContext = menuContext;
    return uiMan;
}
