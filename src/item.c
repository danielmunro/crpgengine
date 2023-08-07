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

ItemManager *createItemManager(const char *indexDir) {
    ItemManager *itemManager = malloc(sizeof(ItemManager));
    return itemManager;
//    Item items[] = (Item[]) {
//            {
//                .name = "potion",
//                .type = CONSUMABLE,
//                .attributes = &(Attributes) { .hp = 20 },
//                .worth = 30,
//            },
//            {
//                .name = "an iron sword",
//                .type = EQUIPMENT,
//                .position = WEAPON,
//                .attributes = &(Attributes) { .strength = 1 },
//                .worth = 300,
//            },
//            {
//                .name = "leather armor",
//                .type = EQUIPMENT,
//                .position = TORSO,
//                .attributes = &(Attributes) {
//                    .constitution = 1,
//                    .ac = &(ArmorClass) {
//                        .bash = 1,
//                        .slash = 1,
//                        .pierce = 1,
//                        .fire = 1,
//                        .water = 1,
//                        .electricity = 2,
//                        .corrosive = 1,
//                        .energy = 1,
//                    }
//                },
//                .worth = 250,
//            }
//    };
//    itemManager->count = sizeof(items) / sizeof(Item);
//    itemManager->items = calloc(itemManager->count, sizeof(Item));
//    for (int i = 0; i < itemManager->count; i++) {
//        itemManager->items[i] = &items[i];
//    }
//    return itemManager;
}
