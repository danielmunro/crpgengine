typedef struct Player {
    Mobile *mob;
    Vector2 pos;
    int isMoving;
    float direction;
    struct timeval lastMovement;
    float moveSpeed;
} Player;

void movePlayer(Player *p, int direction, Vector2 pos) {
    p->mob->isMoving = 1;
    p->mob->direction = direction;
    p->mob->position = pos;
}
