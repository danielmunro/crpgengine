#include "headers/game.h"
#include "headers/user_config.h"
#include "headers/graphics/ui/ui.h"

Log *setupLogging(LogLevel logLevel) {
    // raylib
    SetTraceLogLevel(LOG_WARNING);

    // rpg engine
    return createLog(logLevel);
}

UIConfig *setupUIConfig(Resolution resolution, const char *indexDir) {
    UIData *uiConfigData = loadUIData(indexDir);
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
    const GameConfig *config = g->context->game;
    if (config->validate) {
        validateGameData(g);
    }
    if (config->purgeSaves) {
        purgeSaves(config->indexDir);
    } else if (!config->exit) {
        run(g);
    }
}

Context *globalSetup(const char *configFile) {
    const GameConfig *gameConfig = createGameConfigFromData(loadGameConfigYaml(configFile), GetDirectoryPath(configFile));
    const Log *log = setupLogging(gameConfig->logLevel);
    UserConfig *userConfig = createUserConfig(gameConfig->indexDir);
    const UIConfig *uiConfig = setupUIConfig(userConfig->resolution, gameConfig->indexDir);
    initWindow(
            userConfig->resolution,
            userConfig->fullScreen,
            uiConfig->screen->title,
            uiConfig->screen->targetFrameRate);
    return createContext(
            log,
            gameConfig,
            uiConfig,
            userConfig);
}