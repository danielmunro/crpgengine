Mobile *createTestHumanoid() {
    Mobile *mob = malloc(sizeof(Mobile));
    mob->direction = DIRECTION_DOWN;
    mob->position = (Vector2){ ((float)SCREEN_WIDTH / 2) - (HUMANOID_WIDTH / 2), ((float)SCREEN_HEIGHT / 2) - (HUMANOID_HEIGHT / 2) };
    loadAnimations("resources/animations/fireas.txt", mob->animations);
    return mob;
}

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->mob = createTestHumanoid();
    gettimeofday(&player->lastMovement, NULL);
    return player;
}
