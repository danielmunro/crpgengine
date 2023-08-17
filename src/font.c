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
