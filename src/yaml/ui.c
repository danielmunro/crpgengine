typedef struct {
    const char *default_;
    const char *disable;
    const char *highlight;
    const char *warning;
    const char *danger;
} FontColorsData;

typedef struct {
    const char *name;
    const char *filename;
    const char *color;
    int size;
    int lineHeight;
} FontFamilyData;

typedef struct {
    FontFamilyData *default_;
    FontFamilyData *disable;
    FontFamilyData *highlight;
    FontFamilyData *warning;
    FontFamilyData *danger;
} FontFamiliesData;

typedef struct {
    FontFamiliesData *families;
} FontsData;

typedef struct {
    FontsData *fonts;
} UIData;

static const cyaml_schema_field_t fontFamilyFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, FontFamilyData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "filename", CYAML_FLAG_POINTER, FontFamilyData, filename, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "color", CYAML_FLAG_POINTER, FontFamilyData, color, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "size", CYAML_FLAG_DEFAULT, FontFamilyData, size),
        CYAML_FIELD_INT(
                "lineHeight", CYAML_FLAG_DEFAULT, FontFamilyData, lineHeight),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t fontColorsFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "default", CYAML_FLAG_POINTER, FontColorsData, default_, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "disable", CYAML_FLAG_POINTER, FontColorsData, disable, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "highlight", CYAML_FLAG_POINTER, FontColorsData, highlight, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "warning", CYAML_FLAG_POINTER, FontColorsData, warning, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "danger", CYAML_FLAG_POINTER, FontColorsData, danger, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t familiesFieldSchema[] = {
        CYAML_FIELD_MAPPING_PTR(
                "default", CYAML_FLAG_POINTER, FontFamiliesData, default_, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "disable", CYAML_FLAG_POINTER, FontFamiliesData, disable, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "highlight", CYAML_FLAG_POINTER, FontFamiliesData, highlight, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "warning", CYAML_FLAG_POINTER, FontFamiliesData, warning, fontFamilyFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "danger", CYAML_FLAG_POINTER, FontFamiliesData, danger, fontFamilyFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_value_t fontFamilyTypeEntry = {
        CYAML_VALUE_STRING(CYAML_FLAG_POINTER, char, 0, 255),
};

static const cyaml_schema_field_t fontsFieldSchema[] = {
        CYAML_FIELD_MAPPING_PTR(
                "families", CYAML_FLAG_POINTER, FontsData, families, familiesFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_field_t fontsTopMappingField[] = {
        CYAML_FIELD_MAPPING_PTR(
                "fonts", CYAML_FLAG_POINTER, UIData, fonts, fontsFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_value_t uiSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, UIData, fontsTopMappingField),
};
