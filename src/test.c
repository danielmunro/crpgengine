#include "header.h"
#include "tap.h"

void strToIntTest() {
    ok(strToInt("123042") == 123042, "strToInt works as expected");
}

void createFightInSceneTest() {
    char *args[] = {
            "",
            "-i",
            "examples/tinytown",
            "-l",
            "debug",
    };
    RuntimeArgs *r = createRuntimeArgs(4, args);
    Game *g = createGame(r);
    for (int i = 0; i < 100; i++) {
        Fight *f = createFightInScene(g->scenes[1]);
        char message[255];
        sprintf(message, "beast count is not within expected range: %d", f->beastCount);
        ok(0 < f->beastCount && f->beastCount <= 9, message);
    }
}

int main() {
    initWindow("examples/tinytown");
    plan(101);
    strToIntTest();
    createFightInSceneTest();
    done_testing();
}