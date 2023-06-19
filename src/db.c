#define MAX_DATA_SIZE 2048

typedef struct {
    xmlTextReaderPtr reader;
    Exploration *exploration;
    int objectCount;
    int nextExit;
} SceneReader;

SceneReader *createSceneReader(Exploration *exploration, const char *sceneFile) {
    addDebug(exploration->log, "attempting to load scene file '%s'", sceneFile);
    SceneReader *sceneReader = malloc(sizeof(SceneReader));
    sceneReader->exploration = exploration;
    sceneReader->reader = xmlReaderForFile(sceneFile, NULL, 0);
    sceneReader->objectCount = 0;
    return sceneReader;
}

int getIntAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return strToInt((char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute));
}

float getFloatAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return atoll((char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute));
}

char *getStringAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return (char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute);
}

static void processTilemapNode(SceneReader *sceneReader, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(sceneReader->reader);
    static int tileOpen = 0;
    char *strName = (char *)name;
    if (strcmp(strName, "tileset") == 0) {
        const int width = getIntAttribute(sceneReader->reader, "tilewidth");
        const int height = getIntAttribute(sceneReader->reader, "tilewidth");
        sceneReader->exploration->tilemap->size = (Vector2d){ width, height };
    } else if (strcmp(strName, "image") == 0) {
        char filePath[255];
        sprintf(filePath, "%s/%s", indexDir, getStringAttribute(sceneReader->reader, "source"));
        sceneReader->exploration->tilemap->source = LoadImage(filePath);
    } else if (strcmp(strName, "tile") == 0) {
        if (tileOpen == 1) {
            tileOpen = 0;
            return;
        }
        tileOpen = 1;
        Object *o = createObject();
        o->tile = getIntAttribute(sceneReader->reader, "id");
        sceneReader->exploration->objects[sceneReader->objectCount] = o;
        sceneReader->objectCount++;
    } else if (strcmp(strName, "object") == 0) {
        int x = getIntAttribute(sceneReader->reader, "x");
        int y = getIntAttribute(sceneReader->reader, "y");
        int width = getIntAttribute(sceneReader->reader, "width");
        int height = getIntAttribute(sceneReader->reader, "height");
        Rectangle rect = {(float)x, (float)y, (float)width, (float)height};
        sceneReader->exploration->objects[sceneReader->objectCount - 1]->rect = rect;
        sceneReader->exploration->objects[sceneReader->objectCount - 1]->id = sceneReader->objectCount;
    }
}

void parseTilemapXml(Exploration *e, const char *indexDir, const char *filename) {
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    e->tilemap = tilemap;
    int ret;
    char filePath[255];
    sprintf(filePath, "%s/%s", indexDir, filename);
    SceneReader *sceneReader = createSceneReader(e, filePath);
    if (sceneReader->reader == NULL) {
        addError(e->log, "unable to open file: %s", filename);
        return;
    }
    ret = xmlTextReaderRead(sceneReader->reader);
    while (ret == 1) {
        processTilemapNode(sceneReader, indexDir);
        ret = xmlTextReaderRead(sceneReader->reader);
    }
    addDebug(e->log, "found %d objects", sceneReader->objectCount);
    xmlFreeTextReader(sceneReader->reader);
    if (ret != 0) {
        addError(e->log, "failed to parse file: %s", filename);
    }
}

void parseSceneLayer(Exploration *e, char *rawData) {
    addDebug(e->log, "scene layer %d processing now", e->layerCount - 1);
    char *line = strtok(rawData, "\r\n");
    char *data[MAX_DATA_SIZE];
    int it = 0;
    while (line != NULL && it < MAX_DATA_SIZE) {
        data[it] = line;
        line = strtok(NULL, "\r\n");
        it++;
    }
    int y = 0, x;
    while (y < it) {
        char *val = strtok(data[y], ",");
        x = 0;
        while (val != NULL) {
            e->layers[e->layerCount - 1]->data[y][x] = strToInt(val);
            val = strtok(NULL, ",");
            x++;
        }
        y++;
    }
}

