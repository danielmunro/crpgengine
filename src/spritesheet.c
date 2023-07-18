typedef struct {
    const char *filename;
    Texture2D source;
    int frameWidth;
    int frameHeight;
} Spritesheet;

typedef struct {
    Spritesheet *spritesheets[MAX_SPRITES];
    int spritesCount;
} SpritesheetManager;

Spritesheet *createSpriteSheet(const char *filename, int width, int height) {
    Texture2D tex = LoadTexture(filename);
    Spritesheet *sp = malloc(sizeof(Spritesheet));
    sp->filename = filename;
    sp->source = tex;
    sp->frameWidth = width;
    sp->frameHeight = height;
    return sp;
}

SpritesheetManager *createSpriteSheetManager(Spritesheet *spritesheets[MAX_SPRITES], int count) {
    SpritesheetManager *sm = malloc(sizeof(SpritesheetManager));
    for (int i = 0; i < count; i++) {
        sm->spritesheets[i] = spritesheets[i];
    }
    sm->spritesCount = count;
    return sm;
}

Spritesheet *findSpritesheetByFilename(SpritesheetManager *sm, const char *filename) {
    for (int i = 0; i < sm->spritesCount; i++) {
        if (strcmp(sm->spritesheets[i]->filename, filename) == 0) {
            return sm->spritesheets[i];
        }

    }
    return NULL;
}
