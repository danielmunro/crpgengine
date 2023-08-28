typedef struct {
    Log *log;
    Spell **spells;
    int count;
} SpellManager;

SpellManager *createSpellManager(Log *log, Spell **spells, int count) {
    SpellManager *sp = malloc(sizeof(SpellManager));
    sp->log = log;
    sp->spells = spells;
    sp->count = count;
    return sp;
}
