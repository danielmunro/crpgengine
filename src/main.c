#include "headers/game.h"

int main() {
    globalSetup();
    Game *g = createGame();
    while (getCurrentMenu(g->menus) != NULL) {
        doInGameMenuLoop(g);
    }
    initializeGameForPlayer(g);
    if (config->validate) {
        validateGameData(g);
    }
    if (config->purgeSaves) {
        purgeSaves(config->indexDir);
    } else if (!config->exit) {
        run(g);
    }
    return 0;
}