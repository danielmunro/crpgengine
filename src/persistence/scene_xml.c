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
    Vector2D size;
    Image source;
} Tilemap;

typedef struct {
    Tile **tiles;
    int tilesCount;
    xmlTextReaderPtr reader;
    Tilemap *tilemap;
} TilesetXml;

typedef struct {
    const char *filePath;
    xmlTextReaderPtr reader;
    TilesetXml *tilesetXml;
    Layer **layers;
    int layerCount;
} TilemapXml;

TilemapXml *createTilemapXml(const char *filePath) {
    TilemapXml *t = malloc(sizeof(TilemapXml));
    t->filePath = filePath;
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tilesetXml = NULL;
    t->layers = calloc(MAX_LAYERS, sizeof(Layer));
    t->layerCount = 0;
    return t;
}

TilesetXml *createTilesetXml(const char *filePath) {
    TilesetXml *t = malloc(sizeof(TilesetXml));
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tiles = calloc(MAX_TILES, sizeof(Tile));
    t->tilesCount = 0;
    t->tilemap = malloc(sizeof(Tilemap));
    return t;
}

Vector2D getTileFromIndex(const Tilemap *t, int index) {
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

Rectangle getRectForTile(const Tilemap *t, int index) {
    Vector2D tile = getTileFromIndex(t, index);
    return (Rectangle) {
            (float) (tile.x * t->size.x),
            (float) (tile.y * t->size.y),
            (float) t->size.x,
            (float) t->size.y,
    };
}

