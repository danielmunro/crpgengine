typedef struct {
    const char *scene;
    PlayerData *player;
} SaveData;

static const cyaml_schema_field_t saveTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "scene", CYAML_FLAG_POINTER, SaveData, scene, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "player", CYAML_FLAG_POINTER, SaveData, player, playerTopMappingField),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t saveTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SaveData, saveTopMappingField),
};
