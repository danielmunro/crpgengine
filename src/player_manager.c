#include "headers/util/util.h"
#include "headers/attributes.h"
#include "headers/spell_manager.h"
#include "headers/player.h"
#include "headers/mobile_manager.h"

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
                        am->context->game->indexDir,
                        mob.avatar,
                        am->context->ui->screen->scale),
                mob.hp,
                mob.mana,
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
            save->player->experience,
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
    StartPartyData *data = loadStartPartyData(mm->context->game->indexDir);
    Mobile *mobiles[MAX_PARTY_SIZE];
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        mobiles[i] = NULL;
    }
    for (int i = 0; i < data->mobiles_count; i++) {
        for (int j = 0; j < mm->playableMobileCount; j++) {
            if (strcmp(mm->playableMobiles[j]->id, data->mobiles[i]) == 0) {
                mobiles[i] = mm->playableMobiles[j];
                break;
            }
        }
    }
    const char **storylines = malloc(sizeof(char **));
    Item **items = calloc(MAX_ITEMS, sizeof(Item));
    OpenedChest **openedChests = calloc(MAX_CHESTS, sizeof(OpenedChest));
    Player *p = createPlayer(
            mobiles,
            0,
            getExperienceToLevel(1),
            1,
            0,
            storylines,
            0,
            items,
            0,
            openedChests,
            0);
    loadAllPlayerItems(im, p);
    return p;
}

