#include "headers/graphics/spritesheet.h"

typedef struct {
    Spritesheet *spritesheets[MAX_SPRITESHEETS];
    int spritesCount;
    Context *context;
} SpritesheetManager;

SpritesheetManager *createSpriteSheetManager(
        Spritesheet *spritesheets[MAX_SPRITESHEETS],
        int count,
        Context *c) {
    SpritesheetManager *sm = malloc(sizeof(SpritesheetManager));
    for (int i = 0; i < count; i++) {
        sm->spritesheets[i] = spritesheets[i];
    }
    sm->spritesCount = count;
    sm->context = c;
    return sm;
}

Spritesheet *findSpritesheetByName(SpritesheetManager *sm, const char *name) {
    for (int i = 0; i < sm->spritesCount; i++) {
        if (strcmp(sm->spritesheets[i]->name, name) == 0) {
            return sm->spritesheets[i];
        }
    }
    return NULL;
}
