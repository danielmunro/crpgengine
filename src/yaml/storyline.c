typedef struct {
    char *player;
    char *mob;
    char *condition;
    char *story;
} WhenData;

typedef struct {
    int player;
    char *mob;
    char *action;
    char *message;
    char *story;
    int *position;
    int position_count;
} ThenData;

typedef struct {
    WhenData *when;
    ThenData *then;
    int when_count;
    int then_count;
} StorylineData;

typedef struct {
    StorylineData *storylines;
    int storylines_count;
} StorylinesData;

static const cyaml_schema_value_t thenPositionEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t whenFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "player", CYAML_FLAG_POINTER, WhenData, player, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "condition", CYAML_FLAG_POINTER, WhenData, condition, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "mob", CYAML_FLAG_OPTIONAL, WhenData, mob, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "story", CYAML_FLAG_OPTIONAL, WhenData, story, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t whenSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, WhenData, whenFieldSchema),
};

static const cyaml_schema_field_t thenFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "mob", CYAML_FLAG_OPTIONAL, ThenData, mob, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "player", CYAML_FLAG_OPTIONAL, ThenData, player, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "action", CYAML_FLAG_POINTER, ThenData, action, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "message", CYAML_FLAG_OPTIONAL, ThenData, message, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "story", CYAML_FLAG_OPTIONAL, ThenData, story, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "position", CYAML_FLAG_POINTER_NULL, ThenData, position, &thenPositionEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t thenSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, WhenData, thenFieldSchema),
};

static const cyaml_schema_field_t storylinesFieldSchema[] = {
        CYAML_FIELD_SEQUENCE(
                "when", CYAML_FLAG_POINTER, StorylineData, when, &whenSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "then", CYAML_FLAG_POINTER, StorylineData, then, &thenSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t storylinesSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            WhenData, storylinesFieldSchema),
};

static const cyaml_schema_field_t storylinesTopMappingField[] = {
        CYAML_FIELD_SEQUENCE(
                "storylines", CYAML_FLAG_POINTER, StorylinesData, storylines, &storylinesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t storylinesTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, StorylinesData, storylinesTopMappingField),
};
