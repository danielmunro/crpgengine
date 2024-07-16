#include <string.h>
#include <raylib.h>
#include <mm_malloc.h>
#include <math.h>
#include "headers/context.h"

typedef struct {
    const char *name;
    const char *filename;
    Texture2D source;
    int frameWidth;
    int frameHeight;
    int padding;
    Rectangle *collision;
    float scale;
} Spritesheet;

Spritesheet *createSpriteSheet(
        const char *name,
        const char *filename,
        int width,
        int height,
        int padding,
        Rectangle *collision,
        float scale) {
    Texture2D tex = LoadTexture(filename);
    Spritesheet *sp = malloc(sizeof(Spritesheet));
    sp->name = name;
    sp->filename = filename;
    sp->source = tex;
    sp->frameWidth = width;
    sp->frameHeight = height;
    sp->padding = padding;
    sp->collision = collision;
    sp->scale = scale;
    return sp;
}

void drawImageFromSprite(const Spritesheet *s, Vector2 position, int imageIndex) {
    int columns = (int) roundf((float) s->source.width / (float) (s->frameWidth + s->padding));
    int y = imageIndex / columns;
    int x = imageIndex - (y * columns);
    Rectangle rect = {
            (float) (x * (s->frameWidth + s->padding)),
            (float) (y * (s->frameHeight + s->padding)),
            (float) s->frameWidth,
            (float) s->frameHeight,
    };
    Rectangle dest = {
            position.x,
            position.y,
            (float) abs(s->frameWidth) * s->scale,
            (float) abs(s->frameHeight) * s->scale };
    Vector2 origin = { 0.0f, 0.0f };
    float rotation = 0.0f;
    DrawTexturePro(s->source, rect, dest, origin, rotation, WHITE);
}
