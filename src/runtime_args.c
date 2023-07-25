typedef struct {
    int sceneIndex;
    bool showObjectCollisions;
    bool showPlayerCollision;
    bool showWarpCollisions;
    bool dump;
    bool exit;
    LogLevel logLevel;
    char *indexDir;
} RuntimeArgs;

RuntimeArgs *createRuntimeArgs(int argc, char *argv[]) {
    RuntimeArgs *r = malloc(sizeof(RuntimeArgs));
    r->sceneIndex = -1;
    r->showObjectCollisions = false;
    r->showPlayerCollision = false;
    r->showWarpCollisions = false;
    r->dump = false;
    r->exit = false;
    r->logLevel = INFO;
    r->indexDir = NULL;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                char *collider = strtok(argv[i + 1], ",");
                while (collider != NULL) {
                    for (int j = 0; j < COLLIDE_TYPE_COUNT; j++) {
                        if (strcmp(collider, COLLIDE_TYPE_OBJECTS) == 0) {
                            r->showObjectCollisions = true;
                        } else if (strcmp(collider, COLLIDE_TYPE_PLAYER) == 0) {
                            r->showPlayerCollision = true;
                        } else if (strcmp(collider, COLLIDE_TYPE_WARPS) == 0) {
                            r->showWarpCollisions = true;
                        }
                    }
                    collider = strtok(NULL, ",");
                }
            } else if (argv[i][1] == 's') {
                r->sceneIndex = strToInt(argv[i + 1]);
            } else if (argv[i][1] == 'd') {
                r->dump = true;
            } else if (argv[i][1] == 'e') {
                r->exit = true;
            } else if (argv[i][1] == 'i') {
                r->indexDir = &argv[i + 1][0];
            } else if (argv[i][1] == 'l') {
                r->logLevel = getLogLevelFromString(&argv[i + 1][0]);
                if (r->logLevel == -1) {
                    fprintf(stderr, "log level must be one of [debug, info, warn, error], setting to info\n");
                    r->logLevel = INFO;
                }
            }
        }
    }
    if (r->indexDir == NULL) {
        fprintf(stderr, "indexDir (-i) argument is required\n");
        exit(EXIT_NO_INDEX_DIR);
    }
    return r;
}
