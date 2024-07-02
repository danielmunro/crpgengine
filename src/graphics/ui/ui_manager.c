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

Fonts *createFonts(const char *indexDir, const UIData *uiConfig) {
    Fonts *fonts = malloc(sizeof(Fonts));
    fonts->default_ = createFontFromData(indexDir, uiConfig->fonts->default_);
    fonts->disable = createFontFromData(indexDir, uiConfig->fonts->disable);
    fonts->highlight = createFontFromData(indexDir, uiConfig->fonts->highlight);
    fonts->warning = createFontFromData(indexDir, uiConfig->fonts->warning);
    fonts->danger = createFontFromData(indexDir, uiConfig->fonts->danger);
    return fonts;
}

int getMenuList(UIManager *uiMan) {
    Menu *list[] = {
            createMenu(
                    PARTY_MENU,
                    &getPartyMenuCursorLength,
                    &drawPartyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &partyMenuItemSelected),
            createMenu(
                    PARTY_SELECT_MENU,
                    &getPartySelectMenuCursorLength,
                    &drawPartySelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &partySelectMenuItemSelected),
            createMenu(
                    MAGIC_MENU,
                    &getMagicMenuCursorLength,
                    &drawMagicMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &magicMenuItemSelected),
            createMenu(
                    ITEMS_MENU,
                    &getItemsCursorLength,
                    &drawItemsMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &itemMenuItemSelected),
            createMenu(
                    LOAD_SAVE_MENU,
                    &getLoadCursorLength,
                    &drawLoadMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &loadMenuItemSelected),
            createMenu(
                    QUIT_GAME_MENU,
                    &getQuitCursorLength,
                    drawQuitMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &menuKeyPressed,
                    &quitMenuItemSelected),
            createMenu(
                    ACKNOWLEDGE_SAVE_MENU,
                    &getAcknowledgeCursorLength,
                    &drawAcknowledgeMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &acknowledgeMenuItemSelected),
            createMenu(
                    MOBILE_SELECT_FIGHT_MENU,
                    &getMobileSelectMenuCursorLength,
                    &drawMobileSelectMenuScreen,
                    &getPreviousMobileSelectCursorPosition,
                    &getNextMobileSelectCursorPosition,
                    &menuKeyPressed,
                    &mobileSelectMenuItemSelected),
            createMenu(
                    BEAST_LIST_FIGHT_MENU,
                    &getBeastSelectCursorLength,
                    &drawBeastSelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &beastSelectMenuItemSelected),
            createMenu(
                    ACTION_SELECT_FIGHT_MENU,
                    &getActionSelectCursorLength,
                    &drawActionSelectMenuScreen,
                    &getPreviousActionSelectCursorPosition,
                    &getNextActionSelectCursorPosition,
                    &menuKeyPressed,
                    &actionSelectMenuItemSelected),
            createMenu(
                    BEAST_TARGET_FIGHT_MENU,
                    &geBeastTargetMenuCursorLength,
                    &drawBeastTargetMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &beastTargetMenuItemSelected),
            createMenu(
                    MAGIC_FIGHT_MENU,
                    &getMagicSelectCursorLength,
                    &drawMagicSelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &magicSelectMenuItemSelected),
            createMenu(
                    MOBILE_TARGET_FIGHT_MENU,
                    &getMobileTargetMenuCursorLength,
                    &drawMobileTargetMenuScreen,
                    &getPreviousMobileTargetCursorPosition,
                    &getNextMobileTargetCursorPosition,
                    &menuKeyPressed,
                    &mobileTargetMenuItemSelected),
            createMenu(
                    PARTY_APPLY_MENU,
                    &getPartyApplyMenuCursorLength,
                    &drawPartyApplyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &partyApplyMenuItemSelected),
            createMenu(
                    ITEMS_FIGHT_MENU,
                    &getItemSelectCursorLength,
                    &drawItemSelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &itemSelectMenuItemSelected),
            createMenu(
                    SHOP_WELCOME_MENU,
                    &getShopWelcomeCursorLength,
                    &drawShopWelcomeMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &shopWelcomeMenuItemSelected),
            createMenu(
                    SHOP_BUY_MENU,
                    &getShopBuyCursorLength,
                    &drawShopBuyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &shopBuyMenuItemSelected),
            createMenu(
                    SHOP_CANNOT_AFFORD_MENU,
                    &getCannotAffordCursorLength,
                    &drawCannotAffordMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &cannotAffordMenuItemSelected),
            createMenu(
                    SHOP_QUANTITY_BUY_MENU,
                    &getQuantityBuyCursorLength,
                    &drawQuantityBuyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &quantityBuyKeyPressed,
                    &quantityBuyMenuItemSelected),
            createMenu(
                    SHOP_CONFIRM_PURCHASE_MENU,
                    &getConfirmPurchaseCursorLength,
                    &drawConfirmPurchaseMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &confirmPurchaseMenuItemSelected),
            createMenu(
                    SHOP_SELL_MENU,
                    &getShopSellCursorLength,
                    &drawShopSellMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &shopSellMenuItemSelected),
            createMenu(
                    SHOP_QUANTITY_SELL_MENU,
                    &getQuantitySellCursorLength,
                    &drawQuantitySellMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &quantitySellKeyPressed,
                    &quantitySellMenuItemSelected),
            createMenu(
                    SHOP_CONFIRM_SALE_MENU,
                    &getConfirmSaleCursorLength,
                    &drawConfirmSaleMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &confirmSaleMenuItemSelected),
            createMenu(
                    MAIN_MENU,
                    &getMainMenuCursorLength,
                    &drawMainMenuScreen,
                    &getPreviousMainMenuCursorPosition,
                    &getNextMainMenuCursorPosition,
                    &menuKeyPressed,
                    &mainMenuItemSelected),
            createMenu(
                    SETTINGS_MENU,
                    &getSettingsMenuCursorLength,
                    &drawSettingsMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsMenuItemSelected),
            createMenu(
                    SETTINGS_FULL_SCREEN_MENU,
                    &getSettingsFullScreenMenuCursorLength,
                    &drawSettingsFullScreenMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsFullScreenMenuItemSelected),
            createMenu(
                    SETTINGS_DIFFICULTY_MENU,
                    &getSettingsDifficultyMenuCursorLength,
                    &drawSettingsDifficultyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsDifficultyMenuItemSelected),
            createMenu(
                    SETTINGS_RESOLUTION_MENU,
                    &getSettingsResolutionMenuCursorLength,
                    &drawSettingsResolutionMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsResolutionMenuItemSelected),
    };
    int count = sizeof(list) / sizeof(list[0]);
    for (int i = 0; i < count; i++) {
        uiMan->menus[i] = list[i];
    }
    return count;
}

UIManager *createUIManager(
        UIData *uiConfig,
        UISprite *uiSprite,
        Fonts *fonts,
        MenuContext *menuContext) {
    UIManager *uiMan = malloc(sizeof(UIManager));
    uiMan->fonts = fonts;
    uiMan->menuCount = getMenuList(uiMan);
    uiMan->uiSprite = uiSprite;
    uiMan->config = uiConfig;
    uiMan->menuContext = menuContext;
    return uiMan;
}
