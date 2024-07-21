#include <stdbool.h>

typedef struct {
    bool objects;
    bool player;
    bool warps;
} ShowCollisionsData;

typedef struct {
    const char *indexDir;
    const char *startScene;
    const char *overrideStartScene;
    const char *logLevel;
    bool exit;
    bool immortal;
    bool validate;
    bool randomEncounters;
    bool logMemoryUsage;
    bool purgeSaves;
    bool showFPS;
    ShowCollisionsData *showCollisions;
    int tileSize;
} ConfigData;

static const cyaml_schema_field_t showCollisionsFieldSchema[] = {
        CYAML_FIELD_BOOL(
                "objects", CYAML_BOOL, ShowCollisionsData, objects),
        CYAML_FIELD_BOOL(
                "player", CYAML_BOOL, ShowCollisionsData, player),
        CYAML_FIELD_BOOL(
                "warps", CYAML_BOOL, ShowCollisionsData, warps),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t configTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "indexDir", CYAML_FLAG_POINTER, ConfigData, indexDir, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "startScene", CYAML_FLAG_POINTER, ConfigData, startScene, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "overrideStartScene", CYAML_FLAG_POINTER, ConfigData, overrideStartScene, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "logLevel", CYAML_FLAG_POINTER, ConfigData, logLevel, 0, CYAML_UNLIMITED),
        CYAML_FIELD_BOOL(
                "exit", CYAML_BOOL, ConfigData, exit),
        CYAML_FIELD_BOOL(
                "immortal", CYAML_BOOL, ConfigData, immortal),
        CYAML_FIELD_BOOL(
                "validate", CYAML_BOOL, ConfigData, validate),
        CYAML_FIELD_BOOL(
                "randomEncounters", CYAML_BOOL, ConfigData, randomEncounters),
        CYAML_FIELD_BOOL(
                "logMemoryUsage", CYAML_BOOL, ConfigData, logMemoryUsage),
        CYAML_FIELD_BOOL(
                "purgeSaves", CYAML_BOOL, ConfigData, purgeSaves),
        CYAML_FIELD_BOOL(
                "showFPS", CYAML_BOOL, ConfigData, showFPS),
        CYAML_FIELD_MAPPING_PTR(
                "showCollisions", CYAML_FLAG_POINTER, ConfigData, showCollisions, showCollisionsFieldSchema),
        CYAML_FIELD_INT(
                "tileSize", CYAML_FLAG_DEFAULT, ConfigData, tileSize),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t configTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, ConfigData, configTopMappingField),
};
