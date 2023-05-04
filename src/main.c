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

    Game *g = createGameInstance(sceneIndex, showCollisions, indexDir);
    validateGameData(g);

    if (dump == true) {
        printf("animations\n==============\n");
        for (int i = 0; i < g->animIndex; i++) {
            printf("%d %d %d %d %d\n",
                    g->animations[i]->type,
                    g->animations[i]->isPlaying,
                    g->animations[i]->currentFrame,
                    g->animations[i]->frameRate,
                    g->animations[i]->frameRateCount);
        }
        for (int i = 0; i < MAX_SCENES; i++) {
            if (g->scenes[i] == NULL) {
                break;
            }
            printf("scene %s objects\n===============\n", g->scenes[i]->name);
            for (int o = 0; o < MAX_OBJECTS; o++) {
                if (g->scenes[i]->objects[o] == NULL) {
                    break;
                }
                printf("%d %d\n",
                       g->scenes[i]->objects[o]->id,
                       g->scenes[i]->objects[o]->tile
                );
            }
        }
        return 0;
    }
    run(g);

    return 0;
}