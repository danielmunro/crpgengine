#include "headers/graphics/ui/menus/all_menus.h"
#include "headers/graphics/spritesheet_manager.h"

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

void unloadMenuNoop(MenuContext *mc) {}

int getMenuList(UIManager *uiMan) {
    Menu *list[] = {
            createMenu(
                    PARTY_MENU,
                    &getPartyMenuCursorLength,
                    &drawPartyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &partyMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    PARTY_SELECT_MENU,
                    &getPartySelectMenuCursorLength,
                    &drawPartySelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &partySelectMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    MAGIC_MENU,
                    &getMagicMenuCursorLength,
                    &drawMagicMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &magicMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    ITEMS_MENU,
                    &getItemsCursorLength,
                    &drawItemsMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &itemMenuItemSelected,
                    &unloadItemsMenu),
            createMenu(
                    LOAD_SAVE_MENU,
                    &getLoadCursorLength,
                    &drawLoadMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &loadMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    QUIT_GAME_MENU,
                    &getQuitCursorLength,
                    drawQuitMenuScreen,
                    getDefaultPreviousOption,
                    getDefaultNextOption,
                    &menuKeyPressed,
                    &quitMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    ACKNOWLEDGE_SAVE_MENU,
                    &getAcknowledgeCursorLength,
                    &drawAcknowledgeMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &acknowledgeMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    MOBILE_SELECT_FIGHT_MENU,
                    &getMobileSelectMenuCursorLength,
                    &drawMobileSelectMenuScreen,
                    &getPreviousMobileSelectCursorPosition,
                    &getNextMobileSelectCursorPosition,
                    &menuKeyPressed,
                    &mobileSelectMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    BEAST_LIST_FIGHT_MENU,
                    &getBeastSelectCursorLength,
                    &drawBeastSelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &beastSelectMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    ACTION_SELECT_FIGHT_MENU,
                    &getActionSelectCursorLength,
                    &drawActionSelectMenuScreen,
                    &getPreviousActionSelectCursorPosition,
                    &getNextActionSelectCursorPosition,
                    &menuKeyPressed,
                    &actionSelectMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    BEAST_TARGET_FIGHT_MENU,
                    &geBeastTargetMenuCursorLength,
                    &drawBeastTargetMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &beastTargetMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    MAGIC_FIGHT_MENU,
                    &getMagicSelectCursorLength,
                    &drawMagicSelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &magicSelectMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    MOBILE_TARGET_FIGHT_MENU,
                    &getMobileTargetMenuCursorLength,
                    &drawMobileTargetMenuScreen,
                    &getPreviousMobileTargetCursorPosition,
                    &getNextMobileTargetCursorPosition,
                    &menuKeyPressed,
                    &mobileTargetMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    PARTY_APPLY_MENU,
                    &getPartyApplyMenuCursorLength,
                    &drawPartyApplyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &partyApplyMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    ITEMS_FIGHT_MENU,
                    &getItemSelectCursorLength,
                    &drawItemSelectMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &itemSelectMenuItemSelected,
                    &unloadItemSelectMenu),
            createMenu(
                    SHOP_WELCOME_MENU,
                    &getShopWelcomeCursorLength,
                    &drawShopWelcomeMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &shopWelcomeMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SHOP_BUY_MENU,
                    &getShopBuyCursorLength,
                    &drawShopBuyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &shopBuyMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SHOP_CANNOT_AFFORD_MENU,
                    &getCannotAffordCursorLength,
                    &drawCannotAffordMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &cannotAffordMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SHOP_QUANTITY_BUY_MENU,
                    &getQuantityBuyCursorLength,
                    &drawQuantityBuyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &quantityBuyKeyPressed,
                    &quantityBuyMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SHOP_CONFIRM_PURCHASE_MENU,
                    &getConfirmPurchaseCursorLength,
                    &drawConfirmPurchaseMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &confirmPurchaseMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SHOP_SELL_MENU,
                    &getShopSellCursorLength,
                    &drawShopSellMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &shopSellMenuItemSelected,
                    &unloadItemSellMenu),
            createMenu(
                    SHOP_QUANTITY_SELL_MENU,
                    &getQuantitySellCursorLength,
                    &drawQuantitySellMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &quantitySellKeyPressed,
                    &quantitySellMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SHOP_CONFIRM_SALE_MENU,
                    &getConfirmSaleCursorLength,
                    &drawConfirmSaleMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &confirmSaleMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    MAIN_MENU,
                    &getMainMenuCursorLength,
                    &drawMainMenuScreen,
                    &getPreviousMainMenuCursorPosition,
                    &getNextMainMenuCursorPosition,
                    &menuKeyPressed,
                    &mainMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SETTINGS_MENU,
                    &getSettingsMenuCursorLength,
                    &drawSettingsMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SETTINGS_FULL_SCREEN_MENU,
                    &getSettingsFullScreenMenuCursorLength,
                    &drawSettingsFullScreenMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsFullScreenMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SETTINGS_DIFFICULTY_MENU,
                    &getSettingsDifficultyMenuCursorLength,
                    &drawSettingsDifficultyMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsDifficultyMenuItemSelected,
                    &unloadMenuNoop),
            createMenu(
                    SETTINGS_RESOLUTION_MENU,
                    &getSettingsResolutionMenuCursorLength,
                    &drawSettingsResolutionMenuScreen,
                    &getDefaultPreviousOption,
                    &getDefaultNextOption,
                    &menuKeyPressed,
                    &settingsResolutionMenuItemSelected,
                    &unloadMenuNoop),
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
