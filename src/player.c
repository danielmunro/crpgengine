typedef struct {
    Mobile *party[MAX_PARTY_SIZE];
    int partyCount;
    Mobile *onDeck[MAX_TEAM_SIZE];
    int onDeckCount;
    const char *storylines[MAX_STORIES];
    int storylineCount;
    Item *items[MAX_ITEMS];
    int itemQuantities[MAX_ITEMS];
    int itemCount;
    int coins;
    int secondsPlayed;
    int experience;
    int level;
    Mobile *blockedBy;
    Mobile *engageable;
    bool engaged;
    Log *log;
} Player;

void addItem(Player *player, Item *item) {
    for (int i = 0; i < player->itemCount; i++) {
        if (player->items[i]->name == item->name) {
            player->itemQuantities[i]++;
            return;
        }
    }
    player->items[player->itemCount] = item;
    player->itemQuantities[player->itemCount] = 1;
    player->itemCount++;
}

Player *createPlayer(Log *log, Mobile *mobs[MAX_PARTY_SIZE],
                     int coins, int experience, int level, int secondsPlayed) {
    Player *player = malloc(sizeof(Player));
    player->blockedBy = NULL;
    player->engageable = NULL;
    player->engaged = false;
    player->storylineCount = 0;
    player->onDeckCount = 0;
    player->log = log;
    player->itemCount = 0;
    player->partyCount = 0;
    player->coins = coins;
    player->experience = experience;
    player->level = level;
    player->secondsPlayed = secondsPlayed;
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        player->party[i] = mobs[i];
        if (mobs[i] == NULL && player->partyCount == 0) {
            player->partyCount = i;
        }
    }
    ConsumeAffect *affect = malloc(sizeof(ConsumeAffect));
    affect->hp = 20;
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

AttributesData *createAttributesData(Attributes *a) {
    AttributesData *data = malloc(sizeof(AttributesData));
    data->strength = a->strength;
    data->intelligence = a->intelligence;
    data->wisdom = a->wisdom;
    data->dexterity = a->dexterity;
    data->constitution = a->constitution;
    data->hp = a->hp;
    data->mana = a->mana;
    return data;
}

MobileData createMobDataFromMob(Mobile *mob) {
    return (MobileData) {
            mob->id,
            mob->name,
            mob->animations[0]->name,
            getPositionAsString(mob->position),
            getAnimationStringFromType(mob->direction),
            createAttributesData(mob->attributes),
    };
}

PlayerData *createPlayerData(Player *p) {
    PlayerData *pd = malloc(sizeof(PlayerData));
    pd->coins = p->coins;
    pd->secondsPlayed = p->secondsPlayed;
    pd->experience = p->experience;
    pd->level = p->level;
    pd->storylines_count = p->storylineCount;
    addInfo(p->log, "storylines count :: %d", p->storylineCount);
    pd->storylines = malloc(sizeof(char **));
    for (int i = 0; i < p->storylineCount; i++) {
        addInfo(p->log, "storyline :: %s", p->storylines[i]);
        pd->storylines[i] = &p->storylines[i][0];
    }
    pd->items_count = 0;
    pd->items = (SaveItemData *) malloc(sizeof(p->items));
    for (int i = 0; i < p->itemCount; i++) {
        pd->items[i] = (SaveItemData) {
                p->items[i]->name,
                p->itemQuantities[i]
        };
    }
    pd->items_count = p->itemCount;

    pd->party = malloc(sizeof(MobileData));
    for (int i = 0; i < p->partyCount; i++) {
        pd->party[i] = createMobDataFromMob(p->party[i]);
    }
    pd->party_count = p->partyCount;

    pd->onDeck = malloc(sizeof(MobileData));
    for (int i = 0; i < p->onDeckCount; i++) {
        pd->onDeck[i] = createMobDataFromMob(p->onDeck[i]);
    }
    pd->onDeck_count = p->onDeckCount;
    return pd;
}

int getExperienceToLevel(int level) {
    return (int) pow((double) level, 3.0) + 999;
}

void addStory(Player *p, const char *story) {
    p->storylines[p->storylineCount++] = story;
    addInfo(p->log, "add story to player :: %s", story);
}

bool hasStory(Player *p, const char *story) {
    for (int j = 0; j < p->storylineCount; j++) {
        if (strcmp(story, p->storylines[j]) == 0) {
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
