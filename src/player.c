typedef struct {
    Mobile *party[MAX_PARTY_SIZE];
    Mobile *onDeck[MAX_TEAM_SIZE];
    const char **storylines;
    PlayerItemData **items;
    SaveFiles *saveFiles;
    Mobile *blockedBy;
    Mobile *engageable;
    Log *log;
    bool engaged;
    int coins;
    int secondsPlayed;
    int experience;
    int level;
    int actionGauge;
    int partyCount;
    int itemCount;
    int onDeckCount;
    int storylineCount;
} Player;

void addItem(Player *player, ItemData *item) {
    for (int i = 0; i < player->itemCount; i++) {
        if (strcmp(player->items[i]->name, item->name) == 0) {
            player->items[i]->quantity++;
            return;
        }
    }
    PlayerItemData *it = malloc(sizeof(PlayerItemData));
    it->name = item->name;
    it->quantity = 1;
    player->items[player->itemCount] = it;
    player->itemCount++;
}

Player *createPlayer(Log *log, Mobile *mobs[MAX_PARTY_SIZE],
                     int coins, int experience, int level, int secondsPlayed,
                     const char **storylines, int storylineCount,
                     PlayerItemData *items, int itemCount) {
    Player *player = malloc(sizeof(Player));
    player->blockedBy = NULL;
    player->engageable = NULL;
    player->engaged = false;
    player->storylineCount = storylineCount;
    player->onDeckCount = 0;
    player->log = log;
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
    player->itemCount = itemCount;
    player->items = calloc(MAX_ITEMS, sizeof(PlayerItemData));
    for (int i = 0; i < itemCount; i++) {
        player->items[i] = &items[i];
    }
    return player;
}

Mobile *getPartyLeader(Player *p) {
    return p->party[0];
}

MobileData createMobDataFromMob(Mobile *mob) {
    return (MobileData) {
            mob->id,
            mob->name,
            mob->animations[0]->name,
            getPositionAsString(mob->position),
            getAnimationStringFromType(mob->direction),
            createDataFromAttributes(mob->attributes),
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
    pd->items = (PlayerItemData *) malloc(p->itemCount * sizeof(PlayerItemData));
    pd->party = (MobileData *) malloc(p->partyCount * sizeof(MobileData));
    pd->onDeck = (MobileData *) malloc(p->onDeckCount * sizeof(MobileData));
    pd->storylines = calloc(p->storylineCount, sizeof(char *));
    for (int i = 0; i < p->storylineCount; i++) {
        pd->storylines[i] = &p->storylines[i][0];
    }
    for (int i = 0; i < p->itemCount; i++) {
        pd->items[i] = (PlayerItemData) {
                p->items[i]->name,
                p->items[i]->quantity,
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

SaveFiles *getSaveFiles(const char *indexDir) {
    const char *savesDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *)savesDirectory, "%s/_saves", indexDir);
    char **files = calloc(MAX_SAVE_FILES, sizeof(char *));
    const char **names = calloc(MAX_SAVE_FILES, sizeof (char *));
    unsigned long created[MAX_SAVE_FILES];
    int count = getFilesInDirectory(savesDirectory, files);
    for (int i = 0; i < count; i++) {
        char *filePath = malloc(MAX_FS_PATH_LENGTH);
        sprintf(filePath, "%s/%s", savesDirectory, files[i]);
        SaveData *s = loadSaveData(filePath);
        if (strcmp(files[i], "autosave.yaml") == 0) {
            char *name = malloc(MAX_SAVE_NAME);
            sprintf(name, "(autosave) %s", s->name);
            names[i] = name;
            created[i] = s->time + 1;
            free(name);
        } else {
            names[i] = s->name;
            created[i] = s->time;
        }
        free(filePath);
        free(s);
    }
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (created[i] > created[j]) {
                char *s = &files[i][0];
                files[i] = files[j];
                files[j] = &s[0];
                unsigned long c = created[i];
                created[i] = created[j];
                created[j] = c;
                const char *n = &names[i][0];
                names[i] = names[j];
                names[j] = n;
            }
        }
    }
    SaveFiles *sf = malloc(sizeof(SaveFiles));
    sf->count = count;
    sf->filenames = (const char **)files;
    sf->saveNames = names;
    free(names);
    free(files);
    free((char *)savesDirectory);
    return sf;
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

    printf("item: %s\n", player->items[0]->name);
    printf("quantity: %d\n", player->items[0]->quantity);
    // auto save
    saveFile(player->log, save, indexDir, "autosave.yaml");
    char filename[MAX_FS_PATH_LENGTH];

    // point-in-time save
    sprintf(filename, "save-%lu.yaml", (unsigned long)time(NULL));
    saveFile(player->log, save, indexDir, filename);

    free(date);
    free(name);
    free(save);
    free(player->saveFiles);
    player->saveFiles = getSaveFiles(indexDir);
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


Player *mapSaveDataToPlayer(AnimationManager *am, Log *log, SaveData *save) {
    Mobile *mobs[MAX_PARTY_SIZE];
    addInfo(log, "save file party count :: %d", save->player->party_count);
    for (int i = 0; i < save->player->party_count; i++) {
        Animation *animations[MAX_ANIMATIONS];
        loadAnimationsByName(am, save->player->party[i].animations, animations);
        mobs[i] = createMobile(
                save->player->party[i].id,
                save->player->party[i].name,
                getPositionFromString(save->player->party[i].position),
                getDirectionFromString(save->player->party[i].direction),
                animations);
        mobs[i]->attributes = createAttributesFromData(save->player->party[i].attributes);
    }
    for (int i = save->player->party_count; i < MAX_PARTY_SIZE; i++) {
        mobs[i] = NULL;
    }
    return createPlayer(
            log,
            mobs,
            save->player->coins,
            save->player->experience,
            save->player->level,
            save->player->secondsPlayed,
            save->player->storylines,
            save->player->storylines_count,
            save->player->items,
            save->player->items_count);
}

Player *createNewPlayer(Log *log, AnimationManager *am, const char *indexDir) {
    addInfo(log, "loading player from dir %s", indexDir);
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    Mobile *mobiles[MAX_PARTY_SIZE] = {
            createMobile(
                    "player",
                    "Fireas",
                    (Vector2) {429, 252},
                    DOWN,
                    animations),
            NULL,
            NULL,
            NULL,
    };
    const char **storylines = malloc(sizeof(char **));
    PlayerItemData *items = calloc(MAX_ITEMS, sizeof(PlayerItemData));
    Player *p = createPlayer(
            log,
            mobiles,
            0,
            getExperienceToLevel(1),
            1,
            0,
            storylines,
            0,
            items,
            0);
    return p;
}
