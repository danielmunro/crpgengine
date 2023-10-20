#include <mm_malloc.h>
#include <string.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/persistence/db.h"
#include "headers/tile.h"
#include "headers/exploration.h"
#include "headers/persistence/scene_xml.h"

int getIntAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return TextToInteger((char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute));
}

float getFloatAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return strtof((char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute), NULL);
}

char *getStringAttribute(xmlTextReaderPtr reader, const char *attribute) {
    return (char *) xmlTextReaderGetAttribute(reader, (const xmlChar *) attribute);
}

static void processTilesetNode(Exploration *e, TilesetXml *tilesetXml, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(tilesetXml->reader);
    static Tile *tile;
    static TilesetType tilesetType = TILESET_TYPE_NONE;
    TileSetNodeType nodeType = getTileSetNodeTypeFromString((const char *) name);
    if (nodeType == TILESET_NODE_TYPE_TILESET) {
        addDebug("process tileset main node :: %s", name);
        const int width = getIntAttribute(tilesetXml->reader, "tilewidth");
        const int height = getIntAttribute(tilesetXml->reader, "tileheight");
        e->tilemap->size = (Vector2D) {width, height};
    } else if (nodeType == TILESET_NODE_TYPE_IMAGE) {
        addDebug("process tileset image node :: %s", name);
        char filePath[MAX_FS_PATH_LENGTH];
        getComponentPath(
                filePath,
                indexDir,
                "tilesets",
                getStringAttribute(tilesetXml->reader, "source"));
        e->tilemap->source = LoadImage(filePath);
    } else if (nodeType == TILESET_NODE_TYPE_TILE) {
        addDebug("process tileset tile node :: %s", name);
        if (tile != NULL) {
            addDebug("closing node");
            addTile(e, tile);
            tilesetXml->tiles[tilesetXml->tilesCount] = tile;
            tilesetXml->tilesCount++;
            tile = NULL;
            return;
        }
        int tileId = getIntAttribute(tilesetXml->reader, "id");
        char *type = getStringAttribute(tilesetXml->reader, "type");
        if (type != NULL) {
            tilesetType = getTilesetTypeFromString(type);
        } else {
            tilesetType = TILESET_TYPE_NONE;
        }
        tile = createTile(tileId, tilesetType);
    } else if (nodeType == TILESET_NODE_TYPE_OBJECT) {
        addDebug("process tileset object node :: %s", name);
        tile->object = malloc(sizeof(Rectangle));
        tile->object->x = getFloatAttribute(tilesetXml->reader, "x");
        tile->object->y = getFloatAttribute(tilesetXml->reader, "y");
        tile->object->width = getFloatAttribute(tilesetXml->reader, "width");
        tile->object->height = getFloatAttribute(tilesetXml->reader, "height");
    } else if (nodeType == TILESET_NODE_TYPE_PROPERTY) {
        Property *property = createProperty();
        property->name = getStringAttribute(tilesetXml->reader, "name");
        property->value = getStringAttribute(tilesetXml->reader, "value");
        addProperty(tile, property);
    }
}

