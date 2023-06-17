typedef struct Tilemap {
    Vector2d size;
    Image source;
} Tilemap;

Vector2d getTileFromIndex(Tilemap *t, int index) {
    printf("index %d, %d, %d\n", index, t->source.width, t->size.x);
    int width = t->source.width / t->size.x;
    int x, y;
    y = index / width;
    x = (index % width);
    Vector2d pos = {x - 1, y};
    return pos;
}

Rectangle getRectForTile(Tilemap *t, int index) {
    printf("sanity\n");
    Vector2d tile = getTileFromIndex(t, index);
    printf("vect: %d, %d", tile.x, tile.y);
    printf("rect x, y: %f, %f\n", (float) (tile.x * t->size.x), (float) (tile.x * t->size.y));
    printf("width, height: %d, %d\n", t->size.x, t->size.y);
    return (Rectangle){
            (float) (tile.x * t->size.x),
            (float) (tile.y * t->size.y),
            (float) t->size.x,
            (float) t->size.y,
    };
}
