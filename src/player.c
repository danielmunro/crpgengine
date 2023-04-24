typedef struct Player {
    Mobile *mob;
    Vector2 pos;
    float direction;
    struct timeval lastMovement;
} Player;

void movePlayer(Player *p, int direction, Vector2 pos) {
    p->mob->direction = direction;
    p->mob->position = pos;
}

void checkMoveKey(Player *p, int key, int direction) {
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
        getMobAnimation(p->mob)->isPlaying = 1;
    }
}

void checkInput(Player *p) {
    getMobAnimation(p->mob)->isPlaying = 0;
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
