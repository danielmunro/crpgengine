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

void parseTilesetNode(const struct _xmlNode *node) {
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
            cur = cur->children;
        } else if (strcmp(name, "image") == 0) {
            addInfo("image");
        } else if (strcmp(name, "tile") == 0) {
            addInfo("tile");
        }
        cur = cur->next;
    }
    xmlFreeNode(cur);
    xmlFreeDoc(doc);
}

void parseLayerNode() {
    addInfo("TBD");
}

void parseObjectGroupNode() {
    addInfo("TBD");
}

Map *parseMapNode(xmlNodePtr node) {
    Map *map = createMap();
    map->config->tileSize.x = TextToInteger((const char *) xmlGetProp(node, (const xmlChar *) "tilewidth"));
    map->config->tileSize.y = TextToInteger((const char *) xmlGetProp(node, (const xmlChar *) "tileheight"));
    while (node->children != NULL) {
        const char *name = (const char *) node->children->name;
        if (strcmp(name, "tileset") == 0) {
            addInfo("child :: %s", name);
            parseTilesetNode(node->children);
        } else if (strcmp(name, "layer") == 0) {
            addInfo("child :: %s", name);
            parseLayerNode();
        } else if (strcmp(name, "objectgroup") == 0) {
            addInfo("child :: %s", name);
            parseObjectGroupNode();
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