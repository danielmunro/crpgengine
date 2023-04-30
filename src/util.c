typedef struct Vector2d {
    int x;
    int y;
} Vector2d;

int strToInt(char *value) {
    char *ptr;
    long ret;
    ret = strtol(value, &ptr, 10);
    return (int) ret;
}
