typedef struct Object {
    int id;
    Rectangle rect;
    int tile;
} Object;

Object *createObject() {
    Object *o = malloc(sizeof(Object));
    o->id = 0;
    o->tile = 0;
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
