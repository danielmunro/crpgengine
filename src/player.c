typedef struct Player {
    struct Sprite *sprite;
    int x;
    int y;
    int isMoving;
    long duration;
} Player;

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    player->x = 10;
    player->y = 10;
    player->isMoving = 0;
    player->duration = 0;
    return player;
}
