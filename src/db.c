void loadAnimations(Log *log, const char *file, const char *indexDir, Animation *animations[MAX_ANIMATIONS]) {
    addInfo(log, "load animations file: %s", file);
    AnimationData *animation = loadAnimationYaml(file);
    char filePath[255];
    sprintf(filePath, "%s/animations/%s", indexDir, animation->sprite->file);
    SpriteSheet *sp = createSpriteSheet(
            filePath,
            animation->sprite->size[0],
            animation->sprite->size[1]);
    for (int i = 0; i < animation->slices_count; i++) {
        SliceData *s = &animation->slices[i];
        animations[i] = createAnimation(
                sp,
                getAnimIdFromName(s->name),
                s->frames[0],
                s->frames[1],
                s->rate,
                s->repeat
        );
    }
    addDebug(log, "%d animations loaded", animation->slices_count);
}

void loadMobiles(Scene *scene, const char *indexDir) {
    char directory[255];
    sprintf(directory, "%s/scenes/%s/mobiles", indexDir, scene->name);
    addInfo(scene->log, "load mobiles from %s", directory);
    if (!FileExists(directory)) {
        addInfo(scene->log, "mobiles directory does not exist, skipping -- %s", directory);
        return;
    }
    char *mobFiles[MAX_MOBILES];
    int files = getFilesInDirectory(directory, mobFiles);
    for (int i = 0; i < files; i++) {
        char filePath[255];
        sprintf(filePath, "%s/%s", directory, mobFiles[i]);
        MobileData *mobData = loadMobYaml(filePath);
        Animation *animations[MAX_ANIMATIONS];
        char animationFilePath[255];
        sprintf(animationFilePath, "%s/%s", indexDir, mobData->animations);
        loadAnimations(scene->log, animationFilePath, indexDir, animations);
        Mobile *mob = createMobileFromData(mobData, animations);
        addMobile(scene->exploration, mob);
    }
}

void loadEncounters(Beastiary *beastiary, Scene *scene, EncountersData *data, const char *indexDir) {
    char filePath[255];
    sprintf(filePath, "%s/images/%s", indexDir, data->background);
    scene->encounters->background = LoadTextureFromImage(LoadImage(filePath));
    addDebug(scene->log, "beast count: %d, beastiary count: %d", data->beasts_count, beastiary->beastCount);
    for (int i = 0; i < data->beasts_count; i++) {
        for (int b = 0; b < beastiary->beastCount; b++) {
            if (strcmp(data->beasts[i].id, beastiary->beasts[b]->id) == 0) {
                scene->encounters->beastEncounters[i] = createBeastEncounterFromData(beastiary->beasts[b],
                                                                                     data->beasts[i]);
                scene->encounters->beastEncountersCount++;
                addDebug(scene->log, "scene %s encounter -- %s, max %d",
                       scene->name,
                       scene->encounters->beastEncounters[i]->beast->id,
                       scene->encounters->beastEncounters[i]->max);
                break;
            }
        }
        if (scene->encounters->beastEncounters[i] == NULL) {
            addError(scene->log, "unable to find beast with id: %s", data->beasts[i].id);
        }
    }
    addInfo(scene->log, "done loading encounters for scene %s with beast count %d",
           scene->name,
           scene->encounters->beastEncountersCount);
}

Scene *loadScene(Log *log, Beastiary *beastiary, const char *indexDir, char *sceneName, int showCollisions) {
    addInfo(log, "create scene '%s'", sceneName);
    char filePath[255];
    sprintf(filePath, "%s/scenes/%s", indexDir, sceneName);
    SceneData *sceneData = loadSceneYaml(filePath);
    Scene *scene = createScene(log, showCollisions);

    // scene properties
    scene->name = &sceneName[0];
    setSceneTypeFromString(scene, sceneData->type);
    scene->music = &sceneData->music[0];

    // storylines
    for (int i = 0; i < sceneData->storylines_count; i++) {
        addStoryline(scene, &sceneData->storylines[i]);
    }

    // create scene reader for reading tiled xml
    char sceneDir[255];
    sprintf(sceneDir, "%s/scenes/%s/map", indexDir, sceneName);
    char sceneFilePath[255];
    sprintf(sceneFilePath, "%s/tilemap.tmx", sceneDir);
    TilemapXmlReader *tilemapXmlReader = createTilemapXmlReader(scene->exploration, sceneFilePath);
    addDebug(scene->log, "create scene '%s' tilemap", sceneName);
    parseSceneXml(tilemapXmlReader, sceneDir);

    // load mobiles
    loadMobiles(scene, indexDir);

    if (sceneData->encounters != NULL) {
        loadEncounters(beastiary, scene, sceneData->encounters, indexDir);
    }

    free(tilemapXmlReader);
    addDebug(scene->log, "done parsing scene %s", sceneName);

    return scene;
}

Player *loadPlayer(Log *log, char *indexDir) {
    addInfo(log, "loading player from dir %s", indexDir);
    PlayerData *playerYaml = loadPlayerYaml(log, indexDir);
    Animation *animations[MAX_ANIMATIONS];
    char filePath[255];
    sprintf(filePath, "%s/%s", indexDir, playerYaml->animations);
    loadAnimations(log, filePath, indexDir, animations);
    Player *player = createPlayer(
            log,
            createMobile(
                    "player",
                    playerYaml->name,
                    (Vector2){0, 0},
                    DOWN,
                    animations));
    return player;
}

AudioManager *loadAudioManager(Log *log, char *indexDir) {
    addInfo(log, "load audio manager from dir '%s'", indexDir);
    AudioManager *am = createAudioManager(log);
    assignAudioManagerValues(am, indexDir);
    addInfo(log, "audio manager loaded %d songs", am->musicCount);
    return am;

}
