#include <tap.h>
#include "headers/global_setup.h"
#include "headers/fight_test.h"
#include "headers/mobile_test.h"

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
    Context *c = globalSetup();
    createFightInSceneTest(c);
    canMoveMobTest(c);
    canMobStopMovingTest(c);
    experienceToLevel1Test();
    experienceToLevel51Test();
    done_testing();
}