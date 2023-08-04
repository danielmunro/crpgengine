typedef struct {
    int code;
    const char *scene;
} SceneType;

typedef struct {
    int type;
    const char *name;
    const char *music;
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

typedef struct {
    char **scenes;
    char **sceneFiles;
    char *sceneDirectory;
    int count;
} SceneLoader;

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

Scene *createScene(Log *log, RuntimeArgs *runtimeArgs) {
    Scene *scene = malloc(sizeof(Scene));
    scene->storylineCount = 0;
    scene->fight = NULL;
    scene->encounters = createEncounters();
    scene->log = log;
    scene->exploration = createExploration(log, runtimeArgs);
    scene->controlBlockCount = 0;
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        scene->activeControlBlocks[i] = NULL;
    }
    return scene;
}

SceneLoader *createSceneLoader(const char *indexDir) {
    const char *dir = "/scenes";
    SceneLoader *sceneLoader = malloc(sizeof(SceneLoader));
    sceneLoader->sceneDirectory = (char *)malloc(strlen(indexDir) + strlen(dir));
    sprintf(sceneLoader->sceneDirectory, "%s%s", indexDir, dir);
    sceneLoader->scenes = calloc(MAX_SCENES, sizeof(char *));
    sceneLoader->sceneFiles = calloc(MAX_SCENES, sizeof(char *));
    return sceneLoader;
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
    Then *then = cb->then[cb->progress];
    if (isMovingAndAtDestination(cb)) {
        addInfo(s->log, "mob at destination, control block proceeding :: %s", then->target->name);
        cb->progress++;
    } else if (isAddStoryOutcome(then)) {
        addStory(p, then->story);
        cb->progress++;
    } else if (needsToStartMoving(then)) {
        addInfo(s->log, "mob needs to start moving");
        addMobileMovement(
                s->exploration,
                createMobileMovement(
                        then->target,
                        then->position
                )
        );
        p->engaged = false;
        getPartyLeader(p)->isBeingMoved = true;
    } else if (isFaceDirectionOutcome(then)) {
        addInfo(s->log, "set direction for mob :: %s, %s", then->target->name, then->direction);
        then->target->direction =
                getDirectionFromString(then->direction);
        cb->progress++;
    } else if (needsToChangePosition(then)) {
        Mobile *target = then->target;
        target->position = then->position;
        addInfo(s->log, "change position for mob :: %s, %f, %f",
                target->name, target->position.x, target->position.y);
        cb->progress++;
    } else if (needsToWait(then)) {
        Mobile *target = then->target;
        if (target->waitTimer == -1) {
            addInfo(s->log, "setting initial wait timer");
            target->waitTimer = then->amount;
        }
        struct timeval update;
        gettimeofday(&update, NULL);
        if (update.tv_sec > target->lastTimerUpdate.tv_sec) {
            target->lastTimerUpdate = update;
            target->waitTimer--;
            if (target->waitTimer == 0) {
                addInfo(s->log, "timer done");
                target->waitTimer = -1;
                cb->progress++;
            }
        }
    } else if (needsToLock(then)) {
        Mobile *mob = getPartyLeader(p);
        mob->locked = true;
        resetMoving(mob);
        cb->progress++;
    } else if (needsToUnlock(then)) {
        Mobile *mob = getPartyLeader(p);
        mob->locked = false;
        resetMoving(mob);
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
        if (controlBlocks[i] == controlBlock) {
            return true;
        }
    }
    return false;
}

void controlWhenCheck(Scene *s, Player *p, EventType eventType) {
    addDebug(s->log, "control when check");
    for (int i = 0; i < s->controlBlockCount; i++) {
        ControlBlock *cb = s->controlBlocks[i];
        if (areConditionsMet(cb, p, eventType) && !isAlreadyAdded(s->activeControlBlocks, cb)) {
            addDebug(s->log, "ready to set");
            addActiveControl(s, cb);
            addInfo(s->log, "set active control block %d", i);
            return;
        }
    }
}

bool canTriggerFight(Scene *s, Player *p) {
    if (!isDungeon(s) || isFighting(s) || !isMoving(getPartyLeader(p))) {
        return false;
    }
    return true;
}

void checkControls(Scene *s, Player *p, const char *indexDir) {
    addDebug(s->log, "exploration -- check %d control blocks", s->controlBlockCount);
    controlWhenCheck(s, p, EVENT_GAME_LOOP);
    controlThenCheckAllActive(s, p);
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (s->activeControlBlocks[i] != NULL &&
                needsToRemoveActiveControlBlock(s->activeControlBlocks[i])) {
            s->activeControlBlocks[i]->progress = 0;
            s->activeControlBlocks[i] = NULL;
            save(p, s->name, indexDir);
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


int addSubsceneFiles(SceneLoader *sl) {
    for (int i = 0; i < sl->count; i++) {
        char subSceneDir[MAX_FS_PATH_LENGTH];
        sprintf(subSceneDir, "%s/%s/scenes", sl->sceneDirectory, sl->scenes[i]);
        if (access(subSceneDir, F_OK) == 0) {
            char *subScenes[MAX_SCENES];
            int subCount = getFilesInDirectory(subSceneDir, subScenes);
            for (int j = 0; j < subCount; j++) {
                sl->scenes[sl->count] = subScenes[j];
                char subSceneFile[MAX_FS_PATH_LENGTH];
                sprintf(subSceneFile, "%s/%s", subSceneDir, subScenes[j]);
                sl->sceneFiles[sl->count] = (char *)malloc(strlen(subSceneFile));
                strcpy(sl->sceneFiles[sl->count], subSceneFile);
                sl->count++;
            }
        }
    }
    return sl->count;
}

void buildSceneFilesList(SceneLoader *sl) {
    for (int i = 0; i < sl->count; i++) {
        char *sceneFile = malloc(MAX_FS_PATH_LENGTH);
        sprintf(sceneFile, "%s/%s", sl->sceneDirectory, sl->scenes[i]);
        sl->sceneFiles[i] = &sceneFile[0];
    }
}
