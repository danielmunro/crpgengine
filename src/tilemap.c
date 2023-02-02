typedef struct Tile {
    char value;
    char name[24];
    int x;
    int y;
} Tile;

typedef struct Tilemap {
    Vector2D size;
    Texture2D source;
    char background[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
} Tilemap;

Tilemap *createTestTilemap() {
    Tilemap *t = malloc(sizeof(Tilemap));
    Vector2D size = {16, 16 };
    t->size = size;
    t->source = LoadTexture("resources/overworld.png");

    for (int y = 0; y < MAX_LAYER_SIZE / 4; y++) {
        for (int x = 0; x < MAX_LAYER_SIZE / 4; x++) {
            t->background[y][x] = 0;
        }
    }

    return t;
}

Vector2D getTileFromIndex(Tilemap *t, int index) {
    int width = t->source.width / t->size.x;
    int x, y;
    if (index == 0) {
        x = 0;
        y = 0;
    } else {
        x = width % index;
        y = width / index;
    }
    Vector2D pos = {x, y};
    return pos;
}
