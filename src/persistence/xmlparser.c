#include <mm_malloc.h>
#include <string.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include <libgen.h>
#include "headers/persistence/db.h"
#include "headers/tiles.h"

typedef struct {
    xmlTextReaderPtr reader;
    Exploration *exploration;
} TilemapXmlReader;

TilemapXmlReader *createTilemapXmlReader(Exploration *exploration, const char *sceneFile) {
    addDebug("attempting to load scene file '%s'", sceneFile);
    TilemapXmlReader *reader = malloc(sizeof(TilemapXmlReader));
    reader->exploration = exploration;
    reader->reader = xmlReaderForFile(sceneFile, NULL, XML_PARSE_HUGE);
    return reader;
}

int getIntAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return TextToInteger((char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute));
}

float getFloatAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return strtof((char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute), NULL);
}

char *getStringAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return (char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute);
}

static void processTilesetNode(TilemapXmlReader *tilemapXmlReader, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(tilemapXmlReader->reader);
    static int tileOpen = 0;
    static int lastObjectId = 0;
    TileSetNodeType nodeType = getTileSetNodeTypeFromString((const char *) name);
    if (nodeType == TILESET_NODE_TYPE_TILESET) {
        const int width = getIntAttribute(tilemapXmlReader->reader, "tilewidth");
        const int height = getIntAttribute(tilemapXmlReader->reader, "tileheight");
        tilemapXmlReader->exploration->tilemap->tileset->size = (Vector2D) {width, height};
    } else if (nodeType == TILESET_NODE_TYPE_IMAGE) {
        char filePath[MAX_FS_PATH_LENGTH];
        const char *source = getStringAttribute(tilemapXmlReader->reader, "source");
        sprintf(filePath, "%s/%s", indexDir, source);
        addDebug("check for module tile set image :: %s", filePath);
        if (access(filePath, F_OK) != 0) {
            sprintf(filePath, "%s/%s/%s", config->indexDir, "tilesets", source);
            addDebug("check for global tile set image :: %s", filePath);
            if (access(filePath, F_OK) != 0) {
                addError("no tileset image exists with filename :: %s", source);
                exit(ConfigurationErrorMissingTileset);
            }
        }
        tilemapXmlReader->exploration->tilemap->source = LoadImage(filePath);
    } else if (nodeType == TILESET_NODE_TYPE_TILE) {
        if (tileOpen == 1) {
            tileOpen = 0;
            return;
        }
        tileOpen = 1;
        lastObjectId = getIntAttribute(tilemapXmlReader->reader, "id");
    } else if (nodeType == TILESET_NODE_TYPE_OBJECT) {
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
    e->tilemap = createTilemap();
    int ret;
    char filePath[MAX_FS_PATH_LENGTH];
    const char *baseFile = basename((char *) filename);
    sprintf(filePath, "%s/%s", indexDir, baseFile);
    addInfo("check for module tile set :: %s", filePath);
    if (access(filePath, F_OK) != 0) {
        sprintf(filePath, "%s/%s/%s", config->indexDir, "tilesets", baseFile);
        addInfo("check for global tile set :: %s", filePath);
        if (access(filePath, F_OK) != 0) {
            addError("no tileset exists with filename :: %s", baseFile);
            exit(ConfigurationErrorMissingTileset);
        }
    }
    TilemapXmlReader *tilemapXmlReader = createTilemapXmlReader(e, filePath);
    if (tilemapXmlReader->reader == NULL) {
        addError("unable to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
    ret = xmlTextReaderRead(tilemapXmlReader->reader);
    while (ret == 1) {
        processTilesetNode(tilemapXmlReader, indexDir);
        ret = xmlTextReaderRead(tilemapXmlReader->reader);
    }
    addDebug("found %d objects", tilemapXmlReader->exploration->objectCount);
    xmlFreeTextReader(tilemapXmlReader->reader);
    if (ret != 0) {
        addError("failed to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
    free(tilemapXmlReader);
}

void parseSceneLayer(Exploration *e, const char *rawData) {
    addDebug("scene layer %d processing now", e->layerCount - 1);
    char *newRawData = (char *) rawData;
    char *line = strtok_r(newRawData, "\r\n", &newRawData);
    char *data[MAX_DATA_SIZE];
    int it = 0;
    while (line != NULL && it < MAX_DATA_SIZE) {
        data[it] = line;
        line = strtok_r(newRawData, "\r\n", &newRawData);
        it++;
    }
    int y = 0;
    int x = 0;
    while (y < it) {
        char *val = strtok_r(data[y], ",", &data[y]);
        x = 0;
        while (val != NULL) {
            e->layers[e->layerCount - 1]->data[y][x] = TextToInteger(val);
            val = strtok_r(data[y], ",", &data[y]);
            x++;
        }
        y++;
    }
    e->layers[e->layerCount - 1]->width = x;
    e->layers[e->layerCount - 1]->height = y;
}

void processTilemapNode(TilemapXmlReader *tilemapXmlReader, const char *indexDir) {
    const char *name = (const char *) xmlTextReaderConstName(tilemapXmlReader->reader);
    static int dataOpen = 0;
    static int layerOpen = 0;
    static ObjectType objectType;
    TileMapNodeType nodeType = getTileMapNodeTypeFromString(name);
    addDebug("process scene node :: %s, %d", name, nodeType);
    if (nodeType == TILEMAP_NODE_TYPE_TILESET) {
        char *source = getStringAttribute(tilemapXmlReader->reader, "source");
        parseTilemapXml(tilemapXmlReader->exploration, indexDir, source);
    } else if (nodeType == TILEMAP_NODE_TYPE_LAYER) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        Layer *layer = createLayer();
        char *layerName = getStringAttribute(tilemapXmlReader->reader, "name");
        layer->type = getLayerTypeFromString(layerName);
        tilemapXmlReader->exploration->layers[tilemapXmlReader->exploration->layerCount] = layer;
    } else if (nodeType == TILEMAP_NODE_TYPE_DATA) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(tilemapXmlReader->reader);
        tilemapXmlReader->exploration->layerCount++;
        parseSceneLayer(tilemapXmlReader->exploration, (const char *) data);
    } else if (nodeType == TILEMAP_NODE_TYPE_OBJECT) {
        objectType = getObjectTypeFromString(getStringAttribute(tilemapXmlReader->reader, "type"));
        addDebug("object type: %d", objectType);
        if (objectType == OBJECT_TYPE_ENTRANCE) {
            Rectangle rect = {
                    getFloatAttribute(tilemapXmlReader->reader, "x"),
                    getFloatAttribute(tilemapXmlReader->reader, "y"),
                    getFloatAttribute(tilemapXmlReader->reader, "width"),
                    getFloatAttribute(tilemapXmlReader->reader, "height")
            };
            Entrance *e = createEntrance(getStringAttribute(tilemapXmlReader->reader, "name"), rect);
            tilemapXmlReader->exploration->entrances[tilemapXmlReader->exploration->entranceCount] = e;
            tilemapXmlReader->exploration->entranceCount++;
        } else if (objectType == OBJECT_TYPE_EXIT) {
            Exploration *e = tilemapXmlReader->exploration;
            e->exits[e->exitCount] = createExit();
            e->exits[e->exitCount]->area = (Rectangle) {
                    getFloatAttribute(tilemapXmlReader->reader, "x"),
                    getFloatAttribute(tilemapXmlReader->reader, "y"),
                    getFloatAttribute(tilemapXmlReader->reader, "width"),
                    getFloatAttribute(tilemapXmlReader->reader, "height")
            };
            e->exitCount++;
        } else if (objectType == OBJECT_TYPE_ARRIVE_AT) {
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
        } else if (objectType == OBJECT_TYPE_CHEST) {
            addInfo("chest object found");
        }
    } else if (strcmp(name, "property") == 0) {
        Exploration *e = tilemapXmlReader->exploration;
        char *propName = getStringAttribute(tilemapXmlReader->reader, "name");
        if (objectType == OBJECT_TYPE_EXIT) {
            if (strcmp(propName, "scene") == 0) {
                e->exits[e->exitCount - 1]->scene = getStringAttribute(tilemapXmlReader->reader, "value");
            } else if (strcmp(propName, "to") == 0) {
                e->exits[e->exitCount - 1]->to = getStringAttribute(tilemapXmlReader->reader, "value");
            }
        } else if (objectType == OBJECT_TYPE_ENTRANCE) {
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
        processTilemapNode(tilemapXmlReader, indexDir);
        ret = xmlTextReaderRead(tilemapXmlReader->reader);
    }
    xmlFreeTextReader(tilemapXmlReader->reader);
    if (ret != 0) {
        addError("failed to read scene :: %s, %d", indexDir, ret);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
}