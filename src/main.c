#include "headers/global_setup.h"
#include "headers/context.h"

int main(int argc, char *argv[]) {
    Context *c = globalSetup(getConfigFileName(argc, argv));
    Game *g = createGame(c);
    mainMenuLoop(g);
    runGame(g);
    return 0;
}