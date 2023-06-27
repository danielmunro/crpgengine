typedef struct {
    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
} AttributesData;

static const cyaml_schema_field_t attributesFieldSchema[] = {
        CYAML_FIELD_INT(
                "str", CYAML_FLAG_OPTIONAL, AttributesData, strength),
        CYAML_FIELD_INT(
                "dex", CYAML_FLAG_OPTIONAL, AttributesData, dexterity),
        CYAML_FIELD_INT(
                "int", CYAML_FLAG_OPTIONAL, AttributesData, intelligence),
        CYAML_FIELD_INT(
                "wis", CYAML_FLAG_OPTIONAL, AttributesData, wisdom),
        CYAML_FIELD_INT(
                "con", CYAML_FLAG_OPTIONAL, AttributesData, constitution),
        CYAML_FIELD_END
};
