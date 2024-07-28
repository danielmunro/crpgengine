#include <raylib.h>
#include <mm_malloc.h>
#include <string.h>
#include "headers/util/log.h"
#include "headers/fight.h"
#include "headers/control_manager.h"
#include "headers/mobile_manager.h"
#include "headers/shop.h"
#include "headers/persistence/db.h"
#include "headers/persistence/xmlparser.h"
#include "headers/graphics/spritesheet_manager.h"
#include "headers/persistence/cyaml.h"

void loadAnimations(AnimationManager *am, const SpritesheetManager *sm, const char *file) {
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
        int k = i + am->libraryCount;
        am->library[k] = createAnimation(
                animation->name,
                getAnimationTypeFromString(s->name),
                sp,
                s->frameStart,
                s->frameEnd,
                s->rate,
                s->repeat,
                getDisplayFromString(s->display),
                getPositionFromString(s->position),
                getPlayUntilFromString(s->playUntil)
        );
    }
    am->libraryCount += animation->slices_count;
    addDebug("%d animations loaded", animation->slices_count);
    free(animation);
}

void loadMobiles(MobileManager *mm, Scene *scene, const char *sceneDirectory) {
    char directory[MAX_FS_PATH_LENGTH] = "";
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
        sprintf(filePath, "%s/%s", directory, mobFiles[i]);
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
    sprintf(directory, "%s/player/mobiles", mm->context->indexDir);
    addInfo("load player mobiles from :: %s", directory);
    if (!FileExists(directory)) {
        addError("player mobiles directory does not exist :: %s", directory);
        exit(ConfigurationErrorPlayerMobilesDirectoryDoesNotExist);
    }
    char **mobFiles = calloc(MAX_FILES, sizeof(char *));
    int files = getFilesInDirectory(directory, mobFiles);
    for (int i = 0; i < files; i++) {
        char filePath[MAX_FS_PATH_LENGTH];
        sprintf(filePath, "%s/%s", directory, &mobFiles[i][0]);
        addDebug("loading player mobile :: %s", filePath);
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
    char *filePath = malloc(MAX_FS_PATH_LENGTH);
    sprintf(filePath, "%s/images/%s", indexDir, data->background);
    scene->encounters->backgroundFilePath = filePath;
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

void loadShops(ItemManager *im, Scene *s, const char *sceneDirectory) {
    char shopsDirectory[MAX_FS_PATH_LENGTH];
    sprintf(shopsDirectory, "%s/shops", sceneDirectory);
    if (access(shopsDirectory, F_OK) != 0) {
        addDebug("scene has no shops :: %s", s->name);
        return;
    }
    char **shopFiles = calloc(MAX_FILES, sizeof(char *));
    int fileCount = getFilesInDirectory(shopsDirectory, shopFiles);
    for (int f = 0; f < fileCount; f++) {
        char shopFilePath[MAX_FS_PATH_LENGTH];
        sprintf(shopFilePath, "%s/shops/%s", sceneDirectory, shopFiles[f]);
        const ShopData *data = loadShopYaml(shopFilePath);
        if (data == NULL) {
            addError("shop data is null :: %s", shopFilePath);
            exit(ConfigurationErrorBadShopData);
        }
        ItemWithMarkup **items = calloc(MAX_ITEMS, sizeof(ItemWithMarkup));
        for (int i = 0; i < data->items_count; i++) {
            items[i] = createItemWithMarkup(
                    findItemFromName(im, data->items[i].item),
                    data->items[i].cost);
        }
        s->shops[f] = createShop(
                data->id,
                items,
                data->items_count,
                data->coins,
                createMessagesFromData(data->messages));
        s->shopsCount++;
        free((ShopData *) data);
    }
    free(shopFiles);
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

void mapStorylinesToControlBlocks(ControlManager *cm, Scene *s) {
    addDebug("map storylines to control blocks :: scene name: %s, storyline count: %d",
             s->name, s->storylineCount);
    for (int c = 0; c < s->storylineCount; c++) {
        s->controlBlocks[c] = mapStorylineToControlBlock(cm, s, s->storylines[c]);
        s->controlBlockCount++;
    }
}

Scene *loadScene(
        MobileManager *mm,
        ItemManager *im,
        ControlManager *cm,
        const Beastiary *beastiary,
        Context *c,
        const char *sceneName,
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

    // shops
    loadShops(im, scene, sceneDirectory);

    // create scene reader for reading tiled xml
    char *mapDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf(mapDirectory, "%s/map", sceneDirectory);
    char *tilemapFilePath = malloc(MAX_FS_PATH_LENGTH);
    sprintf(tilemapFilePath, "%s/tilemap.tmx", mapDirectory);
    addDebug("parse scene tilemap :: %s", sceneName);
    scene->map = parseTilemapDocToMap(
            im,
            c,
            scene->id,
            tilemapFilePath,
            mapDirectory);

    // load mobiles
    loadMobiles(mm, scene, sceneDirectory);

    if (sceneData->encounters != NULL) {
        loadEncounters(beastiary, scene, sceneData->encounters, mm->context->indexDir);
    }

    mapStorylinesToControlBlocks(cm, scene);
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

void loadScenes(
        SceneManager *sm,
        MobileManager *mm,
        ItemManager *im,
        Beastiary *beastiary,
        SceneLoader *sl) {
    addDebug("attempting to load scenes");
    for (int i = 0; i < sl->count; i++) {
        sm->scenes[i] = loadScene(
                mm,
                im,
                sm->controlManager,
                beastiary,
                sm->context,
                sl->scenes[i],
                sl->sceneFiles[i]);
        addDebug("scene loaded :: %s (%d)", sm->scenes[i]->name, i);
    }
    sm->count = sl->count;
    if (sm->context->game->validate) {
        validateNoDuplicateSceneIds(sm);
    }
}

void loadScenesFromFiles(
        SceneManager *sm,
        MobileManager *mm,
        ItemManager *im,
        Beastiary *beastiary) {
    SceneLoader *sl = createSceneLoader(sm->context->indexDir);
    loadScenes(
            sm,
            mm,
            im,
            beastiary,
            sl);
    free(sl);
}

AudioManager *loadAudioManager(Context *c) {
    addDebug("load audio manager from dir '%s'", c->indexDir);
    AudioManager *am = createAudioManager(c);
    assignAudioManagerValues(am);
    addDebug("audio manager loaded %d songs", am->musicCount);
    return am;
}

SpritesheetManager *loadSpritesheetManager(Context *c) {
    addDebug("load spritesheet manager :: %s", c->indexDir);
    Spritesheet *spritesheets[MAX_SPRITESHEETS];
    char directory[MAX_FS_PATH_LENGTH];
    sprintf(directory, "%s/spritesheets", c->indexDir);
    char **files = calloc(MAX_FILES, sizeof(char *));
    int filesInDirectory = getFilesInDirectory(directory, files);
    int count = 0;
    for (int i = 0; i < filesInDirectory; i++) {
        if (strcmp(getFilenameExt(&files[i][0]), "yaml") == 0) {
            char dataFilePath[MAX_FS_PATH_LENGTH];
            sprintf(dataFilePath, "%s/%s", directory, files[i]);
            addInfo("spritesheet data :: %s", dataFilePath);
            SpritesheetData *data = loadSpritesheetYaml(dataFilePath);
            char imageFilePath[MAX_FS_PATH_LENGTH];
            sprintf(imageFilePath, "%s/images/sprites/%s", c->indexDir, data->filename);
            if (!FileExists(imageFilePath)) {
                addError("sprite does not exist at path :: %s, %s", data->name, imageFilePath);
            }
            addInfo("spritesheet image :: %s, %s", data->name, imageFilePath);
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
                    collision,
                    c->ui->screen->scale);
            count++;
            free(data);
        }
    }
    free(files);
    addDebug("final spritesheet count :: %d", count);
    return createSpriteSheetManager(spritesheets, count, c);
}

void loadAllItems(ItemManager *itemManager) {
    const ItemsData *itemsData = loadItemYaml(itemManager->context->indexDir);
    itemManager->items = calloc(itemsData->items_count, sizeof(ItemData));
    for (int i = 0; i < itemsData->items_count; i++) {
        itemManager->items[i] = createItemFromData(&itemsData->items[i]);
    }
    itemManager->count = itemsData->items_count;
}

void loadAllAnimations(AnimationManager *am, const SpritesheetManager *sm) {
    char animationsDir[MAX_FS_PATH_LENGTH / 2];
    sprintf(animationsDir, "%s/animations", am->context->indexDir);
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

Beastiary *loadBeastiary(const char *indexDir) {
    BeastiaryData *data = loadBeastiaryYaml(indexDir);
    Beastiary *beastiary = malloc(sizeof(Beastiary));
    beastiary->count = data->beasts_count;
    for (int i = 0; i < data->beasts_count; i++) {
        beastiary->beasts[i] = createBeastFromData(&data->beasts[i], indexDir);
        addDebug("beast '%s' created", beastiary->beasts[i]->id);
    }
    free(data);
    return beastiary;
}

SpellManager *loadSpellManager(const char *indexDir) {
    SpellsData *data = loadSpellData(indexDir);
    Spell **spells = calloc(MAX_SPELLS, sizeof(Spell));
    for (int i = 0; i < data->spells_count; i++) {
        spells[i] = createSpellFromData(data->spells[i]);
        addDebug("spell created :: %s", data->spells[i].type);
    }
    SpellManager *sm = createSpellManager(spells, data->spells_count);
    free(data);
    return sm;
}
