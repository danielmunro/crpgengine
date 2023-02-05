#define MAX_DATA_SIZE 2048

int getIntAttribute(xmlTextReaderPtr reader, char *attribute) {
    return atoi((char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute));
}

char *getStringAttribute(xmlTextReaderPtr reader, char *attribute) {
    return (char *)xmlTextReaderGetAttribute(reader, (const xmlChar*)attribute);
}

static void processTilemapNode(Tilemap *t, xmlTextReaderPtr reader) {
    const xmlChar *name = xmlTextReaderConstName(reader);
    static int objects = 0;
    char *strName = (char *)name;
    if (strcmp(strName, "tileset") == 0) {
        const int width = getIntAttribute(reader, "tilewidth");
        const int height = getIntAttribute(reader, "tilewidth");
        t->size = (Vector2D){ width, height };
    } else if (strcmp(strName, "image") == 0) {
        char source[255] = "./resources/tiled/";
        strcat(source, getStringAttribute(reader, "source"));
        t->source = LoadTexture(source);
    } else if (strcmp(strName, "tile") == 0) {
        Object *o = malloc(sizeof(Object));
        o->tile = getIntAttribute(reader, "id");
        t->objects[objects] = o;
        objects++;
    } else if (strcmp(strName, "object") == 0) {
        int x = getIntAttribute(reader, "x");
        int y = getIntAttribute(reader, "y");
        int width = getIntAttribute(reader, "width");
        int height = getIntAttribute(reader, "height");
        Rectangle rect = {(float)x, (float)y, (float)width, (float)height};
        t->objects[objects - 1]->rect = rect;
    }
}

static Tilemap *parseTilemapXml(const char *filename) {
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    xmlTextReaderPtr reader;
    int ret;
    char source[255] = "./resources/tiled/";
    strcat(source, filename);
    reader = xmlReaderForFile(source, NULL, 0);
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
            s->tilemap->layers[s->layers - 1][y][x] = atoi(val);
            val = strtok(NULL, ",");
            x++;
        }
        y++;
    }
    printf("done processing scene layer\n");
}

void processSceneNode(Scene *s, xmlTextReaderPtr reader) {
    const xmlChar *name = xmlTextReaderConstName(reader);
    static int dataOpen = 0;
    char *strName = (char *)name;
    if (strcmp(strName, "tileset") == 0) {
        char *source = getStringAttribute(reader, "source");
        s->tilemap = parseTilemapXml(source);
    } else if (strcmp(strName, "data") == 0) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(reader);
        s->layers++;
        parseSceneLayer(s, (char *)data);
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

Scene *loadScene(char *sceneName) {
    printf("loading scene %s\n", sceneName);
    char *data = LoadFileText(sceneName);
    Scene *scene = malloc(sizeof(Scene));
    scene->layers = 0;
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