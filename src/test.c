#include "header.h"
#include "tap.h"

Beast *createTestBeast() {
    Beast *beast = malloc(sizeof(Beast));
    beast->id = 0;
    beast->name = "foo";
    beast->gp = 1;
    beast->hp = 20;
    beast->mana = 100;
    beast->level = 100;
    char filePath[255];
    beast->attributes = createEmptyAttributes();
    return beast;
}

void strToIntTest() {
    ok(strToInt("123042") == 123042, "strToInt works as expected");
}

void createFightInSceneTest() {
    Log *log = createLog(ERROR);
    Encounters *e = createEncounters();
    e->beastEncountersCount = 1;
    e->beastEncounters[0] = createBeastEncounter(createTestBeast(), 5);
    for (int i = 0; i < 100; i++) {
        Fight *f = createFightFromEncounters(log, e);
        char message[255];
        sprintf(message, "beast count is not within expected range: %d", f->beastCount);
        ok(0 < f->beastCount && f->beastCount <= 9, message);
    }
}

int main() {
    plan(101);
    strToIntTest();
    createFightInSceneTest();
    done_testing();
}