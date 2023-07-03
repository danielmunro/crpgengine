typedef struct {
    Mobile *mobs[MAX_PARTY_SIZE];
    bool moving[DIRECTION_COUNT];
    struct timeval lastMovement;
    Mobile *blockedBy;
    Mobile *engageable;
    int engaged;
    const char *stories[MAX_STORIES];
    int storyCount;
    Log *log;
    Item *items[MAX_ITEMS];
    int itemCount;
} Player;

void addItem(Player *player, Item *item) {
    player->items[player->itemCount] = item;
    player->itemCount++;
}

Player *createPlayer(Log *log, Mobile *mobs[MAX_PARTY_SIZE]) {
    Player *player = malloc(sizeof(Player));
    player->moving[UP] = false;
    player->moving[DOWN] = false;
    player->moving[LEFT] = false;
    player->moving[RIGHT] = false;
    gettimeofday(&player->lastMovement, NULL);
    player->blockedBy = NULL;
    player->engageable = NULL;
    player->engaged = false;
    player->storyCount = 0;
    player->log = log;
    ConsumeAffect *affect = malloc(sizeof(ConsumeAffect));
    affect->hp = 20;
    player->itemCount = 0;
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        player->mobs[i] = mobs[i];
    }
    addItem(player, createItem(
            CONSUMABLE,
            "id",
            "potion",
            NULL,
            affect));
    return player;
}

Mobile *getPartyLeader(Player *p) {
    return p->mobs[0];
}

void addStory(Player *p, const char *story) {
    p->stories[p->storyCount++] = story;
    addInfo(p->log, "add story to player: %s", p->stories[0]);
}

bool hasStory(Player *p, const char *story) {
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
    p->moving[UP] = false;
    p->moving[DOWN] = false;
    p->moving[LEFT] = false;
    p->moving[RIGHT] = false;
}

void checkMoveKey(Player *p, int key, AnimationDirection direction) {
    if (IsKeyDown(key) && !p->engaged) {
        p->moving[direction] = true;
        Mobile *mob = getPartyLeader(p);
        mob->direction = direction;
        getMobAnimation(mob)->isPlaying = true;
    }
}

void explorationCheckMoveKeys(Player *player) {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        checkMoveKey(player, MOVE_KEYS[i], DIRECTIONS[i]);
    }
}

bool isSpeakingTo(Player *p, Mobile *target) {
    return p->engaged && target == p->engageable;
}

bool isMoving(Player *p) {
    return p->moving[DOWN]
           || p->moving[UP]
           || p->moving[LEFT]
           || p->moving[RIGHT];
}
