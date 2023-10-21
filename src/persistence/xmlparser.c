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

static void processTilesetNode(Tileset *tileset, const char *indexDir) {
    const xmlChar *name = xmlTextReaderConstName(tileset->reader);
    static Tile *tile;
    static TilesetType tilesetType = TILESET_TYPE_NONE;
    TileSetNodeType nodeType = getTileSetNodeTypeFromString((const char *) name);
    if (nodeType == TILESET_NODE_TYPE_TILESET) {
        addDebug("process tileset main node :: %s", name);
        const int width = getIntAttribute(tileset->reader, "tilewidth");
        const int height = getIntAttribute(tileset->reader, "tileheight");
        tileset->size = (Vector2D) {width, height};
    } else if (nodeType == TILESET_NODE_TYPE_IMAGE) {
        addDebug("process tileset image node :: %s", name);
        char filePath[MAX_FS_PATH_LENGTH];
        getComponentPath(
                filePath,
                indexDir,
                "tilesets",
                getStringAttribute(tileset->reader, "source"));
        tileset->source = LoadImage(filePath);
    } else if (nodeType == TILESET_NODE_TYPE_TILE) {
        addDebug("process tileset tile node :: %s", name);
        if (tile != NULL) {
            tileset->tiles[tileset->tilesCount] = tile;
            tileset->tilesCount++;
            tile = NULL;
            return;
        }
        int tileId = getIntAttribute(tileset->reader, "id");
        char *type = getStringAttribute(tileset->reader, "type");
        if (type != NULL) {
            tilesetType = getTilesetTypeFromString(type);
        } else {
            tilesetType = TILESET_TYPE_NONE;
        }
        tile = createTile(tileId, tilesetType);
    } else if (nodeType == TILESET_NODE_TYPE_OBJECT) {
        addDebug("process tileset object node :: %s", name);
        tile->object = malloc(sizeof(Rectangle));
        tile->object->x = getFloatAttribute(tileset->reader, "x");
        tile->object->y = getFloatAttribute(tileset->reader, "y");
        tile->object->width = getFloatAttribute(tileset->reader, "width");
        tile->object->height = getFloatAttribute(tileset->reader, "height");
    } else if (nodeType == TILESET_NODE_TYPE_PROPERTY) {
        Property *property = createProperty();
        property->name = getStringAttribute(tileset->reader, "name");
        property->value = getStringAttribute(tileset->reader, "value");
        addProperty(tile, property);
    }
}

Tileset *parseTileset(const char *indexDir, const char *filename) {
    addDebug("parsing xml tileset at %s/%s", indexDir, filename);
    int ret;
    char filePath[MAX_FS_PATH_LENGTH];
    getComponentPath(
            filePath,
            indexDir,
            "tilesets",
            filename);
    addInfo("component path calculated to :: %s", filePath);
    Tileset *ts = createTilesetXml(filePath);
    ret = xmlTextReaderRead(ts->reader);
    while (ret == 1) {
        processTilesetNode(ts, indexDir);
        ret = xmlTextReaderRead(ts->reader);
    }
    addDebug("found %d tiles", ts->tilesCount);
    xmlFreeTextReader(ts->reader);
    if (ret != 0) {
        addError("failed to parse tilemap xml :: %s", filename);
        exit(ConfigurationErrorMapResourcesUnreadable);
    }
    return ts;
}

void parseSceneLayer(const Tilemap *t, const char *rawData) {
    addDebug("scene layer %d processing now", t->layerCount - 1);
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
            t->layers[t->layerCount - 1]->data[y][x] = TextToInteger(val);
            val = strtok_r(data[y], ",", &data[y]);
            x++;
        }
        y++;
    }
    t->layers[t->layerCount - 1]->width = x;
    t->layers[t->layerCount - 1]->height = y;
}

