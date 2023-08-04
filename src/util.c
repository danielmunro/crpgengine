#include <dirent.h>
#include <unistd.h>

typedef struct {
    int x;
    int y;
} Vector2D;

typedef struct {
    int count;
    const char **filenames;
    const char **saveNames;
} SaveFiles;

int getFilesInDirectory(const char *dir, char **files) {
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

Condition mapCondition(const char *when) {
    int count = sizeof(conditions) / sizeof(char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(conditions[i], when) == 0) {
            return i;
        }
    }
    return -1;
}

int mapOutcome(const char *then) {
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

Vector2 getPositionFromString(const char *position) {
    char pos[255];
    strcpy(pos, position);
    return (Vector2){
        strToInt(strtok(pos, ",")),
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

void setupApp() {
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));
}
