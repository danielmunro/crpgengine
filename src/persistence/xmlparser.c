#include <mm_malloc.h>
#include <string.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/persistence/db.h"
#include "headers/tile.h"
#include "headers/warp.h"
#include "headers/control.h"
#include "headers/map.h"

void parseLayerRawData(const Map *m, const char *rawData) {
    addDebug("scene layer %d processing now", m->layersCount - 1);
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
            m->layers[m->layersCount - 1]->data[y][x] = TextToInteger(val);
            val = strtok_r(data[y], ",", &data[y]);
            x++;
        }
        y++;
    }
    m->layers[m->layersCount - 1]->width = x;
    m->layers[m->layersCount - 1]->height = y;
}

int xmlInt(const xmlNode *node, const char *propName) {
    return TextToInteger((const char *) xmlGetProp(node, (const xmlChar *) propName));
}

const char *xmlString(const xmlNode *node, const char *propName) {
    return (const char *) xmlGetProp(node, (const xmlChar *) propName);
}

RectangleD parseRectangle(const xmlNode *node, int tileSize) {
//    return (RectangleD) {
//            xmlInt(node, PROP_X) / tileSize,
//            xmlInt(node, PROP_Y) / tileSize,
//            xmlInt(node, PROP_WIDTH),
//            xmlInt(node, PROP_HEIGHT),
//    };
    return (RectangleD) {
            xmlInt(node, PROP_X),
            xmlInt(node, PROP_Y),
            xmlInt(node, PROP_WIDTH),
            xmlInt(node, PROP_HEIGHT),
    };
}

void parseTileObjectGroupNode(xmlNodePtr node, Tile *t, int tileSize) {
    while (node != NULL) {
        const NodeName name = getNodeNameFromString((const char *) node->name);
        if (name == NODE_NAME_OBJECT) {
            t->objects[t->objectCount] = createObject(
                    xmlInt(node, PROP_ID),
                    parseRectangle(node, tileSize));
            t->objectCount++;
        }
        node = node->next;
    }
}

void parseTileProperties(xmlNodePtr node, Tile *t) {
    while (node != NULL) {
        const NodeName name = getNodeNameFromString((const char *) node->name);
        if (name == NODE_NAME_PROPERTY) {
            const char *propName = xmlString(node, PROP_NAME);
            const char *propValue = xmlString(node, PROP_VALUE);
            t->properties[t->propertyCount] = createProperty(propName, propValue);
            t->propertyCount++;
        }
        node = node->next;
    }
}

Tile *parseTileNode(xmlNodePtr node, int tileSize) {
    int id = xmlInt(node, PROP_ID);
    TileType type = getTileTypeFromString(xmlString(node, PROP_TYPE));
    Tile *t = createTile(id, type);
    xmlNodePtr cur = node->children;
    while (cur != NULL) {
        const NodeName name = getNodeNameFromString((const char *) cur->name);
        if (name == NODE_NAME_OBJECT_GROUP) {
            parseTileObjectGroupNode(cur->children, t, tileSize);
        } else if (name == NODE_NAME_PROPERTIES) {
            parseTileProperties(cur->children, t);
        }
        cur = cur->next;
    }
    return t;
}

void parseTilesetRootNode(Context *c, xmlNodePtr node, Tileset *t) {
    const char *source = xmlString(node, PROP_SOURCE);
    char filePath[MAX_FS_PATH_LENGTH];
    getComponentPath(filePath, c->indexDir, c->indexDir, "tilesets", source);
    xmlDocPtr doc = xmlParseFile(filePath);
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        addError("unable to find tileset :: %s", source);
        exit(ConfigurationErrorMapResourcesMissing);
    }
    while (cur != NULL) {
        TilesetNodeType type = getTilesetNodeTypeFromString((const char *) cur->name);
        if (type == TILESET_NODE_TYPE_TILESET) {
            cur = cur->children;
        } else if (type == TILESET_NODE_TYPE_IMAGE) {
            char *imagePath = malloc(MAX_FS_PATH_LENGTH);
            sprintf(filePath, "%s/%s/%s", c->indexDir, "images/tilesets", GetFileName(xmlString(cur, "source")));
            sprintf(imagePath, "%s", filePath);
            t->reader = xmlReaderForFile(filePath, NULL, 0);
            t->sourcePath = imagePath;
        } else if (type == TILESET_NODE_TYPE_TILE) {
            t->tiles[t->tilesCount] = parseTileNode(cur, c->game->tileSize);
            t->tilesCount++;
        }
        cur = cur->next;
    }
    free((char *) source);
    xmlFreeNode(cur);
    xmlFreeDoc(doc);
}