void processTilemapNode(Exploration *e, Tilemap *tilemap, const char *indexDir) {
    const char *name = (const char *) xmlTextReaderConstName(tilemap->reader);
    static int dataOpen = 0;
    static int layerOpen = 0;
    static ObjectType objectType;
    TileMapNodeType nodeType = getTileMapNodeTypeFromString(name);
    if (nodeType == TILEMAP_NODE_TYPE_TILESET) {
        addDebug("process tileset xml node :: %s", name);
        char *source = getStringAttribute(tilemap->reader, "source");
        tilemap->tileset = parseTileset(indexDir, source);
    } else if (nodeType == TILEMAP_NODE_TYPE_LAYER) {
        if (layerOpen == 1) {
            layerOpen = 0;
            return;
        }
        layerOpen = 1;
        Layer *layer = createLayer();
        char *layerName = getStringAttribute(tilemap->reader, "name");
        addDebug("create new layer :: %s", layerName);
        layer->type = getLayerTypeFromString(layerName);
        tilemap->layers[tilemap->layerCount] = layer;
    } else if (nodeType == TILEMAP_NODE_TYPE_DATA) {
        if (dataOpen == 1) {
            dataOpen = 0;
            return;
        }
        dataOpen = 1;
        xmlChar *data = xmlTextReaderReadString(tilemap->reader);
        tilemap->layerCount++;
        parseSceneLayer(tilemap, (const char *) data);
    } else if (nodeType == TILEMAP_NODE_TYPE_OBJECT) {
        objectType = getObjectTypeFromString(getStringAttribute(tilemap->reader, "type"));
        addDebug("evaluate object type :: %d", objectType);
        if (objectType == OBJECT_TYPE_ENTRANCE) {
            Rectangle rect = {
                    getFloatAttribute(tilemap->reader, "x"),
                    getFloatAttribute(tilemap->reader, "y"),
                    getFloatAttribute(tilemap->reader, "width"),
                    getFloatAttribute(tilemap->reader, "height")
            };
            e->entrances[e->entranceCount] = createEntrance(
                    getStringAttribute(tilemap->reader, "name"),
                    rect);
            e->entranceCount++;
        } else if (objectType == OBJECT_TYPE_EXIT) {
            e->exits[e->exitCount] = createExit();
            e->exits[e->exitCount]->area = (Rectangle) {
                    getFloatAttribute(tilemap->reader, "x"),
                    getFloatAttribute(tilemap->reader, "y"),
                    getFloatAttribute(tilemap->reader, "width"),
                    getFloatAttribute(tilemap->reader, "height")
            };
            e->exitCount++;
        } else if (objectType == OBJECT_TYPE_ARRIVE_AT) {
            e->arriveAt[e->arriveAtCount] = createArriveAt(
                    getStringAttribute(tilemap->reader, "name"),
                    (Rectangle) {
                            getFloatAttribute(tilemap->reader, "x"),
                            getFloatAttribute(tilemap->reader, "y"),
                            getFloatAttribute(tilemap->reader, "width"),
                            getFloatAttribute(tilemap->reader, "height")
                    });
            e->arriveAtCount++;
        } else if (objectType == OBJECT_TYPE_CHEST) {
            addInfo("chest object found");
        }
    } else if (strcmp(name, "property") == 0) {
        char *propName = getStringAttribute(tilemap->reader, "name");
        if (objectType == OBJECT_TYPE_EXIT) {
            if (strcmp(propName, "scene") == 0) {
                e->exits[e->exitCount - 1]->scene = getStringAttribute(tilemap->reader, "value");
            } else if (strcmp(propName, "to") == 0) {
                e->exits[e->exitCount - 1]->to = getStringAttribute(tilemap->reader, "value");
            }
        } else if (objectType == OBJECT_TYPE_ENTRANCE) {
            if (strcmp(propName, "direction") == 0) {
                e->entrances[e->entranceCount - 1]->direction =
                        getDirectionFromString(getStringAttribute(tilemap->reader, "value"));
            }
        }
    }
}

Tilemap *parseTilemapXml(Exploration *e, const char *filePath, const char *indexDir) {
    Tilemap *tilemapXml = createTilemapXml(filePath);
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
    return tilemapXml;
}