void parseTileset(Exploration *e, TilemapXml *tilemapXml, const char *indexDir, const char *filename) {
    addDebug("parsing xml tileset at %s/%s", indexDir, filename);
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
    tilemapXml->tilesetXml = createTilesetXml(filePath);
    ret = xmlTextReaderRead(tilemapXml->tilesetXml->reader);
    while (ret == 1) {
        processTilesetNode(
                e,
                tilemapXml->tilesetXml,
                indexDir);
        ret = xmlTextReaderRead(tilemapXml->tilesetXml->reader);
    }
    addDebug("found %d tiles", tilemapXml->tilesetXml->tilesCount);
    xmlFreeTextReader(tilemapXml->tilesetXml->reader);
    if (ret != 0) {
        addError("failed to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
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

void processTilemapNode(Exploration *e, TilemapXml *tilemapXml, const char *indexDir) {
    const char *name = (const char *) xmlTextReaderConstName(tilemapXml->reader);
    static int dataOpen = 0;
    static int layerOpen = 0;
    static ObjectType objectType;
    TileMapNodeType nodeType = getTileMapNodeTypeFromString(name);
    if (nodeType == TILEMAP_NODE_TYPE_TILESET) {
        addDebug("process tileset xml node :: %s", name);
        char *source = getStringAttribute(tilemapXml->reader, "source");
        parseTileset(e, tilemapXml, indexDir, source);
    } else if (nodeType == TILEMAP_NODE_TYPE_LAYER) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        Layer *layer = createLayer();
        char *layerName = getStringAttribute(tilemapXml->reader, "name");
        addDebug("create new layer :: %s", layerName);
        layer->type = getLayerTypeFromString(layerName);
        e->layers[e->layerCount] = layer;
    } else if (nodeType == TILEMAP_NODE_TYPE_DATA) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(tilemapXml->reader);
        e->layerCount++;
        parseSceneLayer(e, (const char *) data);
    } else if (nodeType == TILEMAP_NODE_TYPE_OBJECT) {
        objectType = getObjectTypeFromString(getStringAttribute(tilemapXml->reader, "type"));
        addDebug("evaluate object type :: %d", objectType);
        if (objectType == OBJECT_TYPE_ENTRANCE) {
            Rectangle rect = {
                    getFloatAttribute(tilemapXml->reader, "x"),
                    getFloatAttribute(tilemapXml->reader, "y"),
                    getFloatAttribute(tilemapXml->reader, "width"),
                    getFloatAttribute(tilemapXml->reader, "height")
            };
            e->entrances[e->entranceCount] = createEntrance(
                    getStringAttribute(tilemapXml->reader, "name"),
                    rect);
            e->entranceCount++;
        } else if (objectType == OBJECT_TYPE_EXIT) {
            e->exits[e->exitCount] = createExit();
            e->exits[e->exitCount]->area = (Rectangle) {
                    getFloatAttribute(tilemapXml->reader, "x"),
                    getFloatAttribute(tilemapXml->reader, "y"),
                    getFloatAttribute(tilemapXml->reader, "width"),
                    getFloatAttribute(tilemapXml->reader, "height")
            };
            e->exitCount++;
        } else if (objectType == OBJECT_TYPE_ARRIVE_AT) {
            e->arriveAt[e->arriveAtCount] = createArriveAt(
                    getStringAttribute(tilemapXml->reader, "name"),
                    (Rectangle) {
                            getFloatAttribute(tilemapXml->reader, "x"),
                            getFloatAttribute(tilemapXml->reader, "y"),
                            getFloatAttribute(tilemapXml->reader, "width"),
                            getFloatAttribute(tilemapXml->reader, "height")
                    });
            e->arriveAtCount++;
        } else if (objectType == OBJECT_TYPE_CHEST) {
            addInfo("chest object found");
        }
    } else if (strcmp(name, "property") == 0) {
        char *propName = getStringAttribute(tilemapXml->reader, "name");
        if (objectType == OBJECT_TYPE_EXIT) {
            if (strcmp(propName, "scene") == 0) {
                e->exits[e->exitCount - 1]->scene = getStringAttribute(tilemapXml->reader, "value");
            } else if (strcmp(propName, "to") == 0) {
                e->exits[e->exitCount - 1]->to = getStringAttribute(tilemapXml->reader, "value");
            }
        } else if (objectType == OBJECT_TYPE_ENTRANCE) {
            if (strcmp(propName, "direction") == 0) {
                e->entrances[e->entranceCount - 1]->direction =
                        getDirectionFromString(getStringAttribute(tilemapXml->reader, "value"));
            }
        }
    }
}

void parseSceneXml(Exploration *e, TilemapXml *tilemapXml, const char *indexDir) {
    int ret;
    if (tilemapXml->reader == NULL) {
        addError("unable to find map resources for scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesMissing);
    }
    ret = xmlTextReaderRead(tilemapXml->reader);
    while (ret == 1) {
        processTilemapNode(e, tilemapXml, indexDir);
        ret = xmlTextReaderRead(tilemapXml->reader);
    }
    xmlFreeTextReader(tilemapXml->reader);
    if (ret != 0) {
        addError("failed to read scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
}