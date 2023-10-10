#include <raylib.h>
#include <stdio.h>
#include "src/util/runtime_args.h"

typedef struct {
    Texture image;
    const char *filename;
} Avatar;

Avatar *createAvatar(const char *filename) {
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/images/%s", runtimeArgs->indexDir, filename);
    Avatar *a = malloc(sizeof(Avatar));
    a->filename = filename;
    a->image = LoadTextureFromImage(LoadImage(path));
    return a;
}
