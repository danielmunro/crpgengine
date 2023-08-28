#include <unistd.h>

void loadAnimations(AnimationManager *am, SpritesheetManager *sm, const char *file) {
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
                s->frameStart,
                s->frameEnd,
                s->rate,
                s->repeat
        );
    }
    am->libraryCount = animation->slices_count;
    addDebug(am->log, "%d animations loaded", animation->slices_count);
    free(animation);
}

void loadMobiles(MobileManager *mm, Scene *scene, const char *sceneDirectory) {
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/mobiles", sceneDirectory);
    addInfo(scene->log, "load mobiles from %s", directory);
    if (!FileExists(directory)) {
        addInfo(scene->log, "mobiles directory does not exist, skipping -- %s", directory);
        return;
    }
    char **mobFiles = calloc(MAX_FILES, sizeof(char *));
    int files = getFilesInDirectory(directory, mobFiles);
    for (int i = 0; i < files; i++) {
        char *filePath = malloc(1 + strlen(directory) + strlen(&mobFiles[i][0]));
        sprintf(filePath, "%s/%s", directory, &mobFiles[i][0]);
        MobileData *mobData = loadMobYaml(filePath);
        Animation *animations[MAX_ANIMATIONS];
        loadAnimationsByName(mm->animationManager, mobData->animations, animations);
        Mobile *mob = createMobileFromData(mobData, animations);
        addMobileToManager(mm, mob);
        addMobileToExploration(scene->exploration, mob);
        free(filePath);
        free(mobData);
    }
    free(mobFiles);
}

void loadEncounters(Beastiary *beastiary, Scene *scene, EncountersData *data, const char *indexDir) {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/images/%s", indexDir, data->background);
    scene->encounters->background = LoadTextureFromImage(LoadImage(filePath));
    addDebug(scene->log, "beast count: %d, beastiary count: %d", data->beasts_count, beastiary->count);
    for (int i = 0; i < data->beasts_count; i++) {
        for (int b = 0; b < beastiary->count; b++) {
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
    if (access(storylinesDirectory, F_OK) != 0) {
        addInfo(s->log, "scene has no storylines :: %s", s->name);
        return;
    }
    char **storylineFiles = calloc(MAX_FILES, sizeof(char *));
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
            free(storylines);
        }
    }
    free(storylineFiles);
    addInfo(s->log, "added storylines to game :: %d", count);
}

Scene *loadScene(
        Log *log,
        MobileManager *mm,
        Beastiary *beastiary,
        char *sceneName,
        const char *sceneDirectory,
        RuntimeArgs *runtimeArgs) {
    addInfo(log, "create scene '%s'", sceneName);
    SceneData *sceneData = loadSceneYaml(sceneDirectory);
    Scene *scene = createScene(log, runtimeArgs);

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
    loadMobiles(mm, scene, sceneDirectory);

    if (sceneData->encounters != NULL) {
        loadEncounters(beastiary, scene, sceneData->encounters, runtimeArgs->indexDir);
    }

    free(tilemapXmlReader);
    addDebug(scene->log, "done parsing scene %s", sceneName);

    return scene;
}

void loadScenes(
        SceneManager *sm,
        MobileManager *mm,
        Beastiary *beastiary,
        RuntimeArgs *runtimeArgs,
        char *scenes[MAX_SCENES],
        char *sceneDirectories[MAX_SCENES]) {
    addDebug(sm->log, "attempting to load scenes");
    for (int i = 0; i < sm->count; i++) {
        sm->scenes[i] = loadScene(
                sm->log,
                mm,
                beastiary,
                scenes[i],
                sceneDirectories[i],
                runtimeArgs);
        addDebug(sm->log, "scene loaded :: %s (%d)", sm->scenes[i]->name, i);
    }
    for (int i = 0; i < sm->count; i++) {
        addDebug(sm->log, "scene storyline count :: %s -- %d",
                 sm->scenes[i]->name, sm->scenes[i]->storylineCount);
        for (int c = 0; c < sm->scenes[i]->storylineCount; c++) {
            sm->scenes[i]->controlBlocks[c] = mapStorylineToControlBlock(
                    sm->controlManager, sm->scenes[i], sm->scenes[i]->storylines[c]);
            sm->scenes[i]->controlBlockCount++;
        }
    }
}

