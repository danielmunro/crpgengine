#include "headers/util/log.h"
#include "headers/graphics/ui/ui.h"

typedef struct {
    const Log *log;
    const GameConfig *game;
    const UIConfig *ui;
    UserConfig *user;
    const char *indexDir;
} Context;

Context *createContext(
        const Log *log,
        const GameConfig *gameConfig,
        const UIConfig *uiConfig,
        UserConfig *userConfig) {
    Context *c = malloc(sizeof(Context));
    c->log = log;
    c->game = gameConfig;
    c->ui = uiConfig;
    c->user = userConfig;
    c->indexDir = gameConfig->indexDir;
    return c;
}

int tileSize(Context *c) {
    return c->game->tileSize;
}
