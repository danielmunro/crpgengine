typedef struct {
    const char *name;
    int quantity;
} ItemReferenceData;

typedef struct {
    ItemReferenceData *items;
    int items_count;
} ItemsReferenceData;

static const cyaml_schema_field_t itemReferenceFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, ItemReferenceData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "quantity", CYAML_FLAG_OPTIONAL, ItemReferenceData, quantity),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t itemReferenceSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            ItemReferenceData, itemReferenceFieldSchema),
};

static const cyaml_schema_field_t itemsReferenceTopMappingField[] = {
        CYAML_FIELD_SEQUENCE(
                "items", CYAML_FLAG_POINTER, ItemsReferenceData, items,
                &itemReferenceSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t itemsReferenceSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, ItemsReferenceData, itemsReferenceTopMappingField),
};
