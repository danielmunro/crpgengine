typedef struct {
    const char *id;
    const char *name;
    AnimationType direction;
    AnimationType previousDirection;
    Animation *animations[MAX_ANIMATIONS];
    Avatar *avatar;
    Vector2 position;
    bool moving[DIRECTION_COUNT];
    struct timeval lastMovement;
    bool isBeingMoved;
    Attributes *attributes;
    int turnCounter;
    int waitTimer;
    struct timeval lastTimerUpdate;
    bool locked;
    int actionGauge;
    ItemData **equipment;
    int hp;
    int mana;
    Spell **spells;
    int spellCount;
    ActionStep step;
    float hitAnimationTimer;
    bool isFleeing;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    return findAnimation(mob->animations, mob->direction);
}

Mobile *createMobile(
        const char *id,
        const char *name,
        Vector2 position,
        AnimationType direction,
        Animation *animations[MAX_ANIMATIONS],
        Avatar *avatar,
        int hp,
        int mana,
        Attributes *attributes,
        Spell **spells,
        int spellCount) {
    Mobile *mobile = malloc(sizeof(Mobile));
    mobile->id = &id[0];
    mobile->name = &name[0];
    mobile->direction = direction;
    mobile->position = position;
    mobile->moving[UP] = false;
    mobile->moving[DOWN] = false;
    mobile->moving[LEFT] = false;
    mobile->moving[RIGHT] = false;
    mobile->attributes = attributes;
    mobile->turnCounter = 0;
    mobile->waitTimer = -1;
    mobile->locked = false;
    gettimeofday(&mobile->lastTimerUpdate, NULL);
    gettimeofday(&mobile->lastMovement, NULL);
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        mobile->animations[i] = animations[i];
    }
    mobile->avatar = avatar;
    mobile->isBeingMoved = false;
    mobile->actionGauge = 0;
    mobile->equipment = calloc(MAX_EQUIPMENT, sizeof(ItemData));
    mobile->hp = hp;
    mobile->mana = mana;
    mobile->spells = spells;
    mobile->spellCount = spellCount;
    mobile->step = STEP_NONE;
    mobile->hitAnimationTimer = 0;
    mobile->isFleeing = false;
    return mobile;
}

void resetMoving(Mobile *mob) {
    mob->moving[UP] = false;
    mob->moving[DOWN] = false;
    mob->moving[LEFT] = false;
    mob->moving[RIGHT] = false;
    getMobAnimation(mob)->isPlaying = 0;
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
    }
    if (direction == LEFT) {
        return (Vector2) {mob->position.x - 1, mob->position.y};
    }
    if (direction == RIGHT) {
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

void useEntrance(Mobile *mob, Entrance *e) {
    mob->position.x = e->area.x + (e->area.width / 2) - (int) (MOB_COLLISION_WIDTH / 2);
    mob->position.y = e->area.y + (e->area.height / 2) - (int) (MOB_COLLISION_HEIGHT / 2);
    mob->direction = e->direction;
}

bool canPlayerMove(Mobile *mob) {
    return !mob->isBeingMoved && !mob->locked && mob->waitTimer < 0;
}

bool isReadyForAction(Mobile *mob) {
    return mob->actionGauge >= MAX_ACTION_GAUGE && mob->hp > 0 && mob->step == STEP_NONE;
}

Attributes calculateMobileAttributes(Mobile *mob) {
    Attributes calculated = *mob->attributes;
    for (int i = 0; i < MAX_EQUIPMENT; i++) {
        if (mob->equipment[i] != NULL) {
            calculated = combineAttributes(calculated, *createAttributesFromData(mob->equipment[i]->attributes));
        }
    }
    return calculated;
}

void normalizeVitalsForMobile(Mobile *mob) {
    Attributes calculated = calculateMobileAttributes(mob);
    if (mob->hp > calculated.hp) {
        mob->hp = calculated.hp;
    }
    if (mob->hp < 0) {
        mob->hp = 0;
    }
    if (mob->mana > calculated.mana) {
        mob->mana = calculated.mana;
    }
    if (mob->mana < 0) {
        mob->mana = 0;
    }
}

bool canApplyCost(Mobile *caster, Attributes *cost) {
    if (caster->mana < cost->mana) {
        return false;
    }
    return true;
}

bool applyCastCost(Mobile *caster, Attributes *cost) {
    if (!canApplyCost(caster, cost)) {
        return false;
    }
    caster->mana -= cost->mana;
    return true;
}

void applyConsumable(Mobile *mob, Item *item) {
    mob->hp += item->attributes->hp;
    mob->mana += item->attributes->mana;
    normalizeVitalsForMobile(mob);
}

void drawAvatar(Mobile *mob, Vector2 position) {
    DrawTextureRec(
            mob->avatar->image,
            (Rectangle) {
                    0,
                    0,
                    (float) mob->avatar->image.width,
                    (float) mob->avatar->image.height,
            },
            position,
            WHITE);
}
