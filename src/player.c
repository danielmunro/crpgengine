typedef struct {
    Mobile *party[MAX_PARTY_SIZE];
    int partyCount;
    Mobile *onDeck[MAX_TEAM_SIZE];
    int onDeckCount;
    const char **storylines;
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
                     int coins, int experience, int level, int secondsPlayed,
                     const char **storylines, int storylineCount) {
    Player *player = malloc(sizeof(Player));
    player->blockedBy = NULL;
    player->engageable = NULL;
    player->engaged = false;
    player->storylineCount = storylineCount;
    player->onDeckCount = 0;
    player->log = log;
    player->itemCount = 0;
    player->partyCount = 0;
    player->coins = coins;
    player->experience = experience;
    player->level = level;
    player->secondsPlayed = secondsPlayed;
    player->storylines = malloc(sizeof(char **));
    player->storylines = storylines;
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
    addDebug(p->log, "create player data");
    PlayerData *pd = malloc(sizeof(PlayerData));
    pd->coins = p->coins;
    pd->secondsPlayed = p->secondsPlayed;
    pd->experience = p->experience;
    pd->level = p->level;
    pd->storylines_count = p->storylineCount;
    pd->party_count = p->partyCount;
    pd->items_count = p->itemCount;
    pd->onDeck_count = p->onDeckCount;
    pd->items = (SaveItemData *) malloc(p->itemCount * sizeof(SaveItemData));
    pd->party = (MobileData *) malloc(p->partyCount * sizeof(MobileData));
    pd->onDeck = (MobileData *) malloc(p->onDeckCount * sizeof(MobileData));
    pd->storylines = calloc(p->storylineCount, sizeof(char *));
    for (int i = 0; i < p->storylineCount; i++) {
        pd->storylines[i] = &p->storylines[i][0];
    }
    for (int i = 0; i < p->itemCount; i++) {
        pd->items[i] = (SaveItemData) {
                p->items[i]->name,
                p->itemQuantities[i]
        };
    }
    for (int i = 0; i < p->partyCount; i++) {
        pd->party[i] = createMobDataFromMob(p->party[i]);
    }
    for (int i = 0; i < p->onDeckCount; i++) {
        pd->onDeck[i] = createMobDataFromMob(p->onDeck[i]);
    }
    return pd;
}

int getExperienceToLevel(int level) {
    return (int) pow((double) level, 3.0) + 999;
}

void checkMoveKey(Player *p, int key, AnimationType direction) {
    Mobile *mob = getPartyLeader(p);
    if (IsKeyDown(key) && !p->engaged) {
        mob->moving[direction] = true;
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

SaveData *createSaveData(Player *player, const char *scene, const char *saveName) {
    addDebug(player->log, "create save data");
    SaveData *save = malloc(sizeof(SaveData));
    save->name = saveName;
    save->player = createPlayerData(player);
    save->scene = &scene[0];
    save->time = (unsigned long)time(NULL);
    return save;
}

void saveFile(Log *log, SaveData *save, const char *indexDir, const char *filename) {
    char filePathAuto[MAX_FS_PATH_LENGTH];
    sprintf(filePathAuto, "%s/%s/%s", indexDir, "_saves", filename);
    addInfo(log, "player save file :: %s", filePathAuto);
    saveSaveData(save, filePathAuto);
}

void save(Player *player, const char *sceneName, const char *indexDir) {
    addInfo(player->log, "save player progress");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *date = malloc(MAX_DATETIME_LENGTH);
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    char *name = malloc(MAX_SAVE_NAME);
    sprintf(name, "%s - %s - %s", date, getPartyLeader(player)->name, sceneName);
    SaveData *save = createSaveData(
            player,
            sceneName,
            name);

    // auto save
    saveFile(player->log, save, indexDir, "autosave.yaml");
    char filename[MAX_FS_PATH_LENGTH];

    // point-in-time save
    sprintf(filename, "save-%lu.yaml", (unsigned long)time(NULL));
    saveFile(player->log, save, indexDir, filename);

    free(save);
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
