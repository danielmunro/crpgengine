typedef struct {
    char *title;
} ConfigData;

static const cyaml_schema_field_t appConfigTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "title", CYAML_FLAG_POINTER, ConfigData, title, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t appConfigTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, ConfigData, appConfigTopMappingField),
};
