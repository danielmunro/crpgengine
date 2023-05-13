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

void clean(char *value) {
    while (true) {
        if (value[0] != ' ') {
            break;
        }
        memmove(&value[0], &value[1], strlen(value));
    }
    if (value[0] == '"') {
        memmove(&value[0], &value[1], strlen(value));
        unsigned long l = strlen(value);
        if (value[l - 1] == '"') {
            memmove(&value[l - 1], &value[l], 1);
        }
    }
}

void parseKV(char *data, char *result[2]) {
    result[0] = strtok(data, ":");
    result[1] = strtok(NULL, "\r\n");
    clean(result[0]);
    if (result[1] != NULL) {
        clean(result[1]);
    }
}

int parseKVPairs(char *data, char *result[255]) {
    char *rows[255];
    char *row = strtok(data, "\r\n");
    int r = 0;
    while(row != NULL) {
        rows[r] = row;
        row = strtok(NULL, "\r\n");
        r++;
    }
    int j = 0;
    for (int i = 0; i < r; i++) {
        char *kv[2];
        parseKV(rows[i], kv);
        result[j] = kv[0];
        result[j + 1] = kv[1];
        printf("kv: %s, %s\n", kv[0], kv[1]);
        j += 2;
    }
    return j;
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
