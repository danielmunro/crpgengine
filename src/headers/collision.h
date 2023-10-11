#ifndef CJRPGENGINE_COLLISION_H
#define CJRPGENGINE_COLLISION_H

#include <string.h>
#include "src/headers/util/log.h"
#include "errors.h"

const char *CollisionTypes[] = {
        "objects",
        "player",
        "warps",
};

const int CollisionTypeCount = sizeof(CollisionTypes) / sizeof(CollisionTypes[0]);

typedef enum {
    COLLISION_TYPE_OBJECTS,
    COLLISION_TYPE_PLAYER,
    COLLISION_TYPE_WARPS,
} CollisionType;

CollisionType getCollisionTypeFromString(const char *type) {
    for (int i = 0; i < CollisionTypeCount; i++) {
        if (strcmp(type, CollisionTypes[i]) == 0) {
            return i;
        }
    }
    addError("no collision type found :: %s", type);
    exit(ConfigurationErrorUnknownCollisionType);
}

#endif //CJRPGENGINE_COLLISION_H
