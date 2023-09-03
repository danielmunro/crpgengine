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
