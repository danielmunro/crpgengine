#include <tap.h>
#include "headers/global_setup.h"
#include "headers/fight_test.h"
#include "headers/mobile_test.h"
#include "headers/player_test.h"
#include "headers/player_manager_test.h"
#include "headers/fight_manager_test.h"

int main(int argc, char *argv[]) {
    const char *configFile = getConfigFileName(argc, argv);
    Context *c = globalSetup(configFile);
    testBeastCountWithinExpectedRange(c);
    testCanMoveMob(c);
    testMobCanStopMoving(c);
    testGainExperienceToLevel1();
    testGainExperienceToLevel51();
    testCanSavePlayer(c);
    testCanLoadPlayer(c);
    testCanFleeSuccessfully(c);
    testCanFailAttemptingToFlee(c);
    done_testing();
}