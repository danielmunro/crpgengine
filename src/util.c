typedef struct Vector2d {
    int x;
    int y;
} Vector2d;

int strToInt(char *value) {
    int iterations;
    const char *errstr;

    iterations = (int) strtonum(value, 1, 64,	&errstr);
    if (errstr != NULL) {
        printf("number of iterations is %s:	%s", errstr, value);
        exit(1);
    }

    return iterations;
}
