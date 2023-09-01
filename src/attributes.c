typedef struct {
    int bash;
    int slash;
    int pierce;
    int fire;
    int frost;
    int shock;
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

Attributes *createEmptyAttributes() {
    ArmorClass *ac = malloc(sizeof(ArmorClass));
    ac->bash = 0;
    ac->slash = 0;
    ac->pierce = 0;
    ac->fire = 0;
    ac->frost = 0;
    ac->shock = 0;
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = 0;
    a->intelligence = 0;
    a->wisdom = 0;
    a->dexterity = 0;
    a->constitution = 0;
    a->hp = 0;
    a->mana = 0;
    a->ac = ac;
    return a;
}

Attributes *createAttributesFromData(AttributesData *data) {
    if (data == NULL) {
        return NULL;
    }
    ArmorClass *ac = NULL;
    if (data->ac) {
        ac = malloc(sizeof(ArmorClass));
        ac->bash = data->ac->bash;
        ac->slash = data->ac->slash;
        ac->pierce = data->ac->pierce;
        ac->fire = data->ac->fire;
        ac->frost = data->ac->frost;
        ac->shock = data->ac->shock;
    }
    Attributes *a = createEmptyAttributes();
    a->strength = data->strength;
    a->intelligence = data->intelligence;
    a->wisdom = data->wisdom;
    a->dexterity = data->dexterity;
    a->constitution = data->constitution;
    a->hp = data->hp;
    a->mana = data->mana;
    a->ac = ac;
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
    if (a->ac != NULL) {
        ArmorClassData *ac = malloc(sizeof(ArmorClass));
        ac->bash = a->ac->bash;
        ac->slash = a->ac->slash;
        ac->pierce = a->ac->pierce;
        ac->fire = a->ac->fire;
        ac->frost = a->ac->frost;
        ac->shock = a->ac->shock;
        data->ac = ac;
    }
    return data;
}

Attributes *cloneAttributes(const Attributes *a) {
    ArmorClass *ac = NULL;
    if (a->ac) {
        ac = malloc(sizeof(ArmorClass));
        ac->bash = a->ac->bash;
        ac->slash = a->ac->slash;
        ac->pierce = a->ac->pierce;
        ac->fire = a->ac->fire;
        ac->frost = a->ac->frost;
        ac->shock = a->ac->shock;
    }
    Attributes *b = createEmptyAttributes();
    b->strength = a->strength;
    b->intelligence = a->intelligence;
    b->wisdom = a->wisdom;
    b->dexterity = a->dexterity;
    b->constitution = a->constitution;
    b->hp = a->hp;
    b->mana = a->mana;
    b->ac = ac;
    return b;
}

Attributes *createStartingAttributes() {
    Attributes *a = createEmptyAttributes();
    a->hp = STARTING_HP;
    a->mana = STARTING_MANA;
    a->strength = 5;
    return a;
}

Attributes *combineAttributes(const Attributes *a, const Attributes *b) {
    ArmorClass *ac = malloc(sizeof(ArmorClass));
    ac->bash = a->ac != NULL ? a->ac->bash : 0 + b->ac != NULL ? b->ac->bash : 0;
    ac->slash = a->ac != NULL ? a->ac->slash : 0 + b->ac != NULL ? b->ac->slash : 0;
    ac->pierce = a->ac != NULL ? a->ac->pierce : 0 + b->ac != NULL ? b->ac->pierce : 0;
    ac->fire = a->ac != NULL ? a->ac->fire : 0 + b->ac != NULL ? b->ac->fire : 0;
    ac->frost = a->ac != NULL ? a->ac->frost : 0 + b->ac != NULL ? b->ac->frost : 0;
    ac->shock = a->ac != NULL ? a->ac->shock : 0 + b->ac != NULL ? b->ac->shock : 0;
    Attributes *c = createEmptyAttributes();
    c->strength = a->strength + b->strength;
    c->intelligence = a->intelligence + b->intelligence;
    c->wisdom = a->wisdom + b->wisdom;
    c->dexterity = a->dexterity + b->dexterity;
    c->constitution = a->constitution + b->constitution;
    c->hp = a->hp + b->hp;
    c->mana = a->mana + b->mana;
    return c;
}
