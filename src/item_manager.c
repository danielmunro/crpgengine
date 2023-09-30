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

Item *findItemFromName(ItemManager *im, const char *name) {
    for (int i = 0; i < im->count; i++) {
        if (strcmp(name, im->items[i]->name) == 0) {
            return im->items[i];
        }
    }
    addError("no item found with name provided :: %s", name);
    exit(RuntimeErrorUnknownItem);
}
