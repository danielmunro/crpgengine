typedef struct {
    const char *name;
    int quantity;
} PlayerItemData;

static const cyaml_schema_field_t savePlayerItemsFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, PlayerItemData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "quantity", CYAML_FLAG_OPTIONAL, PlayerItemData, quantity),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t savePlayerItemsSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            PlayerItemData, savePlayerItemsFieldSchema),
};
