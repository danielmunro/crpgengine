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

AnimationManager *createTestAnimationManager() {
    AnimationManager *am = createAnimationManager(createLog(INFO));
    SpritesheetManager *sm = loadSpritesheetManager(am->log, "./fixtures");
    loadAllAnimations(am, sm, "./fixtures");
    return am;
}

void strToIntTest() {
    ok(strToInt("123042") == 123042, "strToInt works as expected");
}

void createFightInSceneTest() {
    Log *log = createLog(ERROR);
    Encounters *e = createEncounters();
    e->beastEncountersCount = 1;
    e->beastEncounters[0] = createBeastEncounter(createTestBeast(), 5);
    FightManager *fm = createFightManager(log);
    for (int i = 0; i < 100; i++) {
        createFightFromEncounters(
                fm,
                log,
                e,
                createNewPlayer(
                        log,
                        createTestAnimationManager(),
                        "examples/simple_demo"),
                NULL);
        Fight *f = fm->fight;
        char message[MAX_LOG_LINE_LENGTH];
        sprintf(message, "beast count is within expected range: %d", f->beastCount);
        ok(0 < f->beastCount && f->beastCount <= 9, message);
    }
}

void canMoveMobTest() {
    // given
    float startX = 100, startY = 100;
    Vector2 start = (Vector2){startX, startY};
    AnimationManager *am = createTestAnimationManager();
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);

    Mobile *mob = createMobile("test", "test", start, DOWN, animations, 20, 20, createStartingAttributes());

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
    AnimationManager *am = createTestAnimationManager();
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    Mobile *mob = createMobile(
            "test", "test", start, DOWN,
            animations, STARTING_HP, STARTING_MANA, createStartingAttributes());

    // when
    moveMob(mob, start);

    // then
    ok(mob->position.x == startX && mob->position.y == startY, "mob moved as expected");
    ok(!isMoving(mob), "mob is not moving");
}

void experienceToLevel1Test() {
    // when
    int experience = getExperienceToLevel(1);

    // then
    ok(experience == 1000, "experience value was expected :: %d", experience);
}

void experienceToLevel51Test() {
    // when
    int experience = getExperienceToLevel(51);

    // then
    ok(experience == 133650, "experience value was expected :: %d", experience);
}

int main() {
    initWindow("./fixtures");
    plan(107);
    strToIntTest();
    createFightInSceneTest();
    canMoveMobTest();
    canMobStopMovingTest();
    experienceToLevel1Test();
    experienceToLevel51Test();
    done_testing();
}