#include <math.h>

#include "headers/util/log.h"
#include "headers/direction.h"
#include "headers/persistence/cyaml.h"
#include "headers/mobile.h"
#include "headers/spell.h"
#include "headers/graphics/ui/ui.h"
#include "headers/shop.h"

typedef struct {
    int chestId;
    int sceneId;
} OpenedChest;

typedef struct {
    Mobile **party;
    Mobile **onDeck;
    const char **storylines;
    Item **items;
    Mobile *engageable;
    Dialog *dialog;
    OpenedChest **openedChests;
    int openedChestsCount;
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

OpenedChest *createOpenedChest(int sceneId, int chestId) {
    OpenedChest *o = malloc(sizeof(OpenedChest));
    o->sceneId = sceneId;
    o->chestId = chestId;
    return o;
}

Player *createPlayer(
        Mobile *mobs[4],
        int coins,
        int level,
        int secondsPlayed,
        const char **storylines,
        int storylineCount,
        Item **items,
        int itemCount,
        OpenedChest **openedChests,
        int openedChestsCount) {
    Player *player = malloc(sizeof(Player));
    player->engageable = NULL;
    player->engaged = false;
    player->storylineCount = storylineCount;
    player->onDeckCount = 0;
    player->partyCount = 0;
    player->coins = coins;
    player->level = level;
    player->secondsPlayed = secondsPlayed;
    player->storylines = storylines;
    player->party = calloc(MAX_PARTY_SIZE, sizeof(Mobile));
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (mobs[i] == NULL) {
            break;
        }
        player->party[i] = mobs[i];
        player->partyCount += 1;
    }
    player->itemCount = itemCount;
    player->items = items;
    player->onDeck = calloc(MAX_PARTY_SIZE, sizeof(Mobile));
    player->dialog = NULL;
    player->openedChests = openedChests;
    player->openedChestsCount = openedChestsCount;
    player->coins = 1000;
    return player;
}

void addItem(Player *player, const Item *item) {
    for (int i = 0; i < player->itemCount; i++) {
        if (strcmp(player->items[i]->name, item->name) == 0) {
            player->items[i]->quantity += 1;
            return;
        }
    }
    player->items[player->itemCount] = cloneItem(item);
    player->items[player->itemCount]->quantity = 1;
    player->itemCount++;
}

void removeItem(Player *player, const Item *item) {
    addInfo("remove item :: %s\n", item->name);
    bool foundItem = false;
    for (int i = 0; i < player->itemCount; i++) {
        if (player->items[i] == item) {
            if (player->items[i]->quantity > 1) {
                player->items[i]->quantity -= 1;
                return;
            }
            foundItem = true;
            free(player->items[i]);
        }
        if (foundItem) {
            player->items[i] = player->items[i + 1];
        }
    }
    player->itemCount--;
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
            mob->experience,
            createDataFromAttributes(mob->attributes),
            mapSpellsToData(mob->spells, mob->spellCount),
            mob->spellCount,
    };
}

OpenedChestData createOpenedChestDataFromOpenedChest(const OpenedChest *o) {
    return (OpenedChestData) {
        o->sceneId,
        o->chestId,
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
    pd->openedChests = (OpenedChestData *) malloc(p->openedChestsCount * sizeof(OpenedChestData));
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
    for (int i = 0; i < p->openedChestsCount; i++) {
        pd->openedChests[i] = createOpenedChestDataFromOpenedChest(p->openedChests[i]);
    }
    pd->openedChests_count = p->openedChestsCount;
    return pd;
}

int getExperienceToLevel(int level) {
    return (int) pow((double) level, 3.0) + 999;
}

Direction checkMoveKey(const Player *p, int key, Direction direction) {
    const Mobile *mob = getPartyLeader(p);
    if (IsKeyDown(key) && !p->engaged && mob->amountToMove == 0) {
        return direction;
    }
    return -1;
}

void clearDialog(Player *p) {
    free(p->dialog);
    p->dialog = NULL;
}

Direction mapCheckMoveKeys(const Player *player) {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        if (checkMoveKey(player, MOVE_KEYS[i], DirectionEnums[i]) != -1) {
            return DirectionEnums[i];
        }
    }
    return -1;
}

bool isSpeakingTo(const Player *p, const Mobile *target) {
    return p->engaged && target == p->engageable;
}

void engageWithMobile(Player *p, Mobile *mob) {
    p->engageable = mob;
    addInfo("updating mob direction to: %d", getOppositeDirection(getPartyLeader(p)->direction));
    updateDirection(mob, getOppositeDirection(getPartyLeader(p)->direction));
    addInfo("engaging with %s", p->engageable->name);
    p->engaged = true;
}

void disengageWithMobile(Player *p) {
    p->engaged = false;
    clearDialog(p);
    if (p->engageable != NULL) {
        updateDirection(p->engageable, p->engageable->previousDirection);
    }
}

void addStory(Player *p, const char *story) {
    p->storylines[p->storylineCount++] = story;
    addInfo("add story to player :: %s", story);
}

bool hasStory(const Player *p, const char *story) {
    for (int j = 0; j < p->storylineCount; j++) {
        if (strcmp(story, p->storylines[j]) == 0) {
            return true;
        }
    }
    return false;
}

bool isChestOpened(const Player *p, int sceneId, int chestId) {
    for (int i = 0; i < p->openedChestsCount; i++) {
        if (p->openedChests[i]->sceneId == sceneId
                && p->openedChests[i]->chestId == chestId) {
            return true;
        }
    }
    return false;
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
