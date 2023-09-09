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
    const char *title;
    bool full;
    int width;
    int height;
    float scale;
    int targetFrameRate;
} ScreenData;

typedef struct {
    int down;
    int right;
    int up;
} CursorData;

typedef struct {
    const char *name;
    CursorData *cursors;
} UISpriteData;

typedef struct {
    const char *topColor;
    const char *bottomColor;
} VerticalGradientData;

typedef struct {
    int lineThickness;
    float roundness;
    const char *color;
} UIMenuBorderData;

typedef struct {
    const char *style;
    VerticalGradientData *verticalGradient;
    UIMenuBorderData *border;
    float padding;
} UIMenuData;

typedef struct {
    const char *type;
    float x;
    float y;
    float width;
    float height;
} TextAreaData;

typedef struct {
    UISpriteData *sprite;
    FontsData *fonts;
    ScreenData *screen;
    UIMenuData *menu;
    TextAreaData *textAreas;
    int textAreasCount;
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
        CYAML_FIELD_STRING_PTR(
                "title", CYAML_FLAG_POINTER, ScreenData, title, 0, CYAML_UNLIMITED),
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
        CYAML_FIELD_INT(
                "up", CYAML_FLAG_DEFAULT, CursorData, up),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t spritesheetFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, UISpriteData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "cursors", CYAML_FLAG_POINTER, UISpriteData, cursors, cursorFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t verticalGradientFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "topColor", CYAML_FLAG_POINTER,
                VerticalGradientData, topColor, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "bottomColor", CYAML_FLAG_POINTER,
                VerticalGradientData, bottomColor, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t borderFieldSchema[] = {
        CYAML_FIELD_INT(
                "lineThickness", CYAML_FLAG_DEFAULT, UIMenuBorderData, lineThickness),
        CYAML_FIELD_FLOAT(
                "roundness", CYAML_FLAG_DEFAULT, UIMenuBorderData, roundness),
        CYAML_FIELD_STRING_PTR(
                "color", CYAML_FLAG_POINTER,
                UIMenuBorderData, color, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t menuFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "style", CYAML_FLAG_POINTER,
                UIMenuData, style, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "verticalGradient", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                UIMenuData, verticalGradient, verticalGradientFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "border", CYAML_FLAG_POINTER,
                UIMenuData, border, borderFieldSchema),
        CYAML_FIELD_FLOAT(
                "padding", CYAML_FLAG_DEFAULT, UIMenuData, padding),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t textAreaFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "type", CYAML_FLAG_POINTER,
                TextAreaData, type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_FLOAT(
                "x", CYAML_FLAG_DEFAULT, TextAreaData, x),
        CYAML_FIELD_FLOAT(
                "y", CYAML_FLAG_DEFAULT, TextAreaData, y),
        CYAML_FIELD_FLOAT(
                "width", CYAML_FLAG_DEFAULT, TextAreaData, width),
        CYAML_FIELD_FLOAT(
                "height", CYAML_FLAG_DEFAULT, TextAreaData, height),
        CYAML_FIELD_END,
};

static const cyaml_schema_value_t textAreaSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            TextAreaData, textAreaFieldSchema),
};

static const cyaml_schema_field_t fontsTopMappingField[] = {
        CYAML_FIELD_MAPPING_PTR(
                "sprite", CYAML_FLAG_POINTER, UIData, sprite, spritesheetFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "fonts", CYAML_FLAG_POINTER, UIData, fonts, familiesFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "screen", CYAML_FLAG_POINTER, UIData, screen, screenFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "menu", CYAML_FLAG_POINTER, UIData, menu, menuFieldSchema),
        CYAML_FIELD_SEQUENCE_COUNT(
                "textAreas", CYAML_FLAG_POINTER, UIData, textAreas,
                textAreasCount, &textAreaSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END,
};

static const cyaml_schema_value_t uiSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, UIData, fontsTopMappingField),
};
