typedef struct {
    int width;
    int height;
} ResolutionData;

typedef struct {
    bool fullScreen;
    ResolutionData *resolution;
    const char *difficulty;
} UserConfigData;

static const cyaml_schema_field_t resolutionFieldSchema[] = {
        CYAML_FIELD_INT(
                "width", CYAML_FLAG_DEFAULT, ResolutionData, width),
        CYAML_FIELD_INT(
                "height", CYAML_FLAG_DEFAULT, ResolutionData , height),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t userConfigTopMappingField[] = {
        CYAML_FIELD_BOOL(
                "fullScreen", CYAML_BOOL, UserConfigData, fullScreen),
        CYAML_FIELD_MAPPING_PTR(
                "resolution", CYAML_FLAG_POINTER, UserConfigData, resolution, resolutionFieldSchema),
        CYAML_FIELD_STRING_PTR(
                "difficulty", CYAML_FLAG_POINTER, UserConfigData, difficulty, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t userConfigTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, UserConfigData, userConfigTopMappingField),
};
