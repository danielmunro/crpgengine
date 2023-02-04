#include "header.h"

int main(void) {
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");
    struct Game *g = createGameInstance();
    SetWindowTitle(g->scenes[g->scene]->name);
    run(g);
    UnloadTexture(g->scenes[g->scene]->player->sprite->source);
    CloseWindow();
    return 0;
}