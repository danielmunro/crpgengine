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