typedef struct {
    char *item;
    int cost;
} InventoryData;

typedef struct {
    int id;
    int coins;
    InventoryData *items;
    int items_count;
} ShopData;

static const cyaml_schema_field_t inventoryFieldSchema[] = {
        CYAML_FIELD_INT(
                "cost", CYAML_FLAG_POINTER, InventoryData, cost),
        CYAML_FIELD_STRING_PTR(
                "item", CYAML_FLAG_POINTER,
                InventoryData, item, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t inventorySchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            InventoryData, inventoryFieldSchema),
};

static const cyaml_schema_field_t shopTopMappingField[] = {
        CYAML_FIELD_INT(
                "id", CYAML_FLAG_POINTER, ShopData, id),
        CYAML_FIELD_INT(
                "coins", CYAML_FLAG_POINTER, ShopData, coins),
        CYAML_FIELD_SEQUENCE(
                "items", CYAML_FLAG_POINTER, ShopData, items,
                &inventorySchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t shopTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, ShopData, shopTopMappingField),
};
