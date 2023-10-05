#include "headers/attributes.h"
#include "headers/intent.h"

typedef struct {
    SpellType type;
    const char *name;
    Intent intent;
    int level;
    Attributes *cost;
    Attributes *impact;
    float levelModifier;
} Spell;

SpellType getSpellTypeFromString(const char *type) {
    if (type == NULL) {
        addError("got null in getSpellTypeFromString");
        exit(RuntimeErrorNullSpellType);
    }
    int count = sizeof(Spells) / sizeof(Spells[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(Spells[i], type) == 0) {
            return i;
        }
    }
    addError("spell not found :: %s", type);
    exit(RuntimeErrorNoSpellForType);
}

Spell *createSpellFromData(SpellData data) {
    Spell *s = malloc(sizeof(Spell));
    s->type = getSpellTypeFromString(data.type);
    s->name = data.type;
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
    addError("spell not found :: %d", type);
    exit(RuntimeErrorNoSpellForType);
}


int getSpellAttributeAmount(Spell *spell, int base) {
    if (base == 0) {
        return 0;
    }
    int amount = base + (spell->level * (int) spell->levelModifier);
    if (spell->intent == INTENT_HARM) {
        return -amount;
    } else if (spell->intent == INTENT_HELP) {
        return amount;
    } else {
        addError("unknown intent in getSpellAttributeAmount :: %d", spell->intent);
        exit(RuntimeErrorUnknownIntent);
    }
}
