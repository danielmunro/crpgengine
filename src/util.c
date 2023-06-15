#include <dirent.h>

typedef struct Vector2d {
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
        printf("added scene %s\n", scenes[i]);
        i++;
    }
    closedir(dr);
    return i;
}

int mapCondition(char *when) {
    if (strcmp(when, WHEN_HAS) == 0) {
        return CONDITION_HAS;
    } else if (strcmp(when, WHEN_NOT_HAS) == 0) {
        return CONDITION_NOT_HAS;
    } else if (strcmp(when, WHEN_ENGAGED) == 0) {
        return CONDITION_ENGAGED;
    } else if (strcmp(when, WHEN_AT) == 0) {
        return CONDITION_AT;
    } else if (strcmp(when, WHEN_HAS_STORY) == 0) {
        return CONDITION_HAS_STORY;
    } else if (strcmp(when, WHEN_NOT_HAS_STORY) == 0) {
        return CONDITION_NOT_HAS_STORY;
    }
    return -1;
}

int mapOutcome(char *then) {
    if (strcmp(then, THEN_SPEAK) == 0) {
        return OUTCOME_SPEAK;
    } else if (strcmp(then, THEN_MOVE_TO) == 0) {
        return OUTCOME_MOVE_TO;
    } else if (strcmp(then, THEN_DIRECTION) == 0) {
        return OUTCOME_DIRECTION;
    } else if (strcmp(then, THEN_SPRITE) == 0) {
        return OUTCOME_SPRITE;
    } else if (strcmp(then, THEN_WAIT) == 0) {
        return OUTCOME_WAIT;
    } else if (strcmp(then, THEN_GIVE_ITEM) == 0) {
        return OUTCOME_GIVE_ITEM;
    } else if (strcmp(then, THEN_TAKE) == 0) {
        return OUTCOME_TAKE;
    } else if (strcmp(then, THEN_ADD_STORY) == 0) {
        return OUTCOME_ADD_STORY;
    }
    return -1;
}

int getDirectionFromString(char *value) {
    if (strcmp(value, "up") == 0) {
        return DIRECTION_UP;
    } else if (strcmp(value, "down") == 0) {
        return DIRECTION_DOWN;
    } else if (strcmp(value, "left") == 0) {
        return DIRECTION_LEFT;
    } else if (strcmp(value, "right") == 0) {
        return DIRECTION_RIGHT;
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

char *pathCat(const char *part1, const char *part2) {
    char *result = (char *)malloc(strlen(part1) + strlen(part2) + 1);
    strcat(result, part1);
    if (part1[strlen(part1) - 1] != '/' && part2[0] != '/') {
        // need to add a slash
        strcat(result, "/");
        strcat(result, part2);
    } else if (part1[strlen(part1) - 1] == '/' && part2[0] == '/') {
        strcat(result, &part2[1]);
    } else {
        strcat(result, part2);
    }
    return result;
}
