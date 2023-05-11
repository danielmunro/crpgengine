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

char *assignMobValues(Mobile *mob, char *indexDir) {
    char *playerFile = pathCat(indexDir, "/player.txt");
    char *data = LoadFileText(playerFile);
    char *animationsFragment;
    char *kvpairs[255];
    int pairs = parseKVPairs(data, kvpairs);
    for (int i = 0; i < pairs; i+=2) {
        printf("kv: %s, %s\n", kvpairs[i], kvpairs[i+1]);
        if (strcmp(kvpairs[i], "animations") == 0) {
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
        }
    }
    return animationsFragment;
}
