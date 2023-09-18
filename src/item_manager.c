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

Item *createItemFromReferenceData(ItemManager *im, ItemReferenceData *referenceData) {
    for (int i = 0; i < im->count; i++) {
        if (strcmp(im->items[i]->name, referenceData->name) == 0) {
            ItemData data = createItemData(im->items[i]);
            return createItemFromData(&data);
        }
    }
    return NULL;
}

Item **createItemsFromReferenceData(ItemManager *im, ItemReferenceData *referenceData) {
    Item **items = calloc(referenceData->quantity, sizeof(Item));
    for (int i = 0; i < im->count; i++) {
        if (strcmp(im->items[i]->name, referenceData->name) == 0) {
            for (int j = 0; j < referenceData->quantity; j++) {
                items[j] = im->items[i];
            }
            break;
        }
    }
    return items;
}
