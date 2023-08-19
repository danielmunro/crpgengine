typedef struct {
    Font font;
    Color color;
    float size;
    float spacing;
} FontStyle;

FontStyle *createFontStyle(Font font, Color color, float size, float spacing) {
    FontStyle *fs = malloc(sizeof(FontStyle));
    fs->font = font;
    fs->color = color;
    fs->size = size;
    fs->spacing = spacing;

    return fs;
}

FontStyle *createDefaultFontStyle(Font font) {
    return createFontStyle(
            font,
            WHITE,
            FONT_SIZE,
            1);
}

FontStyle *createDefaultDisabledFontStyle(Font font) {
    return createFontStyle(
            font,
            GRAY,
            FONT_SIZE,
            1);
}

FontStyle *createHighlightedFontStyle(Font font) {
    return createFontStyle(
            font,
            HIGHLIGHT_COLOR,
            FONT_SIZE,
            1);
}

FontStyle *createWarningFontStyle(Font font) {
    return createFontStyle(
            font,
            HIGHLIGHT_COLOR,
            FONT_SIZE,
            1);
}

FontStyle *createDangerFontStyle(Font font) {
    return createFontStyle(
            font,
            DANGER_COLOR,
            FONT_SIZE,
            1);
}
