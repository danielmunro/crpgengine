typedef struct {
    const char *name;
    int quantity;
} SaveItemData;

typedef struct {
    const char *scene;
    const char *position;
    const char **storylines;
    PlayerData *player;
    int storylines_count;
    SaveItemData *items;
    int items_count;
    MobileData *party;
    int party_count;
    MobileData *onDeck;
    int onDeck_count;
} SaveData;

static const cyaml_schema_value_t partySchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, MobileData, mobileTopMappingField),
};

static const cyaml_schema_value_t saveStorylinesEntry = {
        CYAML_VALUE_STRING(CYAML_FLAG_DEFAULT, char *, 0, 255),
};

static const cyaml_schema_field_t saveItemsFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, SaveItemData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "quantity", CYAML_FLAG_OPTIONAL, SaveItemData, quantity),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t saveItemsSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            SaveItemData, saveItemsFieldSchema),
};

static const cyaml_schema_field_t saveTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "scene", CYAML_FLAG_POINTER, SaveData, scene, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "position", CYAML_FLAG_POINTER, SaveData, position, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "player", CYAML_FLAG_POINTER, SaveData, player, playerTopMappingField),
        CYAML_FIELD_SEQUENCE_COUNT(
                "storylines", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SaveData, storylines,
                storylines_count, &saveStorylinesEntry, 0, MAX_STORIES),
        CYAML_FIELD_SEQUENCE(
                "items", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SaveData, items,
                &saveItemsSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "party", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SaveData, party,
                &partySchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "onDeck", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SaveData, onDeck,
                &partySchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t saveTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SaveData, saveTopMappingField),
};
