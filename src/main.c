#include "src/headers/main.h"

int main(int argc, char *argv[]) {
    globalSetup(argc, argv);
    Game *g = createGame();
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