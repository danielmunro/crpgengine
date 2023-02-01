typedef struct Player {
    struct Sprite *sprite;
} Player;

Player *createTestPlayer() {
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    return player;
}
