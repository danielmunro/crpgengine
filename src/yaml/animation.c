typedef struct SpriteYaml {
    char *file;
    int *size;
    int size_count;
} SpriteYaml;

typedef struct SliceYaml {
    char *name;
    char *frames;
    int frames_count;
    int rate;
    int repeat;
} SliceYaml;

typedef struct AnimationYaml {
    SpriteYaml *sprite;
    SliceYaml *slices;
    int slices_count;
} AnimationYaml;

static const cyaml_schema_value_t sizeEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t spriteSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "file", CYAML_FLAG_POINTER, SpriteYaml, file, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "size", CYAML_FLAG_POINTER, SpriteYaml, size, &sizeEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t framesEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t slicesFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, SliceYaml, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "frames", CYAML_FLAG_POINTER, SliceYaml, frames, &framesEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_UINT(
                "rate", CYAML_FLAG_OPTIONAL, SliceYaml, rate),
        CYAML_FIELD_INT(
                "repeat", CYAML_FLAG_OPTIONAL, SliceYaml, repeat),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t slicesSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            SliceYaml, slicesFieldSchema),
};

static const cyaml_schema_field_t animationTopMappingField[] = {
        CYAML_FIELD_MAPPING_PTR(
                "sprite", CYAML_FLAG_POINTER, AnimationYaml, sprite, spriteSchema),
        CYAML_FIELD_SEQUENCE(
                "slices", CYAML_FLAG_POINTER,
                AnimationYaml, slices,
                &slicesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t animationTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, AnimationYaml, animationTopMappingField),
};