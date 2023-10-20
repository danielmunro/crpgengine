#include <mm_malloc.h>
#include <libxml/xmlreader.h>

typedef struct {
    Tile **tiles;
    int tilesCount;
    xmlTextReaderPtr reader;
} TilesetXml;

typedef struct {
    const char *filePath;
    xmlTextReaderPtr reader;
    TilesetXml *tilesetXml;
} TilemapXml;

TilemapXml *createTilemapXml(const char *filePath) {
    TilemapXml *t = malloc(sizeof(TilemapXml));
    t->filePath = filePath;
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tilesetXml = NULL;
    return t;
}

TilesetXml *createTilesetXml(const char *filePath) {
    TilesetXml *t = malloc(sizeof(TilesetXml));
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tiles = calloc(MAX_TILES, sizeof(Tile));
    t->tilesCount = 0;
    return t;
}
