#include "headers/persistence/cyaml.h"
#include "headers/attributes.h"

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
} Item;

typedef struct {
    Item *item;
    int amount;
} ItemList;

typedef struct {
    ItemList *itemList;
    int count;
} ItemListResult;

typedef struct {
    Item *item;
    int quantity;
} ItemWithQuantity;

typedef struct {
    Item *item;
    int worth;
} ItemWithMarkup;

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
        EquipmentPosition position) {
    Item *item = malloc(sizeof(Item));
    item->type = type;
    item->name = name;
    item->worth = worth;
    item->attributes = attributes;
    item->position = position;
    return item;
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
    return createItem(
            getItemTypeFromString(data->type),
            data->name,
            data->worth,
            createAttributesFromData(data->attributes),
            getEquipmentPositionFromString(data->position));
}

ItemData createItemData(const Item *item) {
    return (ItemData) {
            item->name,
            ItemTypes[item->type],
            EquipmentPositions[item->position],
            item->worth,
            createDataFromAttributes(item->attributes),
    };
}
