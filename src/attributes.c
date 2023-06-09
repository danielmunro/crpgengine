typedef struct Attributes {
    int strength;
    int dexterity;
    int intelligence;
    int wisdom;
    int constitution;
} Attributes;

Attributes *createAttributes() {
    Attributes *a = malloc(sizeof(Attributes));
    a->strength = 0;
    a->dexterity = 0;
    a->intelligence = 0;
    a->wisdom = 0;
    a->constitution = 0;
    return a;
}