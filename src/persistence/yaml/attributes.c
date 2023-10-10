#include "cyaml/cyaml.h"

typedef struct {
    int bash;
    int slash;
    int pierce;
    int fire;
    int frost;
    int shock;
    int blast;
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
                "frost", CYAML_FLAG_OPTIONAL, ArmorClassData, frost),
        CYAML_FIELD_INT(
                "shock", CYAML_FLAG_OPTIONAL, ArmorClassData, shock),
        CYAML_FIELD_INT(
                "blast", CYAML_FLAG_OPTIONAL, ArmorClassData, blast),
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
