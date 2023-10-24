#include <mm_malloc.h>
#include <string.h>
#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/persistence/db.h"
#include "headers/tile.h"
#include "headers/warp.h"

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

void parseTileObjectGroupNode(Tile *t, xmlNodePtr cur) {
    while (cur != NULL) {
        if (strcmp((const char *) cur->name, "object") == 0) {
            addInfo("object group child :: %s", (char *) cur->name);
            t->objects[t->objectCount] = createObject(
                    xmlInt(cur, "id"),
                    (Rectangle) {
                            xmlFloat(cur, "x"),
                            xmlFloat(cur, "y"),
                            xmlFloat(cur, "width"),
                            xmlFloat(cur, "height"),
                    });
            t->objectCount++;
        }
        cur = cur->next;
    }
}

void parseTileProperties(xmlNodePtr cur, Tile *t) {
    while (cur != NULL) {
        const char *name = (char *) cur->name;
        addInfo("tile property node :: %s", name);
        if (strcmp(name, "property") == 0) {
            const char *propName = xmlString(cur, "name");
            const char *propValue = xmlString(cur, "value");
            t->properties[t->propertyCount] = createProperty(propName, propValue);
            t->propertyCount++;
            addInfo("prop :: %s, %s", propName, propValue);
        }
        cur = cur->next;
    }
}

Tile *parseTileNode(xmlNodePtr node) {
    int id = xmlInt(node, "id");
    const char *type = xmlString(node, "type");
    Tile *t = createTile(id, type);
    xmlNodePtr cur = node->children;
    while (cur != NULL) {
        const char *name = (const char *) cur->name;
        if (strcmp(name, "objectgroup") == 0) {
            addInfo("object group");
            parseTileObjectGroupNode(t, cur->children);
        } else if (strcmp(name, "properties") == 0) {
            addInfo("properties");
            parseTileProperties(cur->children, t);
        }
        cur = cur->next;
    }
    return t;
}

void parseTilesetNode(xmlNodePtr node, Tileset *t) {
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
            t->size.x = xmlInt(cur, "tilewidth");
            t->size.y = xmlInt(cur, "tileheight");
            cur = cur->children;
        } else if (strcmp(name, "image") == 0) {
            addInfo("image");
            char imagePath[MAX_FS_PATH_LENGTH];
            getComponentPath(imagePath, "", "tilesets", xmlString(cur, "source"));
            t->reader = xmlReaderForFile(filePath, NULL, 0);
            t->source = LoadImage(imagePath);
        } else if (strcmp(name, "tile") == 0) {
            addInfo("tile");
            t->tiles[t->tilesCount] = parseTileNode(cur);
            t->tilesCount++;
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
            map->layers[map->layersCount] = createLayer(layerName);
            map->layersCount++;
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