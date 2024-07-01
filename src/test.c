#include <tap.h>
#include "headers/global_setup.h"
#include "headers/fight_test.h"
#include "headers/mobile_test.h"
#include "headers/player_test.h"

int main() {
    Context *c = globalSetup();
    createFightInSceneTest(c);
    canMoveMobTest(c);
    canMobStopMovingTest(c);
    experienceToLevel1Test();
    experienceToLevel51Test();
    done_testing();
}