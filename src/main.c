#include "headers/game.h"

int main() {
    globalSetup();
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