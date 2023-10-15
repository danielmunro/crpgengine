#include <mm_malloc.h>
#include "headers/persistence/cyaml.h"

typedef struct {
    int bash;
    int slash;
    int pierce;
    int fire;
    int frost;
    int shock;
    int blast;
} ArmorClass;

typedef struct {
    int strength;
    int dexterity;
    int intelligence;
    int wisdom;
    int constitution;
    int hp;
    int mana;
    ArmorClass ac;
} Attributes;

Attributes *createEmptyAttributes() {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = 0;
    a->intelligence = 0;
    a->wisdom = 0;
    a->dexterity = 0;
    a->constitution = 0;
    a->hp = 0;
    a->mana = 0;
    a->ac = (ArmorClass) {
            0, 0, 0, 0, 0, 0, 0,
    };

    return a;
}

ArmorClass createEmptyArmorClass() {
    return (ArmorClass) {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
    };
}

Attributes *createAttributesFromData(const AttributesData *data) {
    if (data == NULL) {
        return NULL;
    }
    Attributes *a = createEmptyAttributes();
    a->strength = data->strength;
    a->intelligence = data->intelligence;
    a->wisdom = data->wisdom;
    a->dexterity = data->dexterity;
    a->constitution = data->constitution;
    a->hp = data->hp;
    a->mana = data->mana;
    a->ac = data->ac == NULL ? createEmptyArmorClass() : (ArmorClass) {
            data->ac->bash,
            data->ac->slash,
            data->ac->pierce,
            data->ac->fire,
            data->ac->frost,
            data->ac->shock,
            data->ac->blast,
    };
    return a;
}

AttributesData *createDataFromAttributes(const Attributes *a) {
    if (a == NULL) {
        return NULL;
    }
    AttributesData *data = malloc(sizeof(AttributesData));
    data->strength = a->strength;
    data->dexterity = a->dexterity;
    data->intelligence = a->intelligence;
    data->wisdom = a->wisdom;
    data->constitution = a->constitution;
    data->hp = a->hp;
    data->mana = a->mana;
    data->ac = NULL;
    ArmorClassData *ac = malloc(sizeof(ArmorClass));
    ac->bash = a->ac.bash;
    ac->slash = a->ac.slash;
    ac->pierce = a->ac.pierce;
    ac->fire = a->ac.fire;
    ac->frost = a->ac.frost;
    ac->shock = a->ac.shock;
    ac->blast = a->ac.blast;
    data->ac = ac;
    return data;
}

Attributes *cloneAttributes(const Attributes *a) {
    Attributes *b = malloc(sizeof(Attributes));
    *b = *a;
    return b;
}

Attributes combineAttributes(const Attributes a, const Attributes b) {
    return (Attributes) {
            a.strength + b.strength,
            a.dexterity + b.dexterity,
            a.intelligence + b.intelligence,
            a.wisdom + b.wisdom,
            a.constitution + b.constitution,
            a.hp + b.hp,
            a.mana + b.mana,
            (ArmorClass) {
                    a.ac.bash + b.ac.bash,
                    a.ac.slash + b.ac.slash,
                    a.ac.pierce + b.ac.pierce,
                    a.ac.fire + b.ac.fire,
                    a.ac.frost + b.ac.frost,
                    a.ac.shock + b.ac.shock,
                    a.ac.blast + b.ac.blast,
            }
    };
}
