typedef struct RuntimeArgs {
    int sceneIndex;
    int showCollisions;
    int dump;
    int debug;
    char *indexDir;
} RuntimeArgs;

RuntimeArgs *createRuntimeArgs(int argc, char *argv[]) {
    RuntimeArgs *r = malloc(sizeof(RuntimeArgs));
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                r->showCollisions = true;
            } else if (argv[i][1] == 's') {
                r->sceneIndex = strToInt(argv[i + 1]);
            } else if (argv[i][1] == 'o') {
                r->dump = true;
            } else if (argv[i][1] == 'i') {
                r->indexDir = &argv[i + 1][0];
            } else if (argv[i][1] == 'd') {
                r->debug = true;
            }
        }
    }
    return r;
}
