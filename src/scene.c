typedef struct {
    int code;
    const char *scene;
} SceneType;

typedef struct {
    int type;
    char *name;
    char *music;
    StorylineData *storylines[MAX_STORIES];
    int storylineCount;
    ControlBlock *controlBlocks[MAX_CONTROLS];
    int controlBlockCount;
    ControlBlock *activeControlBlocks[MAX_ACTIVE_CONTROLS];
    Encounters *encounters;
    Fight *fight;
    Exploration *exploration;
    Log *log;
} Scene;

const SceneType sceneTypes[] = {
        {SCENE_TYPE_TOWN,    "town"},
        {SCENE_TYPE_DUNGEON, "dungeon"},
};

void setSceneTypeFromString(Scene *s, const char *sceneType) {
    int count = sizeof(sceneTypes) / sizeof(SceneType);
    for (int i = 0; i < count; i++) {
        if (strcmp(sceneTypes[i].scene, sceneType) == 0) {
            s->type = sceneTypes[i].code;
            addDebug(s->log, "scene '%s' type set to '%s'", s->name, sceneType);
            return;
        }
    }
    addError(s->log, "scene type not found: %s, setting to default of 'town'", sceneType);
    s->type = SCENE_TYPE_TOWN;
}

Scene *createScene(Log *log, int showCollisions) {
    Scene *scene = malloc(sizeof(Scene));
    scene->storylineCount = 0;
    scene->fight = NULL;
    scene->encounters = createEncounters();
    scene->log = log;
    scene->exploration = createExploration(log, showCollisions);
    scene->controlBlockCount = 0;
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        scene->activeControlBlocks[i] = NULL;
    }
    return scene;
}

bool isExploring(Scene *s) {
    return s->fight == NULL && !getCurrentMenu(s->exploration);
}

bool isFighting(Scene *s) {
    return s->fight != NULL;
}

bool isDungeon(Scene *s) {
    return s->type == SCENE_TYPE_DUNGEON;
}

void addActiveControl(Scene *s, ControlBlock *cb) {
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (s->activeControlBlocks[i] == NULL) {
            s->activeControlBlocks[i] = cb;
            return;
        }
    }
    addError(s->log, "too many active control blocks, cannot add new one");
}

void addStoryline(Scene *scene, StorylineData *storyline) {
    scene->storylines[scene->storylineCount] = storyline;
    scene->storylineCount++;
}

void thenCheck(Scene *s, Player *p, ControlBlock *cb) {
    if (isMovingAndAtDestination(cb)) {
        addInfo(s->log, "mob at destination, control block proceeding");
        cb->progress++;
    }
    if(isControlBlockDone(cb)) {
        return;
    }
    if (isAddStoryOutcome(cb->then[cb->progress])) {
        addStory(p, cb->then[cb->progress]->story);
        cb->progress++;
    } else if (needsToStartMoving(cb->then[cb->progress])) {
        addInfo(s->log, "mob needs to start moving");
        addMobileMovement(
                s->exploration,
                createMobileMovement(
                        cb->then[cb->progress]->target,
                        cb->then[cb->progress]->position
                )
        );
        p->engaged = false;
    } else if (isFaceDirectionOutcome(cb->then[cb->progress])) {
        cb->then[cb->progress]->target->direction =
                getDirectionFromString(cb->then[cb->progress]->direction);
        cb->progress++;
    }
}

void controlThenCheckAllActive(Scene *s, Player *p) {
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (s->activeControlBlocks[i] != NULL) {
            thenCheck(s, p, s->activeControlBlocks[i]);
        }
    }
}

bool isAlreadyAdded(ControlBlock *controlBlocks[MAX_ACTIVE_CONTROLS], ControlBlock *controlBlock) {
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (controlBlocks[i] != NULL && areConditionsEqual(controlBlocks[i]->when, controlBlock->when)) {
            return true;
        }
    }
    return false;
}

void controlWhenCheck(Scene *s, Player *p) {
    for (int i = 0; i < s->controlBlockCount; i++) {
        ControlBlock *cb = s->controlBlocks[i];
        if (areConditionsMet(cb, p) && !isAlreadyAdded(s->activeControlBlocks, cb)) {
            addDebug(s->log, "ready to set");
            addActiveControl(s, cb);
            addInfo(s->log, "set active control block %d", i);
            return;
        }
    }
    addDebug(s->log, "done control check");
}

bool canTriggerFight(Scene *s, Player *p) {
    if (!isDungeon(s) || isFighting(s) || !isMoving(getPartyLeader(p))) {
        return false;
    }
    return true;
}

void checkControls(Scene *s, Player *p) {
    addDebug(s->log, "exploration -- check %d control blocks", s->controlBlockCount);
    controlWhenCheck(s, p);
    controlThenCheckAllActive(s, p);
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (s->activeControlBlocks[i] != NULL &&
                needsToRemoveActiveControlBlock(s->activeControlBlocks[i])) {
            s->activeControlBlocks[i]->progress = 0;
            s->activeControlBlocks[i] = NULL;
        }
    }
}

void checkFights(Scene *s, Player *p) {
    addDebug(s->log, "exploration -- check for fight");
    if (!canTriggerFight(s, p)) {
        return;
    }
    if (rand() % 100 + 1 == 1) {
        s->fight = createFightFromEncounters(s->log, s->encounters);
        Animation *animation = findAnimation(getPartyLeader(p)->animations, LEFT);
        animation->currentFrame = animation->firstFrame;
    }
}

void checkRemoveFight(Scene *s) {
    if (isFightDone(s->fight)) {
        s->fight = NULL;
    }
}
