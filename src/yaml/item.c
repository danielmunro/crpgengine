typedef struct {
    const char *name;
    const char *type;
    const char *position;
    int worth;
    AttributesData *attributes;
} ItemData;

typedef struct {
    ItemData *items;
    int items_count;
} ItemsData;

static const cyaml_schema_field_t itemFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, ItemData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "type", CYAML_FLAG_POINTER, ItemData, type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "position", CYAML_FLAG_OPTIONAL, ItemData, position, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "worth", CYAML_FLAG_OPTIONAL, ItemData, worth),
        CYAML_FIELD_MAPPING_PTR(
                "attributes", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                ItemData, attributes, attributesFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t itemSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            ItemData, itemFieldSchema),
};

static const cyaml_schema_field_t itemsTopMappingField[] = {
        CYAML_FIELD_SEQUENCE(
                "items", CYAML_FLAG_POINTER, ItemsData, items, &itemSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t itemsSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, ItemData, itemsTopMappingField),
};
