typedef struct PlayerYaml {
    char *name;
    char *animations;
    int *position;
    char *direction;
    int position_count;
} PlayerYaml;

static const cyaml_schema_value_t playerPositionEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t playerTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, PlayerYaml, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "animations", CYAML_FLAG_POINTER, PlayerYaml, animations, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "position", CYAML_FLAG_POINTER, PlayerYaml, position, &playerPositionEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "direction", CYAML_FLAG_POINTER, PlayerYaml, direction, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t playerTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, PlayerYaml, playerTopMappingField),
};
