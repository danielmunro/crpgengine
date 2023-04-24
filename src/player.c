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

int checkMoveKey(Player *p, int key, int direction) {
    if (IsKeyDown(key)) {
        Vector2 pos = p->mob->position;
        if (direction == DIRECTION_UP) {
            pos.y -= 1;
        }
        if (direction == DIRECTION_DOWN) {
            pos.y += 1;
        }
        if (direction == DIRECTION_LEFT) {
            pos.x -= 1;
        }
        if (direction == DIRECTION_RIGHT) {
            pos.x += 1;
        }
        movePlayer(p, direction, pos);
        incrementAnimFrame(getMobAnimation(p->mob));
        return 1;
    }
    return 0;
}

void checkInput(Player *p) {
    checkMoveKey(
            p,
            KEY_UP,
            DIRECTION_UP
    );
    checkMoveKey(
            p,
            KEY_DOWN,
            DIRECTION_DOWN
    );
    checkMoveKey(
            p,
            KEY_RIGHT,
            DIRECTION_RIGHT
    );
    checkMoveKey(
            p,
            KEY_LEFT,
            DIRECTION_LEFT
    );
}
