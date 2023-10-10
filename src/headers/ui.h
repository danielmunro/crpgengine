#ifndef CJRPGENGINE_UI_H
#define CJRPGENGINE_UI_H

#include <string.h>
#include "log.h"
#include "errors.h"

#define FPS_X 0
#define FPS_Y 0

#define PARTY_MENU_WIDTH 200

#define FIGHT_PLAYER_Y_MARGIN 40
#define FIGHT_PLAYER_Y_PADDING 24

#define FIGHT_CURSOR_X_OFFSET 5
#define FIGHT_CURSOR_Y_OFFSET 22

#define BEAST_AREA (Rectangle) {20, 20, 240, 240}

#define MAX_LINE_BUFFER 128
#define MAX_MESSAGE_BUFFER 1024

const char *TextAreaTypes[] = {
        "unknown",
        "small",
        "medium",
        "full",
        "bottom",
        "left",
        "right",
        "bottomLeft",
        "bottomMidRight",
        "bottomMid",
        "midRight",
};

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

#define FONT_STYLE_COUNT sizeof(FontStyleTypes) / sizeof(FontStyleTypes[0])

typedef enum {
    FONT_STYLE_DEFAULT,
    FONT_STYLE_DISABLE,
    FONT_STYLE_HIGHLIGHT,
    FONT_STYLE_WARNING,
    FONT_STYLE_DANGER,
} FontStyleType;

FontStyleType getFontStyleType(const char *type) {
    for (int i = 0; i < FONT_STYLE_COUNT; i++) {
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

    // fight
    MOBILE_SELECT_BOX,
    MAGIC_SELECT_BOX,
    ACTION_SELECT_BOX,
    BEAST_SELECT_BOX,
    ITEM_SELECT_FIGHT_BOX,
} TextBoxLabel;

#include "src/graphics/avatar.h"
#include "src/graphics/font.c"
#include "src/graphics/window.c"
#include "src/graphics/ui/ui.c"

#endif // CJRPGENGINE_UI_H
