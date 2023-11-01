#include <raylib.h>
#include <mm_malloc.h>
#include <string.h>
#include "headers/util/log.h"
#include "headers/fight.h"
#include "headers/control_manager.h"
#include "headers/mobile_manager.h"
#include "headers/persistence/db.h"
#include "headers/persistence/xmlparser.h"

void loadAnimations(AnimationManager *am, SpritesheetManager *sm, const char *file) {
    addDebug("load animations file: %s", file);
    AnimationData *animation = loadAnimationYaml(file);
    Spritesheet *sp = findSpritesheetByName(sm, animation->sprite->name);
    if (sp == NULL) {
        addError("spritesheet not found :: %s",
                animation->sprite->name);
        exit(ConfigurationErrorUnknownSpriteSheet);
    }
    for (int i = 0; i < animation->slices_count; i++) {
        const SliceData *s = &animation->slices[i];
        am->library[i] = createAnimation(
                animation->name,
                getDirectionFromString(s->name),
                sp,
                s->frameStart,
                s->frameEnd,
                s->rate,
                s->repeat
        );
    }
    am->libraryCount = animation->slices_count;
    addDebug("%d animations loaded", animation->slices_count);
    free(animation);
}

void loadMobiles(MobileManager *mm, Scene *scene, const char *sceneDirectory) {
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/mobiles", sceneDirectory);
    addDebug("load mobiles from %s", directory);
    if (!FileExists(directory)) {
        addDebug("mobiles directory does not exist, skipping -- %s", directory);
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
        Mobile *mob = createMobileFromData(mm, mobData, animations);
        addMobileToManager(mm, mob);
        addMobileToExploration(scene->map, mob);
        free(filePath);
        free(mobData);
    }
    free(mobFiles);
}

void loadPlayerMobiles(MobileManager *mm) {
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/player/mobiles", config->indexDir);
    addDebug("load player mobiles from %s", directory);
    if (!FileExists(directory)) {
        addError("player mobiles directory does not exist :: %s", directory);
        exit(ConfigurationErrorPlayerMobilesDirectoryDoesNotExist);
    }
    char **mobFiles = calloc(MAX_FILES, sizeof(char *));
    int files = getFilesInDirectory(directory, mobFiles);
    for (int i = 0; i < files; i++) {
        char filePath[MAX_FS_PATH_LENGTH];
        sprintf(filePath, "%s/%s", directory, &mobFiles[i][0]);
        MobileData *mobData = loadMobYaml(filePath);
        Animation *animations[MAX_ANIMATIONS];
        loadAnimationsByName(mm->animationManager, mobData->animations, animations);
        Mobile *mob = createMobileFromData(mm, mobData, animations);
        addPlayerMobileToManager(mm, mob);
        free(mobData);
    }
    free(mobFiles);
}

void loadEncounters(const Beastiary *beastiary, Scene *scene, const EncountersData *data, const char *indexDir) {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/images/%s", indexDir, data->background);
    scene->encounters->background = LoadTextureFromImage(LoadImage(filePath));
    addDebug("beast count: %d, beastiary count: %d", data->beasts_count, beastiary->count);
    for (int i = 0; i < data->beasts_count; i++) {
        for (int b = 0; b < beastiary->count; b++) {
            if (strcmp(data->beasts[i].id, beastiary->beasts[b]->id) == 0) {
                scene->encounters->beastEncounters[i] = createBeastEncounterFromData(beastiary->beasts[b],
                                                                                     data->beasts[i]);
                scene->encounters->beastEncountersCount++;
                addDebug("scene %s encounter -- %s, max %d",
                         scene->name,
                         scene->encounters->beastEncounters[i]->beast->id,
                         scene->encounters->beastEncounters[i]->max);
                break;
            }
        }
        if (scene->encounters->beastEncounters[i] == NULL) {
            addError("unable to find beast from id :: %s", data->beasts[i].id);
            exit(ConfigurationErrorUnknownBeast);
        }
    }
    addDebug("done loading encounters for scene :: scene: %s, beasts: %d",
            scene->name,
            scene->encounters->beastEncountersCount);
}

