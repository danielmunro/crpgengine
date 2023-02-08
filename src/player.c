typedef struct Player {
    struct Sprite *sprite;
    Vector2 pos;
    int isMoving;
    float direction;
    struct timeval lastMovement;
    float moveSpeed;
} Player;

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    player->isMoving = 0;
    player->moveSpeed = 0.9f;
    gettimeofday(&player->lastMovement, NULL);
    return player;
}

void movePlayer(Player *p, int direction, Vector2 pos) {
    p->isMoving = 1;
    p->sprite->direction = direction;
    p->pos = pos;
    gettimeofday(&p->lastMovement, NULL);
}
