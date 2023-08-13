typedef struct {
    Rectangle area;
    int cursor;
} TextBox;

TextBox *createTextBox(Rectangle area) {
    TextBox *textBox = malloc(sizeof(TextBox));
    textBox->area = area;
    textBox->cursor = 0;
    return textBox;
}

void drawText(const char *message, Vector2D position) {
    DrawText(&message[0], position.x, position.y, FONT_SIZE, WHITE);
}

int drawLineInArea(const char *message, Rectangle area, int lineNumber) {
    int charactersPerLine = (int) area.width / FONT_CHARACTER_WIDTH;
    int messageLength = (int) strlen(message);
    int lines = (int) ceilf((float) messageLength / (float) charactersPerLine);
    for (int i = 0; i < lines; i++) {
        char line[charactersPerLine + 1];
        memcpy(line, &message[i * charactersPerLine], charactersPerLine);
        line[charactersPerLine] = '\0';
        drawText(
                &line[0],
                (Vector2D) {
                        (int) area.x + UI_PADDING,
                        (int) area.y + UI_PADDING + (LINE_HEIGHT * (i + lineNumber))
                });
    }
    return lines;
}

void drawTextInArea(const char *message, Rectangle area) {
    char m[MAX_BUFFER];
    strcpy(m, message);
    char *word = strtok(m, " \n");
    int lines = 0;
    int charactersPerLine = (int) area.width / FONT_CHARACTER_WIDTH;
    char buffer[charactersPerLine + 1];
    strcpy(buffer, "");
    while (word != NULL) {
        if (strlen(buffer) + strlen(word) + 1 > charactersPerLine) {
            lines += drawLineInArea(&buffer[0], area, lines);
            strcpy(buffer, word);
        } else {
            if (strcmp(buffer, "") != 0) {
                strcat(buffer, " ");
            }
            strcat(buffer, word);
        }
        word = strtok(NULL, " \n");
    }
    if (strcmp(buffer, "") != 0) {
        drawLineInArea(buffer, area, lines);
    }
    free(word);
}

void drawMenuRect(Rectangle rect) {
//    DrawRectangleGradientH(
//            (int) rect.x,
//            (int) rect.y,
//            (int) rect.width,
//            (int) rect.height,
//            BLUE,
//            DARKBLUE);
    DrawRectangleRounded(
            (Rectangle) { rect.x + 4, rect.y, rect.width - 8, rect.height - 4 },
            (float) 0.005,
            4,
            BLUE);
    DrawRectangleRoundedLines(
            (Rectangle) { rect.x + 4, rect.y, rect.width - 8, rect.height - 4 },
            (float) 0.005,
            4,
            4,
            WHITE);
//    DrawRectangleGradientH(
//            (int) rect.x,
//            (int) rect.y,
//            (int) MENU_BAR_WIDTH,
//            (int) rect.height,
//            WHITE,
//            GRAY);
//    DrawRectangleGradientH(
//            (int) (rect.x + rect.width - MENU_BAR_WIDTH),
//            (int) rect.y,
//            (int) MENU_BAR_WIDTH,
//            (int) rect.height,
//            WHITE,
//            GRAY);
//    DrawRectangleGradientV(
//            (int) rect.x,
//            (int) rect.y,
//            (int) rect.width,
//            MENU_BAR_WIDTH,
//            WHITE,
//            GRAY);
//    DrawRectangleGradientV(
//            (int) rect.x,
//            (int) (rect.y + rect.height - MENU_BAR_WIDTH),
//            (int) rect.width,
//            MENU_BAR_WIDTH,
//            WHITE,
//            GRAY);
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

int line(int line) {
    return line * LINE_HEIGHT;
}

void drawInMenu(TextBox *textBox, const char *text) {
    drawText(text, (Vector2D) {
            (int) textBox->area.x + UI_PADDING,
            (int) textBox->area.y + line(textBox->cursor) + UI_PADDING
    });
    textBox->cursor++;
}
