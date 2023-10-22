#include <mm_malloc.h>
#include <string.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/persistence/db.h"
#include "headers/tile.h"
#include "headers/warp.h"

const char *NodeNames[] = {
        "skip",
        "map",
};

typedef enum {
    NODE_NAME_NONE,
    NODE_NAME_MAP,
} NodeName;

NodeName getNodeNameFromString(const char *name) {
    int count = sizeof(NodeNames) / sizeof(NodeNames[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(NodeNames[i], name) == 0) {
            return i;
        }
    }
    return NODE_NAME_NONE;
}

typedef struct {
    TileMapNodeType type;
} NodeXml;

typedef struct {
    NodeXml **nodes;
} XmlDoc;

NodeXml *createNodeXml(TileMapNodeType type) {
    NodeXml *n = malloc(sizeof(NodeXml));
    n->type = type;
    return n;
}

XmlDoc *createXmlDoc() {
    XmlDoc *t = malloc(sizeof(XmlDoc));
    t->nodes = calloc(MAX_NODES, sizeof(XmlDoc));
    return t;
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
    Tileset *ts = createTileset(filePath);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////

void parseLayerRawData(const Map *m, const char *rawData) {
    addDebug("scene layer %d processing now", m->layers2Count - 1);
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
            m->layers2[m->layers2Count - 1]->data[y][x] = TextToInteger(val);
            val = strtok_r(data[y], ",", &data[y]);
            x++;
        }
        y++;
    }
    m->layers2[m->layers2Count - 1]->width = x;
    m->layers2[m->layers2Count - 1]->height = y;
}

typedef struct {
    const char *name;
    const char *value;
} Property2;

typedef struct {
    Property2 **properties;
    int propertyCount;
    Rectangle object;
    int id;
    const char *type;
} Tile2;

Property2 *createProperty2(const char *name, const char *value) {
    Property2 *p = malloc(sizeof(Property2));
    p->name = name;
    p->value = value;
    return p;
}

Tile2 *createTile2(int id, const char *type) {
    Tile2 *t = malloc(sizeof(Tile2));
    t->id = id;
    t->type = type;
    t->properties = calloc(MAX_PROPERTIES, sizeof(Property));
    t->propertyCount = 0;
    return t;
}

int xmlInt(const xmlNode *node, const char *propName) {
    return TextToInteger((const char *) xmlGetProp(node, (const xmlChar *) propName));
}

float xmlFloat(const xmlNode *node, const char *propName) {
    char *ptr;
    const char *value = (const char *) xmlGetProp(node, (const xmlChar *) propName);
    float f = strtof(value, &ptr);
    if (strcmp(ptr, "") != 0) {
        addError("error parsing xml value :: %s, %s, %s", (const char *) node->name, propName, value);
    }
    free((char *) value);
    return f;
}

const char *xmlString(const xmlNode *node, const char *propName) {
    return (const char *) xmlGetProp(node, (const xmlChar *) propName);
}

Rectangle parseTileObjectGroupNode(xmlNodePtr cur) {
    while (cur != NULL) {
        if (strcmp((const char *) cur->name, "object") == 0) {
            addInfo("object group child :: %s", (char *) cur->name);
            return (Rectangle) {
                    xmlFloat(cur, "x"),
                    xmlFloat(cur, "y"),
                    xmlFloat(cur, "width"),
                    xmlFloat(cur, "height"),
            };
        }
        cur = cur->next;
    }
    addError("object not found in object group");
    exit(ConfigurationErrorObjectNotFound);
}

void parseTileProperties(xmlNodePtr cur, Tile2 *t) {
    while (cur != NULL) {
        const char *name = (char *) cur->name;
        addInfo("tile property node :: %s", name);
        if (strcmp(name, "property") == 0) {
            const char *propName = xmlString(cur, "name");
            const char *propValue = xmlString(cur, "value");
            t->properties[t->propertyCount] = createProperty2(propName, propValue);
            t->propertyCount++;
            addInfo("prop :: %s, %s", propName, propValue);
        }
        cur = cur->next;
    }
}

Tile2 *parseTileNode(xmlNodePtr node) {
    int id = xmlInt(node, "id");
    const char *type = xmlString(node, "type");
    Tile2 *t = createTile2(id, type);
    xmlNodePtr cur = node->children;
    while (cur != NULL) {
        const char *name = (const char *) cur->name;
        if (strcmp(name, "objectgroup") == 0) {
            addInfo("object group");
            t->object = parseTileObjectGroupNode(cur->children);
        } else if (strcmp(name, "properties") == 0) {
            addInfo("properties");
            parseTileProperties(cur->children, t);
        }
        cur = cur->next;
    }
    return t;
}

void parseTilesetNode(xmlNodePtr node, Tileset *tileset) {
    const char *source = (const char *) xmlGetProp(node, (const xmlChar *) "source");
    char filePath[MAX_FS_PATH_LENGTH];
    getComponentPath(filePath, config->indexDir, "tilesets", source);
    xmlDocPtr doc = xmlParseFile(filePath);
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        addError("unable to find tileset :: %s", source);
        exit(ConfigurationErrorMapResourcesMissing);
    }
    while (cur != NULL) {
        const char *name = (const char *) cur->name;
        if (strcmp(name, "tileset") == 0) {
            addInfo("tileset");
            tileset->size.x = xmlInt(cur, "tilewidth");
            tileset->size.y = xmlInt(cur, "tileheight");
            cur = cur->children;
        } else if (strcmp(name, "image") == 0) {
            addInfo("image");
            char imagePath[MAX_FS_PATH_LENGTH];
            getComponentPath(imagePath, "", "tilesets", xmlString(cur, "source"));
            tileset->source = LoadImage(imagePath);
        } else if (strcmp(name, "tile") == 0) {
            addInfo("tile");
            parseTileNode(cur);
        }
        cur = cur->next;
    }
    xmlFreeNode(cur);
    xmlFreeDoc(doc);
}

void parseLayerNode(xmlNodePtr cur, Map *map) {
    const char *layerName = xmlString(cur, "name");
    cur = cur->children;
    while (cur != NULL) {
        const char *name = (const char *) cur->name;
        addInfo("layer node :: %s", name);
        if (strcmp(name, "data") == 0) {
            map->layers2[map->layers2Count] = createLayer2(layerName);
            map->layers2Count++;
            parseLayerRawData(map, (char *) xmlNodeGetContent(cur));
        }
        cur = cur->next;
    }
}

void parseTilemapObjectGroupNode() {
    addInfo("TBD");
}

Map *parseMapNode(xmlNodePtr node) {
    Map *map = createMap();
    map->config->tileSize.x = xmlInt(node, "tilewidth");
    map->config->tileSize.y = xmlInt(node,  "tileheight");
    while (node->children != NULL) {
        const char *name = (const char *) node->children->name;
        if (strcmp(name, "tileset") == 0) {
            parseTilesetNode(node->children, map->tileset);
        } else if (strcmp(name, "layer") == 0) {
            parseLayerNode(node->children, map);
        } else if (strcmp(name, "objectgroup") == 0) {
            parseTilemapObjectGroupNode();
        }
        node->children = node->children->next;
    }
    return map;
}

Map *parseTilemapDoc(const char *filePath, const char *indexDir) {
    xmlDocPtr doc = xmlParseFile(filePath);
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        addError("unable to find map resources for scene :: %s", indexDir);
        exit(ConfigurationErrorMapResourcesMissing);
    }
    Map *map = parseMapNode(cur);
    xmlFreeNode(cur);
    return map;
}