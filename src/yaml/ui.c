typedef struct {
    const char *name;
    const char *filename;
    const char *color;
    int size;
    int lineHeight;
} FontData;

typedef struct {
    FontData *default_;
    FontData *disable;
    FontData *highlight;
    FontData *warning;
    FontData *danger;
} FontsData;

typedef struct {
    FontsData *fonts;
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

static const cyaml_schema_field_t fontsTopMappingField[] = {
        CYAML_FIELD_MAPPING_PTR(
                "fonts", CYAML_FLAG_POINTER, UIData, fonts, familiesFieldSchema),
        CYAML_FIELD_END,
};

static const cyaml_schema_value_t uiSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, UIData, fontsTopMappingField),
};
