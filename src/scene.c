#include <string.h>
#include <mm_malloc.h>
#include <unistd.h>
#include "headers/util/log.h"
#include "headers/map.h"
#include "headers/fight.h"
#include "headers/shop.h"

typedef enum {
    SCENE_TYPE_TOWN,
    SCENE_TYPE_DUNGEON,
} SceneType;

typedef struct {
    SceneType type;
    const char *scene;
} SceneTypeEntry;

const SceneTypeEntry sceneTypes[] = {
        {SCENE_TYPE_TOWN,    "town"},
        {SCENE_TYPE_DUNGEON, "dungeon"},
};

typedef struct {
    int id;
    SceneType type;
    const char *name;
    const char *music;
    StorylineData *storylines[MAX_STORIES];
    int storylineCount;
    ControlBlock *controlBlocks[MAX_CONTROLS];
    int controlBlockCount;
    ControlBlock *activeControlBlocks[MAX_ACTIVE_CONTROLS];
    Encounters *encounters;
    Map *map;
    Shop *shops[MAX_SHOPS];
    int shopsCount;
} Scene;

typedef struct {
    char **scenes;
    char **sceneFiles;
    char *sceneDirectory;
    int count;
} SceneLoader;

SceneType getSceneTypeFromString(const char *sceneType) {
    int count = sizeof(sceneTypes) / sizeof(SceneTypeEntry);
    for (int i = 0; i < count; i++) {
        if (strcmp(sceneTypes[i].scene, sceneType) == 0) {
            return sceneTypes[i].type;
        }
    }
    addError("unknown scene type :: %s", sceneType);
    exit(ConfigurationErrorUnknownSceneType);
}

Scene *createScene(const int id, const char *name, SceneType type, const char *music) {
    Scene *scene = malloc(sizeof(Scene));
    scene->id = id;
    scene->name = name;
    scene->type = type;
    scene->music = music;
    scene->storylineCount = 0;
    scene->encounters = createEncounters();
    scene->controlBlockCount = 0;
    scene->shopsCount = 0;
    for (int i = 0; i < MAX_SHOPS; i++) {
        scene->shops[i] = NULL;
    }
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        scene->activeControlBlocks[i] = NULL;
    }
    return scene;
}

int addSubsceneFiles(SceneLoader *sl) {
    for (int i = 0; i < sl->count; i++) {
        char *subSceneDir = malloc(MAX_FS_PATH_LENGTH);
        sprintf(subSceneDir, "%s/%s/scenes", sl->sceneDirectory, sl->scenes[i]);
        if (access(subSceneDir, F_OK) == 0) {
            char **subScenes = calloc(MAX_SCENES, sizeof(char *));
            int subCount = getFilesInDirectory(subSceneDir, subScenes);
            for (int j = 0; j < subCount; j++) {
                sl->scenes[sl->count] = subScenes[j];
                sl->sceneFiles[sl->count] = malloc(MAX_FS_PATH_LENGTH);
                sprintf(sl->sceneFiles[sl->count], "%s/%s", subSceneDir, subScenes[j]);
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
        sl->sceneFiles[i] = sceneFile;
    }
}

SceneLoader *createSceneLoader(const char *indexDir) {
    const char *dir = "/scenes";
    SceneLoader *sceneLoader = malloc(sizeof(SceneLoader));
    sceneLoader->sceneDirectory = (char *) malloc(strlen(indexDir) + strlen(dir));
    sprintf(sceneLoader->sceneDirectory, "%s%s", indexDir, dir);
    sceneLoader->scenes = calloc(MAX_SCENES, sizeof(char *));
    sceneLoader->sceneFiles = calloc(MAX_SCENES, sizeof(char *));
    addDebug("get scene directories :: %s", sceneLoader->sceneDirectory);
    sceneLoader->count = getFilesInDirectory(sceneLoader->sceneDirectory, sceneLoader->scenes);
    buildSceneFilesList(sceneLoader);
    addSubsceneFiles(sceneLoader);
    addDebug("scene loader found scenes :: %d", sceneLoader->count);
    for (int i = 0; i < sceneLoader->count; i++) {
        addDebug("scene :: %s (%s)", sceneLoader->scenes[i], sceneLoader->sceneFiles[i]);
    }
    return sceneLoader;
}

bool isDungeon(const Scene *s) {
    return s->type == SCENE_TYPE_DUNGEON;
}

void addActiveControl(Scene *s, ControlBlock *cb) {
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (s->activeControlBlocks[i] == NULL) {
            s->activeControlBlocks[i] = cb;
            return;
        }
    }
    addError("too many active control blocks, cannot add new one");
    exit(GameEngineErrorCannotAddMoreActiveControls);
}

void addStoryline(Scene *scene, StorylineData *storyline) {
    scene->storylines[scene->storylineCount] = storyline;
    scene->storylineCount++;
}

bool isAlreadyAdded(ControlBlock *controlBlocks[MAX_ACTIVE_CONTROLS], const ControlBlock *controlBlock) {
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (controlBlocks[i] == controlBlock) {
            return true;
        }
    }
    return false;
}

void controlWhenCheck(Scene *s, const Player *p, EventType eventType) {
    for (int i = 0; i < s->controlBlockCount; i++) {
        ControlBlock *cb = s->controlBlocks[i];
        if (areConditionsMet(cb, p, eventType) && !isAlreadyAdded(s->activeControlBlocks, cb)) {
            addActiveControl(s, cb);
            addInfo("set active control block :: eventType: %d, progress: %d, i: %d", eventType, cb->progress, i);
            return;
        }
    }
}
