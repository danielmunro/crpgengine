#include "header.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    RuntimeArgs *r = createRuntimeArgs(argc, argv);
    if (r->indexDir == NULL) {
        fprintf(stderr, "required argument -i indexDir missing\n");
        return 1;
    }
    initWindow(r->indexDir);
    Game *g = createGame(r);
    validateGameData(g);
    if (r->dump) {
        dumpGame(g);
        return 0;
    }
    postInitWindow();
    run(g);

    return 0;
}