typedef struct {
    int hp;
    int mp;
} ConsumeAffect;

typedef struct {
    ItemType type;
    EquipmentPosition position;
    const char *name;
    Attributes *attributes;
    ConsumeAffect *consumeAffect;
} Item;

typedef struct {
    Item **items;
    int count;
} ItemManager;

Item *createConsumable(
        const char *name,
        ConsumeAffect *consumeAffect) {
    Item *item = malloc(sizeof(Item));
    item->type = CONSUMABLE;
    item->name = &name[0];
    item->consumeAffect = consumeAffect;
    return item;
}

Item *createEquipment(
        const char *name,
        EquipmentPosition position,
        Attributes *attributes) {
    Item *item = malloc(sizeof(Item));
    item->type = EQUIPMENT;
    item->name = &name[0];
    item->position = position;
    item->attributes = attributes;
    return item;
}

ItemManager *createItemManager() {
    ItemManager *itemManager = malloc(sizeof(ItemManager));
    itemManager->items = calloc(2, sizeof(Item));
    Item items[] = (Item[]) {
            {
                    .name = "potion",
                    .type = CONSUMABLE,
                    .attributes = &(Attributes) { .hp = 20 }
            },
            {
                    .name = "an iron sword",
                    .type = EQUIPMENT,
                    .attributes = &(Attributes) { .strength = 1 }
            }
    };
    itemManager->count = sizeof(items) / sizeof(Item);
    for (int i = 0; i < itemManager->count; i++) {
        itemManager->items[i] = &items[i];
    }
    return itemManager;
}
