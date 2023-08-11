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
} ArmorClass;

typedef struct {
    int strength;
    int dexterity;
    int intelligence;
    int wisdom;
    int constitution;
    int hp;
    int mana;
    ArmorClass *ac;
} Attributes;

Attributes *createAttributesFromData(AttributesData *data) {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = data->strength;
    a->dexterity = data->dexterity;
    a->intelligence = data->intelligence;
    a->wisdom = data->wisdom;
    a->constitution = data->constitution;
    a->hp = data->hp;
    a->mana = data->mana;
    a->ac = NULL;
    if (data->ac != NULL) {
        ArmorClass *ac = malloc(sizeof(ArmorClass));
        ac->bash = data->ac->bash;
        ac->slash = data->ac->slash;
        ac->pierce = data->ac->pierce;
        ac->fire = data->ac->fire;
        ac->water = data->ac->water;
        ac->frost = data->ac->frost;
        ac->electricity = data->ac->electricity;
        ac->dark = data->ac->dark;
        ac->light = data->ac->light;
        ac->corrosive = data->ac->corrosive;
        ac->poison = data->ac->poison;
        ac->energy = data->ac->energy;
        a->ac = ac;
    }
    return a;
}

AttributesData *createDataFromAttributes(Attributes *a) {
    AttributesData *data = malloc(sizeof(AttributesData));
    data->strength = a->strength;
    data->dexterity = a->dexterity;
    data->intelligence = a->intelligence;
    data->wisdom = a->wisdom;
    data->constitution = a->constitution;
    data->hp = a->hp;
    data->mana = a->mana;
    data->ac = NULL;
    if (a->ac != NULL) {
        ArmorClassData *ac = malloc(sizeof(ArmorClass));
        ac->bash = a->ac->bash;
        ac->slash = a->ac->slash;
        ac->pierce = a->ac->pierce;
        ac->fire = a->ac->fire;
        ac->water = a->ac->water;
        ac->frost = a->ac->frost;
        ac->electricity = a->ac->electricity;
        ac->dark = a->ac->dark;
        ac->light = a->ac->light;
        ac->corrosive = a->ac->corrosive;
        ac->poison = a->ac->poison;
        ac->energy = a->ac->energy;
        data->ac = ac;
    }
    return data;
}

Attributes *cloneAttributes(Attributes *original) {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = original->strength;
    a->intelligence = original->intelligence;
    a->wisdom = original->wisdom;
    a->dexterity = original->dexterity;
    a->constitution = original->constitution;
    a->hp = original->hp;
    a->mana = original->mana;
    a->ac = NULL;
    if (original->ac != NULL) {
        ArmorClass *ac = malloc(sizeof(ArmorClass));
        ac->bash = original->ac->bash;
        ac->slash = original->ac->slash;
        ac->pierce = original->ac->pierce;
        ac->fire = original->ac->fire;
        ac->water = original->ac->water;
        ac->frost = original->ac->frost;
        ac->electricity = original->ac->electricity;
        ac->dark = original->ac->dark;
        ac->light = original->ac->light;
        ac->corrosive = original->ac->corrosive;
        ac->poison = original->ac->poison;
        ac->energy = original->ac->energy;
        a->ac = ac;
    }
    return a;
}

Attributes *createEmptyAttributes() {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = 0;
    a->intelligence = 0;
    a->wisdom = 0;
    a->dexterity = 0;
    a->constitution = 0;
    a->hp = 0;
    a->mana = 0;
    ArmorClass *ac = malloc(sizeof(ArmorClass));
    ac->bash = 0;
    ac->slash = 0;
    ac->pierce = 0;
    ac->fire = 0;
    ac->water = 0;
    ac->frost = 0;
    ac->electricity = 0;
    ac->dark = 0;
    ac->light = 0;
    ac->corrosive = 0;
    ac->poison = 0;
    ac->energy = 0;
    a->ac = ac;
    return a;
}
