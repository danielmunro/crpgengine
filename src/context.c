#include "headers/util/log.h"
#include "headers/graphics/ui/ui.h"

typedef struct {
    const Log *log;
    const GameConfig *game;
    const UIConfig *ui;
    UserConfig *user;
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
    return c;
}