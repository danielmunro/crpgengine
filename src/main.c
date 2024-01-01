#include "headers/global_setup.h"
#include "headers/context.h"

int main() {
    Context *c = globalSetup();
    Game *g = createGame(c);
    mainMenuLoop(g);
    runGame(g);
    return 0;
}