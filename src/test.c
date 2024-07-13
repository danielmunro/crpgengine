#include <tap.h>
#include "headers/global_setup.h"
#include "headers/fight_test.h"
#include "headers/mobile_test.h"
#include "headers/player_test.h"
#include "headers/player_manager_test.h"
#include "headers/fight_manager_test.h"
#include "headers/graphics/ui/menus/startup/main_menu_test.h"

bool shouldUpdateSnapshots(int argc, char *argv[]) {
    return argc > 1 && strcmp(argv[1], "-u") == 0;
}

int main(int argc, char *argv[]) {
    const char *configFile = getConfigFileName(argc, argv);
    Context *c = globalSetup(configFile);
    bool updateSnapshots = shouldUpdateSnapshots(argc, argv);
    testBeastCountWithinExpectedRange(c);
    testCanMoveMob(c);
    testMobCanStopMoving(c);
    testGainExperienceToLevel1();
    testGainExperienceToLevel51();
    testCanSavePlayer(c);
    testCanLoadPlayer(c);
    testCanFleeSuccessfully(c);
    testCanFailAttemptingToFlee(c);
    testMainMenuRenders(c, updateSnapshots);
    CloseWindow();
    done_testing();
}