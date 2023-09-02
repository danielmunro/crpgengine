#ifndef CJRPGENGINE_UI_H
#define CJRPGENGINE_UI_H

#define FONT_STYLE_COUNT 5
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 450
#define SCALE (float) 1.0

#define LINE_HEIGHT 30
#define UI_PADDING 20
#define DISABLED_COLOR GRAY
#define DEFAULT_COLOR WHITE
#define BOTTOM_MENU_HEIGHT 200
#define BOTTOM_MENU_PLAYER_WIDTH 560
#define BOTTOM_MENU_ACTION_SELECT_WIDTH 300

#define FIGHT_PLAYER_X (SCREEN_WIDTH * 0.8)
#define FIGHT_PLAYER_Y_MARGIN 40
#define FIGHT_PLAYER_Y_PADDING 24

#define FIGHT_CURSOR_X_OFFSET 5
#define FIGHT_CURSOR_Y_OFFSET 22

#define FONT_WARNING_THRESHOLD 0.6
#define FONT_DANGER_THRESHOLD 0.3

#define HP_X_OFFSET 200
#define MANA_X_OFFSET 320
#define ACTION_GAUGE_WIDTH 100
#define ACTION_GAUGE_HEIGHT 10

#define SPRITESHEET_NAME_UI "uipack"

#define CURSOR_RIGHT_INDEX 801
#define CURSOR_DOWN_INDEX 769

#define NOTIFICATION_DECAY_SECONDS 5
#define NOTIFICATION_HEIGHT 75

#define LIGHTBLUE (Color){100, 134, 175, 255}

#define BEAST_AREA (Rectangle) {20, 20, 240, 240}

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
    fprintf(stderr, "font style type not found");
    exit(1);
}

typedef enum {
    ACKNOWLEDGE_BOX,
    ACTION_SELECT_BOX,
    BEAST_SELECT_BOX,
    ITEMS_BOX,
    LOAD_BOX,
    MOBILE_SELECT_BOX,
    PARTY_BOX,
    QUIT_BOX,
    MAGIC_SELECT_BOX,
} TextBoxLabel;

#endif // CJRPGENGINE_UI_H