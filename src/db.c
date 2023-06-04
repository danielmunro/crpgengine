#include <dirent.h>

#define MAX_DATA_SIZE 2048

typedef struct SceneReader {
    xmlTextReaderPtr reader;
    Scene *scene;
    int objectCount;
} SceneReader;

SceneReader *createSceneReader(Scene *scene, const char *sceneFile) {
    SceneReader *sceneReader = malloc(sizeof(SceneReader));
    sceneReader->scene = scene;
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
        sceneReader->scene->tilemap->size = (Vector2d){ width, height };
    } else if (strcmp(strName, "image") == 0) {
        char *source = pathCat(pathCat(indexDir, "/"), getStringAttribute(sceneReader->reader, "source"));
        sceneReader->scene->tilemap->source = LoadImage(source);
    } else if (strcmp(strName, "tile") == 0) {
        if (tileOpen == 1) {
            tileOpen = 0;
            return;
        }
        tileOpen = 1;
        Object *o = malloc(sizeof(Object));
        o->tile = getIntAttribute(sceneReader->reader, "id");
        sceneReader->scene->objects[sceneReader->objectCount] = o;
        sceneReader->objectCount++;
    } else if (strcmp(strName, "object") == 0) {
        int x = getIntAttribute(sceneReader->reader, "x");
        int y = getIntAttribute(sceneReader->reader, "y");
        int width = getIntAttribute(sceneReader->reader, "width");
        int height = getIntAttribute(sceneReader->reader, "height");
        Rectangle rect = {(float)x, (float)y, (float)width, (float)height};
        sceneReader->scene->objects[sceneReader->objectCount - 1]->rect = rect;
        sceneReader->scene->objects[sceneReader->objectCount - 1]->id = sceneReader->objectCount;
    }
}

void parseTilemapXml(Scene *s, const char *indexDir, const char *filename) {
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    s->tilemap = tilemap;
    int ret;
    char *source = pathCat(pathCat(indexDir, "/"), filename);
    printf("parse xml tilemap %s\n", source);
    SceneReader *sceneReader = createSceneReader(s, source);
    if (sceneReader->reader != NULL) {
        ret = xmlTextReaderRead(sceneReader->reader);
        while (ret == 1) {
            processTilemapNode(sceneReader, indexDir);
            ret = xmlTextReaderRead(sceneReader->reader);
        }
        printf("found %d objects\n", sceneReader->objectCount);
        xmlFreeTextReader(sceneReader->reader);
        if (ret != 0) {
            fprintf(stderr, "%s : failed to parse\n", filename);
        }
    } else {
        fprintf(stderr, "Unable to open %s\n", filename);
    }
}

void parseSceneLayer(Scene *s, char *rawData) {
    printf("processing scene '%s' layer %d\n", s->name, s->layerCount - 1);
    char *line = strtok(rawData, "\r\n");
    char *data[MAX_DATA_SIZE];
    int it = 0;
    while (line != NULL && it < MAX_DATA_SIZE) {
        data[it] = line;
        line = strtok(NULL, "\r\n");
        it++;
    }
    int y = 0, x = 0;
    while (y < it) {
        char *val = strtok(data[y], ",");
        x = 0;
        while (val != NULL) {
            s->layers[s->layerCount - 1]->data[y][x] = strToInt(val);
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
        parseTilemapXml(sceneReader->scene, indexDir, source);
    } else if (strcmp(strName, "layer") == 0) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        Layer *layer = malloc(sizeof(Layer));
        char *layerName = getStringAttribute(sceneReader->reader, "name");
        if (strcmp(layerName, "background") == 0) layer->type = LAYER_TYPE_BACKGROUND;
        else if (strcmp(layerName, "midground") == 0) layer->type = LAYER_TYPE_MIDGROUND;
        else if (strcmp(layerName, "foreground") == 0) layer->type = LAYER_TYPE_FOREGROUND;
        else printf("unknown layer: %s\n", layerName);
        sceneReader->scene->layers[sceneReader->scene->layerCount] = layer;
    } else if (strcmp(strName, "data") == 0) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(sceneReader->reader);
        sceneReader->scene->layerCount++;
        parseSceneLayer(sceneReader->scene, (char *)data);
    } else if (strcmp(strName, "object") == 0) {
        char *class = getStringAttribute(sceneReader->reader, "type");
        if (class == NULL) {
            return;
        } else if (strcmp(class, "entrance") == 0) {
            sceneReader->scene->entrance = (Rectangle){
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
            sceneReader->scene->exits[sceneReader->scene->nextExit] = malloc(sizeof(Exit));
            sceneReader->scene->exits[sceneReader->scene->nextExit]->area = (Rectangle){
                    getFloatAttribute(sceneReader->reader, "x"),
                    getFloatAttribute(sceneReader->reader, "y"),
                    getFloatAttribute(sceneReader->reader, "width"),
                    getFloatAttribute(sceneReader->reader, "height")
            };
            sceneReader->scene->nextExit += 1;
        }
    } else if (strcmp(strName, "property") == 0) {
        char *propName = getStringAttribute(sceneReader->reader, "name");
        if (strcmp(propName, "to") == 0) {
            sceneReader->scene->exits[sceneReader->scene->nextExit - 1]->to = getStringAttribute(sceneReader->reader, "value");
        } else if (strcmp(propName, "x") == 0) {
            sceneReader->scene->exits[sceneReader->scene->nextExit - 1]->x = getIntAttribute(sceneReader->reader, "value");
        } else if (strcmp(propName, "y") == 0) {
            sceneReader->scene->exits[sceneReader->scene->nextExit - 1]->y = getIntAttribute(sceneReader->reader, "value");
        }
    }
}

