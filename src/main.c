#include "header.h"

int main(int argc, char *argv[]) {
    RuntimeArgs *r = createRuntimeArgs(argc, argv);

    if (r->indexDir == NULL) {
        fprintf(stderr, "-i indexDir required\n");
        return 1;
    }

    Config *cfg = loadAppConfig(r->indexDir);
    SetTargetFPS(TARGET_FRAMERATE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, cfg->title);
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("not ready");
        exit(1);
    }

    Game *g = createGame(r);
    validateGameData(g);

    if (r->dump) {
        dumpGame(g);
        return 0;
    }

    run(g);

    return 0;
}