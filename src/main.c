#include "header.h"

int main(int argc, char *argv[]) {
    int sceneIndex = 0;
    int showCollisions = false;
    int dump = false;
    char *indexDir = NULL;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                showCollisions = true;
            } else if (argv[i][1] == 's') {
                sceneIndex = strToInt(argv[i + 1]);
            } else if (argv[i][1] == 'd') {
                dump = true;
            } else if (argv[i][1] == 'i') {
                indexDir = argv[i + 1];
            }
        }
    }
    if (indexDir == NULL) {
        fprintf(stderr, "-i indexDir required\n");
        return 1;
    }
    SetTargetFPS(TARGET_FRAMERATE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "crpgengine");

    Game *g = createGame(sceneIndex, showCollisions, indexDir);
    validateGameData(g);

    if (dump == true) {
        dumpGame(g);
        return 0;
    }
    
    run(g);
    return 0;
}