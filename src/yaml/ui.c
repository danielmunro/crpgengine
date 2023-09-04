typedef struct {
    const char *name;
    const char *filename;
    const char *color;
    int size;
    int lineHeight;
    int spacing;
} FontData;

typedef struct {
    FontData *default_;
    FontData *disable;
    FontData *highlight;
    FontData *warning;
    FontData *danger;
} FontsData;

typedef struct {
    bool full;
    int width;
    int height;
    float scale;
    int targetFrameRate;
} ScreenData;

typedef struct {
    int down;
    int right;
} CursorData;

typedef struct {
    const char *name;
    CursorData *cursors;
} UISpriteData;

typedef struct {
    UISpriteData *sprite;
    FontsData *fonts;
    ScreenData *screen;
} UIData;

static const cyaml_schema_field_t fontFamilyFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, FontData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "filename", CYAML_FLAG_POINTER, FontData, filename, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "color", CYAML_FLAG_POINTER, FontData, color, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "size", CYAML_FLAG_DEFAULT, FontData, size),
        CYAML_FIELD_INT(
                "lineHeight", CYAML_FLAG_DEFAULT, FontData, lineHeight),
        CYAML_FIELD_INT(
                "spacing", CYAML_FLAG_DEFAULT, FontData, spacing),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t familiesFieldSchema[] = {
        CYAML_FIELD_MAPPING_PTR(
                "default", CYAML_FLAG_POINTER, FontsData, default_, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "disable", CYAML_FLAG_POINTER, FontsData, disable, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "highlight", CYAML_FLAG_POINTER, FontsData, highlight, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "warning", CYAML_FLAG_POINTER, FontsData, warning, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "danger", CYAML_FLAG_POINTER, FontsData, danger, fontFamilyFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t screenFieldSchema[] = {
        CYAML_FIELD_BOOL(
                "full", CYAML_FLAG_DEFAULT, ScreenData, full),
        CYAML_FIELD_INT(
                "width", CYAML_FLAG_DEFAULT, ScreenData, width),
        CYAML_FIELD_INT(
                "height", CYAML_FLAG_DEFAULT, ScreenData, height),
        CYAML_FIELD_FLOAT(
                "scale", CYAML_FLAG_DEFAULT, ScreenData, scale),
        CYAML_FIELD_INT(
                "targetFrameRate", CYAML_FLAG_DEFAULT, ScreenData, targetFrameRate),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t cursorFieldSchema[] = {
        CYAML_FIELD_INT(
                "down", CYAML_FLAG_DEFAULT, CursorData, down),
        CYAML_FIELD_INT(
                "right", CYAML_FLAG_DEFAULT, CursorData, right),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t spritesheetFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, UISpriteData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "cursors", CYAML_FLAG_POINTER, UISpriteData, cursors, cursorFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t fontsTopMappingField[] = {
        CYAML_FIELD_MAPPING_PTR(
                "sprite", CYAML_FLAG_POINTER, UIData, sprite, spritesheetFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "fonts", CYAML_FLAG_POINTER, UIData, fonts, familiesFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "screen", CYAML_FLAG_POINTER, UIData, screen, screenFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_value_t uiSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, UIData, fontsTopMappingField),
};
