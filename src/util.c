#include <dirent.h>
#include <unistd.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
} RectangleD;

typedef struct {
    int x;
    int y;
} Vector2D;

RectangleD fromRectangle(Rectangle rect) {
    return (RectangleD) {
            (int) rect.x,
            (int) rect.y,
            (int) rect.width,
            (int) rect.height,
    };
}

int getFilesInDirectory(const char *dir, char *files[MAX_FILES]) {
    struct dirent *de;
    DIR *dr = opendir(dir);
    if (dr == NULL) {
        fprintf(stderr, "Could not open scene index directory");
    }
    int i = 0;
    while (true) {
        de = readdir(dr);
        if (de == NULL) {
            break;
        }
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }
        files[i] = (char *) malloc(strlen(de->d_name));
        strcpy(files[i], de->d_name);
        i++;
    }
    closedir(dr);
    return i;
}

Condition mapCondition(char *when) {
    int count = sizeof(conditions) / sizeof(char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(conditions[i], when) == 0) {
            return i;
        }
    }
    return -1;
}

int mapOutcome(char *then) {
    int count = sizeof(outcomes) / sizeof(char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(outcomes[i], then) == 0) {
            return i;
        }
    }
    return -1;
}

int getLogLevelFromString(char *value) {
    int count = (sizeof(logLevels)) / sizeof(LogLevel);
    for (int i = 0; i < count - 1; i++) {
        if (strcmp(logLevels[i], value) == 0) {
            return i;
        }
    }
    return -1;
}

int getDirectionFromString(const char *value) {
    if (strcmp(value, "up") == 0) {
        return UP;
    } else if (strcmp(value, "down") == 0) {
        return DOWN;
    } else if (strcmp(value, "left") == 0) {
        return LEFT;
    } else if (strcmp(value, "right") == 0) {
        return RIGHT;
    } else {
        fprintf(stderr, "invalid direction: %s\n", value);
        return 0;
    }
}

int strToInt(const char *value) {
    char *ptr;
    long ret;
    ret = strtol(value, &ptr, 10);
    return (int) ret;
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

Vector2 getPositionFromString(char *position) {
    return (Vector2){
        strToInt(strtok(position, ",")),
        strToInt(strtok(NULL, ",")),
    };
}

Vector2D vector2DFromVect(Vector2 vect) {
    return (Vector2D) {
            (int) vect.x,
            (int) vect.y
    };
}

bool vector2DEquals(Vector2D a, Vector2D b) {
    return a.x == b.x && a.y == b.y;
}

float normalize(float a, float b) {
    int a1 = (int) a;
    int b1 = (int) b;
    if (a1 > b1) return -1;
    if (a1 < b1) return 1;
    return 0;
}

const char *getFilenameExt(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int addSubsceneFiles(char *scenes[MAX_SCENES], char *sceneFiles[MAX_SCENES], const char *sceneDir, int totalCount) {
    int count = totalCount;
    for (int i = 0; i < totalCount; i++) {
        char subSceneDir[MAX_FS_PATH_LENGTH];
        sprintf(subSceneDir, "%s/%s/scenes", sceneDir, scenes[i]);
        printf("subscenedir :: %s\n", subSceneDir);
        if (access(subSceneDir, F_OK) == 0) {
            printf("found, loading\n");
            char *subScenes[MAX_SCENES];
            int subCount = getFilesInDirectory(subSceneDir, subScenes);
            printf("found sub scenes :: %d\n", subCount);
            for (int j = 0; j < subCount; j++) {
                scenes[totalCount] = subScenes[j];
                char subSceneFile[MAX_FS_PATH_LENGTH];
                sprintf(subSceneFile, "%s/%s", subSceneDir, subScenes[j]);
                sceneFiles[totalCount] = (char *)malloc(strlen(subSceneFile));
                strcpy(sceneFiles[totalCount], subSceneFile);
                printf("sceneFiles :: %s\n", sceneFiles[totalCount]);
                count++;
            }
        }
    }
    return count;
}

void buildSceneFilesList(char *scenes[MAX_SCENES], char *sceneFiles[MAX_SCENES], char *sceneDir, int totalCount) {
    printf("debug :: %s\n", sceneDir);
    for (int i = 0; i < totalCount; i++) {
        printf("scene :: %s\n", scenes[i]);
        char sceneFile[MAX_FS_PATH_LENGTH];
        sprintf(sceneFile, "%s/%s", sceneDir, scenes[i]);
        sceneFiles[i] = (char *)malloc(strlen(sceneFile));
        strcpy(sceneFiles[i], sceneFile);
    }
}
