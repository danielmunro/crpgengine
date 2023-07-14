#include <unistd.h>
#include <sys/stat.h>

void loadAnimations(Log *log, const char *file, const char *indexDir, Animation *animations[MAX_ANIMATIONS]) {
    addInfo(log, "load animations file: %s", file);
    AnimationData *animation = loadAnimationYaml(file);
    char cwd[PATH_MAX] = "";
    getcwd(cwd, sizeof(cwd));
    char filePath[MAX_FS_PATH_LENGTH] = "";
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

void loadMobiles(Scene *scene, const char *indexDir, const char *sceneDir) {
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/mobiles", sceneDir);
    addInfo(scene->log, "load mobiles from %s", directory);
    if (!FileExists(directory)) {
        addInfo(scene->log, "mobiles directory does not exist, skipping -- %s", directory);
        return;
    }
    char *mobFiles[MAX_MOBILES];
    int files = getFilesInDirectory(directory, mobFiles);
    for (int i = 0; i < files; i++) {
//        char filePath[MAX_FS_PATH_LENGTH];
//        sprintf(filePath, "%s/%s", directory, mobFiles[i]);
        MobileData *mobData = loadMobYaml(mobFiles[i]);
        Animation *animations[MAX_ANIMATIONS];
        char animationFilePath[MAX_FS_PATH_LENGTH];
        sprintf(animationFilePath, "%s/%s", indexDir, mobData->animations);
        loadAnimations(scene->log, animationFilePath, indexDir, animations);
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

void loadStorylines(Scene *s, const char *sceneDir) {
    char storylinesDirectory[MAX_FS_PATH_LENGTH];
    sprintf(storylinesDirectory, "%s/storylines", sceneDir);
    addInfo(s->log, "storylines directory :: %s", storylinesDirectory);
    char *storylineFiles[MAX_MOBILES];
    int fileCount = getFilesInDirectory(storylinesDirectory, storylineFiles);
    addDebug(s->log, "storyline files found :: %d", fileCount);
    int count = 0;
    for (int i = 0; i < fileCount; i++) {
        addDebug(s->log, "storyline file path :: %s", storylineFiles[i]);
        StorylinesData *storylines = loadStorylinesYaml(storylineFiles[i]);
        if (storylines != NULL) {
            for (int j = 0; j < storylines->storylines_count; j++) {
                addStoryline(s, &storylines->storylines[j]);
                count++;
            }
        }
    }
    addInfo(s->log, "added storylines to game :: %d", count);
}

Scene *loadScene(Log *log, Beastiary *beastiary, const char *indexDir, const char *sceneDir, int showCollisions) {
    addInfo(log, "load scene :: %s, %s", indexDir, sceneDir);
    SceneData *sceneData = loadSceneYaml(sceneDir);
    Scene *scene = createScene(log, showCollisions);

    // scene properties
    scene->name = sceneDir;
    setSceneTypeFromString(scene, sceneData->type);
    scene->music = &sceneData->music[0];
    
    // storylines
    loadStorylines(scene, sceneDir);

    // create scene reader for reading tiled xml
    char mapDir[MAX_FS_PATH_LENGTH];
    sprintf(mapDir, "%s/map", sceneDir);
    char tilemapFilePath[MAX_FS_PATH_LENGTH];
    sprintf(tilemapFilePath, "%s/tilemap.tmx", mapDir);
    addInfo(log, "map dir :: %s", mapDir);
    TilemapXmlReader *tilemapXmlReader = createTilemapXmlReader(scene->exploration, tilemapFilePath);
    addDebug(scene->log, "create scene '%s' tilemap", sceneDir);
    parseSceneXml(tilemapXmlReader, mapDir);

    // load mobiles
    loadMobiles(scene, indexDir, sceneDir);

    if (sceneData->encounters != NULL) {
        loadEncounters(beastiary, scene, sceneData->encounters, indexDir);
    }

    free(tilemapXmlReader);
    addDebug(scene->log, "done parsing scene %s", sceneDir);

    return scene;
}

Player *loadPlayer(Log *log, char *indexDir) {
    addInfo(log, "loading player from dir %s", indexDir);
    PlayerData *playerYaml = loadPlayerYaml(log, indexDir);
    Animation *animations[MAX_ANIMATIONS];
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/%s", indexDir, playerYaml->animations);
    loadAnimations(log, filePath, indexDir, animations);
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

AudioManager *loadAudioManager(Log *log, char *indexDir) {
    addInfo(log, "load audio manager from dir '%s'", indexDir);
    AudioManager *am = createAudioManager(log);
    assignAudioManagerValues(am, indexDir);
    addInfo(log, "audio manager loaded %d songs", am->musicCount);
    return am;

}
