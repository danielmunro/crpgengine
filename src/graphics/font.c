#include "../headers/util.h"
#include "../headers/ui.h"

typedef struct {
    FontStyleType type;
    Font font;
    Color color;
    float size;
    float lineHeight;
    float spacing;
} FontStyle;

typedef struct {
    FontStyle *default_;
    FontStyle *disable;
    FontStyle *highlight;
    FontStyle *warning;
    FontStyle *danger;
} Fonts;

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
            getColorFromString(data->color),
            (float) data->size,
            (float) data->lineHeight,
            (float) data->spacing);
}
