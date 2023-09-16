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
    return (Vector2) {
            strToInt(strtok((char *) position, ",")),
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
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void setupApp() {
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));
}

void purgeSaves(const char *indexDir) {
    const char *saveDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) saveDirectory, "%s/_saves", indexDir);
    char **files = calloc(MAX_SAVE_FILES, sizeof(char *));
    int count = getFilesInDirectory(saveDirectory, files);
    for (int i = 0; i < count; i++) {
        printf("remove save file: %s\n", files[i]);
        char *filepath = malloc(MAX_FS_PATH_LENGTH);
        sprintf(filepath, "%s/%s", saveDirectory, files[i]);
        remove(filepath);
        free(filepath);
    }
    free(files);
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
    const char *r = strtok((char *) color, ",");
    const char *g = strtok(NULL, ",");
    const char *b = strtok(NULL, ",");
    const char *a = strtok(NULL, ",");

    return (Color) {
            strToInt(r),
            strToInt(g),
            strToInt(b),
            strToInt(a),
    };
}

char *getVital(int current, int max) {
    char *vital = malloc(sizeof(char *));
    sprintf(vital, "%d/%d", current, max);
    return vital;
}

int randomWithLimit(int limit) {
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}
