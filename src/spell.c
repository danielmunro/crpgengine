typedef struct {
    SpellType type;
    Intent intent;
    int level;
    Attributes *cost;
    Attributes *impact;
    float levelModifier;
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
    s->impact = createAttributesFromData(data.impact);
    s->levelModifier = data.levelModifier;
    return s;
}

SpellData createDataFromSpell(Spell *spell) {
    return (SpellData) {
            (char *) Spells[spell->type],
            (char *) Intents[spell->intent],
            spell->level,
            spell->levelModifier,
            createDataFromAttributes(spell->cost),
            createDataFromAttributes(spell->impact),
    };
}

SpellData *mapSpellsToData(Spell **spells, int spellCount) {
    SpellData *spellData = calloc(spellCount, sizeof(SpellData));
    for (int i = 0; i < spellCount; i++) {
        spellData[i] = createDataFromSpell(spells[i]);
    }
    return spellData;
}

Spell *findSpell(Spell **spells, SpellType type) {
    for (int i = 0; i < MAX_SPELLS; i++) {
        if (spells[i]->type == type) {
            return spells[i];
        }
    }
    fprintf(stderr, "spell not found");
    exit(EXIT_UNABLE_TO_FIND_SPELL);
}
