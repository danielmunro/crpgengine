#include "headers/game.h"
#include "headers/user_config.h"
#include "headers/graphics/ui/ui.h"

Log *setupLogging() {
    // raylib
    SetTraceLogLevel(LOG_WARNING);

    // rpg engine
    return createLog(config->logLevel);
}

UIConfig *setupUIConfig(Resolution resolution) {
    UIData *uiConfigData = loadUIData();
    UIConfig *uiConfig = createUIConfig(uiConfigData, resolution);
    free(uiConfigData);
    return uiConfig;
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

Context *globalSetup() {
    const GameConfig *gameConfig = createGameConfigFromData(loadGameConfigYaml());
    const Log *log = setupLogging();
    UserConfig *userConfig = createUserConfig();
    const UIConfig *uiConfig = setupUIConfig(userConfig->resolution);
    initWindow(
            userConfig->resolution,
            userConfig->fullScreen,
            ui->screen->title,
            ui->screen->targetFrameRate);
    return createContext(
            log,
            gameConfig,
            uiConfig,
            userConfig);
}