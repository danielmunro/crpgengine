#include "header.h"

int main(int argc, char *argv[]) {
    UIData *uiCfg = globalSetup(argc, argv);
    Game *g = createGame(uiCfg);
    validateGameData(g);
    if (runtimeArgs->purgeSaves) {
        purgeSaves(runtimeArgs->indexDir);
    } else if (runtimeArgs->dump) {
        dumpGame(g);
    } else if (!runtimeArgs->exit) {
        run(g);
    }
    return 0;
}