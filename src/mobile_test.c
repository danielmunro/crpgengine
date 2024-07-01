void testCanMoveMob(Context *c) {
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

void testMobCanStopMoving(Context *c) {
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
