#include "headers/persistence/cyaml.h"

typedef struct {
    int width;
    int height;
    int left;
    int top;
} CollideData;

typedef struct {
    int width;
    int height;
    int padding;
} FrameData;

typedef struct {
    const char *name;
    const char *filename;
    FrameData *frame;
    CollideData *collide;
} SpritesheetData;

static const cyaml_schema_field_t frameFieldSchema[] = {
        CYAML_FIELD_INT(
                "width", CYAML_FLAG_POINTER, FrameData, width),
        CYAML_FIELD_INT(
                "height", CYAML_FLAG_POINTER, FrameData, height),
        CYAML_FIELD_INT(
                "padding", CYAML_FLAG_POINTER, FrameData, padding),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t collisionFieldSchema[] = {
        CYAML_FIELD_INT(
                "width", CYAML_FLAG_POINTER, CollideData, width),
        CYAML_FIELD_INT(
                "height", CYAML_FLAG_POINTER, CollideData, height),
        CYAML_FIELD_INT(
                "left", CYAML_FLAG_POINTER, CollideData, left),
        CYAML_FIELD_INT(
                "top", CYAML_FLAG_POINTER, CollideData, top),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t spritesheetTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, SpritesheetData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "filename", CYAML_FLAG_POINTER, SpritesheetData, filename, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "frame", CYAML_FLAG_POINTER, SpritesheetData, frame, frameFieldSchema),
        CYAML_FIELD_MAPPING_PTR(
                "collide", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SpritesheetData,
                collide, collisionFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t spritesheetTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SpritesheetData, spritesheetTopMappingField),
};
