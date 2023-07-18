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
        char message[MAX_LOG_LINE_LENGTH];
        sprintf(message, "beast count is within expected range: %d", f->beastCount);
        ok(0 < f->beastCount && f->beastCount <= 9, message);
    }
}

void canMoveMobTest() {
    // given
    float startX = 100, startY = 100;
    Vector2 start = (Vector2){startX, startY};
    AnimationManager *am = createAnimationManager(createLog(INFO));
    SpritesheetManager *sm = loadSpritesheetManager(am->log, "./fixtures");
    Animation *animations[MAX_ANIMATIONS];
    loadAllAnimations(am, sm, "./fixtures");
    loadAnimationsByName(am, "fireas", animations);
    Mobile *mob = createMobile("test", "test", start, DOWN, animations);

    // when
    moveMob(mob, (Vector2){startX + 5, startY + 5});

    // then
    ok(mob->position.x > startX && mob->position.y > startY, "mob moved as expected");
    ok(mob->direction == RIGHT, "mob is facing right");
}

void canMobStopMovingTest() {
    // given
    float startX = 100, startY = 100;
    Vector2 start = (Vector2){startX, startY};
    AnimationManager *am = createAnimationManager(createLog(INFO));
    SpritesheetManager *sm = loadSpritesheetManager(am->log, "./fixtures");
    Animation *animations[MAX_ANIMATIONS];
    loadAllAnimations(am, sm, "./fixtures");
    loadAnimationsByName(am, "fireas", animations);
    Mobile *mob = createMobile("test", "test", start, DOWN, animations);

    // when
    moveMob(mob, start);

    // then
    ok(mob->position.x == startX && mob->position.y == startY, "mob moved as expected");
    ok(!isMoving(mob), "mob is not moving");
}

int main() {
    initWindow("./fixtures");
    plan(105);
    strToIntTest();
    createFightInSceneTest();
    canMoveMobTest();
    canMobStopMovingTest();
    done_testing();
}