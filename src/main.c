#include "header.h"

int main(int argc, char *argv[]) {
    setupApp();
    RuntimeArgs *r = createRuntimeArgs(argc, argv);
    initWindow(r->indexDir);
    Game *g = createGame(r);
    validateGameData(g);
    if (r->dump) {
        dumpGame(g);
        return 0;
    }
    if (!r->exit) {
        run(g);
    }
    return 0;
}