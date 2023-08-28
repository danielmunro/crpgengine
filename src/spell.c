typedef struct {
    SpellType type;
    Intent intent;
    int level;
    Attributes cost;
} Spell;

SpellType getSpellTypeFromString(const char *type) {
    if (type == NULL) {
        fprintf(stderr, "got null in getSpellTypeFromString");
        exit(EXIT_UNABLE_TO_CREATE_SPELL);
    }
    int count = sizeof(Spells) / sizeof(Spells[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(Spells[i], type) == 0) {
            return i;
        }
    }
    fprintf(stderr, "spell not found");
    exit(EXIT_UNABLE_TO_CREATE_SPELL);
}

Spell *createSpellFromData(SpellData data) {
    Spell *s = malloc(sizeof(Spell));
    s->type = getSpellTypeFromString(data.type);
    s->intent = getIntentFromString(data.intent);
    s->level = data.level;
    s->cost = createAttributesFromData(data.cost);
    return s;
}

Spell *findSpell(Spell **spells, SpellType type) {
    for (int i = 0; i < MAX_SPELLS; i ++) {
        if (spells[i]->type == type) {
            return spells[i];
        }
    }
    fprintf(stderr, "spell not found");
    exit(EXIT_UNABLE_TO_FIND_SPELL);
}
