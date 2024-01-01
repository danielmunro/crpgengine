#include "headers/global_setup.h"

int main() {
    UserConfig *userConfig = globalSetup();
    Game *g = createGame(userConfig);
    mainMenuLoop(g);
    runGame(g);
    return 0;
}