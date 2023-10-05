#include <math.h>
#include <time.h>
#include "headers/util.h"
#include "headers/log.h"
#include "headers/direction.h"
#include "headers/cyaml.h"
#include "headers/item.h"
#include "headers/animation.h"
#include "headers/mobile.h"
#include "headers/spell.h"

typedef struct {
    Mobile **party;
    Mobile **onDeck;
    const char **storylines;
    Item **items;
    SaveFiles *saveFiles;
    Mobile *blockedBy;
    Mobile *engageable;
    Dialog *dialog;
    bool engaged;
    int coins;
    int secondsPlayed;
    int experience;
    int level;
    int partyCount;
    int itemCount;
    int onDeckCount;
    int storylineCount;
} Player;

void addItem(Player *player, Item *item) {
    player->items[player->itemCount] = item;
    player->itemCount++;
}

void removeItem(Player *player, Item *item) {
    bool foundItem = false;
    for (int i = 0; i < player->itemCount; i++) {
        if (player->items[i] == item) {
            foundItem = true;
        }
        if (foundItem) {
            player->items[i] = player->items[i + 1];
        }
    }
    player->itemCount--;
}

Player *createPlayer(Mobile *mobs[MAX_PARTY_SIZE],
                     int coins, int experience, int level, int secondsPlayed,
                     const char **storylines, int storylineCount,
                     Item **items, int itemCount) {
    Player *player = malloc(sizeof(Player));
    player->blockedBy = NULL;
    player->engageable = NULL;
    player->engaged = false;
    player->storylineCount = storylineCount;
    player->onDeckCount = 0;
    player->partyCount = 0;
    player->coins = coins;
    player->experience = experience;
    player->level = level;
    player->secondsPlayed = secondsPlayed;
    player->storylines = storylines;
    player->party = calloc(MAX_PARTY_SIZE, sizeof(Mobile));
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (mobs[i] == NULL) {
            break;
        }
        player->party[i] = mobs[i];
        player->partyCount = i + 1;
    }
    player->itemCount = itemCount;
    player->items = items;
    player->onDeck = calloc(MAX_PARTY_SIZE, sizeof(Mobile));
    player->dialog = NULL;
    return player;
}

Mobile *getPartyLeader(const Player *p) {
    return p->party[0];
}

MobileData createMobDataFromMob(Mobile *mob) {
    return (MobileData) {
            mob->id,
            mob->name,
            mob->animations[0]->name,
            mob->avatar->filename,
            getPositionAsString(mob->position),
            getAnimationStringFromType(mob->direction),
            mob->hp,
            mob->mana,
            createDataFromAttributes(mob->attributes),
            mapSpellsToData(mob->spells, mob->spellCount),
            mob->spellCount,
    };
}

