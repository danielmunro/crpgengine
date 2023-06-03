typedef struct MusicYaml {
    char *town;
} MusicYaml;

static const cyaml_schema_field_t musicTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "town", CYAML_FLAG_POINTER, MusicYaml , town, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t musicTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, MusicYaml, musicTopMappingField)
};