#include <unistd.h>

void loadAnimations(AnimationManager *am, SpritesheetManager *sm, const char *file, const char *indexDir) {
    addInfo(am->log, "load animations file: %s", file);
    AnimationData *animation = loadAnimationYaml(file);
    Spritesheet *sp = findSpritesheetByName(sm, animation->sprite->name);
    if (sp == NULL) {
        addFatal(am->log, "spritesheet not found: %s", animation->sprite->name);
    }
    for (int i = 0; i < animation->slices_count; i++) {
        SliceData *s = &animation->slices[i];
        am->library[i] = createAnimation(
                animation->name,
                getAnimationTypeFromName(s->name),
                sp,
                s->frames[0],
                s->frames[1],
                s->rate,
                s->repeat
        );
    }
    am->libraryCount = animation->slices_count;
    addDebug(am->log, "%d animations loaded", animation->slices_count);
}

void loadMobiles(AnimationManager *am, Scene *scene, const char *sceneDirectory) {
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/mobiles", sceneDirectory);
    addInfo(scene->log, "load mobiles from %s", directory);
    if (!FileExists(directory)) {
        addInfo(scene->log, "mobiles directory does not exist, skipping -- %s", directory);
        return;
    }
    char *mobFiles[MAX_MOBILES];
    int files = getFilesInDirectory(directory, mobFiles);
    for (int i = 0; i < files; i++) {
        char filePath[MAX_FS_PATH_LENGTH];
        sprintf(filePath, "%s/%s", directory, mobFiles[i]);
        MobileData *mobData = loadMobYaml(filePath);
        Animation *animations[MAX_ANIMATIONS];
        loadAnimationsByName(am, mobData->animations, animations);
        Mobile *mob = createMobileFromData(mobData, animations);
        addMobile(scene->exploration, mob);
    }
}

void loadEncounters(Beastiary *beastiary, Scene *scene, EncountersData *data, const char *indexDir) {
    char filePath[MAX_FS_PATH_LENGTH];
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

void loadStorylines(Scene *s, const char *sceneDirectory) {
    char storylinesDirectory[MAX_FS_PATH_LENGTH];
    sprintf(storylinesDirectory, "%s/storylines", sceneDirectory);
    addDebug(s->log, "storylines directory :: %s", storylinesDirectory);
    char *storylineFiles[MAX_MOBILES];
    if (access(storylinesDirectory, F_OK) != 0) {
        addInfo(s->log, "scene has no storylines :: %s", s->name);
        return;
    }
    int fileCount = getFilesInDirectory(storylinesDirectory, storylineFiles);
    addDebug(s->log, "storyline files found :: %d", fileCount);
    int count = 0;
    for (int i = 0; i < fileCount; i++) {
        char storylinesFilePath[MAX_FS_PATH_LENGTH];
        sprintf(storylinesFilePath, "%s/storylines/%s", sceneDirectory, storylineFiles[i]);
        addDebug(s->log, "storyline file path :: %s", storylinesFilePath);
        StorylinesData *storylines = loadStorylinesYaml(storylinesFilePath);
        if (storylines != NULL) {
            for (int j = 0; j < storylines->storylines_count; j++) {
                addStoryline(s, &storylines->storylines[j]);
                count++;
            }
        }
    }
    addInfo(s->log, "added storylines to game :: %d", count);
}

Scene *loadScene(
        Log *log,
        AnimationManager *am,
        Beastiary *beastiary,
        const char *indexDir,
        char *sceneName,
        const char *sceneDirectory,
        bool showCollisions) {
    addInfo(log, "create scene '%s'", sceneName);
    SceneData *sceneData = loadSceneYaml(sceneDirectory);
    Scene *scene = createScene(log, showCollisions);

    // scene properties
    scene->name = &sceneName[0];
    setSceneTypeFromString(scene, sceneData->type);
    scene->music = &sceneData->music[0];

    // storylines
    loadStorylines(scene, sceneDirectory);

    // create scene reader for reading tiled xml
    char mapDirectory[MAX_FS_PATH_LENGTH];
    sprintf(mapDirectory, "%s/map", sceneDirectory);
    char tilemapFilePath[MAX_FS_PATH_LENGTH];
    sprintf(tilemapFilePath, "%s/tilemap.tmx", mapDirectory);
    TilemapXmlReader *tilemapXmlReader = createTilemapXmlReader(scene->exploration, tilemapFilePath);
    addDebug(scene->log, "create scene '%s' tilemap", sceneName);
    parseSceneXml(tilemapXmlReader, mapDirectory);

    // load mobiles
    loadMobiles(am, scene, sceneDirectory);

    if (sceneData->encounters != NULL) {
        loadEncounters(beastiary, scene, sceneData->encounters, indexDir);
    }

    free(tilemapXmlReader);
    addDebug(scene->log, "done parsing scene %s", sceneName);

    return scene;
}

Player *loadPlayer(Log *log, AnimationManager *am, char *indexDir) {
    addInfo(log, "loading player from dir %s", indexDir);
    PlayerData *playerYaml = loadPlayerYaml(log, indexDir);
    Animation *animations[MAX_ANIMATIONS];
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/%s", indexDir, playerYaml->animations);
    int count = loadAnimationsByName(am, playerYaml->animations, animations);
    addInfo(am->log, "found animations for player :: %d animations", count);
    Mobile *mobiles[MAX_PARTY_SIZE] = {
            createMobile(
                    "player",
                    playerYaml->name,
                    (Vector2) {0, 0},
                    DOWN,
                    animations),
            NULL,
            NULL,
            NULL,
    };
    Player *player = createPlayer(
            log,
            mobiles
    );
    return player;
}

AudioManager *loadAudioManager(Log *log, const char *indexDir) {
    addInfo(log, "load audio manager from dir '%s'", indexDir);
    AudioManager *am = createAudioManager(log);
    assignAudioManagerValues(am, indexDir);
    addInfo(log, "audio manager loaded %d songs", am->musicCount);
    return am;
}

SpritesheetManager *loadSpritesheetManager(Log *log, const char *indexDir) {
    addInfo(log, "load spritesheet manager :: %s", indexDir);
    Spritesheet *spritesheets[MAX_SPRITES];
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/spritesheets", indexDir);
    char *files[MAX_FILES];
    int filesInDirectory = getFilesInDirectory(directory, files);
    int count = 0;
    for (int i = 0; i < filesInDirectory; i++) {
        if (strcmp(getFilenameExt(files[i]), "yaml") == 0) {
            char dataFilePath[MAX_FS_PATH_LENGTH];
            sprintf(dataFilePath, "%s/%s", directory, files[i]);
            SpritesheetData *data = loadSpritesheetYaml(dataFilePath);
            char imageFilePath[MAX_FS_PATH_LENGTH];
            sprintf(imageFilePath, "%s/%s", directory, data->filename);
            addInfo(log, "spritesheet :: %s, %s", data->name, imageFilePath);
            spritesheets[count] = createSpriteSheet(data->name, imageFilePath, data->frame->width, data->frame->height);
            count++;
        }
    }
    addInfo(log, "spritesheet count :: %d", count);
    return createSpriteSheetManager(spritesheets, count);
}
