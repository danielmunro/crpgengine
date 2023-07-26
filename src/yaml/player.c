typedef struct {
    const char *name;
    const char *animations;
    const char *position;
    const char **storylines;
    int storylines_count;
    int coins;
    int secondsPlayed;
    int experience;
    int level;
} PlayerData;

static const cyaml_schema_value_t playerStorylinesEntry = {
        CYAML_VALUE_STRING(CYAML_FLAG_DEFAULT, char *, 0, 255),
};

static const cyaml_schema_field_t playerTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, PlayerData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "animations", CYAML_FLAG_POINTER, PlayerData, animations, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "position", CYAML_FLAG_POINTER, PlayerData, position, 0, CYAML_UNLIMITED),
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
        CYAML_FIELD_END
};

static const cyaml_schema_value_t playerTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, PlayerData, playerTopMappingField),
};
