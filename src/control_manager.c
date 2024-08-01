#include "headers/util/log.h"
#include "headers/map.h"
#include "headers/notification.h"
#include "headers/control.h"
#include "headers/item.h"
#include "headers/player.h"
#include "headers/scene.h"
#include "headers/mobile_manager.h"
#include "headers/save.h"

typedef struct {
    Context *context;
    Scene *scene;
    Player *player;
    AnimationManager *animations;
    ItemManager *itemManager;
    NotificationManager *notificationManager;
    MobileManager *mobileManager;
    SaveFiles *saveFiles;
} ControlManager;

ControlManager *createControlManager(
        Context *c,
        Player *player,
        AnimationManager *animations,
        ItemManager *itemManager,
        NotificationManager *notificationManager,
        MobileManager *mobileManager,
        SaveFiles *saveFiles) {
    ControlManager *cm = malloc(sizeof(ControlManager));
    cm->context = c;
    cm->player = player;
    cm->animations = animations;
    cm->itemManager = itemManager;
    cm->notificationManager = notificationManager;
    cm->mobileManager = mobileManager;
    cm->saveFiles = saveFiles;
    cm->scene = NULL;
    return cm;
}

int thenCheck(ControlManager *cm, ControlBlock *cb) {
    Then *then = cb->then[cb->progress];
    int progress = 0;
    Mobile *mob = getPartyLeader(cm->player);
    if (then == NULL) {
        return 0;
    } else if (isSpeakOutcome(then) && !cm->player->engaged) {
        addDebug("dialog with mob :: %s", then->target->name);
        progress++;
    } else if (isMovingAndAtDestination(cb)) {
        addDebug("mob at destination, control block proceeding :: %s", then->target->name);
        progress++;
    } else if (isAddStoryOutcome(then)) {
        addStory(cm->player, then->story);
        progress++;
    } else if (needsToStartMoving(then)) {
        addInfo("mob to move :: %s :: %d, %d to %d %d",
                then->target->name,
                then->target->position.x,
                then->target->position.y,
                then->position.x,
                then->position.y);
        then->target->destination = then->position;
        if (then->target == mob) {
            disengageWithMobile(cm->player);
            mob->isBeingMoved = true;
        }
    } else if (isFaceDirectionOutcome(then)) {
        addDebug("set direction for mob :: %s, %s", then->target->name, then->direction);
        then->target->direction =
                getDirectionFromString(then->direction);
        progress++;
    } else if (needsToChangePosition(then)) {
        Mobile *target = then->target;
        target->position = then->position;
        addDebug("change position for mob :: %s, %f, %f",
                target->name, target->position.x, target->position.y);
        progress++;
    } else if (needsToWait(then)) {
        Mobile *target = then->target;
        if (target->waitTimer == -1) {
            addDebug("setting initial wait timer");
            target->waitTimer = then->amount;
        }
        struct timeval update;
        gettimeofday(&update, NULL);
        if (update.tv_sec > target->lastTimerUpdate.tv_sec) {
            target->lastTimerUpdate = update;
            target->waitTimer--;
            if (target->waitTimer == 0) {
                addDebug("timer done");
                target->waitTimer = -1;
                progress++;
            }
        }
    } else if (needsToLock(then)) {
        mob->locked = true;
        resetMoving(mob);
        progress++;
    } else if (needsToUnlock(then)) {
        mob->locked = false;
        resetMoving(mob);
        progress++;
    } else if (needsToSave(then)) {
        addDebug("save player game :: scene: %s", cm->scene->name);
        const SaveFile *s = save(cm->player, cm->scene->name, cm->context->indexDir);
        addSaveFile(cm->saveFiles, s);
        addNotification(
                cm->notificationManager,
                createNotification(SAVED, "Your game has been saved."));
        progress++;
    } else if (needsToReceiveItem(then, mob)) {
        char *message = malloc(MAX_NOTIFICATION_LENGTH);
        sprintf(message, "you received:\n");
        for (int i = 0; i < then->itemCount; i++) {
            if (i > 0) {
                sprintf(message, "%s,", message);
            }
            addDebug("player received item :: %s", then->items[i]);
            addItem(cm->player, findItemFromName(cm->itemManager, then->items[i]));
            sprintf(message, "%s%s", message, then->items[i]);
        }
        addNotification(
                cm->notificationManager,
                createNotification(RECEIVE_QUEST_ITEM, message));
        progress++;
    } else if (needsToLoseItem(then, mob)
               && loseItems(cm->player, then->items, then->itemCount)) {
        char *message = malloc(MAX_NOTIFICATION_LENGTH);
        sprintf(message, "you lost:\n");
        for (int i = 0; i < then->itemCount; i++) {
            if (i > 0) {
                sprintf(message, "%s,", message);
            }
            addDebug("player lost item :: %s", then->items[i]);
            sprintf(message, "%s%s", message, then->items[i]);
        }
        addNotification(
                cm->notificationManager,
                createNotification(LOSE_QUEST_ITEM, message));
        progress++;
    } else if (needsToSleep(then, mob)) {
        addInfo("player going to sleep");
        resetMobAnimations(mob);
        findAnimation(mob->animations, ANIMATION_SLEEP)->isPlaying = true;
        Animation *animation = findAnimation(cm->animations->library, ANIMATION_SLEEP_BUBBLE);
        animation->isPlaying = true;
        addAnimation(cm->animations, animation);
        progress++;
    }
    cb->progress += progress;
    return progress;
}

