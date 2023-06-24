void drawBlueBox(Rectangle rect) {
    DrawRectangleGradientH(
            (int) rect.x,
            (int) rect.y,
            (int) rect.width,
            (int) rect.height,
            BLUE,
            DARKBLUE
    );
}

void drawInGameMenuBox() {
    drawBlueBox((Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
}

void drawText(const char *message, Vector2d position) {
    DrawText(&message[0], position.x, position.y, FONT_SIZE, WHITE);
}

void drawDialogBox(const char *message) {
    drawBlueBox((Rectangle) {0, SCREEN_HEIGHT - 150, SCREEN_WIDTH, SCREEN_HEIGHT});
    unsigned long lines = (strlen(message) / MAX_CHARACTERS_PER_LINE) + 1;
    int startY = SCREEN_HEIGHT - 135;
    for (int i = 0; i < lines; i++) {
        char line[MAX_CHARACTERS_PER_LINE + 1];
        memcpy(line, &message[i * MAX_CHARACTERS_PER_LINE], MAX_CHARACTERS_PER_LINE);
        line[MAX_CHARACTERS_PER_LINE] = '\0';
        drawText(&line[0], (Vector2d) {15, startY + (LINE_HEIGHT * i)});
    }
}

int line(int line) {
    return line * LINE_HEIGHT;
}
