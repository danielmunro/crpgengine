#include "headers/game.h"
#include "headers/user_config.h"

void setupLogging() {
    // raylib
    SetTraceLogLevel(LOG_WARNING);

    // rpg engine
    createLog(config->logLevel);
}

UIData *setupUIConfig(const UserConfig *userConfig) {
    UIData *uiConfigData = loadUIData();
    createUIConfig(uiConfigData, userConfig);
    return uiConfigData;
}

void setupWindow(const UserConfig *userConfig, const UIData *uiData) {
    initWindow(uiData->screen->title, uiData, userConfig);
    if (userConfig->fullScreen) {
        ToggleFullscreen();
    }
}

void mainMenuLoop(Game *g) {
    while (getCurrentMenu(g->menus) != NULL) {
        doInGameMenuLoop(g);
        if (WindowShouldClose()) {
            exit(0);
        }
    }
    initializeGameForPlayer(g);
}

void runGame(Game *g) {
    if (config->validate) {
        validateGameData(g);
    }
    if (config->purgeSaves) {
        purgeSaves(config->indexDir);
    } else if (!config->exit) {
        run(g);
    }
}

int main() {
    createConfigFromData(loadConfigYaml());
    setupLogging();
    UserConfig *userConfig = createUserConfig();
    UIData *uiData = setupUIConfig(userConfig);
    setupWindow(userConfig, uiData);
    Game *g = createGame(userConfig);
    mainMenuLoop(g);
    runGame(g);
    return 0;
}