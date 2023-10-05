#include "headers/audio.h"
#include "headers/exploration.h"
#include "headers/control_manager.h"

typedef struct {
    Scene **scenes;
    int count;
    Scene *current;
    ControlManager *controlManager;
    AnimationManager *animationManager;
    AudioManager *audioManager;
} SceneManager;

SceneManager *createSceneManager(ControlManager *cm, AnimationManager *anManager, AudioManager *auManager) {
    SceneManager *sceneManager = malloc(sizeof(SceneManager));
    sceneManager->current = NULL;
    sceneManager->scenes = calloc(MAX_SCENES, sizeof(Scene));
    sceneManager->controlManager = cm;
    sceneManager->animationManager = anManager;
    sceneManager->audioManager = auManager;
    return sceneManager;
}

void setScene(SceneManager *sm, Scene *scene, Player *player, char *entranceName) {
    addInfo("setting scene to '%s'", scene->name);
    if (sm->current != NULL) {
        unloadLayers(sm->current->exploration);
    }
    sm->current = scene;
    sm->controlManager->scene = scene;
    clearAnimations(sm->animationManager);
    Mobile *mob = getPartyLeader(player);
    addAllAnimations(sm->animationManager, mob->animations);
    if (entranceName != NULL) {
        Entrance *entrance = findEntrance(scene->exploration, entranceName);
        addDebug("set position from scene entrance :: %s", entrance->name);
        useEntrance(mob, entrance);
    }
    addDebug("player position :: %f %f", mob->position.x, mob->position.y);
    renderExplorationLayers(sm->current->exploration);
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
        Player *player,
        SaveData *save) {
    int sceneToUse = runtimeArgs->sceneIndex;
    if (save != NULL && sceneToUse == -1) {
        setScene(sm, findScene(sm, save->scene), player, NULL);
        return;
    }
    if (sceneToUse < 0) {
        sceneToUse = 0;
    }
    setScene(sm, sm->scenes[sceneToUse], player, START_ENTRANCE);
}
