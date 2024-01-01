#include "headers/audio.h"
#include "headers/map.h"
#include "headers/control_manager.h"

typedef struct {
    Scene **scenes;
    int count;
    Scene *current;
    ControlManager *controlManager;
    AnimationManager *animationManager;
    AudioManager *audioManager;
    UserConfig *userConfig;
} SceneManager;

SceneManager *createSceneManager(
        ControlManager *cm,
        AnimationManager *anManager,
        AudioManager *auManager,
        UserConfig *userConfig) {
    SceneManager *sceneManager = malloc(sizeof(SceneManager));
    sceneManager->current = NULL;
    sceneManager->scenes = calloc(MAX_SCENES, sizeof(Scene));
    sceneManager->controlManager = cm;
    sceneManager->animationManager = anManager;
    sceneManager->audioManager = auManager;
    sceneManager->userConfig = userConfig;
    return sceneManager;
}

void unloadScene(const Scene *s) {
    unloadMap(s->map);
    if (isDungeon(s)) {
        UnloadTexture(s->encounters->background);
    }
}

void initScene(const Scene *s) {
    loadMap(s->map);
    if (isDungeon(s)) {
        Image im = LoadImage(s->encounters->backgroundFilePath);
        s->encounters->background = LoadTextureFromImage(im);
        UnloadImage(im);
    }
}

void setScene(SceneManager *sm, Scene *scene, const Player *player, const char *entranceName) {
    addInfo("set scene :: %s, %d", scene->name, scene->type);
    if (sm->current != NULL) {
        unloadScene(sm->current);
    }
    initScene(scene);
    sm->current = scene;
    sm->controlManager->scene = scene;
    clearAnimations(sm->animationManager);
    Mobile *mob = getPartyLeader(player);
    addAllAnimations(sm->animationManager, mob->animations);
    if (entranceName != NULL) {
        useEntrance(mob, findEntrance(scene->map, entranceName));
    }
    playMusic(sm->audioManager, sm->current->music);
    proceedControlsUntilDone(sm->controlManager);
}

Scene *findScene(const SceneManager *sm, const char *name) {
    for (int i = 0; i < sm->count; i++) {
        if (strcmp(sm->scenes[i]->name, name) == 0) {
            return sm->scenes[i];
        }
    }
    return NULL;
}

void setSceneBasedOnSave(
        SceneManager *sm,
        const Player *player,
        const SaveData *save) {
    int overrideStartScene = strcmp(config->overrideStartScene, "");
    if (save != NULL && overrideStartScene == 0) {
        setScene(sm, findScene(sm, save->scene), player, NULL);
    } else if (overrideStartScene != 0) {
        setScene(sm, findScene(sm, config->overrideStartScene), player, START_ENTRANCE);
    } else {
        setScene(sm, findScene(sm, config->startScene), player, START_ENTRANCE);
    }
}
