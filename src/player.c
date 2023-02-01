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
