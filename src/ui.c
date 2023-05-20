void drawDialogBox(char *message) {
    DrawRectangleGradientH(
        0,
        SCREEN_HEIGHT - 150,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        BLUE,
        DARKBLUE
    );
    unsigned long lines = (strlen(message) / MAX_CHARACTERS_PER_LINE) + 1;
    int startY = SCREEN_HEIGHT - 135;
    for (int i = 0; i < lines; i++) {
        char line[MAX_CHARACTERS_PER_LINE + 1];
        memcpy(line, &message[i * MAX_CHARACTERS_PER_LINE], MAX_CHARACTERS_PER_LINE);
        line[MAX_CHARACTERS_PER_LINE] = '\0';
        DrawText(&line[0], 15, startY + (LINE_HEIGHT * i), FONT_SIZE, WHITE);
    }
}