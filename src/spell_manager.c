typedef struct {
    Spell **spells;
    int count;
} SpellManager;

SpellManager *createSpellManager(Spell **spells, int count) {
    SpellManager *sp = malloc(sizeof(SpellManager));
    sp->spells = spells;
    sp->count = count;
    return sp;
}

Spell **mapDataToSpells(SpellManager *sm, SpellData *spellData, int dataCount) {
    Spell **spells = calloc(dataCount, sizeof(Spell));
    for (int i = 0; i < dataCount; i++) {
        for (int j = 0; j < sm->count; j++) {
            if (strcmp(spellData->type, Spells[sm->spells[i]->type]) == 0) {
                spells[i] = sm->spells[j];
            }
        }
    }
    return spells;
}
