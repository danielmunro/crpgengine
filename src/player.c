typedef struct Moving {
    int up;
    int down;
    int left;
    int right;
} Moving;

typedef struct Player {
    Mobile *mob;
    Vector2 pos;
    float direction;
    Moving moving;
    struct timeval lastMovement;
    Mobile *blockedBy;
    Mobile *engageable;
    int engaged;
} Player;

Player *createPlayer() {
    Player *player = malloc(sizeof(Player));
    player->moving.down = 0;
    player->moving.up = 0;
    player->moving.left = 0;
    player->moving.right = 0;
    gettimeofday(&player->lastMovement, NULL);
    player->engageable = NULL;
    player->engaged = false;
    return player;
}

void resetMoving(Player *p) {
    p->moving.up = 0;
    p->moving.down = 0;
    p->moving.left = 0;
    p->moving.right = 0;
}

void checkMoveKey(Player *p, int key, int direction) {
    if (IsKeyDown(key) && !p->engaged) {
        if (direction == DIRECTION_UP) {
            p->moving.up = 1;
        }
        if (direction == DIRECTION_DOWN) {
            p->moving.down = 1;
        }
        if (direction == DIRECTION_LEFT) {
            p->moving.left = 1;
        }
        if (direction == DIRECTION_RIGHT) {
            p->moving.right = 1;
        }
        p->mob->direction = direction;
        getMobAnimation(p->mob)->isPlaying = 1;
    }
}

int isSpeakingTo(Player *p, Mobile *target) {
    return p->engaged && target == p->engageable;
}
