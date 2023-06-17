typedef struct BeastEncounter {
    Beast *beast;
    int max;
} BeastEncounter;

typedef struct Encounters {
    Texture2D background;
    BeastEncounter *beastEncounters[MAX_BEASTS];
    int beastEncountersCount;
} Encounters;

typedef struct Fight {
    Beast *beasts[MAX_BEASTS_IN_FIGHT];
    int beastCount;
} Fight;

Encounters *createEncounters() {
    Encounters *e = malloc(sizeof(Encounters));
    e->beastEncountersCount = 0;
    return e;
}

Fight *createFight(int count, Beast *beasts[MAX_BEASTS_IN_FIGHT]) {
    Fight *fight = malloc(sizeof(Fight));
    fight->beastCount = count;
    for (int i = 0; i < count; i++) {
        fight->beasts[i] = beasts[i];
    }
    return fight;
}
