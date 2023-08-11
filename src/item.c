typedef struct {
    ItemData **items;
    int count;
} ItemManager;

ItemManager *createItemManager() {
    return (ItemManager *) malloc(sizeof(ItemManager));
}

ItemData *findItem(ItemManager *itemManager, const char *name) {
    for (int i = 0; i < itemManager->count; i++) {
        if (strcmp(itemManager->items[i]->name, name) == 0) {
            return itemManager->items[i];
        }
    }
    return NULL;
}
