typedef struct Vector2d {
    int x;
    int y;
} Vector2d;

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
