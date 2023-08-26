typedef struct {
    Log *log;
    Spell **spells;
} SpellManager;

SpellManager *createSpellManager(Log *log) {
    SpellManager *sp = malloc(sizeof(SpellManager));
    sp->log = log;
    sp->spells = calloc(MAX_SPELLS, sizeof(Spell));
    return sp;
}
