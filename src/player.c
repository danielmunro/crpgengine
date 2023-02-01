typedef struct Player {
    struct Sprite *sprite;
    int x;
    int y;
} Player;

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    player->x = 10;
    player->y = 10;
    return player;
}

void movePlayer(Player *p, int direction) {
    moveSprite(p->sprite, direction);
    if (direction == DIRECTION_UP) {
        p->y = p->y - 1;
    } else if (direction == DIRECTION_DOWN) {
        p->y = p->y + 1;
    } else if (direction == DIRECTION_LEFT) {
        p->x = p->x - 1;
    } else if (direction == DIRECTION_RIGHT) {
        p->x = p->x + 1;
    }
}
