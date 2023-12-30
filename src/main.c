#include "headers/game.h"
#include "headers/persistence/load_config.h"
#include "headers/user_config.h"

void mainMenuLoop(Game *g) {
    while (getCurrentMenu(g->menus) != NULL) {
        doInGameMenuLoop(g);
        if (WindowShouldClose()) {
            exit(0);
        }
    }
}

int main() {
    createConfigFromData(loadConfigYaml());
    SetTraceLogLevel(LOG_WARNING);
    createLog(config->logLevel);
    UserConfig *userConfig = createUserConfig();
    globalSetup(userConfig);
    Game *g = createGame(userConfig);
    mainMenuLoop(g);
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