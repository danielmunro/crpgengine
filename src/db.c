#define MAX_DATA_SIZE 2048

int getIntAttribute(xmlTextReaderPtr reader, char *attribute) {
    return atoi((char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute));
}

float getFloatAttribute(xmlTextReaderPtr reader, char *attribute) {
    return atoll((char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute));
}

char *getStringAttribute(xmlTextReaderPtr reader, char *attribute) {
    return (char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute);
}

static void processTilemapNode(Scene *s, xmlTextReaderPtr reader) {
    const xmlChar *name = xmlTextReaderConstName(reader);
    static int tileOpen = 0;
    static int objects = 0;
    char *strName = (char *)name;
    if (strcmp(strName, "tileset") == 0) {
        const int width = getIntAttribute(reader, "tilewidth");
        const int height = getIntAttribute(reader, "tilewidth");
        s->tilemap->size = (Vector2d){width, height };
    } else if (strcmp(strName, "image") == 0) {
        char source[255] = "./resources/tiled/";
        strcat(source, getStringAttribute(reader, "source"));
        s->tilemap->source = LoadTexture(source);
    } else if (strcmp(strName, "tile") == 0) {
        if (tileOpen == 1) {
            tileOpen = 0;
            return;
        }
        tileOpen = 1;
        Object *o = malloc(sizeof(Object));
        o->tile = getIntAttribute(reader, "id");
        s->objects[objects] = o;
        objects++;
    } else if (strcmp(strName, "object") == 0) {
        int id = getIntAttribute(reader, "x");
        int x = getIntAttribute(reader, "x");
        int y = getIntAttribute(reader, "y");
        int width = getIntAttribute(reader, "width");
        int height = getIntAttribute(reader, "height");
        Rectangle rect = {(float)x, (float)y, (float)width, (float)height};
        s->objects[objects - 1]->rect = rect;
        s->objects[objects - 1]->id = id;
    }
}

void *parseTilemapXml(Scene *s, const char *filename) {
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    s->tilemap = tilemap;
    xmlTextReaderPtr reader;
    int ret;
    char source[255] = "./resources/tiled/";
    strcat(source, filename);
    reader = xmlReaderForFile(source, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processTilemapNode(s, reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            fprintf(stderr, "%s : failed to parse\n", filename);
        }
    } else {
        fprintf(stderr, "Unable to open %s\n", filename);
    }
}

void parseSceneLayer(Scene *s, char *rawData) {
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
            s->layers[s->layerCount - 1]->data[y][x] = atoi(val);
            val = strtok(NULL, ",");
            x++;
        }
        y++;
    }
    printf("done processing scene layer\n");
}

void processSceneNode(Scene *s, xmlTextReaderPtr reader) {
    const xmlChar *name = xmlTextReaderConstName(reader);
    static int dataOpen = 0, exitOpen = 0, layerOpen = 0;
    char *strName = (char *)name;
    if (strcmp(strName, "tileset") == 0) {
        char *source = getStringAttribute(reader, "source");
        parseTilemapXml(s, source);
    } else if (strcmp(strName, "layer") == 0) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        s->layers[s->layerCount] = malloc(sizeof(Layer));
        char *layerName = getStringAttribute(reader, "name");
        if (strcmp(layerName, "background") == 0) s->layers[s->layerCount]->type = LAYER_TYPE_BACKGROUND;
        else if (strcmp(layerName, "midground") == 0) s->layers[s->layerCount]->type = LAYER_TYPE_MIDGROUND;
        else if (strcmp(layerName, "foreground") == 0) s->layers[s->layerCount]->type = LAYER_TYPE_FOREGROUND;
        else printf("unknown layer: %s\n", layerName);
    } else if (strcmp(strName, "data") == 0) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(reader);
        s->layerCount++;
        parseSceneLayer(s, (char *)data);
    } else if (strcmp(strName, "object") == 0) {
        char *class = getStringAttribute(reader, "class");
        if (class == NULL) {
            return;
        } else if (strcmp(class, "entrance") == 0) {
            s->entrance = (Rectangle){
                    getFloatAttribute(reader, "x"),
                    getFloatAttribute(reader, "y"),
                    getFloatAttribute(reader, "width"),
                    getFloatAttribute(reader, "height")
            };
        } else if (strcmp(class, "exit") == 0) {
            if (exitOpen) {
                exitOpen = 0;
                return;
            }
            exitOpen = 1;
            s->exit = malloc(sizeof(Exit));
            s->exit->area = (Rectangle){
                    getFloatAttribute(reader, "x"),
                    getFloatAttribute(reader, "y"),
                    getFloatAttribute(reader, "width"),
                    getFloatAttribute(reader, "height")
            };
        }
    } else if (strcmp(strName, "property") == 0) {
        s->exit->to = getStringAttribute(reader, "value");
    }
}

void parseSceneXml(Scene *s, const char *filename) {
    xmlTextReaderPtr reader;
    int ret;
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processSceneNode(s, reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            fprintf(stderr, "%s : failed to parse\n", filename);
        }
    } else {
        fprintf(stderr, "Unable to open %s\n", filename);
    }
}

void assignSceneType(Scene *s, char *sceneType) {
    int i = 0, count = sizeof(sceneTypes) / sizeof(SceneType);
    while (i <= count) {
        if (strcmp(sceneTypes[i].scene, sceneType) == 0) {
            s->type = sceneTypes[i].code;
            break;
        }
        i++;
    }
}

Scene *loadScene(char *sceneName, int showCollisions) {
    printf("loading scene %s\n", sceneName);
    char *data = LoadFileText(sceneName);
    Scene *scene = malloc(sizeof(Scene));
    scene->layerCount = 0;
    scene->showCollisions = showCollisions;
    strcpy(scene->name, strtok(data, "\r\n"));
    char *sceneType = strtok(NULL, "\r\n");
    char *sceneFile = strtok(NULL, "\r\n");

    // create tilemap
    parseSceneXml(scene, sceneFile);

    // assign scene properties
    assignSceneType(scene, sceneType);
    printf("done parsing scene %s\n", sceneName);

    return scene;
}
