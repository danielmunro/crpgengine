typedef struct {
    Rectangle rect;
    int tile;
} Object;

Object *createTileObject(int tile, Rectangle rect) {
    Object *o = malloc(sizeof(Object));
    o->tile = tile;
    o->rect = rect;
    return o;
}

Object *getObject(Object *objects[MAX_OBJECTS], int index) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (objects[i] == NULL) {
            return NULL;
        }
        if (objects[i]->tile == index) {
            return objects[i];
        }
    }
    return NULL;
}