void parseLayerNode(xmlNodePtr node, Map *map) {
    const char *layerName = xmlString(node, PROP_NAME);
    node = node->children;
    while (node != NULL) {
        const NodeName name = getNodeNameFromString((const char *) node->name);
        addDebug("parse layer node :: %s, %d", layerName, name);
        if (name == NODE_NAME_DATA) {
            map->layers[map->layersCount] = createLayer(layerName);
            map->layersCount++;
            parseLayerRawData(map, (char *) xmlNodeGetContent(node));
        }
        node = node->next;
    }
}

Property *parseProperty(xmlNodePtr node, const char *propName) {
    xmlNodePtr cur = node->children;
    while (cur != NULL) {
        const NodeName nodeName = getNodeNameFromString((const char *) cur->name);
        if (nodeName == NODE_NAME_PROPERTIES) {
            cur = cur->children;
        }
        const char *name = xmlString(cur, PROP_NAME);
        if (name != NULL && strcmp(name, propName) == 0) {
            return createProperty(propName, xmlString(cur, PROP_VALUE));
        }
        free((char *) name);
        cur = cur->next;
    }
    return NULL;
}

void parseExit(xmlNodePtr node, Map *m) {
    int id = xmlInt(node, PROP_ID);
    const Property *to = parseProperty(node, PROP_TO);
    const Property *scene = parseProperty(node, PROP_SCENE);
    if (to == NULL || scene == NULL) {
        free((Property *) to);
        free((Property *) scene);
        addWarning("malformed exit, skipping :: id: %d, name: %s", id, xmlString(node, PROP_NAME));
        return;
    }
    m->warps->exits[m->warps->exitCount] = createExit(
            id,
            to->value,
            scene->value,
            parseRectangle(node, m->context->game->tileSize));
    m->warps->exitCount++;
    free((Property *) to);
    free((Property *) scene);
}

void parseEntrance(xmlNodePtr node, Map *m) {
    int id = xmlInt(node, PROP_ID);
    const char *objectName = xmlString(node, PROP_NAME);
    const Property *direction = parseProperty(node, PROP_DIRECTION);
    m->warps->entrances[m->warps->entranceCount] = createEntrance(
            id,
            objectName,
            getDirectionFromString(direction != NULL ? direction->value : DOWN),
            parseRectangle(node, tileSize(m->context)));
    m->warps->entranceCount++;
    free((Property *) direction);
}

void parseTilemapObjectGroupWarpsNode(xmlNodePtr node, Map *m) {
    while (node != NULL) {
        const NodeName name = getNodeNameFromString((const char *) node->name);
        if (name == NODE_NAME_OBJECT) {
            ObjectType type = getObjectTypeFromString(xmlString(node, PROP_TYPE));
            if (type == OBJECT_TYPE_EXIT) {
                parseExit(node, m);
            } else if (type == OBJECT_TYPE_ENTRANCE) {
                parseEntrance(node, m);
            }
        }
        node = node->next;
    }
}

void parseTilemapObjectGroupArriveAtNode(xmlNodePtr node, Map *m) {
    xmlNodePtr cur = node->children;
    while (cur != NULL) {
        const NodeName name = getNodeNameFromString((const char *) node->name);
        if (name == NODE_NAME_OBJECT) {
            int id = xmlInt(cur, PROP_ID);
            const char *objectName = xmlString(cur, PROP_NAME);
            m->warps->arriveAt[m->warps->arriveAtCount] = createArriveAt(
                    id,
                    objectName,
                    parseRectangle(cur, tileSize(m->context)));
            m->warps->arriveAtCount++;
        }
        cur = cur->next;
    }
}

