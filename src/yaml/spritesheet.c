typedef struct {
    int width;
    int height;
} FrameData;

typedef struct {
    char *name;
    char *filename;
    FrameData *frame;
} SpritesheetData;

static const cyaml_schema_field_t frameFieldSchema[] = {
        CYAML_FIELD_INT(
                "width", CYAML_FLAG_POINTER, FrameData, width),
        CYAML_FIELD_INT(
                "height", CYAML_FLAG_POINTER, FrameData, height),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t spritesheetTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, SpritesheetData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "filename", CYAML_FLAG_POINTER, SpritesheetData, filename, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "frame", CYAML_FLAG_POINTER, SpritesheetData, frame, frameFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t spritesheetTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SpritesheetData, spritesheetTopMappingField),
};
