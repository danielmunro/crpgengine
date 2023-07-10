typedef struct {
    char *file;
    int *size;
    int size_count;
} SpriteData;

typedef struct {
    char *name;
    char *frames;
    int frames_count;
    int rate;
    bool repeat;
} SliceData;

typedef struct {
    SpriteData *sprite;
    SliceData *slices;
    int slices_count;
} AnimationData;

static const cyaml_schema_value_t sizeEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t spriteSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "file", CYAML_FLAG_POINTER, SpriteData, file, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "size", CYAML_FLAG_POINTER, SpriteData, size, &sizeEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t framesEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t slicesFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, SliceData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "frames", CYAML_FLAG_POINTER, SliceData, frames, &framesEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "rate", CYAML_FLAG_OPTIONAL, SliceData, rate),
        CYAML_FIELD_BOOL(
                "repeat", CYAML_FLAG_OPTIONAL, SliceData, repeat),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t slicesSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            SliceData, slicesFieldSchema),
};

static const cyaml_schema_field_t animationTopMappingField[] = {
        CYAML_FIELD_MAPPING_PTR(
                "sprite", CYAML_FLAG_POINTER, AnimationData, sprite, spriteSchema),
        CYAML_FIELD_SEQUENCE(
                "slices", CYAML_FLAG_POINTER,
                AnimationData, slices,
                &slicesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t animationTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, AnimationData, animationTopMappingField),
};
