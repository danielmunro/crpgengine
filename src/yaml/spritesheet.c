typedef struct {
    int width;
    int height;
} FrameData;

typedef struct {
    FrameData *frame;
    char *spritesheet;
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
                "spritesheet", CYAML_FLAG_POINTER, SpritesheetData, spritesheet, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "frame", CYAML_FLAG_POINTER, SpritesheetData, frame, frameFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t spritesheetTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SpritesheetData, spritesheetTopMappingField),
};
