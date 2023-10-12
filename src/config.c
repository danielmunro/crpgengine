#include <stdbool.h>
#include "headers/util/log.h"
#include "headers/persistence/cyaml.h"

typedef struct {
    bool dumpState;
    bool exit;
} AfterLoad;

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
    bool forceNewGame;
    bool logMemoryUsage;
    bool purgeSaves;
    bool showFPS;
    AfterLoad *afterLoad;
    ShowCollisions *showCollisions;
} Config;

Config *config;

Config *createConfigFromData(const ConfigData *data) {
    config = malloc(sizeof(Config));
    config->afterLoad = malloc(sizeof(AfterLoad));
    config->showCollisions = malloc(sizeof(ShowCollisions));
    config->logLevel = getLogLevelFromString(data->logLevel);
    config->indexDir = data->indexDir;
    config->startScene = data->startScene;
    config->overrideStartScene = data->overrideStartScene;
    config->saveFile = data->saveFile;
    config->forceNewGame = data->forceNewGame;
    config->logMemoryUsage = data->logMemoryUsage;
    config->purgeSaves = data->purgeSaves;
    config->showFPS = data->showFPS;
    config->afterLoad->dumpState = data->afterLoad->dumpState;
    config->afterLoad->exit = data->afterLoad->exit;
    config->showCollisions->objects = data->showCollisions->objects;
    config->showCollisions->player = data->showCollisions->player;
    config->showCollisions->warps = data->showCollisions->warps;
    return config;
}
