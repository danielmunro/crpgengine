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

Fight *createFight(Beast *beasts[MAX_BEASTS_IN_FIGHT]) {
    Fight *fight = malloc(sizeof(Fight));
    int i = 0;
    while(true) {
        fight->beasts[i] = beasts[i];
        i++;
        if (beasts[i] == NULL) {
            break;
        }
    }
    fight->beastCount = i;
    return fight;
}
