#include <mm_malloc.h>
#include <libxml/xmlreader.h>

typedef struct {
    LayerType type;
    int data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    int width;
    int height;
    bool showCollisions;
} Layer;

typedef struct {
    Tile **tiles;
    int tilesCount;
    xmlTextReaderPtr reader;
    Vector2D size;
    Image source;
} Tileset;

typedef struct {
    const char *filePath;
    xmlTextReaderPtr reader;
    Tileset *tileset;
    Layer **layers;
    int layerCount;
} Tilemap;

Tilemap *createTilemapXml(const char *filePath) {
    Tilemap *t = malloc(sizeof(Tilemap));
    t->filePath = filePath;
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tileset = NULL;
    t->layers = calloc(MAX_LAYERS, sizeof(Layer));
    t->layerCount = 0;
    return t;
}

Tileset *createTilesetXml(const char *filePath) {
    Tileset *t = malloc(sizeof(Tileset));
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tiles = calloc(MAX_TILES, sizeof(Tile));
    t->tilesCount = 0;
    return t;
}

Vector2D getTileFromIndex(const Tileset *t, int index) {
    int width = t->source.width / t->size.x;
    int y = index / width;
    int x = (index % width);
    if (x - 1 < 0) {
        y--;
        x = width;
    }
    Vector2D pos = {x - 1, y};
    return pos;
}

Rectangle getRectForTile(const Tileset *t, int index) {
    Vector2D tile = getTileFromIndex(t, index);
    return (Rectangle) {
            (float) (tile.x * t->size.x),
            (float) (tile.y * t->size.y),
            (float) t->size.x,
            (float) t->size.y,
    };
}

