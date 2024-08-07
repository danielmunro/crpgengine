#include <stdbool.h>
#include "headers/persistence/cyaml.h"

typedef struct {
    const char *name;
} SpriteData;

typedef struct {
    const char *name;
    int frameStart;
    int frameEnd;
    int rate;
    bool repeat;
    const char *display;
    const char *position;
    const char *playUntil;
} SliceData;

typedef struct {
    const char *name;
    SpriteData *sprite;
    SliceData *slices;
    int slices_count;
} AnimationData;

static const cyaml_schema_field_t spriteSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, SpriteData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t slicesFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, SliceData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "frameStart", CYAML_FLAG_DEFAULT, SliceData, frameStart),
        CYAML_FIELD_INT(
                "frameEnd", CYAML_FLAG_DEFAULT, SliceData, frameEnd),
        CYAML_FIELD_INT(
                "rate", CYAML_FLAG_OPTIONAL, SliceData, rate),
        CYAML_FIELD_BOOL(
                "repeat", CYAML_FLAG_OPTIONAL, SliceData, repeat),
        CYAML_FIELD_STRING_PTR(
                "display", CYAML_FLAG_POINTER, SliceData, display, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "position", CYAML_FLAG_POINTER, SliceData, position, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "playUntil", CYAML_FLAG_POINTER, SliceData, playUntil, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t slicesSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            SliceData, slicesFieldSchema),
};

static const cyaml_schema_field_t animationTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, AnimationData, name, 0, CYAML_UNLIMITED),
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
