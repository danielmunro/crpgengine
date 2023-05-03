typedef struct Vector2d {
    int x;
    int y;
} Vector2d;

int strToInt(const char *value) {
    char *ptr;
    long ret;
    ret = strtol(value, &ptr, 10);
    return (int) ret;
}

char *pathCat(const char *part1, const char *part2) {
    char *result = (char *)malloc(strlen(part1) + strlen(part2) + 1);
    strcat(result, part1);
    if (part1[strlen(part1) - 1] != '/') {
        strcat(result, "/");
    }
    strcat(result, part2);
    return result;
}
