#include "tap.h"
#include "headers/test.h"
#include "headers/global_setup.h"
#include "headers/fight_test.h"
#include "headers/util/test_util.h"

void canMoveMobTest(Context *c) {
    plan(2);

    // given
    float startX = 100;
    float startY = 100;
    Vector2 start = (Vector2) {startX, startY};
    AnimationManager *am = createTestAnimationManager(c);
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);

    Mobile *mob = createMobile(
            "test",
            "test",
            start,
            DIRECTION_DOWN,
            animations,
            createAvatar(
                    c->indexDir,
                    "fireas.png",
                    c->ui->screen->scale),
            20,
            20,
            createTestAttributes(),
            mapDataToSpells(NULL, NULL, 0),
            0,
            false);

    // when
    moveMob(
            mob,
            (Vector2) {startX + 5, startY + 5},
            (float) c->ui->screen->targetFrameRate);

    // then
    ok(mob->position.x > startX && mob->position.y > startY, "mob moved as expected");
    ok(mob->direction == DIRECTION_RIGHT, "mob is facing right");
}

void canMobStopMovingTest(Context *c) {
    plan(2);

    // given
    float startX = 100;
    float startY = 100;
    Vector2 start = (Vector2) {startX, startY};
    AnimationManager *am = createTestAnimationManager(c);
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    Mobile *mob = createMobile(
            "test",
            "test",
            start,
            DIRECTION_DOWN,
            animations,
            createAvatar(
                    c->indexDir,
                    "fireas.png",
                    c->ui->screen->scale),
            STARTING_HP,
            STARTING_MANA,
            createTestAttributes(),
            mapDataToSpells(NULL, NULL, 0),
            0,
            false);

    // when
    moveMob(mob, start, (float) c->ui->screen->targetFrameRate);

    // then
    ok(mob->position.x == startX && mob->position.y == startY, "mob moved as expected");
    ok(!isMoving(mob), "mob is not moving");
}

void experienceToLevel1Test() {
    plan(1);

    // when
    int experience = getExperienceToLevel(1);

    // then
    ok(experience == 1000, "experience value was expected :: %d", experience);
}

void experienceToLevel51Test() {
    plan(1);

    // when
    int experience = getExperienceToLevel(51);

    // then
    ok(experience == 133650, "experience value was expected :: %d", experience);
}

int main() {
    Context *c = globalSetup();
    createFightInSceneTest(c);
    canMoveMobTest(c);
    canMobStopMovingTest(c);
    experienceToLevel1Test();
    experienceToLevel51Test();
    done_testing();
}