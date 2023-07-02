typedef struct {
    int strength;
    int dexterity;
    int intelligence;
    int wisdom;
    int constitution;
} Attributes;

Attributes *createAttributesFromData(AttributesData *data) {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = data->strength;
    a->dexterity = data->dexterity;
    a->intelligence = data->intelligence;
    a->wisdom = data->wisdom;
    a->constitution = data->constitution;
    return a;
}

Attributes *cloneAttributes(Attributes *original) {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = original->strength;
    a->intelligence = original->intelligence;
    a->wisdom = original->wisdom;
    a->dexterity = original->dexterity;
    a->constitution = original->constitution;
    return a;
}

Attributes *createEmptyAttributes() {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = 0;
    a->intelligence = 0;
    a->wisdom = 0;
    a->dexterity = 0;
    a->constitution = 0;

    return a;
}