int controlThenCheckAllActive(ControlManager *cm) {
    int progress = 0;
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (cm->scene->activeControlBlocks[i] != NULL) {
            progress += thenCheck(cm, cm->scene->activeControlBlocks[i]);
        }
    }
    return progress;
}

void checkControls(ControlManager *cm) {
    controlWhenCheck(cm->scene, cm->player, EVENT_GAME_LOOP);
    controlThenCheckAllActive(cm);
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (cm->scene->activeControlBlocks[i] != NULL &&
            needsToRemoveActiveControlBlock(cm->scene->activeControlBlocks[i])) {
            cm->scene->activeControlBlocks[i]->progress = 0;
            cm->scene->activeControlBlocks[i] = NULL;
            if (cm->player->engaged) {
                disengageWithMobile(cm->player);
            }
        }
    }
}

void proceedControlsUntilDone(ControlManager *cm) {
    controlWhenCheck(cm->scene, cm->player, EVENT_SCENE_LOADED);
    bool isMakingProgress = true;
    while (isMakingProgress) {
        isMakingProgress = controlThenCheckAllActive(cm) > 0;
        checkControls(cm);
    }
}

When *mapWhen(ControlManager *cm, const Scene *s, WhenData wd) {
    Mobile *trigger = NULL;
    Mobile *mob = getPartyLeader(cm->player);
    ArriveAt *arriveAt = NULL;
    if (wd.mob != NULL) {
        trigger = findMobById(cm->mobileManager, wd.mob);
        addDebug("mobile trigger is '%s'", trigger->name);
    }
    if (wd.arriveAt != NULL) {
        const MapWarps *mw = s->map->warps;
        for (int i = 0; i < mw->arriveAtCount; i++) {
            if (strcmp(mw->arriveAt[i]->name, wd.arriveAt) == 0) {
                arriveAt = mw->arriveAt[i];
                break;
            }
        }
    }
    Condition c = mapCondition(wd.condition != NULL ? wd.condition : wd.player);
    addDebug("condition: %s, mapped to: %d, story: %s",
             wd.condition,
             c,
             wd.story);
    return createWhen(
            mob,
            trigger,
            c,
            wd.story,
            wd.items,
            wd.itemsCount,
            arriveAt);
}

Then *mapThen(ControlManager *cm, ThenData td) {
    Mobile *target;
    Mobile *mob = getPartyLeader(cm->player);
    if (td.player) {
        target = mob;
    } else {
        target = findMobById(cm->mobileManager, td.mob);
    }
    Vector2D pos;
    if (td.position != NULL) {
        pos = getPositionFromString(td.position);
    } else {
        pos = (Vector2D) {0, 0};
    }
    if (td.dialog != NULL) {
        td.action = OUTCOME_DIALOG;
    }
    int amount = 0;
    if (hasAmountProperty(td)) {
        amount = td.amount;
    }
    Outcome o = mapOutcome(td.action);
    addDebug("then story is '%s', outcome: %d, message: %s",
             td.story, o, td.dialog);
    return createThen(
            target,
            &td.dialog[0],
            &td.story[0],
            &td.direction[0],
            td.items,
            td.itemsCount,
            o,
            (Vector2D) {
                pos.x * tileSize(cm->context),
                pos.y * tileSize(cm->context),
            },
            td.parallel,
            amount
    );
}

ControlBlock *mapStorylineToControlBlock(ControlManager *cm, const Scene *scene, const StorylineData *storyline) {
    ControlBlock *c = createControlBlock();
    c->whenCount = storyline->when_count;
    c->thenCount = storyline->then_count;
    addDebug("processing storyline with %d when and %d then clauses",
             storyline->when_count, storyline->then_count);
    for (int i = 0; i < storyline->when_count; i++) {
        c->when[i] = mapWhen(cm, scene, storyline->when[i]);
    }
    for (int i = 0; i < storyline->then_count; i++) {
        c->then[i] = mapThen(cm, storyline->then[i]);
    }
    addDebug("done processing when/then clauses");
    return c;
}
