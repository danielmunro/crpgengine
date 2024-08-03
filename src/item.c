#include "headers/persistence/cyaml.h"
#include "headers/attributes.h"

#define MAX_ITEM_AFFECTS 32

const char *ItemAffects[] = {
        "none",
        "raise",
};

typedef enum {
    ITEM_AFFECT_NONE,
    ITEM_AFFECT_RAISE,
} ItemAffect;

const ItemAffect ItemAffectsEnum[] = {
        ITEM_AFFECT_NONE,
        ITEM_AFFECT_RAISE,
};

const char *ItemTypes[] = {
        "none",
        "consumable",
        "equipment",
        "crafting material",
        "quest",
};

typedef enum {
    ITEM_TYPE_NONE,
    ITEM_TYPE_CONSUMABLE,
    ITEM_TYPE_EQUIPMENT,
    ITEM_TYPE_CRAFTING_MATERIAL,
    ITEM_TYPE_QUEST,
} ItemType;

const char *EquipmentPositions[] = {
        "none",
        "weapon",
        "torso",
        "wrist",
        "accessory",
};

typedef enum {
    POSITION_NONE,
    POSITION_WEAPON,
    POSITION_TORSO,
    POSITION_WRIST,
    POSITION_ACCESSORY,
} EquipmentPosition;

typedef struct {
    ItemType type;
    const char *name;
    int worth;
    Attributes *attributes;
    EquipmentPosition position;
    ItemAffect *affects;
    int affectsCount;
    int quantity;
} Item;

typedef struct {
    Item *item;
    int amount;
} ItemList;

typedef struct {
    Item *item;
    int quantity;
} ItemWithQuantity;

typedef struct {
    Item *item;
    int worth;
} ItemWithMarkup;

ItemAffect *mapStringsToItemAffects(const char **affectStrings, int count) {
    ItemAffect *affects = calloc(count, sizeof(ItemAffect));
    int totalAffectCount = sizeof(ItemAffects) / sizeof(ItemAffects[0]);
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < totalAffectCount; j++) {
            if (strcmp(ItemAffects[j], affectStrings[i]) == 0) {
                affects[i] = j;
            }
        }
    }
    return affects;
}

const char **mapItemAffectsToStrings(const ItemAffect *affects, int count) {
    const char **affectStrings = calloc(count, sizeof(char *));
    for (int i = 0; i < count; i++) {
        affectStrings[i] = ItemAffects[affects[i]];
    }
    return affectStrings;
}

ItemType getItemTypeFromString(const char *type) {
    int count = sizeof(ItemTypes) / sizeof(ItemTypes[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(ItemTypes[i], type) == 0) {
            return i;
        }
    }
    return ITEM_TYPE_NONE;
}

EquipmentPosition getEquipmentPositionFromString(const char *position) {
    if (position == NULL) {
        return POSITION_NONE;
    }
    int count = sizeof(EquipmentPositions) / sizeof(EquipmentPositions[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(EquipmentPositions[i], position) == 0) {
            return i;
        }
    }
    return POSITION_NONE;
}

Item *createItem(
        ItemType type,
        const char *name,
        int worth,
        Attributes *attributes,
        EquipmentPosition position,
        const ItemAffect *affects,
        int affectsCount,
        int quantity) {
    Item *item = malloc(sizeof(Item));
    item->type = type;
    item->name = name;
    item->worth = worth;
    item->attributes = attributes;
    item->position = position;
    item->affects = calloc(MAX_ITEM_AFFECTS, sizeof (ItemAffect));
    for (int i = 0; i < affectsCount; i++) {
        item->affects[i] = affects[i];
    }
    item->affectsCount = affectsCount;
    item->quantity = quantity;
    return item;
}

Item *cloneItem(const Item *item) {
    Attributes *a = NULL;
    if (item->attributes != NULL) {
        a = cloneAttributes(item->attributes);
    }
    return createItem(
            item->type,
            item->name,
            item->worth,
            a,
            item->position,
            item->affects,
            item->affectsCount,
            item->quantity);
}

ItemWithQuantity *createItemWithQuantity(Item *item, int quantity) {
    ItemWithQuantity *iq = malloc(sizeof(ItemWithQuantity));
    iq->item = item;
    iq->quantity = quantity;
    return iq;
}

ItemWithMarkup *createItemWithMarkup(Item *item, int worth) {
    ItemWithMarkup *im = malloc(sizeof(ItemWithMarkup));
    im->item = item;
    im->worth = worth;
    return im;
}

Item *createItemFromData(const ItemData *data) {
    ItemAffect *affects = mapStringsToItemAffects(data->affects, data->affects_count);
    Item *item = createItem(
            getItemTypeFromString(data->type),
            data->name,
            data->worth,
            createAttributesFromData(data->attributes),
            getEquipmentPositionFromString(data->position),
            affects,
            data->affects_count,
            data->quantity);
    free(affects);
    return item;
}

ItemData createItemData(const Item *item) {
    const char **itemAffects = mapItemAffectsToStrings(item->affects, item->affectsCount);
    return (ItemData) {
            item->name,
            ItemTypes[item->type],
            EquipmentPositions[item->position],
            item->worth,
            createDataFromAttributes(item->attributes),
            itemAffects,
            item->affectsCount,
    };
}
