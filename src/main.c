#include "header.h"

int main(int argc, char *argv[]) {
    setupApp();
    RuntimeArgs *r = createRuntimeArgs(argc, argv);
    ConfigData *appCfg = loadAppConfigYaml(r->indexDir);
    UIData *uiCfg = loadUIData(r->indexDir);
    initWindow(appCfg->title, uiCfg);
    Game *g = createGame(uiCfg, r);
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