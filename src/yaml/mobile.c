typedef struct MobileYaml {
    char *id;
    char *name;
    char *animations;
    int *position;
    char *direction;
    int position_count;
} MobileYaml;

static const cyaml_schema_value_t mobilePositionEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t mobileTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "id", CYAML_FLAG_POINTER, MobileYaml, id, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, MobileYaml, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "animations", CYAML_FLAG_POINTER, MobileYaml, animations, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "position", CYAML_FLAG_POINTER, MobileYaml, position, &mobilePositionEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "direction", CYAML_FLAG_POINTER, MobileYaml, direction, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t mobileTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, MobileYaml, mobileTopMappingField),
};
