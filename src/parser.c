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
        if (kv[0][0] != '#') {
            result[j] = kv[0];
            result[j + 1] = kv[1];
            printf("kv: %s, %s\n", kv[0], kv[1]);
            j += 2;
        }
    }
    return j;
}

int isSpecial(char *key) {
    if (strcmp(key, CONTROL_WHEN) == 0 || strcmp(key, CONTROL_THEN) == 0) {
        return true;
    }
    return false;
}

int getControlTypeFromString(char *control) {
    if (strcmp(control, CONTROL_WHEN) == 0) {
        return CONTROL_TYPE_WHEN;
    } else if (strcmp(control, CONTROL_THEN) == 0) {
        return CONTROL_TYPE_THEN;
    }
    return CONTROL_TYPE_NONE;
}

void assignMusicValues(AudioManager *am, char *indexDir) {
    MusicYaml *music = loadMusicYaml(indexDir);

    char *basePath = pathCat(indexDir, "audio");
    char *filePath = pathCat(basePath, music->town);

    Music_ *m = malloc(sizeof(Music_));
    m->name = "town";
    m->music = LoadMusicStream(filePath);

    am->music[am->musicCount] = m;
    am->musicCount++;
}

void assignSoundValues(AudioManager *am, char *indexDir) {
    char *soundFile = pathCat(indexDir, "/sounds.txt");
    char *data = LoadFileText(soundFile);
    char *kvpairs[255];
    int pairs = parseKVPairs(data, kvpairs);
    char *basePath = pathCat(indexDir, "audio");
    for (int i = 0; i < pairs; i+=2) {
        Music_ *m = malloc(sizeof(Music_));
        m->name = kvpairs[0];
        char *filePath = pathCat(basePath, kvpairs[1]);
        m->music = LoadMusicStream(filePath);
    }
}

void assignAudioManagerValues(AudioManager *am, char *indexDir) {
    assignMusicValues(am, indexDir);
    assignSoundValues(am, indexDir);
}

char *assignMobValues(Scene *scene, Mobile *mob, char *dataFile) {
    char *data = LoadFileText(dataFile);
    char *animationsFragment;
    char *kvpairs[255];
    int pairs = parseKVPairs(data, kvpairs);
    int controlBlock = CONTROL_TYPE_NONE;
    int controlBlocks = 0;
    ControlBlockInt *cb;
    for (int i = 0; i < pairs; i+=2) {
        printf("iterate %d %d %d %s\n", i, controlBlocks, controlBlock, kvpairs[i]);
        if (isSpecial(kvpairs[i])) {
            controlBlock = getControlTypeFromString(kvpairs[i]);
            if (controlBlock == CONTROL_TYPE_WHEN) {
                scene->controlBlocksInt[controlBlocks] = createControlBlockInt(controlBlock);
                cb = scene->controlBlocksInt[controlBlocks];
            }
        } else if (controlBlock == CONTROL_TYPE_WHEN && strcmp(kvpairs[i], CONTROL_END) != 0) {
            cb->when[cb->whenCount][0] = kvpairs[i];
            cb->when[cb->whenCount][1] = kvpairs[i + 1];
            cb->whenCount++;
        } else if (controlBlock == CONTROL_TYPE_THEN && strcmp(kvpairs[i], CONTROL_END) != 0) {
            cb->then[cb->thenCount][0] = kvpairs[i];
            cb->then[cb->thenCount][1] = kvpairs[i + 1];
            cb->thenCount++;
            printf("then count: %d\n", cb->thenCount);
        } else if (controlBlock != CONTROL_TYPE_NONE && strcmp(kvpairs[i], CONTROL_END) == 0) {
            controlBlock = CONTROL_TYPE_NONE;
            controlBlocks++;
        } else if (strcmp(kvpairs[i], CONTROL_ANIMATIONS) == 0) {
            animationsFragment = kvpairs[i + 1];
        } else if (strcmp(kvpairs[i], CONTROL_NAME) == 0) {
            mob->name = &kvpairs[i + 1][0];
            printf("setting mob name to %s\n", mob->name);
        } else if (strcmp(kvpairs[i], CONTROL_COORDINATES) == 0) {
            char *x = strtok(kvpairs[i + 1], ",");
            char *y = strtok(NULL, ",");
            mob->position.x = (float) strToInt(x);
            mob->position.y = (float) strToInt(y);
        } else if (strcmp(kvpairs[i], CONTROL_DIRECTION) == 0) {
            mob->direction = getDirectionFromString(kvpairs[i + 1]);
        } else if (strcmp(kvpairs[i], CONTROL_ID) == 0) {
            mob->id = &kvpairs[i + 1][0];
        } else if (strcmp(kvpairs[i], CONTROL_HAS_STORY) == 0 || strcmp(kvpairs[i], CONTROL_NOT_HAS_STORY) == 0) {
            cb->when[cb->whenCount][0] = &kvpairs[i][0];
            cb->when[cb->whenCount][1] = &kvpairs[i + 1][0];
            cb->whenCount++;
        }
    }
    return animationsFragment;
}
