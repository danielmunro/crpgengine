#include "headers/game.h"
#include "headers/user_config.h"
#include "headers/graphics/ui/ui.h"

void setupLogging() {
    // raylib
    SetTraceLogLevel(LOG_WARNING);

    // rpg engine
    createLog(config->logLevel);
}

void setupUIConfig(Resolution resolution) {
    UIData *uiConfigData = loadUIData();
    createUIConfig(uiConfigData, resolution);
    free(uiConfigData);
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

UserConfig *globalSetup() {
    createConfigFromData(loadConfigYaml());
    setupLogging();
    UserConfig *userConfig = createUserConfig();
    setupUIConfig(userConfig->resolution);
    initWindow(
            userConfig->resolution,
            userConfig->fullScreen,
            ui->screen->title,
            ui->screen->targetFrameRate);
    return userConfig;
}