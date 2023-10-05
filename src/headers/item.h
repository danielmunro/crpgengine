#ifndef CJRPGENGINE_ITEM_H
#define CJRPGENGINE_ITEM_H

#define MAX_ITEM_LIST_BUFFER 10000

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

#include "src/item.c"
#include "src/item_manager.c"

#endif //CJRPGENGINE_ITEM_H
