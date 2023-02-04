typedef struct Player {
    struct Sprite *sprite;
    Vector2 pos;
    int isMoving;
    struct timeval lastMovement;
} Player;

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    player->pos.x = 10;
    player->pos.y = 10;
    player->isMoving = 0;
    gettimeofday(&player->lastMovement, NULL);
    return player;
}

void movePlayer(Player *p, int direction) {
    p->isMoving = 1;
    p->sprite->direction = direction;
    if (direction == DIRECTION_UP) {
        p->pos.y -= 0.1f;
    } else if (direction == DIRECTION_DOWN) {
        p->pos.y += 0.1f;
    }
    if (direction == DIRECTION_LEFT) {
        p->pos.x -= 0.1f;
    } else if (direction == DIRECTION_RIGHT) {
        p->pos.x += 0.1f;
    }
    gettimeofday(&p->lastMovement, NULL);
}
