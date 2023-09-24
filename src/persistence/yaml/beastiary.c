typedef struct {
    const char *id;
    const char *name;
    const char *image;
    AttributesData *attributes;
    int attributes_count;
    int level;
    int hp;
    int mana;
    int gp;
} BeastData;

typedef struct {
    BeastData *beasts;
    int beasts_count;
} BeastiaryData;

static const cyaml_schema_field_t beastFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "id", CYAML_FLAG_POINTER, BeastData, id, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "name", CYAML_FLAG_POINTER, BeastData, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "image", CYAML_FLAG_POINTER, BeastData, image, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "level", CYAML_FLAG_OPTIONAL, BeastData, level),
        CYAML_FIELD_INT(
                "hp", CYAML_FLAG_OPTIONAL, BeastData, hp),
        CYAML_FIELD_INT(
                "mana", CYAML_FLAG_OPTIONAL, BeastData, mana),
        CYAML_FIELD_INT(
                "gp", CYAML_FLAG_OPTIONAL, BeastData, gp),
        CYAML_FIELD_MAPPING_PTR(
                "attributes", CYAML_FLAG_POINTER, BeastData, attributes, attributesFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t beastSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            BeastData, beastFieldSchema),
};

static const cyaml_schema_field_t beastiaryTopMappingField[] = {
        CYAML_FIELD_SEQUENCE(
                "beasts", CYAML_FLAG_POINTER, BeastiaryData, beasts, &beastSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t beastiaryTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, BeastiaryData, beastiaryTopMappingField),
};
