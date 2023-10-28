#include <math.h>
#include <time.h>

#include "headers/util/util.h"
#include "headers/util/log.h"
#include "headers/direction.h"
#include "headers/persistence/cyaml.h"
#include "headers/graphics/animation.h"
#include "headers/mobile.h"
#include "headers/spell.h"
#include "headers/persistence/db.h"
#include "headers/graphics/ui/ui.h"
#include "headers/tile.h"

typedef struct {
    Mobile *mob;
    const Chest *chest;
    const Tile *tile;
} Blocking;

typedef struct {
    Mobile **party;
    Mobile **onDeck;
    const char **storylines;
    Item **items;
    SaveFiles *saveFiles;
    Blocking *blocking;
    Mobile *engageable;
    Dialog *dialog;
    const char **openedChests;
    bool engaged;
    int coins;
    int secondsPlayed;
    int experience;
    int level;
    int partyCount;
    int itemCount;
    int onDeckCount;
    int storylineCount;
    int openedChestCount;
} Player;

Player *createPlayer(Mobile *mobs[MAX_PARTY_SIZE],
                     int coins, int experience, int level, int secondsPlayed,
                     const char **storylines, int storylineCount,
                     Item **items, int itemCount) {
    Player *player = malloc(sizeof(Player));
    player->blocking = malloc(sizeof(Blocking));
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
    player->openedChests = calloc(MAX_CHESTS, sizeof(const char *));
    return player;
}

void addItem(Player *player, Item *item) {
    player->items[player->itemCount] = item;
    player->itemCount++;
}

void removeItem(Player *player, const Item *item) {
    addInfo("remove item :: %s\n", item->name);
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

void setBlockedByTile(Player *p, const Tile *t) {
    p->blocking->mob = NULL;
    p->blocking->chest = NULL;
    p->blocking->tile = t;
}

void setBlockedByMob(Player *p, Mobile *mob) {
    p->blocking->mob = mob;
    p->blocking->chest = NULL;
    p->blocking->tile = NULL;
}

void setBlockedByChest(Player *p, const Chest *chest) {
    p->blocking->mob = NULL;
    p->blocking->chest = chest;
    p->blocking->tile = NULL;
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
            Directions[mob->direction],
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

void checkMoveKey(const Player *p, int key, Direction direction) {
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

void mapCheckMoveKeys(const Player *player) {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        checkMoveKey(player, MOVE_KEYS[i], DirectionEnums[i]);
    }
}

bool isSpeakingTo(const Player *p, const Mobile *target) {
    return p->engaged && target == p->engageable;
}

void engageWithMobile(Player *p) {
    p->engageable = p->blocking->mob;
    addInfo("updating mob direction to: %d", getOppositeDirection(getPartyLeader(p)->direction));
    updateDirection(p->blocking->mob, getOppositeDirection(getPartyLeader(p)->direction));
    addInfo("engaging with %s", p->engageable->name);
    p->engaged = true;
}

void disengageWithMobile(Player *p) {
    p->engaged = false;
    clearDialog(p);
    if (p->blocking->mob != NULL) {
        updateDirection(p->blocking->mob, p->blocking->mob->previousDirection);
    }
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

void saveFile(const SaveData *save, const char *indexDir, const char *filename) {
    char filePathAuto[MAX_FS_PATH_LENGTH];
    sprintf(filePathAuto, "%s/%s/%s", indexDir, "_saves", filename);
    addInfo("player save file :: %s", filePathAuto);
    saveSaveData(save, filePathAuto);
}

SaveFiles *getSaveFiles() {
    const char *savesDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) savesDirectory, "%s/_saves", config->indexDir);
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
    struct tm result;
    struct tm tm = *localtime_r(&t, &result);
    char *date = malloc(MAX_DATETIME_LENGTH);
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec);
    char *name = malloc(MAX_SAVE_NAME);
    sprintf(name, "%s - %s - %s", date, getPartyLeader(player)->name, sceneName);
    SaveData *save = createSaveData(
            player,
            sceneName,
            name);

    // auto save
    saveFile(save, config->indexDir, "autosave.yaml");
    char filename[MAX_FS_PATH_LENGTH];

    // point-in-time save
    sprintf(filename, "save-%lu.yaml", (unsigned long) time(NULL));
    saveFile(save, config->indexDir, filename);

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

bool hasStory(const Player *p, const char *story) {
    for (int j = 0; j < p->storylineCount; j++) {
        if (strcmp(story, p->storylines[j]) == 0) {
            addDebug("player has story: %s", story);
            return true;
        }
    }
    addDebug("player does not have story: %s", story);
    return false;
}

bool isChestOpened(const Player *p, const char *key) {
    for (int i = 0; i < p->openedChestCount; i++) {
        if (strcmp(p->openedChests[i], key) == 0) {
            return true;
        }
    }
    return false;
}

void loadAllPlayerItems(ItemManager *im, Player *p) {
    ItemsReferenceData *itemsData = loadItemsReferenceData();
    for (int i = 0; i < itemsData->items_count; i++) {
        Item **items = createItemsFromReferenceData(im, &itemsData->items[i]);
        for (int j = 0; j < itemsData->items[i].quantity; j++) {
            addItem(p, items[j]);
        }
        free(items);
    }
}

ItemListResult createItemList(const Player *p) {
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

bool losesItemQuantity(Player *player, const ItemReferenceData *ird) {
    addInfo("player losing item :: %s", ird->name);
    int found = 0;
    int itemIndexes[ird->quantity];
    for (int i = 0; i < player->itemCount; i++) {
        if (strcmp(ird->name, player->items[i]->name) == 0) {
            itemIndexes[found] = i;
            found++;
            if (found == ird->quantity) {
                break;
            }
        }
    }
    if (found == ird->quantity) {
        for (int i = 0; i < ird->quantity; i++) {
            removeItem(player, player->items[itemIndexes[i]]);
        }
        return true;
    }
    addWarning("player didn't have enough items to give :: %d remaining", ird->quantity);
    return false;
}
