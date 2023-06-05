typedef struct MobileYaml {
    char *id;
} MobileYaml;

static const cyaml_schema_field_t mobileTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "id", CYAML_FLAG_POINTER, MobileYaml, id, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t mobileTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, MobileYaml, mobileTopMappingField),
};
