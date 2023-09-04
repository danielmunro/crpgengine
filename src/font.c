typedef struct {
    FontStyleType type;
    Font font;
    Color color;
    float size;
    float lineHeight;
    float spacing;
} FontStyle;

FontStyle *createFontStyle(FontStyleType type, Font font, Color color, float size, float lineHeight, float spacing) {
    FontStyle *fs = malloc(sizeof(FontStyle));
    fs->type = type;
    fs->font = font;
    fs->color = color;
    fs->size = size;
    fs->lineHeight = lineHeight;
    fs->spacing = spacing;
    return fs;
}

FontStyle *createFontFromData(FontData *data) {
    char path[MAX_FS_PATH_LENGTH] = "";
    sprintf(path, "%s/fonts/%s", runtimeArgs->indexDir, data->filename);
    Font font = LoadFont(path);
    return createFontStyle(
            getFontStyleType(data->name),
            font,
            getFontColor(data->color),
            (float) data->size,
            (float) data->lineHeight,
            (float) data->spacing);
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
