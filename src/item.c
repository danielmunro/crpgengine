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
    int worth;
} Item;

typedef struct {
    Item **items;
    int count;
} ItemManager;
//
//Item *createConsumable(
//        const char *name,
//        ConsumeAffect *consumeAffect,
//        int worth) {
//    Item *item = malloc(sizeof(Item));
//    item->type = CONSUMABLE;
//    item->name = &name[0];
//    item->consumeAffect = consumeAffect;
//    item->worth = worth;
//    return item;
//}
//
//Item *createEquipment(
//        const char *name,
//        EquipmentPosition position,
//        Attributes *attributes,
//        int worth) {
//    Item *item = malloc(sizeof(Item));
//    item->type = EQUIPMENT;
//    item->name = &name[0];
//    item->position = position;
//    item->attributes = attributes;
//    item->worth = worth;
//    return item;
//}

ItemManager *createItemManager() {
    ItemManager *itemManager = malloc(sizeof(ItemManager));
    Item items[] = (Item[]) {
            {
                .name = "potion",
                .type = CONSUMABLE,
                .attributes = &(Attributes) { .hp = 20 }
            },
            {
                .name = "an iron sword",
                .type = EQUIPMENT,
                .position = WEAPON,
                .attributes = &(Attributes) { .strength = 1 }
            },
            {
                .name = "leather armor",
                .type = EQUIPMENT,
                .position = TORSO,
                .attributes = &(Attributes) { .constitution = 1 }
            }
    };
    itemManager->count = sizeof(items) / sizeof(Item);
    itemManager->items = calloc(itemManager->count, sizeof(Item));
    for (int i = 0; i < itemManager->count; i++) {
        itemManager->items[i] = &items[i];
    }
    return itemManager;
}
