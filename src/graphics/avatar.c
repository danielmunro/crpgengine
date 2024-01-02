#include <raylib.h>
#include <stdio.h>
#include "headers/config.h"
#include "headers/graphics/ui/ui.h"

typedef struct {
    Texture image;
    const char *filename;
    float scale;
} Avatar;

Avatar *createAvatar(const char *indexDir, const char *filename, float scale) {
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/images/%s", indexDir, filename);
    Avatar *a = malloc(sizeof(Avatar));
    a->filename = filename;
    a->scale = scale;
    Image im = LoadImage(path);
    a->image = LoadTextureFromImage(im);
    UnloadImage(im);
    return a;
}

float getAvatarWidth(const Avatar *a) {
    return (float) a->image.width * a->scale;
}

float getAvatarHeight(const Avatar *a) {
    return (float) a->image.height * a->scale;
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
                getAvatarWidth(a),
                getAvatarHeight(a),
            },
            origin,
            rotation,
            WHITE);
}
