typedef struct Attributes {
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