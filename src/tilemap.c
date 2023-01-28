typedef struct Tile {
    char value;
    char name[24];
    int x;
    int y;
} Tile;

typedef struct Tilemap {
    int width;
    int height;
    Texture2D backgroundLayer;
    Texture2D source;
    char background[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    Tile tiles[MAX_TILES];
} Tilemap;

Tilemap *createTestTilemap() {
    Tilemap *t = malloc(sizeof(Tilemap));
    t->width = 16;
    t->height = 16;
    t->source = LoadTexture("resources/overworld.png");
    strcpy(t->tiles[0].name, "foo");
    t->tiles[0].value = '0';
    t->tiles[0].x = 0;
    t->tiles[0].y = 0;

//    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
//        for (int x = 0; x < MAX_LAYER_SIZE; x++) {
//
//        }
//    }

    return t;
}
