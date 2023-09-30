#ifndef CJRPGENGINE_COLLISION_H
#define CJRPGENGINE_COLLISION_H

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
    fprintf(stderr, "no collision type found :: %s", type);
    exit(RuntimeErrorUnknownCollisionType);
}

#endif //CJRPGENGINE_COLLISION_H
