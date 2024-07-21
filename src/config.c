#include <stdbool.h>
#include "headers/util/log.h"
#include "headers/persistence/cyaml.h"
#include "headers/util/util.h"

typedef struct {
    bool objects;
    bool player;
    bool warps;
} ShowCollisions;

typedef struct {
    LogLevel logLevel;
    const char *indexDir;
    const char *startScene;
    const char *overrideStartScene;
    const char *saveFile;
    bool exit;
    bool immortal;
    bool validate;
    bool randomEncounters;
    bool logMemoryUsage;
    bool purgeSaves;
    bool showFPS;
    ShowCollisions *showCollisions;
    int tileSize;
} GameConfig;

GameConfig *createGameConfigFromData(const ConfigData *data, const char *basePath) {
    GameConfig *config = malloc(sizeof(GameConfig));
    config->logLevel = getLogLevelFromString(data->logLevel);
    config->indexDir = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) config->indexDir, "%s/%s", basePath, data->indexDir);
    config->startScene = data->startScene;
    config->overrideStartScene = data->overrideStartScene;
    config->exit = data->exit;
    config->immortal = data->immortal;
    config->validate = data->validate;
    config->randomEncounters = data->randomEncounters;
    config->logMemoryUsage = data->logMemoryUsage;
    config->purgeSaves = data->purgeSaves;
    config->showFPS = data->showFPS;
    config->showCollisions = malloc(sizeof(ShowCollisions));
    config->showCollisions->objects = data->showCollisions->objects;
    config->showCollisions->player = data->showCollisions->player;
    config->showCollisions->warps = data->showCollisions->warps;
    config->tileSize = data->tileSize;
    return config;
}
