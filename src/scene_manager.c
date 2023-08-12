typedef struct {
    Log *log;
    Scene **scenes;
    int count;
    Scene *currentScene;
    ControlManager *controlManager;
    AnimationManager *animationManager;
    AudioManager *audioManager;
} SceneManager;

SceneManager *createSceneManager(Log *log, ControlManager *cm, AnimationManager *anManager, AudioManager *auManager) {
    SceneManager *sceneManager = malloc(sizeof(SceneManager));
    sceneManager->log = log;
    sceneManager->currentScene = NULL;
    sceneManager->scenes = calloc(MAX_SCENES, sizeof(Scene));
    sceneManager->controlManager = cm;
    sceneManager->animationManager = anManager;
    sceneManager->audioManager = auManager;
    return sceneManager;
}

void setScene(SceneManager *sm, Scene *scene, Player *player, char *entranceName) {
    addInfo(sm->log, "setting scene to '%s'", scene->name);
    if (sm->currentScene != NULL) {
        unloadLayers(sm->currentScene->exploration);
    }
    sm->currentScene = scene;
    sm->controlManager->scene = scene;
    clearAnimations(sm->animationManager);
    Mobile *mob = getPartyLeader(player);
    addAllAnimations(sm->animationManager, mob->animations);
    if (entranceName != NULL) {
        Entrance *entrance = findEntrance(scene->exploration, entranceName);
        addDebug(sm->log, "set position from scene entrance :: %s", entrance->name);
        useEntrance(mob, entrance);
    }
    addDebug(sm->log, "player position :: %f %f", mob->position.x, mob->position.y);
    renderExplorationLayers(sm->currentScene->exploration);
    playMusic(sm->audioManager, sm->currentScene->music);
    proceedControlsUntilDone(sm->controlManager);
}
