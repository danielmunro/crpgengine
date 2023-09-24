typedef struct {
    const char *id;
    int max;
} BeastEncounterData;

typedef struct {
    BeastEncounterData *beasts;
    int beasts_count;
    const char *background;
} EncountersData;

typedef struct {
    const char *type;
    const char *music;
    EncountersData *encounters;
} SceneData;

static const cyaml_schema_field_t beastsFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "id", CYAML_FLAG_POINTER, BeastEncounterData, id, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "max", CYAML_FLAG_POINTER, BeastEncounterData, max),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t beastsSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, BeastEncounterData, beastsFieldSchema),
};

static const cyaml_schema_field_t encountersFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "background", CYAML_FLAG_POINTER, EncountersData, background, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "beasts", CYAML_FLAG_POINTER, EncountersData, beasts, &beastsSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t sceneTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "type", CYAML_FLAG_POINTER, SceneData, type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "encounters", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SceneData, encounters, encountersFieldSchema),
        CYAML_FIELD_STRING_PTR(
                "music", CYAML_FLAG_POINTER, SceneData, music, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t sceneTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SceneData, sceneTopMappingField),
};
