#include <dirent.h>

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

int getFilesInDirectory(const char *dir, char *scenes[MAX_SCENES]) {
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
        scenes[i] = (char *) malloc(strlen(de->d_name));
        strcpy(scenes[i], de->d_name);
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

bool vectorsEqual(Vector2 v1, Vector2 v2) {
    return v1.x == v2.x && v1.y == v2.y;
}
