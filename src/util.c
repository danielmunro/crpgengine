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
    if (part1[strlen(part1) - 1] != '/' && part2[0] != '/') {
        // need to add a slash
        strcat(result, "/");
    } else if (part1[strlen(part1) - 1] == '/' && part2[0] == '/') {
        // need to remove a slash
        char intermediate[strlen(part1) - 1];
        strncpy(intermediate, part1, strlen(part1) - 1);
        result = &intermediate[0];
    }
    strcat(result, part2);
    return result;
}