void loadScenesFromFiles(
        SceneManager *sm,
        MobileManager *mobileManager,
        Beastiary *beastiary,
        RuntimeArgs *runtimeArgs) {
    SceneLoader *sl = createSceneLoader(runtimeArgs->indexDir);
    addDebug(sm->log, "get scene directories :: %s", sl->sceneDirectory);
    sl->count = getFilesInDirectory(sl->sceneDirectory, sl->scenes);
    addDebug(sm->log, "top level count :: %d", sl->count);
    buildSceneFilesList(sl);
    sm->count = addSubsceneFiles(sl);
    for (int i = 0; i < sm->count; i++) {
        addInfo(sm->log, "found scene: %s, %s", sl->scenes[i], sl->sceneFiles[i]);
    }
    loadScenes(sm, mobileManager, beastiary,
               runtimeArgs, sl->scenes, sl->sceneFiles);
    free(sl);
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
    char **files = calloc(MAX_FILES, sizeof(char *));
    int filesInDirectory = getFilesInDirectory(directory, files);
    int count = 0;
    for (int i = 0; i < filesInDirectory; i++) {
        if (strcmp(getFilenameExt(&files[i][0]), "yaml") == 0) {
            char dataFilePath[MAX_FS_PATH_LENGTH];
            sprintf(dataFilePath, "%s/%s", directory, files[i]);
            SpritesheetData *data = loadSpritesheetYaml(dataFilePath);
            char imageFilePath[MAX_FS_PATH_LENGTH];
            sprintf(imageFilePath, "%s/%s", directory, data->filename);
            addInfo(log, "spritesheet :: %s, %s", data->name, imageFilePath);
            spritesheets[count] = createSpriteSheet(
                    data->name,
                    imageFilePath,
                    data->frame->width,
                    data->frame->height,
                    data->frame->padding);
            count++;
            free(data);
        }
    }
    free(files);
    addInfo(log, "spritesheet count :: %d", count);
    return createSpriteSheetManager(spritesheets, count);
}

void loadAllItems(ItemManager *itemManager, const char *indexDir) {
    const char *itemsFile = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *)itemsFile, "%s/items.yaml", indexDir);
    ItemsData *itemsData = loadItemYaml(itemsFile);
    itemManager->items = calloc(itemsData->items_count, sizeof(ItemData));
    for (int i = 0; i < itemsData->items_count; i++) {
        itemManager->items[i] = createItemFromData(&itemsData->items[i]);
    }
    itemManager->count = itemsData->items_count;
    free((char *)itemsFile);
}

void loadAllAnimations(AnimationManager *am, SpritesheetManager *sm, const char *indexDir) {
    char animationsDir[MAX_FS_PATH_LENGTH / 2];
    sprintf(animationsDir, "%s/animations", indexDir);
    char **files = calloc(MAX_FILES, sizeof(char *));
    int count = getFilesInDirectory(animationsDir, files);
    for (int i = 0; i < count; i++) {
        if (strcmp(getFilenameExt(files[i]), "yaml") == 0) {
            char animationFile[MAX_FS_PATH_LENGTH];
            sprintf(animationFile, "%s/%s", animationsDir, files[i]);
            loadAnimations(am, sm, animationFile);
        }
    }
    free(files);
}

Beastiary *loadBeastiary(Log *log, const char *indexDir) {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/beastiary.yaml", indexDir);
    BeastiaryData *data = loadBeastiaryYaml(filePath);
    Beastiary *beastiary = malloc(sizeof(Beastiary));
    beastiary->log = log;
    beastiary->count = data->beasts_count;
    for (int i = 0; i < data->beasts_count; i++) {
        beastiary->beasts[i] = createBeastFromData(indexDir, &data->beasts[i]);
        addDebug(beastiary->log, "beast '%s' created", beastiary->beasts[i]->id);
    }
    free(data);
    return beastiary;
}

SpellManager *loadSpellManager(Log *log, const char *indexDir) {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/spells.yaml", indexDir);
    SpellsData *data = loadSpellData(filePath);
    Spell **spells = calloc(MAX_SPELLS, sizeof(Spell));
    for (int i = 0; i < data->spells_count; i++) {
        spells[i] = createSpellFromData(data->spells[i]);
        addDebug(log, "spell created :: %s", data->spells[i].type);
    }
    SpellManager *sm = createSpellManager(log, spells, data->spells_count);
    free(data);
    return sm;
}
