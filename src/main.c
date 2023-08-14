#include "header.h"

int main(int argc, char *argv[]) {
    setupApp();
    RuntimeArgs *r = createRuntimeArgs(argc, argv);
    ConfigData *cfg = loadAppConfigYaml(r->indexDir);
    initWindow(r->indexDir);
    Game *g = createGame(cfg, r);
    validateGameData(g);
    if (r->purgeSaves) {
        purgeSaves(r->indexDir);
    } else if (r->dump) {
        dumpGame(g);
    } else if (!r->exit) {
        run(g);
    }
    return 0;
}