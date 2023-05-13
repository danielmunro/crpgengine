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

int isControl(char *key) {
    if (strcmp(key, "when") == 0) {
        return true;
    }
    return false;
}

void assignConfigValues(Config *config, char *indexDir) {
    char *gameFile = pathCat(indexDir, "/config.txt");
    char *data = LoadFileText(gameFile);
    char *kvpairs[255];
    int pairs = parseKVPairs(data, kvpairs);
    for (int i = 0; i < pairs; i+=2) {
        if (strcmp(kvpairs[i], CONTROL_TITLE) == 0) {
            config->title = &kvpairs[i + 1][0];
        }
    }
}

char *assignMobValues(Mobile *mob, char *dataFile) {
    char *data = LoadFileText(dataFile);
    char *animationsFragment;
    char *kvpairs[255];
    int pairs = parseKVPairs(data, kvpairs);
    int isInControlBlock = false;
    int controlBlocks = 0;
    for (int i = 0; i < pairs; i+=2) {
        if (isControl(kvpairs[i])) {
            mob->controlBlocks[controlBlocks] = createControlBlock(kvpairs[i], kvpairs[i + 1]);
            isInControlBlock = true;
        } else if (isInControlBlock && strcmp(kvpairs[i], CONTROL_END) != 0) {
            mob->controlBlocks[controlBlocks]->instructions[mob->controlBlocks[controlBlocks]->instructionCount][0] = kvpairs[i];
            mob->controlBlocks[controlBlocks]->instructions[mob->controlBlocks[controlBlocks]->instructionCount][1] = kvpairs[i + 1];
            mob->controlBlocks[controlBlocks]->instructionCount++;
        } else if (mob->controlBlocks[controlBlocks] != NULL &&
                    strcmp(kvpairs[i], CONTROL_END) == 0 &&
                    strcmp(kvpairs[i + 1], mob->controlBlocks[controlBlocks]->condition) == 0) {
            isInControlBlock = false;
            controlBlocks++;
        } else if (strcmp(kvpairs[i], CONTROL_ANIMATIONS) == 0) {
            animationsFragment = kvpairs[i + 1];
        } else if (strcmp(kvpairs[i], CONTROL_NAME) == 0) {
            mob->name = &kvpairs[i][0];
        } else if (strcmp(kvpairs[i], CONTROL_COORDINATES) == 0) {
            char *x = strtok(kvpairs[i + 1], ",");
            char *y = strtok(NULL, ",");
            mob->position.x = (float) strToInt(x);
            mob->position.y = (float) strToInt(y);
        } else if (strcmp(kvpairs[i], CONTROL_DIRECTION) == 0) {
            mob->direction = getDirectionFromString(kvpairs[i + 1]);
        } else if (strcmp(kvpairs[i], CONTROL_ID) == 0) {
            mob->id = &kvpairs[i + 1][0];
        }
    }
    return animationsFragment;
}
