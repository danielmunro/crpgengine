#include "header.h"

int showCollisions = 0;

int main(int argc, char *argv[]) {
    char sceneResourceFile[255];
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                showCollisions = 1;
            } else if (argv[i][1] == 's') {
                memset(sceneResourceFile, '\0', sizeof(sceneResourceFile));
                strcpy(sceneResourceFile, &argv[i][2]);
            }
        }
    }
    if (sceneResourceFile[0] == '\0') {
        fprintf(stderr, "scene required, argument is -s\n");
        return 1;
    }
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");
    struct Game *g = createGameInstance(sceneResourceFile, showCollisions);
    Scene *s = getScene(g);
    SetWindowTitle(s->name);
    run(g);
    CloseWindow();
    return 0;
}