void parseSceneXml(SceneReader *sceneReader, const char *indexDir) {
    int ret;
    if (sceneReader->reader != NULL) {
        ret = xmlTextReaderRead(sceneReader->reader);
        while (ret == 1) {
            processSceneNode(sceneReader, indexDir);
            ret = xmlTextReaderRead(sceneReader->reader);
        }
        xmlFreeTextReader(sceneReader->reader);
        if (ret != 0) {
            fprintf(stderr, "%s : failed to read scene\n", sceneReader->scene->name);
        }
    } else {
        fprintf(stderr, "Unable to file for %s scene\n", sceneReader->scene->name);
    }
}

void assignSceneType(Scene *s, const char *sceneType) {
    int i = 0, count = sizeof(sceneTypes) / sizeof(SceneType);
    while (i <= count) {
        if (strcmp(sceneTypes[i].scene, sceneType) == 0) {
            s->type = sceneTypes[i].code;
            break;
        }
        i++;
    }
}

int getFilesInDirectory(const char *dir, char *scenes[MAX_SCENES]) {
    struct dirent *de;
    DIR *dr = opendir(dir);
    if (dr == NULL) {
        fprintf(stderr, "Could not open scene index directory");
    }
    int i = 0;
    while (true) {
        de = readdir(dr);
        if (de == NULL) {
            break;
        }
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }
        scenes[i] = (char *) malloc(strlen(de->d_name));
        strcpy(scenes[i], de->d_name);
        printf("added scene %s\n", scenes[i]);
        i++;
    }
    closedir(dr);
    return i;
}

void loadAnimations(const char *file, const char *indexDir, Animation *animations[MAX_ANIMATIONS]) {
    AnimationYaml *animation = loadAnimationYaml(file);
    char *imagePath = pathCat(indexDir, pathCat("animations", animation->sprite->file));
    SpriteSheet *sp = createSpriteSheet(imagePath, animation->sprite->size[0], animation->sprite->size[1]);
    for (int i = 0; i < animation->slices_count; i++) {
        SliceYaml *s = &animation->slices[i];
        animations[i] = createAnimation(
                sp,
                getAnimIdFromName(s->name),
                s->frames[0],
                s->frames[1],
                s->rate,
                s->repeat
        );
    }
    printf("%d animations loaded\n", animation->slices_count);
}

void loadMobiles(Scene *scene, const char *indexDir) {
    char *mobDir = pathCat(indexDir, pathCat("/scenes/", pathCat(scene->name, "/mobiles")));
    printf("load mobiles from %s\n", mobDir);
    if (!FileExists(mobDir)) {
        printf("file does not exist, skipping mob loading\n");
        return;
    }
    char *mobFiles[MAX_MOBILES];
    int files = getFilesInDirectory(mobDir, mobFiles);
    for (int i = 0; i < files; i++) {
        char *mobDataFile = pathCat(mobDir, pathCat("/", mobFiles[i]));
        scene->mobiles[i] = createMobile();
        char *animationsFragment = assignMobValues(scene, scene->mobiles[i], mobDataFile);
        char *animationsFile = pathCat(pathCat(indexDir, "/"), animationsFragment);
        printf("load mobile animations\n");
        loadAnimations(animationsFile, indexDir, scene->mobiles[0]->animations);
        printf("done load mobile animations\n");
    }
}

Scene *loadScene(const char *indexDir, const char *sceneName, int showCollisions) {
    printf("parse scene '%s' index\n", sceneName);
    char *indexFile = pathCat(pathCat(pathCat(indexDir, "/scenes"), sceneName), "/scene.txt");
    char *data = LoadFileText(indexFile);
    Scene *scene = createScene();
    scene->showCollisions = showCollisions;
    strcpy(scene->name, sceneName);
    char *kvpairs[255];
    int pairs = parseKVPairs(data, kvpairs);
    for (int i = 0; i < pairs; i+=2) {
        if (strcmp(kvpairs[i], SCENE_ATTRIBUTE_TYPE) == 0) {
            assignSceneType(scene, kvpairs[i + 1]);
        } else if (strcmp(kvpairs[i], "music") == 0) {
            printf("setting scene music to '%s'\n", kvpairs[i + 1]);
            scene->music = &kvpairs[i + 1][0];
            printf("scene->music debug: %s\n", scene->music);
        }
    }
    char *sceneFile = pathCat(pathCat(pathCat(indexDir, "/scenes"), sceneName), "/tilemap.tmx");
    SceneReader *sceneReader = createSceneReader(scene, sceneFile);

    // create tilemap
    printf("create scene '%s' tilemap\n", sceneName);
    char *sceneDir = pathCat(pathCat(indexDir, "/scenes"), sceneName);
    parseSceneXml(sceneReader, sceneDir);

    // load mobiles
    loadMobiles(scene, indexDir);

    free(sceneReader);
    printf("done parsing scene %s\n", sceneName);

    return scene;
}

Player *loadPlayer(char *indexDir) {
    printf("loading player from dir %s\n", indexDir);
    Player *player = createPlayer();
    player->mob = createMobile();
    PlayerYaml *playerYaml = loadPlayerYaml(indexDir);
    player->mob->name = playerYaml->name;
    player->mob->direction = getDirectionFromString(playerYaml->direction);
    char *animationsFile = pathCat(pathCat(indexDir, "/"), playerYaml->animations);
    loadAnimations(animationsFile, indexDir, player->mob->animations);
    player->mob->position.x = (float) playerYaml->position[0];
    player->mob->position.y = (float) playerYaml->position[1];
    return player;
}

AudioManager *loadAudioManager(char *indexDir) {
    printf("loading audio manager from dir %s\n", indexDir);
    AudioManager *am = createAudioManager();
    assignAudioManagerValues(am, indexDir);
    printf("audio manager loaded %d songs\n", am->musicCount);
    return am;

}
