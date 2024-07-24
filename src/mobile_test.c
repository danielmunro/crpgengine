#include <tap.h>
#include "headers/map_input_handler.h"
#include "headers/spell_manager.h"
#include "headers/util/test_util.h"

void testCanMoveMob(Context *c) {
    // setup
    int startX = 100;
    int startY = 100;
    Vector2D start = (Vector2D) {startX, startY};
    AnimationManager *am = createTestAnimationManager(c);
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    Spell **spells = calloc(0, sizeof(Spell));
    MobileManager *mm = createMobileManager(createSpellManager(spells, 0), am, c);
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
    addMobileToManager(mm, mob);

    // when
    for (int i = 0; i < c->game->tileSize * 2; i++) {
        moveMob(
                mob,
                (Vector2D) {startX + c->game->tileSize, startY + c->game->tileSize},
                tileSize(c));
        evaluateMovement(mm);
    }

    // then
    ok(mob->position.x > startX && mob->position.y > startY, "mob moved as expected");
    ok(mob->direction == DIRECTION_DOWN, "mob is facing down");
}

void testMobCanStopMoving(Context *c) {
    // given
    int startX = 100;
    int startY = 100;
    Vector2D start = (Vector2D) {startX, startY};
    const AnimationManager *am = createTestAnimationManager(c);
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    Avatar *avatar = createAvatar(
            c->indexDir,
            "fireas.png",
            c->ui->screen->scale);
    Mobile *mob = createMobile(
            "test",
            "test",
            start,
            DIRECTION_DOWN,
            animations,
            avatar,
            STARTING_HP,
            STARTING_MANA,
            createTestAttributes(),
            mapDataToSpells(NULL, NULL, 0),
            0,
            false);

    // when
    moveMob(mob, start, tileSize(c));

    // then
    ok(mob->position.x == startX && mob->position.y == startY, "mob moved as expected");
    ok(!isMoving(mob), "mob is not moving");
}