void loadStorylines(Scene *s, const char *sceneDirectory) {
    char storylinesDirectory[MAX_FS_PATH_LENGTH];
    sprintf(storylinesDirectory, "%s/storylines", sceneDirectory);
    addDebug("storylines directory :: %s", storylinesDirectory);
    if (access(storylinesDirectory, F_OK) != 0) {
        addDebug("scene has no storylines :: %s", s->name);
        return;
    }
    char **storylineFiles = calloc(MAX_FILES, sizeof(char *));
    int fileCount = getFilesInDirectory(storylinesDirectory, storylineFiles);
    addDebug("storyline files found :: %d", fileCount);
    int count = 0;
    for (int i = 0; i < fileCount; i++) {
        char storylinesFilePath[MAX_FS_PATH_LENGTH];
        sprintf(storylinesFilePath, "%s/storylines/%s", sceneDirectory, storylineFiles[i]);
        addDebug("storyline file path :: %s", storylinesFilePath);
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
    addDebug("added storylines to game :: %d", count);
}

Scene *loadScene(
        MobileManager *mm,
        ItemManager *im,
        const Beastiary *beastiary,
        char *sceneName,
        const char *sceneDirectory) {
    addDebug("create scene :: %s", sceneName);
    const SceneData *sceneData = loadSceneYaml(sceneDirectory);
    Scene *scene = createScene(
            sceneData->id,
            sceneName,
            getSceneTypeFromString(sceneData->type),
            sceneData->music);

    // storylines
    loadStorylines(scene, sceneDirectory);

    // create scene reader for reading tiled xml
    char mapDirectory[MAX_FS_PATH_LENGTH];
    sprintf(mapDirectory, "%s/map", sceneDirectory);
    char tilemapFilePath[MAX_FS_PATH_LENGTH];
    sprintf(tilemapFilePath, "%s/tilemap.tmx", mapDirectory);
    addDebug("create scene '%s' tilemap", sceneName);
    scene->map = parseTilemapDocToMap(im, scene->id, tilemapFilePath, mapDirectory);

    // load mobiles
    loadMobiles(mm, scene, sceneDirectory);

    if (sceneData->encounters != NULL) {
        loadEncounters(beastiary, scene, sceneData->encounters, config->indexDir);
    }

    addDebug("done parsing scene %s", sceneName);

    return scene;
}

void validateNoDuplicateSceneIds(SceneManager *sm) {
    for (int i = 0; i < sm->count; i++) {
        for (int j = i + 1; j < sm->count; j++) {
            if (sm->scenes[i]->id == sm->scenes[j]->id) {
                addError("duplicate scene IDs defined :: id: %d, first: %s, second: %s",
                         sm->scenes[i]->id,
                         sm->scenes[i]->name,
                         sm->scenes[j]->name);
                exit(ConfigurationErrorSceneIdCollision);
            }
        }
    }
}

void mapStorylinesToControlBlocks(ControlManager *cm, Scene *s) {
    addDebug("map storylines to control blocks :: scene name: %s, storyline count: %d",
             s->name, s->storylineCount);
    for (int c = 0; c < s->storylineCount; c++) {
        s->controlBlocks[c] = mapStorylineToControlBlock(cm, s, s->storylines[c]);
        s->controlBlockCount++;
    }
}

void loadScenes(
        SceneManager *sm,
        MobileManager *mm,
        ItemManager *im,
        Beastiary *beastiary,
        SceneLoader *sl) {
    addDebug("attempting to load scenes");
    for (int i = 0; i < sm->count; i++) {
        Scene *s = loadScene(
                mm,
                im,
                beastiary,
                sl->scenes[i],
                sl->sceneFiles[i]);
        mapStorylinesToControlBlocks(sm->controlManager, s);
        sm->scenes[i] = s;
        addDebug("scene loaded :: %s (%d)", sm->scenes[i]->name, i);
    }
    validateNoDuplicateSceneIds(sm);
}

void loadScenesFromFiles(
        SceneManager *sm,
        MobileManager *mm,
        ItemManager *im,
        Beastiary *beastiary) {
    SceneLoader *sl = createSceneLoader(config->indexDir);
    addDebug("get scene directories :: %s", sl->sceneDirectory);
    sl->count = getFilesInDirectory(sl->sceneDirectory, sl->scenes);
    buildSceneFilesList(sl);
    sm->count = addSubsceneFiles(sl);
    addDebug("scene loader found scenes :: %d", sm->count);
    for (int i = 0; i < sm->count; i++) {
        addDebug("scene :: %s (%s)", sl->scenes[i], sl->sceneFiles[i]);
    }
    loadScenes(
            sm,
            mm,
            im,
            beastiary,
            sl);
    free(sl);
}

AudioManager *loadAudioManager() {
    addDebug("load audio manager from dir '%s'", config->indexDir);
    AudioManager *am = createAudioManager();
    assignAudioManagerValues(am);
    addDebug("audio manager loaded %d songs", am->musicCount);
    return am;
}

SpritesheetManager *loadSpritesheetManager() {
    addDebug("load spritesheet manager :: %s", config->indexDir);
    Spritesheet *spritesheets[MAX_SPRITESHEETS];
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/spritesheets", config->indexDir);
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
            addDebug("spritesheet :: %s, %s", data->name, imageFilePath);
            Rectangle *collision = malloc(sizeof(Rectangle));
            if (data->collide != NULL) {
                collision->width = (float) data->collide->width;
                collision->height = (float) data->collide->height;
                collision->x = (float) data->collide->left;
                collision->y = (float) data->collide->top;
            }
            spritesheets[count] = createSpriteSheet(
                    data->name,
                    imageFilePath,
                    data->frame->width,
                    data->frame->height,
                    data->frame->padding,
                    collision);
            count++;
            free(data);
        }
    }
    free(files);
    addDebug("final spritesheet count :: %d", count);
    return createSpriteSheetManager(spritesheets, count);
}

