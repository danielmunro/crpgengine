typedef struct Player {
    struct Sprite *sprite;
    Vector2 pos;
    int isMoving;
    struct timeval lastMovement;
} Player;

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    player->pos.x = 20;
    player->pos.y = 20;
    player->isMoving = 0;
    gettimeofday(&player->lastMovement, NULL);
    return player;
}

void movePlayer(Player *p, int direction, Vector2 pos) {
    p->isMoving = 1;
    p->sprite->direction = direction;
    p->pos = pos;
    gettimeofday(&p->lastMovement, NULL);
}