PlayerData *createPlayerData(const Player *p) {
    addDebug("create player data");
    PlayerData *pd = malloc(sizeof(PlayerData));
    pd->coins = p->coins;
    pd->secondsPlayed = p->secondsPlayed;
    pd->experience = p->experience;
    pd->level = p->level;
    pd->storylines_count = p->storylineCount;
    pd->party_count = p->partyCount;
    pd->items_count = p->itemCount;
    pd->onDeck_count = p->onDeckCount;
    pd->items = (ItemData *) malloc(p->itemCount * sizeof(ItemData));
    pd->party = (MobileData *) malloc(p->partyCount * sizeof(MobileData));
    pd->onDeck = (MobileData *) malloc(p->onDeckCount * sizeof(MobileData));
    pd->storylines = calloc(p->storylineCount, sizeof(char *));
    for (int i = 0; i < p->storylineCount; i++) {
        pd->storylines[i] = &p->storylines[i][0];
    }
    for (int i = 0; i < p->itemCount; i++) {
        pd->items[i] = createItemData(p->items[i]);
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

void checkMoveKey(Player *p, int key, Direction direction) {
    Mobile *mob = getPartyLeader(p);
    if (IsKeyDown(key) && !p->engaged) {
        mob->moving[direction] = true;
        mob->direction = direction;
        getMobAnimation(mob)->isPlaying = true;
    }
}

void clearDialog(Player *p) {
    free(p->dialog);
    p->dialog = NULL;
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
    addInfo("updating mob direction to: %d", getOppositeDirection(getPartyLeader(p)->direction));
    updateDirection(p->blockedBy, getOppositeDirection(getPartyLeader(p)->direction));
    addInfo("engaging with %s", p->engageable->name);
    p->engaged = true;
}

void disengageWithMobile(Player *p) {
    p->engaged = false;
    clearDialog(p);
    updateDirection(p->blockedBy, p->blockedBy->previousDirection);
}

SaveData *createSaveData(const Player *player, const char *scene, const char *saveName) {
    addDebug("create save data");
    SaveData *save = malloc(sizeof(SaveData));
    save->name = saveName;
    save->player = createPlayerData(player);
    save->scene = &scene[0];
    save->time = (unsigned long) time(NULL);
    return save;
}

void saveFile(SaveData *save, const char *indexDir, const char *filename) {
    char filePathAuto[MAX_FS_PATH_LENGTH];
    sprintf(filePathAuto, "%s/%s/%s", indexDir, "_saves", filename);
    addInfo("player save file :: %s", filePathAuto);
    saveSaveData(save, filePathAuto);
}

SaveFiles *getSaveFiles() {
    const char *savesDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) savesDirectory, "%s/_saves", runtimeArgs->indexDir);
    char **files = calloc(MAX_SAVE_FILES, sizeof(char *));
    const char **names = calloc(MAX_SAVE_FILES, sizeof(char *));
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
    sf->filenames = (const char **) files;
    sf->saveNames = names;
    free(names);
    free(files);
    free((char *) savesDirectory);
    return sf;
}

void save(Player *player, const char *sceneName) {
    addInfo("save player progress");
    time_t t = time(NULL);
    struct tm tm = *localtime_r(&t, NULL);
    char *date = malloc(MAX_DATETIME_LENGTH);
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec);
    char *name = malloc(MAX_SAVE_NAME);
    sprintf(name, "%s - %s - %s", date, getPartyLeader(player)->name, sceneName);
    SaveData *save = createSaveData(
            player,
            sceneName,
            name);

    printf("item: %s\n", player->items[0]->name);
    // auto save
    saveFile(save, runtimeArgs->indexDir, "autosave.yaml");
    char filename[MAX_FS_PATH_LENGTH];

    // point-in-time save
    sprintf(filename, "save-%lu.yaml", (unsigned long) time(NULL));
    saveFile(save, runtimeArgs->indexDir, filename);

    free(date);
    free(name);
    free(save);
    free(player->saveFiles);
    player->saveFiles = getSaveFiles();
}

void addStory(Player *p, const char *story) {
    p->storylines[p->storylineCount++] = story;
    addInfo("add story to player :: %s", story);
}

bool hasStory(Player *p, const char *story) {
    for (int j = 0; j < p->storylineCount; j++) {
        if (strcmp(story, p->storylines[j]) == 0) {
            addDebug("player has story: %s", story);
            return true;
        }
    }
    addDebug("player does not have story: %s", story);
    return false;
}

void loadAllPlayerItems(ItemManager *im, Player *p) {
    const char *itemsFile = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) itemsFile, "%s/player/items.yaml", runtimeArgs->indexDir);
    ItemsReferenceData *itemsData = loadItemsReferenceData(itemsFile);
    int itemCount = 0;
    for (int i = 0; i < itemsData->items_count; i++) {
        Item **items = createItemsFromReferenceData(im, &itemsData->items[i]);
        for (int j = 0; j < itemsData->items[i].quantity; j++) {
            addItem(p, items[j]);
            itemCount++;
        }
    }
    p->itemCount = itemCount;
    free((char *) itemsFile);
}

ItemListResult createItemList(Player *p) {
    ItemList *itemList = calloc(p->itemCount, sizeof(ItemList));
    int count = 0;
    for (int i = 0; i < p->itemCount; i++) {
        const char *name = p->items[i]->name;
        bool found = false;
        for (int j = 0; j < count; j++) {
            if (strcmp(name, itemList[j].item->name) == 0) {
                itemList[j].amount += 1;
                found = true;
                break;
            }
        }
        if (!found) {
            itemList[count] = (ItemList) {
                    p->items[i],
                    1,
            };
            count++;
        }
    }

    return (ItemListResult) {
            itemList,
            count,
    };
}
