int getIntAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return TextToInteger((char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute));
}

float getFloatAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return atoll((char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute));
}

char *getStringAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return (char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute);
}

static void processTilemapNode(TilemapXmlReader *tilemapXmlReader, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(tilemapXmlReader->reader);
    static int tileOpen = 0, lastObjectId = 0;
    char *strName = (char *) name;
    if (strcmp(strName, "tileset") == 0) {
        const int width = getIntAttribute(tilemapXmlReader->reader, "tilewidth");
        const int height = getIntAttribute(tilemapXmlReader->reader, "tilewidth");
        tilemapXmlReader->exploration->tilemap->size = (Vector2D) {width, height};
    } else if (strcmp(strName, "image") == 0) {
        char filePath[MAX_FS_PATH_LENGTH];
        sprintf(filePath, "%s/%s", indexDir, getStringAttribute(tilemapXmlReader->reader, "source"));
        tilemapXmlReader->exploration->tilemap->source = LoadImage(filePath);
    } else if (strcmp(strName, "tile") == 0) {
        if (tileOpen == 1) {
            tileOpen = 0;
            return;
        }
        tileOpen = 1;
        lastObjectId = getIntAttribute(tilemapXmlReader->reader, "id");
    } else if (strcmp(strName, "object") == 0) {
        Rectangle rect = {
                getFloatAttribute(tilemapXmlReader->reader, "x"),
                getFloatAttribute(tilemapXmlReader->reader, "y"),
                getFloatAttribute(tilemapXmlReader->reader, "width"),
                getFloatAttribute(tilemapXmlReader->reader, "height")};
        Object *o = createTileObject(lastObjectId, rect);
        addObject(tilemapXmlReader->exploration, o);
    }
}

