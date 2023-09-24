typedef struct {
    SpellManager *spellManager;
    AnimationManager *animationManager;
    Mobile **mobiles;
    int mobileCount;
    Mobile **playableMobiles;
    int playableMobileCount;
} MobileManager;

MobileManager *createMobileManager(SpellManager *sm, AnimationManager *am) {
    MobileManager *m = malloc(sizeof(MobileManager));
    m->spellManager = sm;
    m->animationManager = am;
    m->mobiles = calloc(MAX_MOBILES, sizeof(Mobile));
    m->mobileCount = 0;
    m->playableMobiles = calloc(MAX_MOBILES, sizeof(Mobile));
    m->playableMobileCount = 0;
    return m;
}

void addMobileToManager(MobileManager *mm, Mobile *mob) {
    mm->mobiles[mm->mobileCount] = mob;
    mm->mobileCount++;
}

void addPlayerMobileToManager(MobileManager *mm, Mobile *mob) {
    mm->playableMobiles[mm->playableMobileCount] = mob;
    mm->playableMobileCount++;
}

Mobile *findMobById(MobileManager *mm, const char *id) {
    for (int i = 0; i < mm->mobileCount; i++) {
        if (strcmp(mm->mobiles[i]->id, id) == 0) {
            return mm->mobiles[i];
        }
    }
    addError("mob not found: %s", id);
    exit(EXIT_MOBILE_NOT_FOUND);
}

Mobile *createMobileFromData(MobileManager *mm, MobileData *data, Animation *animations[MAX_ANIMATIONS]) {
    Mobile *mob = createMobile(
            data->id,
            data->name,
            getPositionFromString(data->position),
            getDirectionFromString(data->direction),
            animations,
            createAvatar(data->avatar),
            data->hp,
            data->mana,
            createAttributesFromData(data->attributes),
            mapDataToSpells(mm->spellManager, data->spells, data->spells_count),
            data->spells_count);
    for (int i = 0; i < data->startSpells_count; i++) {
        mob->spells[mob->spellCount + i] = findSpell(
                mm->spellManager->spells,
                getSpellTypeFromString(data->startSpells[i]));
        mob->spellCount++;
    }
    return mob;
}
