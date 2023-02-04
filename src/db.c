#define MAX_DATA_SIZE 2048

int getIntVal(xmlTextReaderPtr reader, char *attribute) {
    return atoi((char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute));
}

char *getStringVal(xmlTextReaderPtr reader, char *attribute) {
    return (char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute);
}

static void processTilemapNode(Tilemap *t, xmlTextReaderPtr reader) {
    const xmlChar *name, *value;
    static int objects = 0;

    name = xmlTextReaderConstName(reader);
    if (name == NULL)
        name = BAD_CAST "--";

//    value = xmlTextReaderConstValue(reader);

//    printf("%d %d %s %d %d\n",
//           xmlTextReaderDepth(reader),
//           xmlTextReaderNodeType(reader),
//           name,
//           xmlTextReaderIsEmptyElement(reader),
//           xmlTextReaderHasValue(reader));
    char *strName = (char *)name;
    if (strcmp(strName, "tileset") == 0) {
        const int width = getIntVal(reader, "tilewidth");
        const int height = getIntVal(reader, "tilewidth");
        t->size = (Vector2D){ width, height };
    } else if (strcmp(strName, "image") == 0) {
        char source[255] = "./resources/tiled/";
        strcat(source, getStringVal(reader, "source"));
        t->source = LoadTexture(source);
    } else if (strcmp(strName, "tile") == 0) {
        Object *o = malloc(sizeof(Object));
        o->tile = getIntVal(reader, "id");
        t->objects[objects] = o;
        objects++;
    } else if (strcmp(strName, "object") == 0) {
        int x = getIntVal(reader, "x");
        int y = getIntVal(reader, "y");
        int width = getIntVal(reader, "width");
        int height = getIntVal(reader, "height");
        Rectangle rect = {(float)x, (float)y, (float)width, (float)height};
        t->objects[objects - 1]->rect = rect;
    }
}

static Tilemap *parseTilemapXml(const char *filename) {
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processTilemapNode(tilemap, reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            fprintf(stderr, "%s : failed to parse\n", filename);
        }
    } else {
        fprintf(stderr, "Unable to open %s\n", filename);
    }
    return tilemap;
}


void parseSceneLayer(Tilemap *t, char *layer, int i) {
    char *rawData = LoadFileText(layer);
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
            t->layers[i][y][x] = atoi(val);
            val = strtok(NULL, ",");
            x++;
        }
        y++;
    }
}

void parseTilemapLayers(Tilemap *t, char *layers[MAX_LAYER_COUNT], int layerCount) {
    int i = 0;
    while (i < layerCount) {
        printf("parsing scene layer %s\n", layers[i]);
        parseSceneLayer(t, layers[i], i);
        i++;
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

Scene *loadScene(char *sceneName) {
    printf("loading scene %s\n", sceneName);
    char *data = LoadFileText(sceneName);
    Scene *scene = malloc(sizeof(Scene));
    strcpy(scene->name, strtok(data, "\r\n"));
    char *sceneType = strtok(NULL, "\r\n");
    char *layers[MAX_LAYER_COUNT];
    int layerCount = 0;
    char *pch = strtok(NULL, "\r\n");
    while (pch != NULL) {
        layers[layerCount] = pch;
        printf("adding to layers: %s\n", pch);
        pch = strtok(NULL, "\r\n");
        layerCount++;
    }
    printf("layers found: %d\n", layerCount);

    // create tilemap
    Tilemap *tilemap = parseTilemapXml("./resources/tiled/tinytown.tsx");
    parseTilemapLayers(tilemap, layers, layerCount);

    // assign scene properties
    assignSceneType(scene, sceneType);
    scene->layers = layerCount;
    scene->tilemap = tilemap;
    printf("done parsing scene %s\n", sceneName);

    return scene;
}