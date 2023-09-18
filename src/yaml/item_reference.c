typedef struct {
    const char *name;
    int quantity;
} ItemReferenceData;

static const cyaml_schema_field_t itemsReferenceFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, ItemReferenceData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "quantity", CYAML_FLAG_OPTIONAL, ItemReferenceData, quantity),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t itemsReferenceSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            ItemReferenceData, itemsReferenceFieldSchema),
};
