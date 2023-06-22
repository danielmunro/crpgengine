typedef struct {
    int sceneIndex;
    int showCollisions;
    int dump;
    int logLevel;
    char *indexDir;
} RuntimeArgs;

RuntimeArgs *createRuntimeArgs(int argc, char *argv[]) {
    RuntimeArgs *r = malloc(sizeof(RuntimeArgs));
    r->sceneIndex = 0;
    r->showCollisions = false;
    r->dump = false;
    r->logLevel = INFO;
    r->indexDir = NULL;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                r->showCollisions = true;
            } else if (argv[i][1] == 's') {
                r->sceneIndex = strToInt(argv[i + 1]);
            } else if (argv[i][1] == 'd') {
                r->dump = true;
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