void processSceneNode(SceneReader *sceneReader, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(sceneReader->reader);
    static int dataOpen = 0, exitOpen = 0, layerOpen = 0;
    char *strName = (char *)name;
    if (strcmp(strName, "tileset") == 0) {
        char *source = getStringAttribute(sceneReader->reader, "source");
        parseTilemapXml(sceneReader->exploration, indexDir, source);
    } else if (strcmp(strName, "layer") == 0) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        Layer *layer = malloc(sizeof(Layer));
        layer->type = -1;
        char *layerName = getStringAttribute(sceneReader->reader, "name");
        if (strcmp(layerName, "background") == 0) layer->type = BACKGROUND;
        else if (strcmp(layerName, "midground") == 0) layer->type = MIDGROUND;
        else if (strcmp(layerName, "foreground") == 0) layer->type = FOREGROUND;
        else addError(sceneReader->exploration->log, "unknown layer: %s", layerName);
        sceneReader->exploration->layers[sceneReader->exploration->layerCount] = layer;
    } else if (strcmp(strName, "data") == 0) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(sceneReader->reader);
        sceneReader->exploration->layerCount++;
        parseSceneLayer(sceneReader->exploration, (char *)data);
    } else if (strcmp(strName, "object") == 0) {
        char *class = getStringAttribute(sceneReader->reader, "type");
        if (class == NULL) {
            return;
        } else if (strcmp(class, "entrance") == 0) {
            sceneReader->exploration->entrance = (Rectangle){
                    getFloatAttribute(sceneReader->reader, "x"),
                    getFloatAttribute(sceneReader->reader, "y"),
                    getFloatAttribute(sceneReader->reader, "width"),
                    getFloatAttribute(sceneReader->reader, "height")
            };
        } else if (strcmp(class, "exit") == 0) {
            if (exitOpen) {
                exitOpen = 0;
                return;
            }
            exitOpen = 1;
            sceneReader->exploration->exits[sceneReader->nextExit] = createExit();
            sceneReader->exploration->exits[sceneReader->nextExit]->area = (Rectangle){
                    getFloatAttribute(sceneReader->reader, "x"),
                    getFloatAttribute(sceneReader->reader, "y"),
                    getFloatAttribute(sceneReader->reader, "width"),
                    getFloatAttribute(sceneReader->reader, "height")
            };
            sceneReader->nextExit += 1;
        }
    } else if (strcmp(strName, "property") == 0) {
        char *propName = getStringAttribute(sceneReader->reader, "name");
        if (strcmp(propName, "to") == 0) {
            sceneReader->exploration->exits[sceneReader->nextExit - 1]->to = getStringAttribute(sceneReader->reader, "value");
        } else if (strcmp(propName, "x") == 0) {
            sceneReader->exploration->exits[sceneReader->nextExit - 1]->x = getIntAttribute(sceneReader->reader, "value");
        } else if (strcmp(propName, "y") == 0) {
            sceneReader->exploration->exits[sceneReader->nextExit - 1]->y = getIntAttribute(sceneReader->reader, "value");
        }
    }
}

void parseSceneXml(SceneReader *sceneReader, const char *indexDir) {
    int ret;
    if (sceneReader->reader == NULL) {
        addError(sceneReader->exploration->log, "unable to find file for scene");
        return;
    }
    ret = xmlTextReaderRead(sceneReader->reader);
    while (ret == 1) {
        processSceneNode(sceneReader, indexDir);
        ret = xmlTextReaderRead(sceneReader->reader);
    }
    xmlFreeTextReader(sceneReader->reader);
    if (ret != 0) {
        addError(sceneReader->exploration->log,"failed to read scene");
    }
}

