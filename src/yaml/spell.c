typedef struct {
    char *type;
    char *intent;
    int level;
    AttributesData *cost;
} SpellData;

typedef struct {
    SpellData *spells;
    int spells_count;
} SpellsData;

static const cyaml_schema_field_t spellFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "type", CYAML_FLAG_POINTER, SpellData, type, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "intent", CYAML_FLAG_POINTER, SpellData, intent, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT(
                "level", CYAML_FLAG_OPTIONAL, SpellData, level),
        CYAML_FIELD_MAPPING_PTR(
                "cost", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                SpellData, cost, attributesFieldSchema),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t spellSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, SpellData, spellFieldSchema),
};

static const cyaml_schema_field_t spellTopMappingField[] = {
        CYAML_FIELD_SEQUENCE(
                "spells", CYAML_FLAG_POINTER, SpellsData, spells, &spellSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t spellsTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, SpellsData, spellTopMappingField),
};
