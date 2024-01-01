#include "headers/game.h"
#include "headers/user_config.h"

void mainMenuLoop(Game *g) {
    while (getCurrentMenu(g->menus) != NULL) {
        doInGameMenuLoop(g);
        if (WindowShouldClose()) {
            exit(0);
        }
    }
    initializeGameForPlayer(g);
}

UserConfig *globalSetup() {
    createConfigFromData(loadConfigYaml());
    SetTraceLogLevel(LOG_WARNING);
    createLog(config->logLevel);
    UserConfig *userConfig = createUserConfig();
    UIData *uiCfg = loadUIData();
    createUIConfig(uiCfg, userConfig);
    initWindow(uiCfg->screen->title, uiCfg, userConfig);
    if (userConfig->fullScreen) {
        ToggleFullscreen();
    }
    return userConfig;
}

int main() {
    UserConfig *userConfig = globalSetup();
    Game *g = createGame(userConfig);
    mainMenuLoop(g);
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