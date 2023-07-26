typedef struct {
    const char *id;
    const char *name;
    const char *animations;
    const char *position;
    char *direction;
    AttributesData *attributes;
} MobileData;

static const cyaml_schema_field_t mobileTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "id", CYAML_FLAG_POINTER, MobileData, id, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, MobileData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "animations", CYAML_FLAG_POINTER, MobileData, animations, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "position", CYAML_FLAG_POINTER, MobileData, position, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "direction", CYAML_FLAG_POINTER, MobileData, direction, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "attributes", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                MobileData, attributes, attributesFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t mobileTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, MobileData, mobileTopMappingField),
};