void loadAllItems(ItemManager *itemManager) {
    const ItemsData *itemsData = loadItemYaml();
    itemManager->items = calloc(itemsData->items_count, sizeof(ItemData));
    for (int i = 0; i < itemsData->items_count; i++) {
        itemManager->items[i] = createItemFromData(&itemsData->items[i]);
    }
    itemManager->count = itemsData->items_count;
}

void loadAllAnimations(AnimationManager *am, SpritesheetManager *sm) {
    char animationsDir[MAX_FS_PATH_LENGTH / 2];
    sprintf(animationsDir, "%s/animations", config->indexDir);
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

Beastiary *loadBeastiary() {
    BeastiaryData *data = loadBeastiaryYaml();
    Beastiary *beastiary = malloc(sizeof(Beastiary));
    beastiary->count = data->beasts_count;
    for (int i = 0; i < data->beasts_count; i++) {
        beastiary->beasts[i] = createBeastFromData(&data->beasts[i]);
        addDebug("beast '%s' created", beastiary->beasts[i]->id);
    }
    free(data);
    return beastiary;
}

SpellManager *loadSpellManager() {
    SpellsData *data = loadSpellData();
    Spell **spells = calloc(MAX_SPELLS, sizeof(Spell));
    for (int i = 0; i < data->spells_count; i++) {
        spells[i] = createSpellFromData(data->spells[i]);
        addDebug("spell created :: %s", data->spells[i].type);
    }
    SpellManager *sm = createSpellManager(spells, data->spells_count);
    free(data);
    return sm;
}
