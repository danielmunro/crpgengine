#include "header.h"

int main(int argc, char *argv[]) {
    setupApp();
    createRuntimeArgs(argc, argv);
    ConfigData *appCfg = loadAppConfigYaml(runtimeArgs->indexDir);
    UIData *uiCfg = loadUIData(runtimeArgs->indexDir);
    createLog(runtimeArgs->logLevel);
    initWindow(appCfg->title, uiCfg);
    Game *g = createGame(uiCfg);
    validateGameData(g);
    if (runtimeArgs->purgeSaves) {
        purgeSaves(runtimeArgs->indexDir);
    } else if (runtimeArgs->dump) {
        dumpGame(g);
    } else if (!runtimeArgs->exit) {
        run(g);
    }
    return 0;
}