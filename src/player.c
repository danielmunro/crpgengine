typedef struct Player {
    struct Sprite *sprite;
    Mobile *mob;
    Vector2 pos;
    int isMoving;
    float direction;
    struct timeval lastMovement;
    float moveSpeed;
} Player;

Mobile *createTestHumanoid() {
    Mobile *mob = malloc(sizeof(Mobile));
    mob->direction = DIRECTION_DOWN;
    mob->position = (Vector2){ ((float)SCREEN_WIDTH / 2) - (HUMANOID_WIDTH / 2), ((float)SCREEN_HEIGHT / 2) - (HUMANOID_HEIGHT / 2) };
    mob->isMoving = 0;
    loadAnimations("resources/animations/fireas.txt", mob->animations);
    return mob;
}

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->mob = createTestHumanoid();
    player->isMoving = 0;
    player->moveSpeed = 0.9f;
    gettimeofday(&player->lastMovement, NULL);
    return player;
}

void movePlayer(Player *p, int direction, Vector2 pos) {
    p->mob->isMoving = 1;
    p->mob->direction = direction;
    p->mob->position = pos;
}
