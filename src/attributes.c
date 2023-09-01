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
            }
    };
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

Attributes createAttributesFromData(AttributesData *data) {
    if (data == NULL) {
        return createEmptyAttributes();
    }
    return (Attributes) {
        data->strength,
        data->dexterity,
        data->intelligence,
        data->wisdom,
        data->constitution,
        data->hp,
        data->mana,
        data->ac == NULL ? createEmptyArmorClass() : (ArmorClass) {
            data->ac->bash,
            data->ac->slash,
            data->ac->pierce,
            data->ac->fire,
            data->ac->frost,
            data->ac->shock,
            data->ac->blast,
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
    ac->frost = a.ac.frost;
    ac->shock = a.ac.shock;
    ac->blast = a.ac.blast;
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
            a.ac.frost,
            a.ac.shock,
            a.ac.blast,
        }
    };
}

Attributes createStartingAttributes() {
    Attributes a = createEmptyAttributes();
    a.hp = STARTING_HP;
    a.mana = STARTING_MANA;
    a.strength = 5;
    return a;
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
