#include <dirent.h>

typedef struct {
    int x;
    int y;
} Vector2d;

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

int mapCondition(char *when) {
    int count = sizeof(conditions) / sizeof(char*);
    for (int i = 0; i < count; i++) {
        if (strcmp(conditions[i], when) == 0) {
            return i;
        }
    }
    return -1;
}

int mapOutcome(char *then) {
    int count = sizeof(outcomes) / sizeof(char*);
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

int getDirectionFromString(char *value) {
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
