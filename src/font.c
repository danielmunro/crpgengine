typedef struct {
    FontStyleType type;
    Font font;
    Color color;
    float size;
    float spacing;
} FontStyle;

FontStyle *createFontStyle(FontStyleType type, Font font, Color color, float size, float spacing) {
    FontStyle *fs = malloc(sizeof(FontStyle));
    fs->type = type;
    fs->font = font;
    fs->color = color;
    fs->size = size;
    fs->spacing = spacing;
    return fs;
}

FontStyle *createDefaultFontStyle(Font font) {
    return createFontStyle(
            FONT_STYLE_DEFAULT,
            font,
            DEFAULT_COLOR,
            FONT_SIZE,
            1);
}

FontStyle *createDisabledFontStyle(Font font) {
    return createFontStyle(
            FONT_STYLE_DISABLED,
            font,
            DISABLED_COLOR,
            FONT_SIZE,
            1);
}

FontStyle *createHighlightedFontStyle(Font font) {
    return createFontStyle(
            FONT_STYLE_HIGHLIGHTED,
            font,
            HIGHLIGHT_COLOR,
            FONT_SIZE,
            1);
}

FontStyle *createWarningFontStyle(Font font) {
    return createFontStyle(
            FONT_STYLE_WARNING,
            font,
            WARNING_COLOR,
            FONT_SIZE,
            1);
}

FontStyle *createDangerFontStyle(Font font) {
    return createFontStyle(
            FONT_STYLE_DANGER,
            font,
            DANGER_COLOR,
            FONT_SIZE,
            1);
}


FontStyle *getFontStyle(FontStyle **fonts, FontStyleType fsType) {
    for (int i = 0; i < FONT_STYLE_COUNT; i++) {
        if (fonts[i]->type == fsType) {
            return fonts[i];
        }
    }
    fprintf(stderr, "font style type not found: %d\n", fsType);
    exit(EXIT_FONT_STYLE_NOT_FOUND);
}
