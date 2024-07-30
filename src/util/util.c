#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <mm_malloc.h>
#include <sys/time.h>
#include <math.h>
#include <sys/resource.h>
#include <raylib.h>
#include <unistd.h>
#include <libgen.h>
#include "headers/util/log.h"
#include "headers/errors.h"

typedef struct {
    int x;
    int y;
} Vector2D;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} RectangleD;

Rectangle rectangleDtoRectangle(RectangleD rect) {
    return (Rectangle) {
            (float) rect.x,
            (float) rect.y,
            (float) rect.width,
            (float) rect.height,
    };
}

char *trim(char *str) {
    char *end;

    while (isspace((unsigned char) *str)) str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    end[1] = '\0';

    return str;
}

int getFilesInDirectory(const char *dir, char **files) {
    struct dirent *de;
    DIR *dr = opendir(dir);
    if (dr == NULL) {
        addError("getFilesInDirectory: could not open directory :: %s", dir);
    }
    int i = 0;
    while (true) {
        de = readdir(dr);
        if (de == NULL) {
            break;
        }
        if (strcmp(de->d_name, ".") == 0
            || strcmp(de->d_name, "..") == 0
            || strcmp(de->d_name, ".keep") == 0) {
            continue;
        }
        files[i] = malloc(MAX_FS_PATH_LENGTH);
        strcpy(files[i], &de->d_name[0]);
        i++;
    }
    closedir(dr);
    free(de);
    return i;
}

int min(int a, int b) {
    if (a > b) {
        return b;
    }
    return a;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

Vector2D getPositionFromString(const char *position) {
    char *p = (char *) position;
    const char *x = trim(strtok_r(p, ",", &p));
    const char *y = trim(strtok_r(p, ",", &p));
    return (Vector2D) {
            TextToInteger(x),
            TextToInteger(y),
    };
}

bool vector2DEquals(Vector2D a, Vector2D b) {
    return a.x == b.x && a.y == b.y;
}

const char *getFilenameExt(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

double reportMaxMemory() {
    int who = RUSAGE_SELF;
    struct rusage usage;
    getrusage(who, &usage);
    double memoryInMb = (double) usage.ru_maxrss / 1000000;
    addInfo("max memory: %f\n", memoryInMb);
    return memoryInMb;
}

const char *getAutosaveFile(const char *indexDir) {
    const char *autosaveFilePath = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) autosaveFilePath, "%s/_saves/autosave.yaml", indexDir);
    return autosaveFilePath;
}

double getTimeInMS() {
    struct timeval end;
    gettimeofday(&end, NULL);
    double timeInterval = (double) end.tv_sec * 1000.0;
    timeInterval += end.tv_usec / 1000.0;
    return timeInterval;
}

Color getColorFromString(const char *color) {
    char *c = (char *) color;
    int r = TextToInteger(trim(strtok_r(c, ",", &c)));
    int g = TextToInteger(trim(strtok_r(c, ",", &c)));
    int b = TextToInteger(trim(strtok_r(c, ",", &c)));
    int a = TextToInteger(trim(strtok_r(c, ",", &c)));
    return (Color) {
            (unsigned char) r,
            (unsigned char) g,
            (unsigned char) b,
            (unsigned char) a,
    };
}

void getVital(int current, int max, char result[MAX_VITALS_LENGTH]) {
    sprintf(result, "%d/%d", current, max);
}

float getScrollOffset(float lineHeight, int cursorLine, float areaHeight) {
    float cursorY = ((float) cursorLine + 1) * lineHeight;
    float diff = cursorY - areaHeight;
    if (diff >= 0) {
        return ceilf(diff + lineHeight);
    }
    return 0;
}

void getComponentPath(
        char dest[MAX_FS_PATH_LENGTH],
        const char *indexDir,
        const char *moduleIndexDir,
        const char *componentName,
        const char *filename) {
    const char *baseFile = basename((char *) filename);
    sprintf(dest, "%s/%s", moduleIndexDir, baseFile);
    addDebug("check for module component :: %s, %s", componentName, dest);
    if (access(dest, F_OK) != 0) {
        sprintf(dest, "%s/%s/%s", indexDir, componentName, baseFile);
        addDebug("check for global component :: %s, %s", componentName, dest);
        if (access(dest, F_OK) != 0) {
            addError("no component exists with filename :: %s", baseFile);
            exit(ConfigurationErrorMissingComponent);
        }
    }
}

Texture loadImageTexture(const char *filePath) {
    Image im = LoadImage(filePath);
    Texture tex = LoadTextureFromImage(im);
    UnloadImage(im);
    return tex;
}

char *getConfigFileName(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            return argv[i + 1];
        }
    }
    return "config.yaml";
}

Rectangle getRectForSpriteByIndex(int spriteWidth, int width, int height, int index) {
    int spritesPerRow = spriteWidth / width;
    int y = index / spritesPerRow;
    int x = (index % spritesPerRow);
    Rectangle pos = {
            (float) (x * width),
            (float) (y * height),
            (float) width,
            (float) height,
    };
    return pos;
}

Rectangle getRectForSpriteByTile(int spriteWidth, int tileSize, int index) {
    return getRectForSpriteByIndex(spriteWidth, tileSize, tileSize, index - 1);
}
