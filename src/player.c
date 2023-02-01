typedef struct Player {
    struct Sprite *sprite;
    int x;
    int y;
    Vector2D offset;
} Player;

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    player->x = 10;
    player->y = 10;
    Vector2D offset = {0, 0};
    player->offset = offset;
    return player;
}

void animatePlayer(Player *p) {
    if (p->offset.x < 0) p->offset.x = p->offset.x + 1;
    else if (p->offset.x > 0) p->offset.x = p->offset.x - 1;
    else if (p->offset.y < 0) p->offset.y = p->offset.y + 1;
    else if (p->offset.y > 0) p->offset.y = p->offset.y - 1;
    if (p->offset.x != 0 || p->offset.y != 0) animateSprite(p->sprite);
}
