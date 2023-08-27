typedef struct {
    const char *name;
    const char *filename;
    Texture2D source;
    int frameWidth;
    int frameHeight;
    int padding;
} Spritesheet;

typedef struct {
    Spritesheet *spritesheets[MAX_SPRITES];
    int spritesCount;
} SpritesheetManager;

Spritesheet *createSpriteSheet(
        const char *name,
        const char *filename,
        int width,
        int height,
        int padding) {
    Texture2D tex = LoadTexture(filename);
    Spritesheet *sp = malloc(sizeof(Spritesheet));
    sp->name = name;
    sp->filename = filename;
    sp->source = tex;
    sp->frameWidth = width;
    sp->frameHeight = height;
    sp->padding = padding;
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

Spritesheet *findSpritesheetByName(SpritesheetManager *sm, const char *name) {
    for (int i = 0; i < sm->spritesCount; i++) {
        if (strcmp(sm->spritesheets[i]->name, name) == 0) {
            return sm->spritesheets[i];
        }
    }
    return NULL;
}

void drawImageFromSprite(Spritesheet *s, Vector2 position, int imageIndex) {
    int columns = (int) roundf((float) s->source.width / (float)(s->frameWidth + s->padding));
    int y = imageIndex / columns;
    int x = imageIndex - (y * columns);
    Rectangle rect = {
            (float) (x * (s->frameWidth + s->padding)),
            (float) (y * (s->frameHeight + s->padding)),
            (float) s->frameWidth,
            (float) s->frameHeight,
    };
    DrawTextureRec(
            s->source,
            rect,
            position,
            WHITE
    );
}

void drawRightCursor(Spritesheet *sprites, Vector2 position) {
    drawImageFromSprite(sprites, (Vector2) {
            position.x + FIGHT_CURSOR_X_OFFSET,
            position.y + FIGHT_CURSOR_Y_OFFSET,
    }, CURSOR_RIGHT_INDEX);
}

void drawDownCursor(Spritesheet *sprites, Vector2 position) {
    drawImageFromSprite(sprites, (Vector2) {
            position.x + FIGHT_CURSOR_X_OFFSET,
            position.y + FIGHT_CURSOR_Y_OFFSET,
    }, CURSOR_DOWN_INDEX);
}
