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
    ControlBlock *activeControlBlock;
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
    scene->activeControlBlock = NULL;
    scene->fight = NULL;
    scene->encounters = createEncounters();
    scene->log = log;
    scene->exploration = createExploration(log, showCollisions);
    scene->controlBlockCount = 0;
    return scene;
}

int isExploring(Scene *s) {
    return s->fight == NULL && !getCurrentMenu(s->exploration);
}

int isFighting(Scene *s) {
    return s->fight != NULL;
}

int isDungeon(Scene *s) {
    return s->type == SCENE_TYPE_DUNGEON;
}

void addStoryline(Scene *scene, StorylineData *storyline) {
    scene->storylines[scene->storylineCount] = storyline;
    scene->storylineCount++;
}

void controlThenCheck(Scene *s, Player *p) {
    if (s->activeControlBlock == NULL || s->activeControlBlock->progress > s->activeControlBlock->thenCount) {
        return;
    }
    ControlBlock *cb = s->activeControlBlock;
    if (cb->then[cb->progress]->outcome == ADD_STORY) {
        addDebug(s->log, "add storyline '%s' for player", cb->then[cb->progress]->story);
        addStory(p, cb->then[cb->progress]->story);
        cb->progress++;
    }
}

int needsEngagedAndIsNot(int condition, Player *p, Mobile *mobileTrigger) {
    return condition == ENGAGED && !isSpeakingTo(p, mobileTrigger);
}

int needsStoryAndMissing(int condition, Player *p, const char *story) {
    return condition == HAS_STORY && !hasStory(p, story);
}

int needsNotHaveStoryAndPresent(int condition, Player *p, const char *story) {
    return condition == NOT_HAS_STORY && hasStory(p, story);
}

void controlWhenCheck(Scene *s, Player *p) {
    if (s->activeControlBlock != NULL) {
        return;
    }
    for (int i = 0; i < s->controlBlockCount; i++) {
        ControlBlock *cb = s->controlBlocks[i];
        int matched = true;
        for (int c = 0; c < cb->whenCount; c++) {
            if (needsEngagedAndIsNot(cb->when[c]->condition, p, cb->when[c]->trigger)
                || needsStoryAndMissing(cb->when[c]->condition, p, cb->when[c]->story)
                || needsNotHaveStoryAndPresent(cb->when[c]->condition, p, cb->when[c]->story)) {
                matched = false;
            }
            if (!matched) {
                break;
            }
        }
        if (matched) {
            s->activeControlBlock = cb;
            addDebug(s->log, "set active control block %d, progress %d", i, s->activeControlBlock->progress);
            return;
        }
    }
    addDebug(s->log, "done control check");
}

int canTriggerFight(Scene *s, Player *p) {
    if (!isDungeon(s) || isFighting(s) || !isMoving(p)) {
        return false;
    }
    return true;
}

void checkControls(Scene *s, Player *p) {
    addDebug(s->log, "exploration -- check control blocks");
    controlWhenCheck(s, p);
    controlThenCheck(s, p);
    if (needsToRemoveActiveControlBlock(s->activeControlBlock)) {
        s->activeControlBlock->progress = 0;
        s->activeControlBlock = NULL;
    }
}

void checkFights(Scene *s, Player *p) {
    addDebug(s->log, "exploration -- check for fight");
    if (!canTriggerFight(s, p)) {
        return;
    }
    int chance = rand() % 100 + 1;
    if (chance == 1) {
        s->fight = createFightFromEncounters(s->log, s->encounters);
        Animation *animation = findAnimation(p->mob->animations, LEFT);
        animation->currentFrame = animation->firstFrame;
    }
}

void checkRemoveFight(Scene *s) {
    if (isFightDone(s->fight)) {
        s->fight = NULL;
    }
}
