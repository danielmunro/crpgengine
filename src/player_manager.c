#include "headers/util/util.h"
#include "headers/attributes.h"
#include "headers/spell_manager.h"
#include "headers/player.h"
#include "headers/mobile_manager.h"
#include "headers/item.h"

Player *mapSaveDataToPlayer(SpellManager *sm, AnimationManager *am, SaveData *save, bool immortal) {
    Mobile *mobs[MAX_PARTY_SIZE];
    addDebug("save file party count :: %d", save->player->party_count);
    for (int i = 0; i < save->player->party_count; i++) {
        Animation *animations[MAX_ANIMATIONS];
        MobileData mob = save->player->party[i];
        loadAnimationsByName(am, mob.animations, animations);
        mobs[i] = createMobile(
                mob.id,
                mob.name,
                getPositionFromString(mob.position),
                getDirectionFromString(mob.direction),
                animations,
                createAvatar(
                        am->context->indexDir,
                        mob.avatar,
                        am->context->ui->screen->scale),
                mob.hp,
                mob.mana,
                mob.experience,
                createAttributesFromData(mob.attributes),
                mapDataToSpells(sm, mob.spells, mob.spells_count),
                mob.spells_count,
                immortal);
    }
    for (int i = save->player->party_count; i < MAX_PARTY_SIZE; i++) {
        mobs[i] = NULL;
    }
    Item **items = calloc(MAX_ITEMS, sizeof(Item));
    for (int i = 0; i < save->player->items_count; i++) {
        items[i] = createItemFromData(&save->player->items[i]);
    }
    OpenedChest **openedChests = calloc(MAX_CHESTS, sizeof(OpenedChest));
    for (int i = 0; i < save->player->openedChests_count; i++) {
        openedChests[i] = createOpenedChest(
                save->player->openedChests[i].sceneId,
                save->player->openedChests[i].chestId);
    }
    return createPlayer(
            mobs,
            save->player->coins,
            save->player->level,
            save->player->secondsPlayed,
            save->player->storylines,
            save->player->storylines_count,
            items,
            save->player->items_count,
            openedChests,
            save->player->openedChests_count);
}

Player *createNewPlayer(MobileManager *mm, ItemManager *im) {
    addInfo("creating new player");
    NewPlayerData *data = loadNewPlayerData(mm->context->indexDir);
    Mobile *mobiles[MAX_PARTY_SIZE];
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        mobiles[i] = NULL;
    }
    for (int i = 0; i < data->partyCount; i++) {
        for (int j = 0; j < mm->playableMobileCount; j++) {
            if (strcmp(mm->playableMobiles[j]->id, data->party[i]) == 0) {
                mobiles[i] = mm->playableMobiles[j];
                break;
            }
        }
    }
    const char **storylines = malloc(sizeof(char **));
    Item **items = calloc(MAX_ITEMS, sizeof(Item));
    int itemCount = 0;
    for (int i = 0; i < data->itemsCount; i++) {
        for (int j = 0; j < im->count; j++) {
            if (strcmp(data->items[i].name, im->items[j]->name) == 0) {
                items[itemCount] = cloneItem(im->items[j]);
                items[itemCount]->quantity = data->items[i].quantity;
                itemCount++;
            }
        }
    }
    OpenedChest **openedChests = calloc(MAX_CHESTS, sizeof(OpenedChest));
    Player *p = createPlayer(
            mobiles,
            data->coins,
            1,
            0,
            storylines,
            0,
            items,
            itemCount,
            openedChests,
            0);
    return p;
}

