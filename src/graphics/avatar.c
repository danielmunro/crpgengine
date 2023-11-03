#include <raylib.h>
#include <stdio.h>
#include "headers/config.h"
#include "headers/graphics/ui/ui.h"

typedef struct {
    Texture image;
    const char *filename;
} Avatar;

Avatar *createAvatar(const char *filename) {
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/images/%s", config->indexDir, filename);
    Avatar *a = malloc(sizeof(Avatar));
    a->filename = filename;
    Image im = LoadImage(path);
    a->image = LoadTextureFromImage(im);
    UnloadImage(im);
    return a;
}

void drawAvatar(const Avatar *a, Vector2 position) {
    Vector2 origin = { 0.0f, 0.0f };
    float rotation = 0.0f;
    DrawTexturePro(
            a->image,
            (Rectangle) {
                    0,
                    0,
                    (float) a->image.width,
                    (float) a->image.height,
            },
            (Rectangle) {
                position.x,
                position.y,
                (float) a->image.width * ui->screen->scale,
                (float) a->image.height * ui->screen->scale,
            },
            origin,
            rotation,
            WHITE);
}
