typedef struct SceneData {
    char *type;
    char *music;
} SceneData;

static const cyaml_schema_field_t sceneTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "type", CYAML_FLAG_POINTER, SceneData, type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "music", CYAML_FLAG_POINTER, SceneData, music, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t sceneTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SceneData, sceneTopMappingField),
};
