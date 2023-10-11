#include "headers/util/util.h"
#include "headers/attributes.h"
#include "headers/spell_manager.h"
#include "headers/player.h"
#include "headers/graphics/ui/ui.h"
#include "headers/mobile_manager.h"

Player *mapSaveDataToPlayer(SpellManager *sm, AnimationManager *am, SaveData *save) {
    Mobile *mobs[MAX_PARTY_SIZE];
    addInfo("save file party count :: %d", save->player->party_count);
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
                createAvatar(mob.avatar),
                mob.hp,
                mob.mana,
                createAttributesFromData(mob.attributes),
                mapDataToSpells(sm, mob.spells, mob.spells_count),
                mob.spells_count);
    }
    for (int i = save->player->party_count; i < MAX_PARTY_SIZE; i++) {
        mobs[i] = NULL;
    }
    Item **items = calloc(MAX_ITEMS, sizeof(Item));
    for (int i = 0; i < save->player->items_count; i++) {
        items[i] = createItemFromData(&save->player->items[i]);
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
            save->player->items_count);
}

Player *createNewPlayer(MobileManager *mm, ItemManager *im) {
    addInfo("creating new player");
    StartPartyData *data = loadStartPartyData();
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
    Player *p = createPlayer(
            mobiles,
            0,
            getExperienceToLevel(1),
            1,
            0,
            storylines,
            0,
            items,
            0);
    loadAllPlayerItems(im, p);
    return p;
}

