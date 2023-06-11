typedef struct BeastEncounterData {
    char *id;
    int max;
} BeastEncounterData;

typedef struct WhenData {
    char *player;
    char *mob;
    char *condition;
    char *story;
} WhenData;

typedef struct ThenData {
    int player;
    char *mob;
    char *action;
    char *message;
    char *story;
} ThenData;

typedef struct StorylineData {
    WhenData *when;
    ThenData *then;
    int when_count;
    int then_count;
} StorylineData;

typedef struct FightData {
    BeastEncounterData *beasts;
    int beasts_count;
    char *background;
} FightData;

typedef struct SceneData {
    char *type;
    char *music;
    StorylineData *storylines;
    int storylines_count;
    FightData *fight;
} SceneData;

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

static const cyaml_schema_field_t beastsFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "id", CYAML_FLAG_POINTER, BeastEncounterData, id, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "max", CYAML_FLAG_POINTER, BeastEncounterData, max),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t beastsSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, BeastEncounterData, beastsFieldSchema),
};

static const cyaml_schema_field_t fightFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "background", CYAML_FLAG_POINTER, FightData, background, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "beasts", CYAML_FLAG_POINTER, FightData, beasts, &beastsSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t sceneTopMappingField[] = {
        CYAML_FIELD_STRING_PTR(
                "type", CYAML_FLAG_POINTER, SceneData, type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "fight", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SceneData, fight, fightFieldSchema),
        CYAML_FIELD_STRING_PTR(
                "music", CYAML_FLAG_POINTER, SceneData, music, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "storylines", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, SceneData, storylines, &storylinesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t sceneTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SceneData, sceneTopMappingField),
};
