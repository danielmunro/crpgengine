#ifndef CJRPGENGINE_UI_H
#define CJRPGENGINE_UI_H

#define FPS_X 0
#define FPS_Y 0

#define UI_PADDING 20
#define BOTTOM_MENU_HEIGHT 200
#define BOTTOM_MENU_PLAYER_WIDTH 560
#define BOTTOM_MENU_ACTION_SELECT_WIDTH 300

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
