typedef struct {
    ItemData **items;
    int count;
} ItemManager;

ItemManager *createItemManager() {
    return (ItemManager *) malloc(sizeof(ItemManager));
}
