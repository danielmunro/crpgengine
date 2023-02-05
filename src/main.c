#include "header.h"

int showCollisions = 0;

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("argv[%u] = %s\n", i, argv[i]);
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                showCollisions = 1;
                printf("DING\n");
            }
        }
    }
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");
    struct Game *g = createGameInstance(showCollisions);
    SetWindowTitle(g->scenes[g->scene]->name);
    run(g);
    UnloadTexture(g->scenes[g->scene]->player->sprite->source);
    CloseWindow();
    return 0;
}