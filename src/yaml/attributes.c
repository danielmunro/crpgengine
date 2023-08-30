typedef struct {
    int bash;
    int slash;
    int pierce;
    int fire;
    int water;
    int frost;
    int electricity;
    int dark;
    int light;
    int corrosive;
    int poison;
    int energy;
} ArmorClassData;

typedef struct {
    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
    int hp;
    int mana;
    ArmorClassData *ac;
} AttributesData;

AttributesData *createEmptyAttributesData() {
    AttributesData *a = malloc(sizeof(AttributesData));
    a->strength = 0;
    a->dexterity = 0;
    a->constitution = 0;
    a->wisdom = 0;
    a->intelligence = 0;
    a->hp = 0;
    a->mana = 0;
    a->ac = NULL;
    return a;
}

static const cyaml_schema_field_t acFieldSchema[] = {
        CYAML_FIELD_INT(
                "bash", CYAML_FLAG_OPTIONAL, ArmorClassData, bash),
        CYAML_FIELD_INT(
                "slash", CYAML_FLAG_OPTIONAL, ArmorClassData, slash),
        CYAML_FIELD_INT(
                "pierce", CYAML_FLAG_OPTIONAL, ArmorClassData, pierce),
        CYAML_FIELD_INT(
                "fire", CYAML_FLAG_OPTIONAL, ArmorClassData, fire),
        CYAML_FIELD_INT(
                "water", CYAML_FLAG_OPTIONAL, ArmorClassData, water),
        CYAML_FIELD_INT(
                "frost", CYAML_FLAG_OPTIONAL, ArmorClassData, frost),
        CYAML_FIELD_INT(
                "electricity", CYAML_FLAG_OPTIONAL, ArmorClassData, electricity),
        CYAML_FIELD_INT(
                "dark", CYAML_FLAG_OPTIONAL, ArmorClassData, dark),
        CYAML_FIELD_INT(
                "light", CYAML_FLAG_OPTIONAL, ArmorClassData, light),
        CYAML_FIELD_INT(
                "corrosive", CYAML_FLAG_OPTIONAL, ArmorClassData, corrosive),
        CYAML_FIELD_INT(
                "poison", CYAML_FLAG_OPTIONAL, ArmorClassData, poison),
        CYAML_FIELD_INT(
                "energy", CYAML_FLAG_OPTIONAL, ArmorClassData, energy),
};

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
        CYAML_FIELD_INT(
                "hp", CYAML_FLAG_OPTIONAL, AttributesData, hp),
        CYAML_FIELD_INT(
                "mana", CYAML_FLAG_OPTIONAL, AttributesData, mana),
        CYAML_FIELD_MAPPING_PTR(
                "ac", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                AttributesData, ac, acFieldSchema),
        CYAML_FIELD_END
};
