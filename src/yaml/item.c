typedef struct {
    const char *name;
    int quantity;
} ItemData;

static const cyaml_schema_field_t saveItemsFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, ItemData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "quantity", CYAML_FLAG_OPTIONAL, ItemData, quantity),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t saveItemsSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            ItemData, saveItemsFieldSchema),
};
