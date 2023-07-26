typedef struct {
    Animation *animations[MAX_ANIMATIONS];
    const char *id;
    const char *name;
    AnimationType direction;
    AnimationType previousDirection;
    Vector2 position;
    bool moving[DIRECTION_COUNT];
    struct timeval lastMovement;
    bool isBeingMoved;
    Attributes *attributes;
    int turnCounter;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    return findAnimation(mob->animations, mob->direction);
}

Mobile *createMobile(const char *id, const char *name, Vector2 position, AnimationType direction,
                     Animation *animations[MAX_ANIMATIONS]) {
    Mobile *mobile = malloc(sizeof(Mobile));
    mobile->id = &id[0];
    mobile->name = &name[0];
    mobile->direction = direction;
    mobile->position = position;
    mobile->moving[UP] = false;
    mobile->moving[DOWN] = false;
    mobile->moving[LEFT] = false;
    mobile->moving[RIGHT] = false;
    mobile->attributes = createEmptyAttributes();
    mobile->turnCounter = 0;
    gettimeofday(&mobile->lastMovement, NULL);
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        mobile->animations[i] = animations[i];
    }
    mobile->isBeingMoved = false;
    return mobile;
}

Mobile *createMobileFromData(MobileData *data, Animation *animations[MAX_ANIMATIONS]) {
    Mobile *mob = createMobile(
            data->id,
            data->name,
            getPositionFromString(data->position),
            getDirectionFromString(data->direction),
            animations);
    return mob;
}

void resetMoving(Mobile *mob) {
    mob->moving[UP] = false;
    mob->moving[DOWN] = false;
    mob->moving[LEFT] = false;
    mob->moving[RIGHT] = false;
}

bool isMoving(Mobile *mob) {
    return mob->moving[DOWN]
           || mob->moving[UP]
           || mob->moving[LEFT]
           || mob->moving[RIGHT];
}

Rectangle getMobileRectangle(Mobile *mob) {
    return (Rectangle) {
            mob->position.x,
            mob->position.y + MOB_COLLISION_HEIGHT_OFFSET,
            MOB_COLLISION_WIDTH,
            MOB_COLLISION_HEIGHT,
    };
}

Vector2 getMoveFor(Mobile *mob, AnimationType direction) {
    if (direction == UP) {
        return (Vector2) {mob->position.x, mob->position.y - 1};
    } else if (direction == DOWN) {
        return (Vector2) {mob->position.x, mob->position.y + 1};
    } if (direction == LEFT) {
        return (Vector2) {mob->position.x - 1, mob->position.y};
    } if (direction == RIGHT) {
        return (Vector2) {mob->position.x + 1, mob->position.y};
    }
    return mob->position;
}

void updateDirection(Mobile *mob, AnimationType direction) {
    mob->previousDirection = mob->direction;
    mob->direction = direction;
}

bool moveMob(Mobile *mob, Vector2 destination) {
    resetMoving(mob);
    float x = normalize(mob->position.x, destination.x);
    float y = normalize(mob->position.y, destination.y);
    mob->position.x += x;
    mob->position.y += y;
    bool moved = x != 0 || y != 0;
    Animation *animation = getMobAnimation(mob);
    animation->isPlaying = moved;
    if (moved) {
        if (x > 0) mob->direction = RIGHT;
        else if (x < 0) mob->direction = LEFT;
        else if (y > 0) mob->direction = DOWN;
        else mob->direction = UP;
        mob->moving[mob->direction] = true;
    }
    incrementAnimationFrame(animation);
    return moved;
}

char *getPositionAsString(Vector2 position) {
    char *value = malloc(255);
    sprintf(value, "%.1f, %.1f", position.x, position.y);
    return value;
}
