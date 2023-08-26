typedef struct {
    Item **items;
    int count;
} ItemManager;

ItemManager *createItemManager() {
    ItemManager *im = malloc(sizeof(ItemManager));
    im->items = calloc(MAX_ITEMS, sizeof(Item));
    im->count = 0;
    return im;
}

Item *findItem(Item **items, const char *name) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i] == NULL) {
            return NULL;
        }
        if (strcmp(items[i]->name, name) == 0) {
            return items[i];
        }
    }
    return NULL;
}

Item *createItemFromPlayerData(ItemManager *im, PlayerItemData data) {
    for (int i = 0; i < im->count; i++) {
        if (strcmp(im->items[i]->name, data.name) == 0) {
            return im->items[i];
        }
    }
    return NULL;
}
