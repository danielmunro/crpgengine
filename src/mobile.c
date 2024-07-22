#include <mm_malloc.h>
#include <stdio.h>
#include "headers/persistence/cyaml.h"
#include "headers/util/util.h"
#include "headers/warp.h"
#include "headers/spell.h"
#include "headers/item.h"
#include "headers/graphics/avatar.h"
#include "headers/graphics/animation.h"
#include "headers/action_type.h"

typedef struct {
    const char *id;
    const char *name;
    Direction direction;
    Direction previousDirection;
    Animation *animations[MAX_ANIMATIONS];
    Avatar *avatar;
    Vector2D position;
    bool moving[DIRECTION_COUNT];
    struct timeval lastMovement;
    bool isBeingMoved;
    Attributes *attributes;
    int turnCounter;
    int waitTimer;
    struct timeval lastTimerUpdate;
    bool locked;
    int actionGauge;
    Item **equipment;
    int hp;
    int mana;
    Spell **spells;
    int spellCount;
    ActionStep step;
    float hitAnimationTimer;
    bool isFleeing;
    bool immortal;
    int amountToMove;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    return findAnimation(mob->animations, mob->direction);
}

Mobile *createMobile(
        const char *id,
        const char *name,
        Vector2D position,
        Direction direction,
        Animation *animations[MAX_ANIMATIONS],
        Avatar *avatar,
        int hp,
        int mana,
        Attributes *attributes,
        Spell **spells,
        int spellCount,
        bool immortal) {
    Mobile *mobile = malloc(sizeof(Mobile));
    mobile->id = &id[0];
    mobile->name = &name[0];
    mobile->direction = direction;
    mobile->position = position;
    mobile->moving[DIRECTION_UP] = false;
    mobile->moving[DIRECTION_DOWN] = false;
    mobile->moving[DIRECTION_LEFT] = false;
    mobile->moving[DIRECTION_RIGHT] = false;
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
    mobile->immortal = immortal;
    mobile->amountToMove = 0;
    return mobile;
}

void resetMoving(Mobile *mob) {
    mob->moving[DIRECTION_UP] = false;
    mob->moving[DIRECTION_DOWN] = false;
    mob->moving[DIRECTION_LEFT] = false;
    mob->moving[DIRECTION_RIGHT] = false;
    getMobAnimation(mob)->isPlaying = false;
}

bool isMoving(const Mobile *mob) {
    return mob->moving[DIRECTION_DOWN]
           || mob->moving[DIRECTION_UP]
           || mob->moving[DIRECTION_LEFT]
           || mob->moving[DIRECTION_RIGHT];
}

Rectangle getMobileRectangle(Mobile *mob) {
    const Rectangle *c = getMobAnimation(mob)->spriteSheet->collision;
    return (Rectangle) {
            (float) mob->position.x + c->x,
            (float) mob->position.y + c->y,
            c->width,
            c->height,
    };
}

float getMoveAmount(float fps) {
    return 60 / fps;
}

void updateDirection(Mobile *mob, Direction direction) {
    mob->previousDirection = mob->direction;
    mob->direction = direction;
}
float normalizeMoveAmount(float a, float b, float fps) {
    int a1 = (int) a;
    int b1 = (int) b;
    if (a1 > b1) return -getMoveAmount(fps);
    if (a1 < b1) return getMoveAmount(fps);
    return 0;
}

bool moveMob(Mobile *mob, Vector2D destination, float targetFPS) {
    resetMoving(mob);
    int x = 0;
    int y = 0;
    if (mob->position.x > destination.x) {
        x = -1;
    } else if (mob->position.x < destination.x) {
        x = 1;
    }
    if (mob->position.y > destination.y) {
        y = -1;
    } else if (mob->position.y < destination.y) {
        y = 1;
    }
    mob->position.x += x;
    mob->position.y += y;
    bool moved = x != 0 || y != 0;
    Animation *animation = getMobAnimation(mob);
    animation->isPlaying = moved;
    if (moved) {
        if (x > 0) mob->direction = DIRECTION_RIGHT;
        else if (x < 0) mob->direction = DIRECTION_LEFT;
        else if (y > 0) mob->direction = DIRECTION_DOWN;
        else mob->direction = DIRECTION_UP;
        mob->moving[mob->direction] = true;
    }
    incrementAnimationFrame(animation, targetFPS);
    return moved;
}

char *getPositionAsString(Vector2D position) {
    char *value = malloc(255);
    sprintf(value, "%d, %d", position.x, position.y);
    return value;
}

void useEntrance(Mobile *mob, const Entrance *e) {
    const Rectangle *c = getMobAnimation(mob)->spriteSheet->collision;
    addInfo("use entrance :: %s, %d, %d", e->name, e->area.x, e->area.y);
    mob->position.x = e->area.x;
    mob->position.y = (e->area.y - (int) c->y);
    mob->direction = e->direction;
}

bool canPlayerMove(const Mobile *mob) {
    return !mob->isBeingMoved && !mob->locked && mob->waitTimer < 0 && mob->amountToMove == 0;
}

bool isReadyForAction(Mobile *mob) {
    return mob->actionGauge >= MAX_ACTION_GAUGE && mob->hp > 0 && mob->step == STEP_NONE;
}

Attributes calculateMobileAttributes(Mobile *mob) {
    Attributes calculated = *mob->attributes;
    for (int i = 0; i < MAX_EQUIPMENT; i++) {
        if (mob->equipment[i] != NULL) {
            calculated = combineAttributes(calculated, *mob->equipment[i]->attributes);
        }
    }
    return calculated;
}

void normalizeVitalsForMobile(Mobile *mob) {
    Attributes calculated = calculateMobileAttributes(mob);
    if (mob->hp > calculated.hp) {
        mob->hp = calculated.hp;
    }
     if (mob->hp <= 0) {
        mob->hp = mob->immortal ? 1 : 0;
    }
    if (mob->mana > calculated.mana) {
        mob->mana = calculated.mana;
    }
    if (mob->mana < 0) {
        mob->mana = 0;
    }
}

bool canApplyCost(const Mobile *caster, const Attributes *cost) {
    if (caster->mana < cost->mana) {
        return false;
    }
    return true;
}

bool applyCastCost(Mobile *caster, const Attributes *cost) {
    if (!canApplyCost(caster, cost)) {
        return false;
    }
    caster->mana -= cost->mana;
    return true;
}

void applyConsumable(Mobile *mob, const Item *item) {
    mob->hp += item->attributes->hp;
    mob->mana += item->attributes->mana;
    normalizeVitalsForMobile(mob);
}

void executeSpellOnMobile(Mobile *mob, const Spell *spell) {
    mob->hp += getSpellAttributeAmount(spell, spell->impact->hp);
    mob->mana += getSpellAttributeAmount(spell, spell->impact->mana);
    normalizeVitalsForMobile(mob);
}
