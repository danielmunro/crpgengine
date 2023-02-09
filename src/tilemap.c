typedef struct Object {
    int id;
    Rectangle rect;
    int tile;
} Object;

typedef struct Layer {
    int type;
    char data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
} Layer;

typedef struct Tilemap {
    Vector2d size;
    Texture2D source;
    Layer *layers[LAYER_COUNT];
    Object *objects[MAX_OBJECTS];
} Tilemap;

Vector2d getTileFromIndex(Tilemap *t, int index) {
    int width = t->source.width / t->size.x;
    int x, y;
    y = index / width;
    x = (index % width);
    Vector2d pos = {x - 1, y};
    return pos;
}

Rectangle getRectForTile(Tilemap *t, int index) {
    Vector2d tile = getTileFromIndex(t, index);
    return (Rectangle){
            (float) (tile.x * t->size.x),
            (float) (tile.y * t->size.y),
            (float) t->size.x,
            (float) t->size.y,
    };
}
