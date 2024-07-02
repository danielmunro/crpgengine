#include <tap.h>
#include "headers/global_setup.h"
#include "headers/fight_test.h"
#include "headers/mobile_test.h"
#include "headers/player_test.h"
#include "headers/player_manager_test.h"

int main() {
    Context *c = globalSetup();
    testBeastCountWithinExpectedRange(c);
    testCanMoveMob(c);
    testMobCanStopMoving(c);
    testGainExperienceToLevel1();
    testGainExperienceToLevel51();
    testCanSavePlayer();
    done_testing();
}