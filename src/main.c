#include "header.h"

int main(int argc, char *argv[]) {
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));
    RuntimeArgs *r = createRuntimeArgs(argc, argv);
    char saveDir[MAX_FS_PATH_LENGTH];
    sprintf(saveDir, "%s/_saves/autosave.yaml", r->indexDir);
    printf("save dir: %s\n", saveDir);
    SaveData *save = loadSaveData(saveDir);
    printf("%d\n", save->coins);
    exit(1);
    initWindow(r->indexDir);
    Game *g = createGame(r);
    validateGameData(g);
    if (r->dump) {
        dumpGame(g);
        return 0;
    }
    run(g);

    return 0;
}