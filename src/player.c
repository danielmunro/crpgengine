typedef struct {
    int up;
    int down;
    int left;
    int right;
} Moving;

typedef struct {
    Mobile *mob;
    Moving moving;
    struct timeval lastMovement;
    Mobile *blockedBy;
    Mobile *engageable;
    int engaged;
    char *stories[MAX_STORIES];
    int storyCount;
    Log *log;
} Player;

Player *createPlayer(Log *log) {
    Player *player = malloc(sizeof(Player));
    player->moving.down = 0;
    player->moving.up = 0;
    player->moving.left = 0;
    player->moving.right = 0;
    gettimeofday(&player->lastMovement, NULL);
    player->blockedBy = NULL;
    player->engageable = NULL;
    player->engaged = false;
    player->storyCount = 0;
    player->log = log;
    return player;
}

void addStory(Player *p, char *story) {
    p->stories[p->storyCount++] = story;
    addInfo(p->log, "add story to player: %s", p->stories[0]);
}

int hasStory(Player *p, char *story) {
    for (int j = 0; j < p->storyCount; j++) {
        if (strcmp(story, p->stories[j]) == 0) {
            addDebug(p->log, "player has story: %s", story);
            return true;
        }
    }
    addDebug(p->log, "player does not have story: %s", story);
    return false;
}

void resetMoving(Player *p) {
    p->moving.up = 0;
    p->moving.down = 0;
    p->moving.left = 0;
    p->moving.right = 0;
}

void checkMoveKey(Player *p, int key, int direction) {
    if (IsKeyDown(key) && !p->engaged) {
        if (direction == UP) {
            p->moving.up = 1;
        }
        if (direction == DOWN) {
            p->moving.down = 1;
        }
        if (direction == LEFT) {
            p->moving.left = 1;
        }
        if (direction == RIGHT) {
            p->moving.right = 1;
        }
        p->mob->direction = direction;
        getMobAnimation(p->mob)->isPlaying = 1;
    }
}

int isSpeakingTo(Player *p, Mobile *target) {
    return p->engaged && target == p->engageable;
}

int isMoving(Player *p) {
    return p->moving.down
           || p->moving.up
           || p->moving.left
           || p->moving.right;
}
