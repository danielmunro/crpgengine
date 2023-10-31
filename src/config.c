#include <stdbool.h>
#include "headers/util/log.h"
#include "headers/persistence/cyaml.h"

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
    bool randomEncounters;
    bool forceNewGame;
    bool logMemoryUsage;
    bool purgeSaves;
    bool showFPS;
    ShowCollisions *showCollisions;
} Config;

Config *config;

Config *createConfigFromData(const ConfigData *data) {
    config = malloc(sizeof(Config));
    config->showCollisions = malloc(sizeof(ShowCollisions));
    config->logLevel = getLogLevelFromString(data->logLevel);
    config->indexDir = data->indexDir;
    config->startScene = data->startScene;
    config->overrideStartScene = data->overrideStartScene;
    config->saveFile = data->saveFile;
    config->exit = data->exit;
    config->immortal = data->immortal;
    config->randomEncounters = data->randomEncounters;
    config->forceNewGame = data->forceNewGame;
    config->logMemoryUsage = data->logMemoryUsage;
    config->purgeSaves = data->purgeSaves;
    config->showFPS = data->showFPS;
    config->showCollisions->objects = data->showCollisions->objects;
    config->showCollisions->player = data->showCollisions->player;
    config->showCollisions->warps = data->showCollisions->warps;
    return config;
}
