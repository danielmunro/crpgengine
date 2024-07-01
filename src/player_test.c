#include <tap.h>
#include "headers/player.h"

void testGainExperienceToLevel1() {
    // when
    int experience = getExperienceToLevel(1);

    // then
    ok(experience == 1000, "experience value was expected :: %d", experience);
}

void testGainExperienceToLevel51() {
    // when
    int experience = getExperienceToLevel(51);

    // then
    ok(experience == 133650, "experience value was expected :: %d", experience);
}
