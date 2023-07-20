typedef struct {
    Mobile *party[MAX_PARTY_SIZE];
    int partyCount;
    Mobile *team[MAX_TEAM_SIZE];
    int teamCount;
    const char *stories[MAX_STORIES];
    int storyCount;
    Item *items[MAX_ITEMS];
    int itemCount;
    int coins;
    int secondsPlayed;
    Mobile *blockedBy;
    Mobile *engageable;
    bool engaged;
    Log *log;
} Player;

void addItem(Player *player, Item *item) {
    player->items[player->itemCount] = item;
    player->itemCount++;
}

Player *createPlayer(Log *log, Mobile *mobs[MAX_PARTY_SIZE]) {
    Player *player = malloc(sizeof(Player));
    player->blockedBy = NULL;
    player->engageable = NULL;
    player->engaged = false;
    player->storyCount = 0;
    player->teamCount = 1;
    player->log = log;
    ConsumeAffect *affect = malloc(sizeof(ConsumeAffect));
    affect->hp = 20;
    player->itemCount = 0;
    player->partyCount = 0;
    player->coins = 0;
    player->secondsPlayed;
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        player->party[i] = mobs[i];
        if (mobs[i] == NULL && player->partyCount == 0) {
            player->partyCount = i;
        }
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
    return p->party[0];
}

void addStory(Player *p, const char *story) {
    p->stories[p->storyCount++] = story;
    addInfo(p->log, "add story to player :: %s", story);
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

void checkMoveKey(Player *p, int key, AnimationType direction) {
    if (IsKeyDown(key) && !p->engaged) {
        getPartyLeader(p)->moving[direction] = true;
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

void engageWithMobile(Player *p) {
    p->engageable = p->blockedBy;
    addInfo(p->log, "updating mob direction to: %d", getOppositeDirection(getPartyLeader(p)->direction));
    updateDirection(p->blockedBy, getOppositeDirection(getPartyLeader(p)->direction));
    addInfo(p->log, "engaging with %s", p->engageable->name);
    p->engaged = true;
}

void disengageWithMobile(Player *p) {
    p->engaged = false;
    updateDirection(p->blockedBy, p->blockedBy->previousDirection);
}
