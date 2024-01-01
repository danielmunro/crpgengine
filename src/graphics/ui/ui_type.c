#include <string.h>

typedef enum {
    TEXT_AREA_UNKNOWN,
    TEXT_AREA_SMALL,
    TEXT_AREA_MEDIUM,
    TEXT_AREA_FULL,
    TEXT_AREA_BOTTOM,
    TEXT_AREA_LEFT,
    TEXT_AREA_RIGHT,
    TEXT_AREA_BOTTOM_LEFT,
    TEXT_AREA_BOTTOM_MID_RIGHT,
    TEXT_AREA_BOTTOM_MID,
    TEXT_AREA_MID_RIGHT,
} TextAreaType;

const char *FontStyleTypes[] = {
        "default",
        "disable",
        "highlight",
        "warning",
        "danger",
};

typedef enum {
    FONT_STYLE_DEFAULT,
    FONT_STYLE_DISABLE,
    FONT_STYLE_HIGHLIGHT,
    FONT_STYLE_WARNING,
    FONT_STYLE_DANGER,
} FontStyleType;

FontStyleType getFontStyleType(const char *type) {
    int count = sizeof(FontStyleTypes) / sizeof(FontStyleTypes[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(type, FontStyleTypes[i]) == 0) {
            return i;
        }
    }
    addError("font style type not found");
    exit(ConfigurationErrorUnknownFontStyleType);
}

typedef enum {
    ACKNOWLEDGE_BOX,
    ITEMS_BOX,
    MAGIC_BOX,
    LOAD_BOX,
    PARTY_APPLY_BOX,
    PARTY_SELECT_BOX,
    IN_GAME_MENU_BOX,
    PARTY_BOX,
    QUIT_BOX,
    SHOP_WELCOME_BOX,
    SHOP_BUY_BOX,
    SHOP_COSTS_BOX,
    SHOP_QUANTITY_COSTS_BOX,
    SHOP_SELL_BOX,
    SHOP_GOODBYE_BOX,
    CANNOT_AFFORD_BOX,
    QUANTITY_SELECT_BOX,
    MAIN_MENU_BOX,
    SETTINGS_NAMES_BOX,
    SETTINGS_VALUES_BOX,

    // fight
    MOBILE_SELECT_BOX,
    MAGIC_SELECT_BOX,
    ACTION_SELECT_BOX,
    BEAST_SELECT_BOX,
    ITEM_SELECT_FIGHT_BOX,
} TextBoxLabel;
