typedef struct {
    Rectangle area;
    Font font;
    int cursor;
} TextBox;

TextBox *createTextBox(Rectangle area, Font font) {
    TextBox *textBox = malloc(sizeof(TextBox));
    textBox->area = area;
    textBox->font = font;
    textBox->cursor = 0;
    return textBox;
}

void drawText(const char *message, Vector2 position, Font font) {
    DrawTextEx(font, message, position, FONT_SIZE, 1, WHITE);
}

void drawLineInArea(const char *message, Rectangle area, int lineNumber, Font font) {
    drawText(
            message,
            (Vector2) {
                    area.x + UI_PADDING,
                    area.y + UI_PADDING + (float) (LINE_HEIGHT * lineNumber)
            }, font);
}

void drawTextInArea(const char *message, Rectangle area, Font font) {
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
        Vector2 testArea = MeasureTextEx(font, test, FONT_SIZE, 1);
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
    DrawRectangleRounded(
            (Rectangle) { rect.x + 4, rect.y + 4, rect.width - 8, rect.height - 8 },
            (float) 0.005,
            4,
            BLUE);
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
    Rectangle rect = (Rectangle) {0, SCREEN_HEIGHT - 150, SCREEN_WIDTH, 150};
    drawMenuRect(rect);
    return rect;
}

Rectangle drawBottomLeftMenu() {
    Rectangle rect = (Rectangle) {0, SCREEN_HEIGHT - 150, SCREEN_WIDTH - 600, 150};
    drawMenuRect(rect);
    return rect;
}

Rectangle drawBottomRightMenu() {
    Rectangle rect = (Rectangle) {SCREEN_WIDTH - 600, SCREEN_HEIGHT - 150, 600, 150};
    drawMenuRect(rect);
    return rect;
}

float line(int line) {
    return (float) line * LINE_HEIGHT;
}

void drawInMenu(TextBox *textBox, const char *text) {
    drawText(text, (Vector2) {
            textBox->area.x + UI_PADDING,
            textBox->area.y + line(textBox->cursor) + UI_PADDING
    }, textBox->font);
    textBox->cursor++;
}
