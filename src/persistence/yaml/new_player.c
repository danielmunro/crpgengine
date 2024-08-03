#include "headers/persistence/cyaml.h"

typedef struct {
    const char **party;
    int partyCount;
    const ItemReferenceData *items;
    int itemsCount;
    int coins;
} NewPlayerData;

static const cyaml_schema_value_t newPlayerMobilesSchema = {
        CYAML_VALUE_STRING(CYAML_FLAG_POINTER, char, 0, CYAML_UNLIMITED),
};

static const cyaml_schema_value_t newPlayerItemsSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, ItemReferenceData, itemReferenceFieldSchema),
};


static const cyaml_schema_field_t newPlayerTopMappingField[] = {
        CYAML_FIELD_SEQUENCE_COUNT(
                "party", CYAML_FLAG_POINTER,
                NewPlayerData, party, partyCount,
                &newPlayerMobilesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE_COUNT(
                "items", CYAML_FLAG_POINTER,
                NewPlayerData, items, itemsCount,
                &newPlayerItemsSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "coins", CYAML_FLAG_DEFAULT, NewPlayerData, coins),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t newPlayerTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, NewPlayerData, newPlayerTopMappingField),
};
