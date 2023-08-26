typedef struct {
    Log *log;
    Scene *scene;
    Player *player;
    RuntimeArgs *runtimeArgs;
    ItemManager *itemManager;
    NotificationManager *notificationManager;
    MobileManager *mobileManager;
} ControlManager;

ControlManager *createControlManager(
        Log *log,
        Player *player,
        RuntimeArgs *runtimeArgs,
        ItemManager *itemManager,
        NotificationManager *notificationManager,
        MobileManager *mobileManager) {
    ControlManager *cm = malloc(sizeof(ControlManager));
    cm->log = log;
    cm->player = player;
    cm->runtimeArgs = runtimeArgs;
    cm->itemManager = itemManager;
    cm->notificationManager = notificationManager;
    cm->mobileManager = mobileManager;
    cm->scene = NULL;
    return cm;
}

int thenCheck(ControlManager *cm, ControlBlock *cb) {
    Then *then = cb->then[cb->progress];
    int progress = 0;
    if (isSpeakOutcome(then) && !cm->player->engaged) {
        addInfo(cm->log, "is speak outcome");
        progress++;
    } else if (isMovingAndAtDestination(cb)) {
        addInfo(cm->log, "mob at destination, control block proceeding :: %s", then->target->name);
        progress++;
    } else if (isAddStoryOutcome(then)) {
        addStory(cm->player, then->story);
        progress++;
    } else if (needsToStartMoving(then)) {
        addInfo(cm->log, "mob needs to start moving");
        addMobileMovement(
                cm->scene->exploration,
                createMobileMovement(
                        then->target,
                        then->position
                )
        );
        cm->player->engaged = false;
        getPartyLeader(cm->player)->isBeingMoved = true;
    } else if (isFaceDirectionOutcome(then)) {
        addInfo(cm->log, "set direction for mob :: %s, %s", then->target->name, then->direction);
        then->target->direction =
                getDirectionFromString(then->direction);
        progress++;
    } else if (needsToChangePosition(then)) {
        Mobile *target = then->target;
        target->position = then->position;
        addInfo(cm->log, "change position for mob :: %s, %f, %f",
                target->name, target->position.x, target->position.y);
        progress++;
    } else if (needsToWait(then)) {
        Mobile *target = then->target;
        if (target->waitTimer == -1) {
            addInfo(cm->log, "setting initial wait timer");
            target->waitTimer = then->amount;
        }
        struct timeval update;
        gettimeofday(&update, NULL);
        if (update.tv_sec > target->lastTimerUpdate.tv_sec) {
            target->lastTimerUpdate = update;
            target->waitTimer--;
            if (target->waitTimer == 0) {
                addInfo(cm->log, "timer done");
                target->waitTimer = -1;
                progress++;
            }
        }
    } else if (needsToLock(then)) {
        Mobile *mob = getPartyLeader(cm->player);
        mob->locked = true;
        resetMoving(mob);
        progress++;
    } else if (needsToUnlock(then)) {
        Mobile *mob = getPartyLeader(cm->player);
        mob->locked = false;
        resetMoving(mob);
        progress++;
    } else if (needsToSave(then)) {
        save(cm->player, cm->scene->name, cm->runtimeArgs->indexDir);
        addNotification(
                cm->notificationManager,
                createNotification(SAVED, "Your game has been saved."));
        progress++;
    } else if (needsToReceiveItem(then, getPartyLeader(cm->player))) {
        addInfo(cm->log, "player receiving item: %s", then->item);
        addItem(cm->player, findItem(cm->itemManager->items, then->item));
        const char *message = malloc(64);
        sprintf((char *)message, "you received:\n%s", then->item);
        addNotification(
                cm->notificationManager,
                createNotification(RECEIVE_QUEST_ITEM, message));
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
    addDebug(cm->log, "exploration -- check %d control blocks",
             cm->scene->controlBlockCount);
    controlWhenCheck(cm->scene, cm->player, EVENT_GAME_LOOP);
    controlThenCheckAllActive(cm);
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (cm->scene->activeControlBlocks[i] != NULL &&
            needsToRemoveActiveControlBlock(cm->scene->activeControlBlocks[i])) {
            cm->scene->activeControlBlocks[i]->progress = 0;
            cm->scene->activeControlBlocks[i] = NULL;
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

When *mapWhen(ControlManager *cm, Scene *s, WhenData wd) {
    Mobile *trigger = NULL;
    Mobile *mob = getPartyLeader(cm->player);
    ArriveAt *arriveAt = NULL;
    if (wd.mob != NULL) {
        trigger = findMobById(cm->mobileManager, wd.mob);
        addDebug(cm->log, "mobile trigger is '%s'", trigger->name);
    }
    if (wd.arriveAt != NULL) {
        Exploration *e = s->exploration;
        for (int i = 0; i < e->arriveAtCount; i++) {
            if (strcmp(e->arriveAt[i]->name, wd.arriveAt) == 0) {
                arriveAt = e->arriveAt[i];
                break;
            }
        }
    }
    Condition c = mapCondition(wd.condition);
    addDebug(cm->log, "condition: %s, mapped to: %d, story: %s",
             wd.condition,
             c,
             wd.story);
    return createWhen(
            mob,
            trigger,
            c,
            wd.story,
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
    Vector2 pos;
    if (td.position != NULL) {
        pos = getPositionFromString(td.position);
    } else {
        pos = (Vector2){0,0};
    }
    int amount = 0;
    if (hasAmountProperty(td)) {
        amount = td.amount;
    }
    Outcome o = mapOutcome(td.action);
    addDebug(cm->log, "then story is '%s', outcome: %d, message: %s",
             td.story, o, td.message);
    return createThen(
            target,
            &td.message[0],
            &td.story[0],
            &td.direction[0],
            &td.item[0],
            o,
            pos,
            td.parallel,
            amount
    );
}

ControlBlock *mapStorylineToControlBlock(ControlManager *cm, Scene *scene, StorylineData *storyline) {
    ControlBlock *c = createControlBlock();
    c->whenCount = storyline->when_count;
    c->thenCount = storyline->then_count;
    addDebug(cm->log, "processing storyline with %d when and %d then clauses",
             storyline->when_count, storyline->then_count);
    for (int i = 0; i < storyline->when_count; i++) {
        c->when[i] = mapWhen(cm, scene, storyline->when[i]);
    }
    for (int i = 0; i < storyline->then_count; i++) {
        c->then[i] = mapThen(cm, storyline->then[i]);
    }
    addDebug(cm->log, "done processing when/then clauses");
    return c;
}
