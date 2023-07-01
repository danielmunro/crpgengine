typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
} Moving;

typedef struct {
    Mobile *mobs[MAX_PARTY_SIZE];
    Moving moving;
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
    player->moving.down = false;
    player->moving.up = false;
    player->moving.left = false;
    player->moving.right = false;
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
    p->moving.up = false;
    p->moving.down = false;
    p->moving.left = false;
    p->moving.right = false;
}

void checkMoveKey(Player *p, int key, AnimationDirection direction) {
    if (IsKeyDown(key) && !p->engaged) {
        if (direction == UP) {
            p->moving.up = true;
        }
        if (direction == DOWN) {
            p->moving.down = true;
        }
        if (direction == LEFT) {
            p->moving.left = true;
        }
        if (direction == RIGHT) {
            p->moving.right = true;
        }
        Mobile *mob = getPartyLeader(p);
        mob->direction = direction;
        getMobAnimation(mob)->isPlaying = true;
    }
}

bool isSpeakingTo(Player *p, Mobile *target) {
    return p->engaged && target == p->engageable;
}

bool isMoving(Player *p) {
    return p->moving.down
           || p->moving.up
           || p->moving.left
           || p->moving.right;
}
