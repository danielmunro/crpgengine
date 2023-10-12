#include "headers/game.h"

int main(int argc, char *argv[]) {
    globalSetup(argc, argv);
    Game *g = createGame();
    validateGameData(g);
    if (config->purgeSaves) {
        purgeSaves(config->indexDir);
    } else if (config->afterLoad->dumpState) {
        dumpGame(g);
    } else if (!config->afterLoad->exit) {
        run(g);
    }
    return 0;
}