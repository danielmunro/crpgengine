#include "headers/test.h"
#include "headers/persistence/loader.h"
#include "headers/attributes.h"

AnimationManager *createTestAnimationManager(Context *c) {
    AnimationManager *am = createAnimationManager(c);
    SpritesheetManager *sm = loadSpritesheetManager(c);
    loadAllAnimations(am, sm);
    free(sm);
    return am;
}

Attributes *createTestAttributes() {
    Attributes *a = createEmptyAttributes();
    a->hp = STARTING_HP;
    a->mana = STARTING_MANA;
    a->strength = 5;
    return a;
}
