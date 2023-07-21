typedef struct {
    char *name;
    char *animation;
    AttributesData *attributes;
} MobGroupData;

typedef struct {
    char *type;
    int quantity;
} SaveItemData;

typedef struct {
    char *storyline;
} SaveStorylineData;

typedef struct {
    int coins;
    char *scene;
    char *position;
    int secondsPlayed;
    SaveStorylineData *storylines;
    int storylines_count;
    SaveItemData *items;
    int items_count;
    MobGroupData *party;
    int party_count;
    MobGroupData *onDeck;
    int onDeck_count;
} SaveData;

SaveData *createSaveData(
        char *scene,
        Vector2 position,
        int coins,
        int secondsPlayed,
        SaveItemData *saveItemData,
        int itemsCount,
        MobGroupData *party,
        int partyCount,
        MobGroupData *onDeck,
        int onDeckCount) {
    SaveData *save = malloc(sizeof(SaveData));
    save->scene = scene;
    char pos[255];
    sprintf(pos, "%f, %f", position.x, position.y);
    save->position = &pos[0];
    save->coins = coins;
    save->secondsPlayed = secondsPlayed;
    save->items = saveItemData;
    save->items_count = itemsCount;
    save->party = party;
    save->party_count = partyCount;
    save->onDeck = onDeck;
    save->onDeck_count = onDeckCount;
    return save;
}

static const cyaml_schema_field_t mobGroupFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, MobGroupData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "animation", CYAML_FLAG_POINTER, MobGroupData, animation, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "attributes", CYAML_FLAG_POINTER, MobGroupData, attributes, attributesFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t partySchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            MobGroupData, mobGroupFieldSchema),
};

static const cyaml_schema_value_t saveStorylinesEntry = {
        CYAML_VALUE_STRING(CYAML_FLAG_DEFAULT, char *, 0, 255),
};

static const cyaml_schema_field_t saveItemsFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "type", CYAML_FLAG_POINTER, SaveItemData , type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "quantity", CYAML_FLAG_OPTIONAL, SaveItemData , quantity),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t saveItemsSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            SaveItemData, saveItemsFieldSchema),
};

static const cyaml_schema_field_t saveTopMappingField[] = {
        CYAML_FIELD_INT(
                "coins", CYAML_FLAG_SCALAR_PLAIN, SaveData, coins),
        CYAML_FIELD_STRING_PTR(
                "scene", CYAML_FLAG_POINTER, SaveData, scene, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "position", CYAML_FLAG_POINTER, SaveData, position, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "secondsPlayed", CYAML_FLAG_SCALAR_PLAIN, SaveData, secondsPlayed),
        CYAML_FIELD_SEQUENCE(
                "storylines", CYAML_FLAG_POINTER, SaveData, storylines,
                &saveStorylinesEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "items", CYAML_SEQUENCE, SaveData, items,
                &saveItemsSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "party", CYAML_SEQUENCE,
                SaveData, party,
                &partySchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "onDeck", CYAML_SEQUENCE,
                SaveData, onDeck,
                &partySchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t saveTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SaveData, saveTopMappingField),
};