void parseTilemapXml(Exploration *e, const char *indexDir, const char *filename) {
    addDebug("parsing xml tilemap at %s/%s", indexDir, filename);
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    e->tilemap = tilemap;
    int ret;
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/%s", indexDir, filename);
    TilemapXmlReader *tilemapXmlReader = createTilemapXmlReader(e, filePath);
    if (tilemapXmlReader->reader == NULL) {
        addError("unable to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
    ret = xmlTextReaderRead(tilemapXmlReader->reader);
    while (ret == 1) {
        processTilemapNode(tilemapXmlReader, indexDir);
        ret = xmlTextReaderRead(tilemapXmlReader->reader);
    }
    addDebug("found %d objects", tilemapXmlReader->exploration->objectCount);
    xmlFreeTextReader(tilemapXmlReader->reader);
    if (ret != 0) {
        addError("failed to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
}

void parseSceneLayer(Exploration *e, char *rawData) {
    addDebug("scene layer %d processing now", e->layerCount - 1);
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
            e->layers[e->layerCount - 1]->data[y][x] = TextToInteger(val);
            val = strtok(NULL, ",");
            x++;
        }
        y++;
    }
    e->layers[e->layerCount - 1]->width = x;
    e->layers[e->layerCount - 1]->height = y;
}

void processSceneNode(TilemapXmlReader *tilemapXmlReader, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(tilemapXmlReader->reader);
    static int dataOpen = 0, exitOpen = 0, layerOpen = 0, objectType;
    char *strName = (char *) name;
    addDebug("process scene node -- %s", strName);
    if (strcmp(strName, "tileset") == 0) {
        char *source = getStringAttribute(tilemapXmlReader->reader, "source");
        parseTilemapXml(tilemapXmlReader->exploration, indexDir, source);
    } else if (strcmp(strName, "layer") == 0) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        Layer *layer = createLayer();
        char *layerName = getStringAttribute(tilemapXmlReader->reader, "name");
        if (strcmp(layerName, "background") == 0) layer->type = BACKGROUND;
        else if (strcmp(layerName, "midground") == 0) layer->type = MIDGROUND;
        else if (strcmp(layerName, "foreground") == 0) layer->type = FOREGROUND;
        else {
            addError("unknown layer :: %s", layerName);
            exit(ConfigurationErrorUnknownLayer);
        }
        tilemapXmlReader->exploration->layers[tilemapXmlReader->exploration->layerCount] = layer;
    } else if (strcmp(strName, "data") == 0) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(tilemapXmlReader->reader);
        tilemapXmlReader->exploration->layerCount++;
        parseSceneLayer(tilemapXmlReader->exploration, (char *) data);
    } else if (strcmp(strName, "object") == 0) {
        char *type = getStringAttribute(tilemapXmlReader->reader, "type");
        addDebug("object type: %s", type);
        if (type == NULL) {
            return;
        } else if (strcmp(type, "entrance") == 0) {
            Rectangle rect = {
                    getFloatAttribute(tilemapXmlReader->reader, "x"),
                    getFloatAttribute(tilemapXmlReader->reader, "y"),
                    getFloatAttribute(tilemapXmlReader->reader, "width"),
                    getFloatAttribute(tilemapXmlReader->reader, "height")
            };
            Entrance *e = createEntrance(getStringAttribute(tilemapXmlReader->reader, "name"), rect);
            tilemapXmlReader->exploration->entrances[tilemapXmlReader->exploration->entranceCount] = e;
            tilemapXmlReader->exploration->entranceCount++;
            objectType = ENTRANCE;
        } else if (strcmp(type, "exit") == 0) {
            Exploration *e = tilemapXmlReader->exploration;
            e->exits[e->exitCount] = createExit();
            e->exits[e->exitCount]->area = (Rectangle) {
                    getFloatAttribute(tilemapXmlReader->reader, "x"),
                    getFloatAttribute(tilemapXmlReader->reader, "y"),
                    getFloatAttribute(tilemapXmlReader->reader, "width"),
                    getFloatAttribute(tilemapXmlReader->reader, "height")
            };
            e->exitCount++;
            objectType = EXIT;
        } else if (strcmp(type, "arrive_at") == 0) {
            Exploration *e = tilemapXmlReader->exploration;
            e->arriveAt[e->arriveAtCount] = createArriveAt(
                    getStringAttribute(tilemapXmlReader->reader, "name"),
                    (Rectangle) {
                            getFloatAttribute(tilemapXmlReader->reader, "x"),
                            getFloatAttribute(tilemapXmlReader->reader, "y"),
                            getFloatAttribute(tilemapXmlReader->reader, "width"),
                            getFloatAttribute(tilemapXmlReader->reader, "height")
                    });
            e->arriveAtCount++;
        }
    } else if (strcmp(strName, "property") == 0) {
        Exploration *e = tilemapXmlReader->exploration;
        char *propName = getStringAttribute(tilemapXmlReader->reader, "name");
        if (objectType == EXIT) {
            if (strcmp(propName, "scene") == 0) {
                e->exits[e->exitCount - 1]->scene = getStringAttribute(tilemapXmlReader->reader, "value");
            } else if (strcmp(propName, "to") == 0) {
                e->exits[e->exitCount - 1]->to = getStringAttribute(tilemapXmlReader->reader, "value");
            }
        } else if (objectType == ENTRANCE) {
            if (strcmp(propName, "direction") == 0) {
                e->entrances[e->entranceCount - 1]->direction =
                        getDirectionFromString(getStringAttribute(tilemapXmlReader->reader, "value"));
            }
        }
    }
}

void parseSceneXml(TilemapXmlReader *tilemapXmlReader, const char *indexDir) {
    int ret;
    if (tilemapXmlReader->reader == NULL) {
        addError("unable to find map resources for scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesMissing);
    }
    ret = xmlTextReaderRead(tilemapXmlReader->reader);
    while (ret == 1) {
        processSceneNode(tilemapXmlReader, indexDir);
        ret = xmlTextReaderRead(tilemapXmlReader->reader);
    }
    xmlFreeTextReader(tilemapXmlReader->reader);
    if (ret != 0) {
        addError("failed to read scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
}