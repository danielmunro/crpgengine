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
    static int tileOpen = 0, lastObjectId = 0;
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
        lastObjectId = getIntAttribute(sceneReader->reader, "id");
    } else if (strcmp(strName, "object") == 0) {
        Rectangle rect = {
                getFloatAttribute(sceneReader->reader, "x"),
                getFloatAttribute(sceneReader->reader, "y"),
                getFloatAttribute(sceneReader->reader, "width"),
                getFloatAttribute(sceneReader->reader, "height")};
        Object *o = createTileObject(lastObjectId, rect);
        sceneReader->exploration->objects[sceneReader->objectCount] = o;
        sceneReader->objectCount++;
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
    static int dataOpen = 0, exitOpen = 0, layerOpen = 0, objectType;
    char *strName = (char *)name;
    Log *log = sceneReader->exploration->log;
    addDebug(log, "process scene node -- %s", strName);
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
        else addError(log, "unknown layer: %s", layerName);
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
        char *type = getStringAttribute(sceneReader->reader, "type");
        addDebug(log, "object type: %s", type);
        if (type == NULL) {
            return;
        } else if (strcmp(type, "entrance") == 0) {
            Rectangle rect = {
                    getFloatAttribute(sceneReader->reader, "x"),
                    getFloatAttribute(sceneReader->reader, "y"),
                    getFloatAttribute(sceneReader->reader, "width"),
                    getFloatAttribute(sceneReader->reader, "height")
            };
            Entrance *e = createEntrance(getStringAttribute(sceneReader->reader, "name"), rect);
            sceneReader->exploration->entrances[sceneReader->exploration->entranceCount] = e;
            sceneReader->exploration->entranceCount++;
            objectType = ENTRANCE;
        } else if (strcmp(type, "exit") == 0) {
            Exploration *e = sceneReader->exploration;
            e->exits[e->exitCount] = createExit();
            e->exits[e->exitCount]->area = (Rectangle){
                    getFloatAttribute(sceneReader->reader, "x"),
                    getFloatAttribute(sceneReader->reader, "y"),
                    getFloatAttribute(sceneReader->reader, "width"),
                    getFloatAttribute(sceneReader->reader, "height")
            };
            e->exitCount += 1;
            objectType = EXIT;
        }
    } else if (strcmp(strName, "property") == 0) {
        Exploration *e = sceneReader->exploration;
        char *propName = getStringAttribute(sceneReader->reader, "name");
        if (objectType == EXIT) {
            if (strcmp(propName, "scene") == 0) {
                e->exits[e->exitCount - 1]->scene = getStringAttribute(sceneReader->reader, "value");
            } else if (strcmp(propName, "to") == 0) {
                e->exits[e->exitCount - 1]->to = getStringAttribute(sceneReader->reader, "value");
            }
        } else if (objectType == ENTRANCE) {
            if (strcmp(propName, "direction") == 0) {
                e->entrances[e->entranceCount - 1]->direction =
                        getDirectionFromString(getStringAttribute(sceneReader->reader, "value"));
            }
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