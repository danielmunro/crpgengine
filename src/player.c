typedef struct Player {
    Mobile *mob;
    Vector2 pos;
    float direction;
    struct timeval lastMovement;
} Player;

void checkMoveKey(Player *p, int key, int direction) {
    if (IsKeyDown(key)) {
        if (direction == DIRECTION_UP) {
            p->mob->position.y -= 1;
        }
        if (direction == DIRECTION_DOWN) {
            p->mob->position.y += 1;
        }
        if (direction == DIRECTION_LEFT) {
            p->mob->position.x -= 1;
        }
        if (direction == DIRECTION_RIGHT) {
            p->mob->position.x += 1;
        }
        p->mob->direction = direction;
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
