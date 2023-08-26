typedef struct {
    Rectangle area;
    FontStyle *fontStyle;
    TextBoxLabel label;
    int cursor;
} TextBox;

TextBox *createTextBox(Rectangle area, FontStyle *fontStyle, TextBoxLabel label) {
    TextBox *textBox = malloc(sizeof(TextBox));
    textBox->area = area;
    textBox->fontStyle = fontStyle;
    textBox->label = label;
    textBox->cursor = 0;
    return textBox;
}

void drawText(const char *message, Vector2 position, FontStyle *fontStyle) {
    DrawTextEx(
            fontStyle->font,
            message,
            position,
            fontStyle->size,
            fontStyle->spacing,
            fontStyle->color);
}

void drawLineInArea(const char *message, Rectangle area, int lineNumber, FontStyle *font) {
    drawText(
            message,
            (Vector2) {
                    area.x + UI_PADDING,
                    area.y + UI_PADDING + (float) (LINE_HEIGHT * lineNumber)
            }, font);
}

void drawTextInArea(const char *message, Rectangle area, FontStyle *font) {
    char m[MAX_MESSAGE_BUFFER];
    char buffer[MAX_LINE_BUFFER];
    strcpy(m, message);
    strcpy(buffer, "");
    char *word = strtok(m, " ");
    int line = 0;
    char test[MAX_LINE_BUFFER];
    while (word != NULL) {
        strcpy(test, buffer);
        if (strcmp(test, "") != 0) {
            strcat(test, " ");
        }
        strcat(test, word);
        Vector2 testArea = MeasureTextEx(font->font, test, FONT_SIZE, 1);
        if (testArea.x > area.width - UI_PADDING * 2) {
            drawLineInArea(buffer, area, line, font);
            line++;
            strcpy(buffer, word);
        } else {
            strcpy(buffer, test);
        }
        word = strtok(NULL, " ");
    }
    if (strcmp(buffer, "") != 0) {
        drawLineInArea(buffer, area, line, font);
    }
    free(word);
}

void drawMenuRect(Rectangle rect) {
    DrawRectangleGradientV(
            (int) rect.x + 4,
            (int) rect.y + 4,
            (int) rect.width - 8,
            (int) rect.height - 8,
            LIGHTBLUE,
            DARKBLUE);
    DrawRectangleRoundedLines(
            (Rectangle) { rect.x + 4, rect.y + 4, rect.width - 8, rect.height - 8 },
            (float) 0.005,
            4,
            4,
            WHITE);
}

Rectangle drawSmallMenu() {
    float marginX = (float) SCREEN_WIDTH / 10, marginY = (float) SCREEN_HEIGHT / 10;
    Rectangle alertBox = {
            marginX,
            marginY,
            SCREEN_WIDTH - (marginX * 2),
            SCREEN_HEIGHT - (marginY * 2)};
    drawMenuRect(alertBox);
    return alertBox;
}

Rectangle drawMediumMenu() {
    float marginX = (float) SCREEN_WIDTH / 5, marginY = (float) SCREEN_HEIGHT / 5;
    Rectangle alertBox = {
            marginX,
            marginY,
            SCREEN_WIDTH - (marginX * 2),
            SCREEN_HEIGHT - (marginY * 2)};
    drawMenuRect(alertBox);
    return alertBox;
}

Rectangle drawFullscreenMenu() {
    Rectangle rect = (Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    drawMenuRect(rect);
    return rect;
}

Rectangle drawBottomMenu() {
    Rectangle rect = (Rectangle) {
        0,
        SCREEN_HEIGHT - BOTTOM_MENU_HEIGHT,
        SCREEN_WIDTH,
        BOTTOM_MENU_HEIGHT,
        };
    drawMenuRect(rect);
    return rect;
}

Rectangle drawBottomLeftMenu() {
    Rectangle rect = (Rectangle) {
        0,
        SCREEN_HEIGHT - BOTTOM_MENU_HEIGHT,
        SCREEN_WIDTH - BOTTOM_MENU_PLAYER_WIDTH,
        BOTTOM_MENU_HEIGHT,
    };
    drawMenuRect(rect);
    return rect;
}

Rectangle drawBottomRightMenu() {
    Rectangle rect = (Rectangle) {
        SCREEN_WIDTH - BOTTOM_MENU_PLAYER_WIDTH,
        SCREEN_HEIGHT - BOTTOM_MENU_HEIGHT,
        BOTTOM_MENU_PLAYER_WIDTH,
        BOTTOM_MENU_HEIGHT,
    };
    drawMenuRect(rect);
    return rect;
}

Rectangle drawActionSelectMenu() {
    Rectangle rect = (Rectangle) {
            SCREEN_WIDTH - BOTTOM_MENU_PLAYER_WIDTH,
            SCREEN_HEIGHT - BOTTOM_MENU_HEIGHT,
            BOTTOM_MENU_ACTION_SELECT_WIDTH,
            BOTTOM_MENU_HEIGHT,
    };
    drawMenuRect(rect);
    return rect;
}

float line(int line) {
    return (float) line * LINE_HEIGHT;
}

void drawInMenuWithStyle(TextBox *tb, FontStyle *fs, const char *text) {
    drawText(text, (Vector2) {
            tb->area.x + UI_PADDING,
            tb->area.y + line(tb->cursor) + UI_PADDING
    }, fs);
    tb->cursor++;
}

void drawInMenu(TextBox *textBox, const char *text) {
    drawInMenuWithStyle(textBox, textBox->fontStyle, text);
}
