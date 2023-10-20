#include <mm_malloc.h>
#include <string.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/persistence/db.h"
#include "headers/tile.h"
#include "headers/exploration.h"

typedef struct {
    xmlTextReaderPtr reader;
    Exploration *exploration;
//    Layer layers[MAX_LAYERS];
//    Entrance entrances[MAX_ENTRANCES];
//    Exit exits[MAX_EXITS];
//    Tile **tiles;
//    int tilesCount;
} TiledXmlReader;

TiledXmlReader *createTilemapXmlReader(Exploration *exploration, const char *sceneFile) {
    addDebug("attempting to load scene file :: %s", sceneFile);
    TiledXmlReader *reader = malloc(sizeof(TiledXmlReader));
    reader->exploration = exploration;
    reader->reader = xmlReaderForFile(sceneFile, NULL, 0);
//    reader->tiles = calloc(MAX_TILES, sizeof(Tile));
//    reader->tilesCount = 0;
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

static void processTilesetNode(TiledXmlReader *xmlReader, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(xmlReader->reader);
    static Tile *tile;
    static TilesetType tilesetType = TILESET_TYPE_NONE;
    Exploration *e = xmlReader->exploration;
    TileSetNodeType nodeType = getTileSetNodeTypeFromString((const char *) name);
    if (nodeType == TILESET_NODE_TYPE_TILESET) {
        addDebug("process tileset main node :: %s", name);
        const int width = getIntAttribute(xmlReader->reader, "tilewidth");
        const int height = getIntAttribute(xmlReader->reader, "tileheight");
        e->tilemap->size = (Vector2D) {width, height};
    } else if (nodeType == TILESET_NODE_TYPE_IMAGE) {
        addDebug("process tileset image node :: %s", name);
        char filePath[MAX_FS_PATH_LENGTH];
        getComponentPath(
                filePath,
                indexDir,
                "tilesets",
                getStringAttribute(xmlReader->reader, "source"));
        e->tilemap->source = LoadImage(filePath);
    } else if (nodeType == TILESET_NODE_TYPE_TILE) {
        addDebug("process tileset tile node :: %s", name);
        if (tile != NULL) {
            addTile(e, tile);
//            xmlReader->tiles[xmlReader->tilesCount] = tile;
//            xmlReader->tilesCount++;
            tile = NULL;
            return;
        }
        int tileId = getIntAttribute(xmlReader->reader, "id");
        char *type = getStringAttribute(xmlReader->reader, "type");
        if (type != NULL) {
            tilesetType = getTilesetTypeFromString(type);
        } else {
            tilesetType = TILESET_TYPE_NONE;
        }
        tile = createTile(tileId, tilesetType);
    } else if (nodeType == TILESET_NODE_TYPE_OBJECT) {
        addDebug("process tileset object node :: %s", name);
        tile->object = malloc(sizeof(Rectangle));
        tile->object->x = getFloatAttribute(xmlReader->reader, "x");
        tile->object->y = getFloatAttribute(xmlReader->reader, "y");
        tile->object->width = getFloatAttribute(xmlReader->reader, "width");
        tile->object->height = getFloatAttribute(xmlReader->reader, "height");
    } else if (nodeType == TILESET_NODE_TYPE_PROPERTY) {
        Property *property = createProperty();
        property->name = getStringAttribute(xmlReader->reader, "name");
        property->value = getStringAttribute(xmlReader->reader, "value");
        addProperty(tile, property);
    }
}

void parseTilemapXml(Exploration *e, const char *indexDir, const char *filename) {
    addDebug("parsing xml tilemap at %s/%s", indexDir, filename);
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    e->tilemap = tilemap;
    int ret;
    char filePath[MAX_FS_PATH_LENGTH];
    getComponentPath(
            filePath,
            indexDir,
            "tilesets",
            filename);
    addInfo("component path calculated to :: %s", filePath);
    TiledXmlReader *tilemapXmlReader = createTilemapXmlReader(e, filePath);
    if (tilemapXmlReader->reader == NULL) {
        addError("unable to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
    ret = xmlTextReaderRead(tilemapXmlReader->reader);
    while (ret == 1) {
        processTilesetNode(tilemapXmlReader, indexDir);
        ret = xmlTextReaderRead(tilemapXmlReader->reader);
    }
    addDebug("found %d tiles", tilemapXmlReader->exploration->tilesCount);
    xmlFreeTextReader(tilemapXmlReader->reader);
    if (ret != 0) {
        addError("failed to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
    free(tilemapXmlReader);
}

void parseSceneLayer(const Exploration *e, const char *rawData) {
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
        const char *val = strtok_r(data[y], ",", &data[y]);
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

void processTilemapNode(TiledXmlReader *xmlReader, const char *indexDir) {
    const char *name = (const char *) xmlTextReaderConstName(xmlReader->reader);
    static int dataOpen = 0;
    static int layerOpen = 0;
    static ObjectType objectType;
    TileMapNodeType nodeType = getTileMapNodeTypeFromString(name);
    if (nodeType == TILEMAP_NODE_TYPE_TILESET) {
        addDebug("process tileset xml node :: %s", name);
        char *source = getStringAttribute(xmlReader->reader, "source");
        parseTilemapXml(xmlReader->exploration, indexDir, source);
    } else if (nodeType == TILEMAP_NODE_TYPE_LAYER) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        Layer *layer = createLayer();
        char *layerName = getStringAttribute(xmlReader->reader, "name");
        addDebug("create new layer :: %s", layerName);
        layer->type = getLayerTypeFromString(layerName);
        xmlReader->exploration->layers[xmlReader->exploration->layerCount] = layer;
    } else if (nodeType == TILEMAP_NODE_TYPE_DATA) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(xmlReader->reader);
        xmlReader->exploration->layerCount++;
        parseSceneLayer(xmlReader->exploration, (const char *) data);
    } else if (nodeType == TILEMAP_NODE_TYPE_OBJECT) {
        objectType = getObjectTypeFromString(getStringAttribute(xmlReader->reader, "type"));
        addDebug("evaluate object type :: %d", objectType);
        if (objectType == OBJECT_TYPE_ENTRANCE) {
            Rectangle rect = {
                    getFloatAttribute(xmlReader->reader, "x"),
                    getFloatAttribute(xmlReader->reader, "y"),
                    getFloatAttribute(xmlReader->reader, "width"),
                    getFloatAttribute(xmlReader->reader, "height")
            };
            Entrance *e = createEntrance(getStringAttribute(xmlReader->reader, "name"), rect);
            xmlReader->exploration->entrances[xmlReader->exploration->entranceCount] = e;
            xmlReader->exploration->entranceCount++;
        } else if (objectType == OBJECT_TYPE_EXIT) {
            Exploration *e = xmlReader->exploration;
            e->exits[e->exitCount] = createExit();
            e->exits[e->exitCount]->area = (Rectangle) {
                    getFloatAttribute(xmlReader->reader, "x"),
                    getFloatAttribute(xmlReader->reader, "y"),
                    getFloatAttribute(xmlReader->reader, "width"),
                    getFloatAttribute(xmlReader->reader, "height")
            };
            e->exitCount++;
        } else if (objectType == OBJECT_TYPE_ARRIVE_AT) {
            Exploration *e = xmlReader->exploration;
            e->arriveAt[e->arriveAtCount] = createArriveAt(
                    getStringAttribute(xmlReader->reader, "name"),
                    (Rectangle) {
                            getFloatAttribute(xmlReader->reader, "x"),
                            getFloatAttribute(xmlReader->reader, "y"),
                            getFloatAttribute(xmlReader->reader, "width"),
                            getFloatAttribute(xmlReader->reader, "height")
                    });
            e->arriveAtCount++;
        } else if (objectType == OBJECT_TYPE_CHEST) {
            addInfo("chest object found");
        }
    } else if (strcmp(name, "property") == 0) {
        Exploration *e = xmlReader->exploration;
        char *propName = getStringAttribute(xmlReader->reader, "name");
        if (objectType == OBJECT_TYPE_EXIT) {
            if (strcmp(propName, "scene") == 0) {
                e->exits[e->exitCount - 1]->scene = getStringAttribute(xmlReader->reader, "value");
            } else if (strcmp(propName, "to") == 0) {
                e->exits[e->exitCount - 1]->to = getStringAttribute(xmlReader->reader, "value");
            }
        } else if (objectType == OBJECT_TYPE_ENTRANCE) {
            if (strcmp(propName, "direction") == 0) {
                e->entrances[e->entranceCount - 1]->direction =
                        getDirectionFromString(getStringAttribute(xmlReader->reader, "value"));
            }
        }
    }
}

void parseSceneXml(TiledXmlReader *xmlReader, const char *indexDir) {
    int ret;
    if (xmlReader->reader == NULL) {
        addError("unable to find map resources for scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesMissing);
    }
    ret = xmlTextReaderRead(xmlReader->reader);
    while (ret == 1) {
        processTilemapNode(xmlReader, indexDir);
        ret = xmlTextReaderRead(xmlReader->reader);
    }
    xmlFreeTextReader(xmlReader->reader);
    if (ret != 0) {
        addError("failed to read scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
}