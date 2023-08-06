typedef struct {
    const char **storylines;
    int storylines_count;
    ItemData *items;
    int items_count;
    MobileData *party;
    int party_count;
    MobileData *onDeck;
    int onDeck_count;
    int coins;
    int secondsPlayed;
    int experience;
    int level;
} PlayerData;

static const cyaml_schema_value_t playerStorylinesEntry = {
        CYAML_VALUE_STRING(CYAML_FLAG_POINTER, char, 0, 255),
};

static const cyaml_schema_value_t partySchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, MobileData, mobileTopMappingField),
};

static const cyaml_schema_field_t playerTopMappingField[] = {
        CYAML_FIELD_SEQUENCE_COUNT(
                "storylines", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, PlayerData, storylines,
                storylines_count, &playerStorylinesEntry, 0, MAX_STORIES),
        CYAML_FIELD_INT(
                "coins", CYAML_FLAG_DEFAULT, PlayerData, coins),
        CYAML_FIELD_INT(
                "secondsPlayed", CYAML_FLAG_DEFAULT, PlayerData, secondsPlayed),
        CYAML_FIELD_INT(
                "experience", CYAML_FLAG_DEFAULT, PlayerData, experience),
        CYAML_FIELD_INT(
                "level", CYAML_FLAG_DEFAULT, PlayerData, level),
        CYAML_FIELD_SEQUENCE(
                "items", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, PlayerData, items,
                &saveItemsSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "party", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, PlayerData, party,
                &partySchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "onDeck", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, PlayerData, onDeck,
                &partySchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t playerTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, PlayerData, playerTopMappingField),
};
