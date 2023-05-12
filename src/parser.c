int isControl(char *key) {
    if (strcmp(key, "when_has") == 0 ||
        strcmp(key, "when_not_has") == 0) {
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
        if (strcmp(kvpairs[i], "title") == 0) {
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
    int instructions = 0;
    for (int i = 0; i < pairs; i+=2) {
        if (isControl(kvpairs[i])) {
            mob->controlBlocks[controlBlocks] = createControlBlock(kvpairs[i], kvpairs[i + 1]);
            isInControlBlock = true;
            instructions = 0;
        } else if (isInControlBlock && strcmp(kvpairs[i], "end") != 0) {
            mob->controlBlocks[controlBlocks]->instructions[instructions][0] = kvpairs[i];
            mob->controlBlocks[controlBlocks]->instructions[instructions][1] = kvpairs[i + 1];
            instructions++;
        } else if (mob->controlBlocks[controlBlocks] != NULL &&
                    strcmp(kvpairs[i], "end") == 0 &&
                    strcmp(kvpairs[i + 1], mob->controlBlocks[controlBlocks]->condition) == 0) {
            isInControlBlock = false;
            controlBlocks++;
        } else if (strcmp(kvpairs[i], "animations") == 0) {
            animationsFragment = kvpairs[i + 1];
        } else if (strcmp(kvpairs[i], "name") == 0) {
            mob->name = &kvpairs[i][0];
        } else if (strcmp(kvpairs[i], "coordinates") == 0) {
            char *x = strtok(kvpairs[i + 1], ",");
            char *y = strtok(NULL, ",");
            mob->position.x = (float) strToInt(x);
            mob->position.y = (float) strToInt(y);
        } else if (strcmp(kvpairs[i], "direction") == 0) {
            mob->direction = getDirectionFromString(kvpairs[i + 1]);
        } else if (strcmp(kvpairs[i], "id") == 0) {
            mob->id = &kvpairs[i + 1][0];
        }
    }
    return animationsFragment;
}