void parseTilemapObjectGroupChestNode(xmlNodePtr node, ItemManager *im, Map *m) {
    while (node != NULL) {
        const NodeName name = getNodeNameFromString((const char *) node->name);
        if (name == NODE_NAME_OBJECT) {
            const Property *item = parseProperty(node, PROP_ITEM);
            const Property *quantity = parseProperty(node, PROP_QUANTITY);
            const Property *coins = parseProperty(node, PROP_COINS);
            m->chests[m->chestCount] = createChest(
                    xmlInt(node, "id"),
                    createItemWithQuantity(
                            findItemFromName(im, item->value),
                            TextToInteger(quantity->value)),
                    coins != NULL ? TextToInteger(coins->value) : 0,
                    parseRectangle(node, tileSize(m->context)));
            m->chestCount++;
            free((Property *) item);
            free((Property *) quantity);
            free((Property *) coins);
        }
        node = node->next;
    }
}

void parseTilemapObjectGroupShopNode(xmlNodePtr node, Map *m) {
    while (node != NULL) {
        const NodeName name = getNodeNameFromString((const char *) node->name);
        if (name == NODE_NAME_OBJECT) {
            const Property *propName = parseProperty(node, PROP_NAME);
            const Property *propId = parseProperty(node, PROP_ID);
            Object *o = createObject(
                    xmlInt(node, "id"),
                    parseRectangle(node, tileSize(m->context)));
            m->shopTiles[m->shopTileCount] = createShopTile(
                    TextToInteger(propId->value),
                    propName->value,
                    o);
            m->shopTileCount++;
        }
        node = node->next;
    }
}

void parseTilemapObjectGroupNode(xmlNodePtr node, ItemManager *im, Map *m) {
    const TilemapObjectGroupName groupName = getTilemapObjectGroupNameFromString(
            xmlString(node, PROP_NAME));
    if (groupName == TILEMAP_OBJECT_GROUP_WARPS) {
        parseTilemapObjectGroupWarpsNode(node->children, m);
    } else if (groupName == TILEMAP_OBJECT_GROUP_ARRIVE_AT) {
        parseTilemapObjectGroupArriveAtNode(node->children, m);
    } else if (groupName == TILEMAP_OBJECT_GROUP_CHESTS) {
        parseTilemapObjectGroupChestNode(node->children, im, m);
    } else if (groupName == TILEMAP_OBJECT_GROUP_SHOPS) {
        parseTilemapObjectGroupShopNode(node->children, m);
    }
}

void assignChestTiles(Map *m) {
    for (int i = 0; i < m->tileset->tilesCount; i++) {
        if (m->tileset->tiles[i]->type == TILE_TYPE_CHEST_EMPTY) {
            m->openedChest = m->tileset->tiles[i];
        } else if (m->tileset->tiles[i]->type == TILE_TYPE_CHEST_FULL) {
            m->closedChest = m->tileset->tiles[i];
        }
    }
}

void parseTilemapRootNode(Map *map, xmlNodePtr node, ItemManager *im) {
    map->config->tileSize.x = xmlInt(node, PROP_TILE_WIDTH);
    map->config->tileSize.y = xmlInt(node, PROP_TILE_HEIGHT);
    while (node->children != NULL) {
        TilemapNodeType type = getTileMapNodeTypeFromString((const char *) node->children->name);
        addDebug("parse tile map node type :: %s", node->children->name);
        if (type == TILEMAP_NODE_TYPE_TILESET) {
            parseTilesetRootNode(map->context, node->children, map->tileset);
        } else if (type == TILEMAP_NODE_TYPE_LAYER) {
            parseLayerNode(node->children, map);
        } else if (type == TILEMAP_NODE_TYPE_OBJECTGROUP) {
            parseTilemapObjectGroupNode(node->children, im, map);
        }
        node->children = node->children->next;
    }
    assignChestTiles(map);
}

Map *parseTilemapDocToMap(
        ItemManager *im,
        Context *c,
        int id,
        const char *filePath,
        const char *indexDir) {
    xmlDocPtr doc = xmlParseFile(filePath);
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        addError("unable to find map resources for scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesMissing);
    }
    Map *map = createMap(id, c);
    parseTilemapRootNode(map, cur, im);
    xmlFreeNode(cur);
    return map;
}