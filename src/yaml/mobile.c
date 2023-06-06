typedef struct WhenYaml {
    char *player;
    char *mob;
    char *condition;
    char *story;
} WhenYaml;

typedef struct ThenYaml {
    int player;
    char *mob;
    char *action;
    char *message;
    char *story;
} ThenYaml;

typedef struct StorylineYaml {
    WhenYaml when[255];
    ThenYaml then[255];
    int when_count;
    int then_count;
} StorylineYaml;

typedef struct MobileYaml {
    char *id;
    char *name;
    char *animations;
    int *position;
    int position_count;
    char *direction;
    StorylineYaml storylines[255];
    int storylines_count;
} MobileYaml;

static const cyaml_schema_value_t mobilePositionEntry = {
        CYAML_VALUE_INT(CYAML_FLAG_DEFAULT, int),
};

static const cyaml_schema_field_t whenFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "player", CYAML_FLAG_POINTER, WhenYaml, player, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "condition", CYAML_FLAG_POINTER, WhenYaml, condition, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "mob", CYAML_FLAG_OPTIONAL, WhenYaml, mob, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "story", CYAML_FLAG_OPTIONAL, WhenYaml, story, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t whenSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, WhenYaml, whenFieldSchema),
};

static const cyaml_schema_field_t storylinesFieldSchema[] = {
        CYAML_FIELD_SEQUENCE(
                "when", CYAML_FLAG_POINTER, StorylineYaml, when, &whenSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t storylinesSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            WhenYaml, storylinesFieldSchema),
};

static const cyaml_schema_field_t mobileTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "id", CYAML_FLAG_POINTER, MobileYaml, id, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, MobileYaml, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "animations", CYAML_FLAG_POINTER, MobileYaml, animations, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "position", CYAML_FLAG_POINTER, MobileYaml, position, &mobilePositionEntry, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "direction", CYAML_FLAG_POINTER, MobileYaml, direction, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "storylines", CYAML_FLAG_POINTER, MobileYaml, storylines, &storylinesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t mobileTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, MobileYaml, mobileTopMappingField),
};
