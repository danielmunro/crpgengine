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

void checkFights(Scene *s, Player *p, Timing *timing) {
    addDebug(s->log, "exploration -- check for fight");
    if (!canTriggerFight(s, p)) {
        return;
    }
    if (rand() % 100 + 1 == 1) {
        s->fight = createFightFromEncounters(
                s->log,
                s->encounters);
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
            char **subScenes = calloc(MAX_SCENES, sizeof(char *));
            int subCount = getFilesInDirectory(subSceneDir, subScenes);
            for (int j = 0; j < subCount; j++) {
                sl->scenes[sl->count] = subScenes[j];
                char subSceneFile[MAX_FS_PATH_LENGTH];
                sprintf(subSceneFile, "%s/%s", subSceneDir, subScenes[j]);
                sl->sceneFiles[sl->count] = (char *)malloc(strlen(subSceneFile));
                strcpy(sl->sceneFiles[sl->count], subSceneFile);
                sl->count++;
            }
            free(subScenes);
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
