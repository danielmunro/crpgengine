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

void drawMenuRect(Rectangle rect) {
    DrawRectangleGradientH(
            (int) rect.x,
            (int) rect.y,
            (int) rect.width,
            (int) rect.height,
            BLUE,
            DARKBLUE
    );
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

Rectangle drawBottomMenu(const char *message) {
    Rectangle rect = (Rectangle) {0, SCREEN_HEIGHT - 150, SCREEN_WIDTH, SCREEN_HEIGHT};
    drawMenuRect(rect);
    unsigned long lines = (strlen(message) / MAX_CHARACTERS_PER_LINE) + 1;
    int startY = SCREEN_HEIGHT - 135;
    for (int i = 0; i < lines; i++) {
        char line[MAX_CHARACTERS_PER_LINE + 1];
        memcpy(line, &message[i * MAX_CHARACTERS_PER_LINE], MAX_CHARACTERS_PER_LINE);
        line[MAX_CHARACTERS_PER_LINE] = '\0';
        drawText(&line[0], (Vector2D) {15, startY + (LINE_HEIGHT * i)});
    }
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
