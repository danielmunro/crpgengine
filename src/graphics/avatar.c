#include <raylib.h>
#include <stdio.h>
#include "headers/config.h"

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
