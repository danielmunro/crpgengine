typedef struct WhenYaml {
    char *player;
    char *mob;
    char *condition;
    char *story;
} WhenYaml;

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

static const cyaml_schema_value_t mobileTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, WhenYaml , whenFieldSchema),
};