void loadAnimations(Log *log, const char *file, const char *indexDir, Animation *animations[MAX_ANIMATIONS]) {
    addDebug(log, "load animations file: %s", file);
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

BeastEncounter *mapBeastEncounterFromData(Beast *beast, BeastEncounterData data) {
    BeastEncounter *beastEncounter = malloc(sizeof(BeastEncounter));
    beastEncounter->beast = beast;
    beastEncounter->max = data.max;
    return beastEncounter;
}

Mobile *mapMobileFromData(Log *log, MobileData *data, const char *indexDir) {
    Mobile *mob = createMobile();
    mob->id = &data->id[0];
    mob->name = &data->name[0];
    mob->direction = getDirectionFromString(data->direction);
    mob->position.x = (float) data->position[0];
    mob->position.y = (float) data->position[1];
    char filePath[255];
    sprintf(filePath, "%s/%s", indexDir, data->animations);
    loadAnimations(log, filePath, indexDir, mob->animations);
    return mob;
}

void loadMobiles(Scene *scene, const char *indexDir) {
    char directory[255];
    sprintf(directory, "%s/scenes/%s/mobiles", indexDir, scene->name);
    addDebug(scene->log, "load mobiles from %s", directory);
    if (!FileExists(directory)) {
        addError(scene->log, "file does not exist, skipping mob loading");
        return;
    }
    char *mobFiles[MAX_MOBILES];
    int files = getFilesInDirectory(directory, mobFiles);
    for (int i = 0; i < files; i++) {
        char filePath[255];
        sprintf(filePath, "%s/%s", directory, mobFiles[i]);
        MobileData *mobData = loadMobYaml(filePath);
        Mobile *mob = mapMobileFromData(scene->log, mobData, indexDir);
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
                scene->encounters->beastEncounters[i] = mapBeastEncounterFromData(beastiary->beasts[b], data->beasts[i]);
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
    addDebug(scene->log, "done loading encounters for scene %s with beast count %d",
           scene->name,
           scene->encounters->beastEncountersCount);
}

Scene *loadScene(Log *log, Beastiary *beastiary, const char *indexDir, char *sceneName, int showCollisions) {
    addDebug(log, "create scene '%s'", sceneName);
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
    sprintf(sceneDir, "%s/scenes/%s", indexDir, sceneName);

    char sceneFilePath[255];
    sprintf(sceneFilePath, "%s/tilemap.tmx", sceneDir);
    SceneReader *sceneReader = createSceneReader(scene->exploration, sceneFilePath);
    addDebug(scene->log, "create scene '%s' tilemap", sceneName);
    parseSceneXml(sceneReader, sceneDir);

    // load mobiles
    loadMobiles(scene, indexDir);

    if (sceneData->encounters != NULL) {
        loadEncounters(beastiary, scene, sceneData->encounters, indexDir);
    }

    free(sceneReader);
    addDebug(scene->log, "done parsing scene %s", sceneName);

    return scene;
}

Player *loadPlayer(Log *log, char *indexDir) {
    addDebug(log, "loading player from dir %s", indexDir);
    Player *player = createPlayer(log);
    player->mob = createMobile();
    PlayerData *playerYaml = loadPlayerYaml(log, indexDir);
    player->mob->name = playerYaml->name;
    player->mob->direction = getDirectionFromString(playerYaml->direction);
    char filePath[255];
    sprintf(filePath, "%s/%s", indexDir, playerYaml->animations);
    loadAnimations(log, filePath, indexDir, player->mob->animations);
    player->mob->position.x = (float) playerYaml->position[0];
    player->mob->position.y = (float) playerYaml->position[1];
    return player;
}

AudioManager *loadAudioManager(Log *log, char *indexDir) {
    addDebug(log, "loading audio manager from dir '%s'", indexDir);
    AudioManager *am = createAudioManager(log);
    assignAudioManagerValues(am, indexDir);
    addDebug(log, "audio manager loaded %d songs", am->musicCount);
    return am;

}
