typedef struct {
    int hp;
    int mp;
} ConsumeAffect;

typedef struct {
    const char *name;
    int quantity;
} Item;

typedef struct {
    ItemData **items;
    int count;
} ItemManager;

ItemManager *createItemManager() {
    return (ItemManager *) malloc(sizeof(ItemManager));
}
