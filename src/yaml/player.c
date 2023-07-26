typedef struct {
    const char *name;
    const char *animations;
} PlayerData;

static const cyaml_schema_field_t playerTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, PlayerData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "animations", CYAML_FLAG_POINTER, PlayerData, animations, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t playerTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, PlayerData, playerTopMappingField),
};
