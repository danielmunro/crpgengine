Mobile *createTestHumanoid() {
    Mobile *mob = createMobile();
    mob->direction = DIRECTION_DOWN;
    mob->position = (Vector2){ ((float)SCREEN_WIDTH / 2) - (HUMANOID_WIDTH / 2), ((float)SCREEN_HEIGHT / 2) - (HUMANOID_HEIGHT / 2) };
    loadAnimations("examples/tinytown/animations/fireas.txt", mob->animations);
    return mob;
}

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->mob = createTestHumanoid();
    player->moving.down = 0;
    player->moving.up = 0;
    player->moving.left = 0;
    player->moving.right = 0;
    gettimeofday(&player->lastMovement, NULL);
    return player;
}
