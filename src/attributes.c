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
    ArmorClass ac;
} Attributes;

Attributes createAttributesFromData(AttributesData *data) {
    return (Attributes) {
        data->strength,
        data->dexterity,
        data->intelligence,
        data->wisdom,
        data->constitution,
        data->hp,
        data->mana,
        (ArmorClass) {
            data->ac->bash,
            data->ac->slash,
            data->ac->pierce,
            data->ac->fire,
            data->ac->water,
            data->ac->frost,
            data->ac->electricity,
            data->ac->dark,
            data->ac->light,
            data->ac->corrosive,
            data->ac->poison,
            data->ac->energy,
        }
    };
}

AttributesData *createDataFromAttributes(const Attributes a) {
    AttributesData *data = malloc(sizeof(AttributesData));
    data->strength = a.strength;
    data->dexterity = a.dexterity;
    data->intelligence = a.intelligence;
    data->wisdom = a.wisdom;
    data->constitution = a.constitution;
    data->hp = a.hp;
    data->mana = a.mana;
    data->ac = NULL;
    ArmorClassData *ac = malloc(sizeof(ArmorClass));
    ac->bash = a.ac.bash;
    ac->slash = a.ac.slash;
    ac->pierce = a.ac.pierce;
    ac->fire = a.ac.fire;
    ac->water = a.ac.water;
    ac->frost = a.ac.frost;
    ac->electricity = a.ac.electricity;
    ac->dark = a.ac.dark;
    ac->light = a.ac.light;
    ac->corrosive = a.ac.corrosive;
    ac->poison = a.ac.poison;
    ac->energy = a.ac.energy;
    data->ac = ac;
    return data;
}

Attributes cloneAttributes(const Attributes a) {
    return (Attributes) {
        a.strength,
        a.dexterity,
        a.intelligence,
        a.wisdom,
        a.constitution,
        a.hp,
        a.mana,
        (ArmorClass) {
            a.ac.bash,
            a.ac.slash,
            a.ac.pierce,
            a.ac.fire,
            a.ac.water,
            a.ac.frost,
            a.ac.electricity,
            a.ac.dark,
            a.ac.light,
            a.ac.corrosive,
            a.ac.poison,
            a.ac.energy,
        }
    };
}

Attributes createEmptyAttributes() {
    return (Attributes) {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        (ArmorClass) {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        }
    };
}
